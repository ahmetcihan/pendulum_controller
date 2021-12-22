#include "fuzzy_pid.h"
#include "ui_fuzzy_pid.h"
#include "dc_motor_pc.h"

fuzzy_pid::fuzzy_pid(DC_Motor_PC *master, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fuzzy_pid)
{
    ui->setupUi(this);
    dcMotorPc = master;

    QThread* thread_1 = new QThread(this);
    thread_timer = new QTimer(0); //parent must be null
    thread_timer->setInterval(25);
    thread_timer->moveToThread(thread_1);
    connect(thread_timer, SIGNAL(timeout()), SLOT(fuzpid_thread_handler()), Qt::DirectConnection);
    QObject::connect(thread_1, SIGNAL(started()), thread_timer, SLOT(start()));
#ifdef CONFIG_ARM
    QTimer::singleShot(10000,thread_1,SLOT(start()));
#else
    QTimer::singleShot(1000,thread_1,SLOT(start()));
#endif

    hard_stop = false;
    breaking_case = 0;
    max_load_value = 0;
    test_finished = false;
    to_gui.test_finished = false;
    test_label_status = false;
    command_silencer = false;
    load_value = 0;
    displacement_value = 0;
    ch3_value = 0;
    ch4_value = 0;
    error.max_load = false;
    error.max_displacement = false;
    error.go_home_load = false;
    run_pid = false;
    run_cbr = false;
    relay_auto_man = RELAY_ON;
    relay_start_stop = RELAY_OFF;
    current_pace_rate = 0;
    current_displacement_rate = 0;
    PID_first_in = true;
    communication_established = false;
    step_abs_position = 0;
    step_motor_in_test = 0;
    LS_up_error = 0;
    LS_down_error = 0;
    TMC_command = TMC_STOP;

    bessel_filter_coeffs();
}
void fuzzy_pid::fuzpid_thread_handler(void){
    static bool switcher = true;
    static u8 counter = 0;
    static u8 bound = 5;

    if(command_silencer == false){
        counter++;
        if(counter > bound){
            counter = 0;
            if(switcher){
                always_send();
                bound = 2;
            }
            else{
                read_parameters();
                bound = 0;
            }
            switcher = !switcher;
        }
    }
}
void fuzzy_pid::send_data_order(char *base_array, const char *array, u8 first_index, u8 last_index){
    u8 i;

    for(i = first_index; i < last_index+1; i++){
        base_array[i] = array[i];
    }
}
u8 fuzzy_pid::read_data_order(QByteArray base_array, const char *array, u8 first_index, u8 last_index){
    u8 i;
    u8 condition;

    condition = 1;
    for(i = first_index; i < last_index+1; i++){
        condition = ((condition == 1)&&(base_array[i] == array[i])) ? 1 : 0;
    }
    return condition;
}
u32 fuzzy_pid::crc_chk(u8* data, u8 length) {
    int j;
    u32 reg_crc=0xFFFF;

    while( length-- ) {
        reg_crc^= *data++;
        for (j=0; j<8; j++ ) {
            if( reg_crc & 0x01 ) {
                reg_crc = (reg_crc >> 1)^0xA001;
            }
            else {
                reg_crc = (reg_crc>>1);
            }
        }
    }
    return reg_crc;
}
void fuzzy_pid::read_parameters(void){
    QByteArray data_array;
    data_array.resize(74);
    data_array = pSerial->readAll();

    static u32 missed = 0;
    static u32 unread = 0;
    static u32 read = 0;
    static QTime pace_timer;
    static QTime displacement_timer;
    static u8 opening_stabilization_counter = 10;
    static u8 tmc_autotuning_tmp = 0;
    static float old_load = 0;
    static double old_displacement = 0;
    static bool plot_graphics = false;
    float tmc_pace_rate;
    u32 fcrc;
    u8 crc_low,crc_high;
    float calibrated[4];
    u8 usart_debugger_u8;
    s32 usart_debugger_s32;
    float usart_debugger_float[3];

#ifdef MATLAB_RECORDINGS
    static u32 counter = 0;

    QTextStream log(&dcMotorPc->matlab_log);
#endif

    if(read_data_order(data_array,"ANS",0,2)){
        fcrc = crc_chk((u8*)data_array.data(),72);
        crc_high = (fcrc)%256;
        crc_low = (fcrc)/256;
        if((crc_high == (u8)data_array[72])&&(crc_low == (u8)data_array[73])){
            communication_established = true;
            read++;
            for (u8 i = 0; i < 4; i++){
                char_to_f.s8_val[0] = (u8)data_array[5*i + 3];
                char_to_f.s8_val[1] = (u8)data_array[5*i + 4];
                char_to_f.s8_val[2] = (u8)data_array[5*i + 5];
                char_to_f.s8_val[3] = (u8)data_array[5*i + 6];
                to_gui.gain[i] = (u8)data_array[5*i + 7];
                to_gui.signed_raw[i] = char_to_f.s32_val/(s32)RAW_DATA_DIVIDER;
            }
            step_abs_position = 65536* (u8)data_array[31] + 256* (u8)data_array[32] + (u8)data_array[33];

            for (u8 i = 0; i < 4; i++){
                char_to_f.s8_val[0] = (u8)data_array[4*i + 34];
                char_to_f.s8_val[1] = (u8)data_array[4*i + 35];
                char_to_f.s8_val[2] = (u8)data_array[4*i + 36];
                char_to_f.s8_val[3] = (u8)data_array[4*i + 37];
                calibrated[i] = char_to_f.float_val;
                //qDebug() << "cal : " << calibrated[i];
            }

            char_to_f.u8_val[0] = (u8)data_array[51];
            char_to_f.u8_val[1] = (u8)data_array[52];
            char_to_f.u8_val[2] = (u8)data_array[53];
            char_to_f.u8_val[3] = (u8)data_array[54];
            tmc_pace_rate = char_to_f.float_val;

            usart_debugger_u8 = (u8)data_array[50];

            char_to_f.u8_val[0] = (u8)data_array[55];
            char_to_f.u8_val[1] = (u8)data_array[56];
            char_to_f.u8_val[2] = (u8)data_array[57];
            char_to_f.u8_val[3] = (u8)data_array[58];
            usart_debugger_s32 = char_to_f.s32_val;

            char_to_f.u8_val[0] = (u8)data_array[59];
            char_to_f.u8_val[1] = (u8)data_array[60];
            char_to_f.u8_val[2] = (u8)data_array[61];
            char_to_f.u8_val[3] = (u8)data_array[62];
            usart_debugger_float[0] = char_to_f.float_val;
            char_to_f.u8_val[0] = (u8)data_array[63];
            char_to_f.u8_val[1] = (u8)data_array[64];
            char_to_f.u8_val[2] = (u8)data_array[65];
            char_to_f.u8_val[3] = (u8)data_array[66];
            usart_debugger_float[1] = char_to_f.float_val;
            char_to_f.u8_val[0] = (u8)data_array[67];
            char_to_f.u8_val[1] = (u8)data_array[68];
            char_to_f.u8_val[2] = (u8)data_array[69];
            char_to_f.u8_val[3] = (u8)data_array[70];
            usart_debugger_float[2] = char_to_f.float_val;

//            qDebug() << "step_tmp :" << usart_debugger_u8 << "step_timer :" << usart_debugger_u32 << "average_last_step : " << usart_debugger_float[0]
//                     << "meta_count" << usart_debugger_float[1] << "filtered_pace_rate" << usart_debugger_float[2] ;

//            qDebug() << "0 :" << usart_debugger_u8 << "0 :" << usart_debugger_s32 << "raw : " << usart_debugger_float[0]
//                     << "unfiltered pace" << usart_debugger_float[1] << "filtered pace" << usart_debugger_float[2] ;

            tmc_autotuning_in_operation = (u8)data_array[71];
//            qDebug() << "tmc_autotuning_in_operation" << tmc_autotuning_in_operation;

            switch(tmc_autotuning_tmp){
            case 0:
                if(tmc_autotuning_in_operation == 1){
                    tmc_autotuning_tmp = 1;
                }
                break;
            case 1:
                if(tmc_autotuning_in_operation == 0){
                    tmc_autotuning_tmp = 0;
                    emit cohen_coon_kp(QString::number(usart_debugger_float[0],'f',3));
                    emit cohen_coon_ki(QString::number(usart_debugger_float[1],'f',3));
                    emit cohen_coon_kd(QString::number(usart_debugger_float[2],'f',3));
                    dcMotorPc->step_response_handle();
                }
                break;
            }

            if(opening_stabilization_counter > 0){
                opening_stabilization_counter--;
                return;
            }

            load_value = calibrated[0];

            double unfiltered_load = load_value;
            double unfiltered_displacement = calibrated[1];

            ch3_value           = calibrated[2];
            ch4_value           = calibrated[3];

            load_value = EMA(&unfiltered_load,8);
            displacement_value = EMA_displacement(&unfiltered_displacement,32);

            if(from_gui.test_type == LOAD_CONTROL){
                usart_pace_rate = ((double)1000 * (load_value - old_load)) / (double)pace_timer.elapsed();
                pace_timer.restart();
                old_load = load_value;
                current_pace_rate = bessel_filter(usart_pace_rate);
                current_displacement_rate = 0;
            }
            else{
                usart_displacement_rate = (((double)60000 * (displacement_value - old_displacement)) / (double)displacement_timer.elapsed());
                displacement_timer.restart();
                old_displacement = displacement_value;
                current_displacement_rate = bessel_filter(usart_displacement_rate);
                current_pace_rate = 0;
            }
            to_gui.input_status[0] = (u8)data_array[23] - 0x30;
            to_gui.input_status[1] = (u8)data_array[24] - 0x30;
            to_gui.input_status[2] = (u8)data_array[25] - 0x30;
            to_gui.input_status[3] = (u8)data_array[26] - 0x30;

            if(LS_down_error == 0){
                if(to_gui.input_status[1] == 1){
                    LS_down_error = 1;
                    hard_stop = 1;
                }
            }
            else{
                if(to_gui.input_status[1] == 0){
                    LS_down_error = 0;
                }
            }

            if(LS_up_error == 0){
                if(to_gui.input_status[0] == 1){
                    LS_up_error = 1;
                    hard_stop = 1;
                }
            }
            else{
                if(to_gui.input_status[0] == 0){
                    LS_up_error = 0;
                }
            }

            to_gui.ch_polarity[0] = (u8)data_array[27] - 0x30;
            to_gui.ch_polarity[1] = (u8)data_array[28] - 0x30;
            to_gui.ch_polarity[2] = (u8)data_array[29] - 0x30;
            to_gui.ch_polarity[3] = (u8)data_array[30] - 0x30;

            //current_pace_rate = IIR_Filter(&usart_pace_rate,12);
            //double fir_pace_rate = classic_MA(&usart_pace_rate,12);
            //double wma = WMA(&usart_pace_rate,12);
            //double bessel_value = bessel_filter(usart_pace_rate);
            //qDebug() << "bessel_value" << bessel_value;

#ifdef MATLAB_RECORDINGS
            if(counter == 0){
                log << "counter" << "," << "unfiltered" << "," << "bessel" << "," << "SMA" << "," << "alpha-beta"  << "\n";
            }
            log << counter++ << "," << usart_debugger_s32 << "," << usart_debugger_float[0] << ","
                << usart_debugger_float[1] << "," << usart_debugger_float[2] << "\n";
#endif

            if(hard_stop){
                hard_stop = false;
                breaking_case = 0;
                max_load_value = 0;
                test_finished = true;
            }
            if(from_gui.test_type == CBR){
                if(run_cbr == true){
                    emit cbr_process(load_value,displacement_value);
                }
            }

            if((test_status == TEST_RUNNING)||(test_status == TEST_PAUSED)){
                if(plot_graphics == false){
                    if(load_value >= dcMotorPc->parameters[from_gui.test_type].zero_suppression){
                        plot_graphics = true;
                        dcMotorPc->PLOT_first_in = true;
                        dcMotorPc->load_graphic_timer->start();
                    }
                }
                if(dcMotorPc->step_response_status == false){
                    if(plot_graphics == false){
                        if(load_value >= dcMotorPc->parameters[from_gui.test_type].zero_suppression){
                            run_pid = true;
                            plot_graphics = true;
                            if(from_gui.test_type == CBR){
                                emit zero_suppression_exceeded_cbr();
                            }
                            else if(from_gui.test_type == MARSHALL){
                                emit zero_suppression_exceeded_marshall();
                            }
                            else {
                                dcMotorPc->load_graphic_timer->start();
                            }
                        }
                    }
                    if(load_value > max_load_value){
                        max_load_value = load_value;
                    }
                    peak_load = max_load_value;

                    switch (breaking_case) {
                        case 0:
                            breaking_case++;
                            break;
                        case 1:
                            if(load_value > 0){
                                if(load_value >= dcMotorPc->parameters[from_gui.test_type].load_threshold){ //load threshold
                                    breaking_case++;
                                    test_label_status = true;
                                }
                            }
                            break;
                        case 2:
                            if(load_value <= (max_load_value - (max_load_value * dcMotorPc->parameters[from_gui.test_type].break_percentage)/100)){ //break percentage
                                test_finished = true;
                            }
                            break;
                        default:
                            break;
                    }

                    if(test_status == TEST_RUNNING){
                        if(run_pid){
                            if((from_gui.test_type == CBR) || (from_gui.test_type == MARSHALL)){
                                dcMotorPc->speed_correction(dcMotorPc->ui.doubleSpinBox_test_start_speed->value());
                            }
                            else{
                                dcMotorPc->speed_correction(discrete_PID_dac());
                            }
                        }
                    }
                    else{
                        dcMotorPc->speed_correction(0);
                    }
                }
                else{
                    //step_response();
                }
            }
            else{   // if test_status is STOPPED
                breaking_case = 0;
                max_load_value = 0;
            }

            if(test_finished){
                test_finished = false;
                run_pid = false;
                dcMotorPc->speed_correction(0);
                plot_graphics = false;

                relay_start_stop = RELAY_OFF;
                PID_first_in = true;
                step_stop();

                if((test_status == TEST_RUNNING)||(test_status == TEST_PAUSED)){
                    test_status = TEST_JUST_FINISHED;
                }
                else{
                    test_status = TEST_STOPPED;
                }
                to_gui.test_finished = true;
            }

        }
        else{
            missed++;
        }
    }
    else{
        unread++;
    }
//    qDebug() << read << "read";
//    qDebug() << unread << "unread";
//    qDebug() << missed << "missed";
}
fuzzy_pid::~fuzzy_pid()
{
    delete ui;
}
