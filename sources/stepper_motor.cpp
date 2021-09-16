#include "fuzzy_pid.h"
#include "ui_fuzzy_pid.h"
#include "dc_motor_pc.h"


void fuzzy_pid::step_run_left(void){
    step_motor_in_test = 1;
    step_motor_command = STEPPER_COMMAND_RUN_LEFT;
}
void fuzzy_pid::step_run_right(void){
    step_motor_in_test = 1;
    step_motor_command = STEPPER_COMMAND_RUN_RIGHT;
}
void fuzzy_pid::step_stop(void){
    step_motor_in_test = 0;
    step_motor_command = STEPPER_COMMAND_STOP;
}
void fuzzy_pid::step_pos_reset(void){
    step_motor_command = STEPPER_COMMAND_POS_RESET;
}
void fuzzy_pid::step_go_pos(void){
    step_motor_command = STEPPER_COMMAND_GO_POS;
}
