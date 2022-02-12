#include "fuzzy_pid.h"
#include "ui_fuzzy_pid.h"
#include "dc_motor_pc.h"


void fuzzy_pid::step_run_up(void){
    if(LS_up_error == 0){
        step_motor_in_test = 1;
        step_motor_command = STEPPER_COMMAND_RUN_UP;
    }
}
void fuzzy_pid::step_run_down(void){
    if(LS_down_error == 0){
        step_motor_in_test = 1;
        step_motor_command = STEPPER_COMMAND_RUN_DOWN;
    }
}
void fuzzy_pid::step_stop(void){
    step_motor_in_test = 0;
    dcMotorPc->step_motor_speed = 0;
    step_motor_command = STEPPER_COMMAND_STOP;
    TMC_command = TMC_STOP;
}
void fuzzy_pid::step_pos_reset(void){
    step_motor_command = STEPPER_COMMAND_POS_RESET;
}
void fuzzy_pid::step_go_pos(void){
    step_motor_command = STEPPER_COMMAND_GO_POS;
}
u32 DC_Motor_PC::speed_correction(double val){
    step_motor_speed = ((double) ui.doubleSpinBox_motor_rpm->value() * val)/ (double) ui.doubleSpinBox_displ_speed->value();
    return step_motor_speed;
}
void fuzzy_pid::TMC_operation(void){
    TMC_command = TMC_RUN;
}
void fuzzy_pid::TMC_autotuning(void){
    TMC_command = TMC_AUTOTUNING;
}
void fuzzy_pid::TMC_PID(void){
    TMC_command = TMC_PENDULUM_PID;
}
void fuzzy_pid::TMC_HeadUp(void){
    TMC_command = TMC_PENDULUM_HEADUP;
}
void fuzzy_pid::TMC_PID_DOWN(void){
    TMC_command = TMC_PENDULUM_PID_DOWN;
}
void fuzzy_pid::TMC_LQR_DOWN(void){
    TMC_command = TMC_PENDULUM_LQR_DOWN;
}
void fuzzy_pid::TMC_LQR_UP(void){
    TMC_command = TMC_PENDULUM_LQR_UP;
}
