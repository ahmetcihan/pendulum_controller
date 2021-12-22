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
