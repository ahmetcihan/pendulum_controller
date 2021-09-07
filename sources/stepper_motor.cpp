#include "fuzzy_pid.h"
#include "ui_fuzzy_pid.h"
#include "dc_motor_pc.h"


void fuzzy_pid::step_run_left(void){
    step_motor_command = STEPPER_COMMAND_RUN_LEFT;
    dcMotorPc->step_motor_speed = dcMotorPc->ui.spinBox_step_motor_speed->value();
}
void fuzzy_pid::step_run_right(void){
    step_motor_command = STEPPER_COMMAND_RUN_RIGHT;
    dcMotorPc->step_motor_speed = dcMotorPc->ui.spinBox_step_motor_speed->value();
}
void fuzzy_pid::step_stop(void){
    step_motor_command = STEPPER_COMMAND_STOP;
}
void fuzzy_pid::step_pos_reset(void){
    step_motor_command = STEPPER_COMMAND_POS_RESET;
}
void fuzzy_pid::step_go_pos(void){
    step_motor_command = STEPPER_COMMAND_GO_POS;
}
