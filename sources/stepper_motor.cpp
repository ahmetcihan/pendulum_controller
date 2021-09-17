#include "fuzzy_pid.h"
#include "ui_fuzzy_pid.h"
#include "dc_motor_pc.h"


void fuzzy_pid::step_run_up(void){
    step_motor_in_test = 1;
    step_motor_command = STEPPER_COMMAND_RUN_UP;
}
void fuzzy_pid::step_run_down(void){
    step_motor_in_test = 1;
    step_motor_command = STEPPER_COMMAND_RUN_DOWN;
}
void fuzzy_pid::step_stop(void){
    step_motor_in_test = 0;
    dcMotorPc->step_motor_speed = 0;
    step_motor_command = STEPPER_COMMAND_STOP;
}
void fuzzy_pid::step_pos_reset(void){
    step_motor_command = STEPPER_COMMAND_POS_RESET;
}
void fuzzy_pid::step_go_pos(void){
    step_motor_command = STEPPER_COMMAND_GO_POS;
}
