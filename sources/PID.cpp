#include "dc_motor_pc.h"

void DC_Motor_PC::pid_combobox_handle(int val){
    ui.doubleSpinBox_kp_pace->setValue(pid[val].Kp);
    ui.doubleSpinBox_ki_pace->setValue(pid[val].Ki);
    ui.doubleSpinBox_kd_pace->setValue(pid[val].Kd);
}
void DC_Motor_PC::save_pid_values(){
    pid[ui.comboBox_pid->currentIndex()].Kp = ui.doubleSpinBox_kp_pace->value();
    pid[ui.comboBox_pid->currentIndex()].Ki = ui.doubleSpinBox_ki_pace->value();
    pid[ui.comboBox_pid->currentIndex()].Kd = ui.doubleSpinBox_kd_pace->value();
}
double fuzzy_pid::discrete_PID_dac(){
    static double last_error[3] = {0};
    static double output = 0;
    static u8 first_move = 0;

    float error = 0;
    double a,b,c;
    double Ts = pid_delta_t_dac.elapsed();
    static double kp,ki,kd;
    //static double sse = 0;

    if(PID_first_in){
        PID_first_in = false;
        first_move = 7;
        //output = dcMotorPc->dac_voltage_to_raw((double)0.1 * dcMotorPc->ui.spinBox_start_speed_percentage->value());
        output = dcMotorPc->ui.doubleSpinBox_test_start_speed->value();
        last_error[0] = 0;
        last_error[1] = 0;
        last_error[2] = 0;
        //sse = 0;
        kp = dcMotorPc->pid[from_gui.test_type].Kp;
        ki = dcMotorPc->pid[from_gui.test_type].Ki;
        kd = dcMotorPc->pid[from_gui.test_type].Kd;
    }
    else{
        //qDebug() << "kp-ki-kd" << kp << "-" << ki << "-" << kd;
        if(from_gui.test_type == LOAD_CONTROL){
            error = dcMotorPc->ui.doubleSpinBox_pace_rate->value() - current_pace_rate;
        }
        else{
            error = dcMotorPc->ui.doubleSpinBox_displacement_rate->value() - current_displacement_rate;
        }

        last_error[2] = last_error[1];
        last_error[1] = last_error[0];
        last_error[0] = error;

//        sse +=(error*error);
//        qDebug() << "sse : " << sse;

        a = kp + kd/Ts; //bu daha iyi, birkaç kaynakta da bu yazıyor zaten
        b = -kp + (ki*Ts) - (2*kd)/Ts;
        c = kd/Ts;

//        a = kp + (ki*Ts / 2.0) + kd/Ts; //bu daha iyi, birkaç kaynakta da bu yazıyor zaten
//        b = -kp + (ki*Ts / 2.0) - (2.0*kd)/Ts;
//        c = kd/Ts;

        output = output + (a*last_error[0] + b*last_error[1] + c*last_error[2]);
        //qDebug() << "raw_output: " << output;
        if(output > 900) output = 900; // TODO: change this with a max speed limit

#if 1
        if(output < 0) output = 0;
#else   // over sensitive systems
        if(output > 0){
            dcMotorPc->servo.down = 1;
        }
        else{
            dcMotorPc->servo.up = 1;
        }
#endif
    }
    pid_delta_t_dac.restart();
    if(first_move > 0){
        first_move--;
        output = 0.5;  //wait until pace_rate IIRfilter get filled
    }
    //qDebug() << output;
    return qAbs(output);  //in fact, this is a mm/min value
}
