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
    thread_timer->setInterval(8);
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
    fuzzy_raw_servo_speed = 0;
    send_fuzzy_raw_servo_speed = 0;
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
    data_array.resize(29);
    data_array = pSerial->readAll();

    static u32 missed = 0;
    static u32 unread = 0;
    static u32 read = 0;
    static QTime pace_timer;
    static QTime displacement_timer;
    u32 divide = 32;
    u32 fcrc;
    u8 crc_low,crc_high;
    static u8 opening_stabilization_counter = 10;
    s32 value[5];
    static s32 old_raw[5] = {0};
    static float old_load = 0;
    static double old_displacement = 0;
    static bool plot_graphics = false;
    char raw_sign;

#ifdef MATLAB_RECORDINGS
    static u32 counter = 0;

    QTextStream log(&dcMotorPc->matlab_log);
#endif

    if(read_data_order(data_array,"ANS",0,2)){
        fcrc = crc_chk((u8*)data_array.data(),27);
        crc_high = (fcrc)%256;
        crc_low = (fcrc)/256;
        if((crc_high == (u8)data_array[27])&&(crc_low == (u8)data_array[28])){
            communication_established = true;
            read++;
            for (u8 i = 0; i < 5; i++){
                value[i] = 256*256*(u8)data_array[3+4*i] + 256*(u8)data_array[4+4*i] + (u8)data_array[5+4*i];
                if(i < 4) value[i] = value[i]/divide;
                if(abs(value[i] - old_raw[i]) < 60000){
                    to_gui.signed_raw[i] = value[i];
                    to_gui.gain[i] = ((u8)data_array[6+4*i] & 0x0F);
//                    if(i == 1){
//                        raw_sign = ((((u8)data_array[6+4*i]) & 0x10) == 0x10) ? '-' : '+';
//                    }
//                    else{
//                        raw_sign = ((((u8)data_array[6+4*i]) & 0x10) == 0x10) ? '+' : '-';
//                    }
                    raw_sign = ((((u8)data_array[6+4*i]) & 0x10) == 0x10) ? '+' : '-';

                    if(raw_sign == '-'){
                        to_gui.signed_raw[i] = -1*to_gui.signed_raw[i];
                    }
                }
                else{
                    #ifdef CONFIG_x86
                    qDebug() << QString("raw error channel %1").arg(i);
                    #endif
                }
                old_raw[i] = value[i];
            }

            if(opening_stabilization_counter > 0){
                opening_stabilization_counter--;
                return;
            }

            switch(dcMotorPc->load_calibration_channel){
            case LOAD_1:
                load_value          = evaluate_calibrated_values_ascending(0);  //load
                break;
            case LOAD_2:
                load_value          = evaluate_calibrated_values_ascending(1);  //load
                break;
            case LOAD_3:
                load_value          = evaluate_calibrated_values_ascending(2);  //load
                break;
            }

            double unfiltered_load = load_value;
            double unfiltered_displacement;

            if(from_gui.test_type == MARSHALL){
                unfiltered_displacement  = -evaluate_calibrated_values_ascending(3);  //ch2
            }
            else{
                unfiltered_displacement  = evaluate_calibrated_values_ascending(3);  //ch2
            }
            ch3_value           = evaluate_calibrated_values_ascending(4);  //ch3
            ch4_value           = evaluate_calibrated_values_ascending(5);  //ch4
            encoder             = evaluate_calibrated_values_ascending(6);  //displ

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

            to_gui.ch_polarity[0] = (u8)data_array[23] - 0x30;
            to_gui.ch_polarity[1] = (u8)data_array[24] - 0x30;
            to_gui.ch_polarity[2] = (u8)data_array[25] - 0x30;
            to_gui.ch_polarity[3] = (u8)data_array[26] - 0x30;
            //current_pace_rate = IIR_Filter(&usart_pace_rate,12);
            //double fir_pace_rate = classic_MA(&usart_pace_rate,12);
            //double wma = WMA(&usart_pace_rate,12);
            //double bessel_value = bessel_filter(usart_pace_rate);
            //qDebug() << "bessel_value" << bessel_value;

#ifdef MATLAB_RECORDINGS
            if(counter == 0){
                log << "counter" << "," << "usart_pace_rate" << "," << "current_pace_rate" << ","
                    << "unfiltered_load" << "," << "load_value" << "," << "value[0]" << ","
                    << "encoder" << "," << "value[4]" << ","
                    << "value[1]" << "," << "value[2]" << "," << "value[3]" << "\n";
            }
            log << counter++ << "," << usart_pace_rate << "," << current_pace_rate << ","
                << unfiltered_load << "," << load_value << "," << value[0] << ","
                << encoder << "," << value[4] << ","
                << value[1] << "," << value[2] << "," << value[3] << "\n";
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
                if(dcMotorPc->step_response_status == false){
                    if(plot_graphics == false){
                        if(load_value >= dcMotorPc->parameters[from_gui.test_type].zero_suppression){
                            run_pid = true;
                            plot_graphics = true;
                            if(from_gui.test_type == CBR){
                                emit zero_suppression_exceeded();
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
                            dcMotorPc->speed_correction(discrete_PID_dac());
                        }
                    }
                    else{
                        dcMotorPc->speed_correction(0);
                    }
                }
                else{
                    step_response();
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
                dcMotorPc->servo.stop = 1;

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
