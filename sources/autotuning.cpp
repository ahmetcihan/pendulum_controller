#include "dc_motor_pc.h"

void DC_Motor_PC::step_response_handle(void){
    if(prevent_double_click()) return;
    if(step_response_status == false){
        step_response_status = true;
        reset_plot_pace();
        ui.pushButton_step_response->setText(trUtf8("STOP AUTOTUNING"));
        //fuzpid->test_status = TEST_RUNNING;
        pace_graphic_timer->start();
        fuzpid->TMC_command = TMC_AUTOTUNING;
    }
    else{
        step_response_status = false;
        speed_correction(0);
        pace_graphic_timer->stop();
        fuzpid->step_stop();
        fuzpid->test_status = TEST_STOPPED;
        ui.pushButton_step_response->setText(trUtf8("START AUTOTUNING"));
        fuzpid->relay_start_stop = RELAY_OFF;
        fuzpid->TMC_command = TMC_STOP;
    }
}
