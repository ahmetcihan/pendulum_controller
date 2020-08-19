#include "dc_motor_pc.h"

void DC_Motor_PC::handle_JOG_down_button(){
    do_not_change_focus = true;
    servo.up = 1;
    JOG.up = true;
    if(fuzpid->error.go_home_load){
        fuzpid->error.go_home_load = false;
        ui.label_alert_status->setText(trUtf8("NO ALERT"));
    }

#ifdef CONFIG_x86
    qDebug(__FUNCTION__);
#endif
}
void DC_Motor_PC::release_JOG_down_button(){
    do_not_change_focus = false;
    QTimer::singleShot(100,this,SLOT(keyboard_delayed_operation()));

    servo.stop = 1;
    JOG.up = false;
#ifdef CONFIG_x86
    qDebug(__FUNCTION__);
#endif
}
void DC_Motor_PC::handle_JOG_up_button(){
    do_not_change_focus = true;
    servo.down = 1;
    JOG.down = true;
    if(fuzpid->error.go_home_load){
        fuzpid->error.go_home_load = false;
        ui.label_alert_status->setText(trUtf8("NO ALERT"));
    }
#ifdef CONFIG_x86
    qDebug(__FUNCTION__);
#endif
}
void DC_Motor_PC::release_JOG_up_button(){
    do_not_change_focus = false;
    QTimer::singleShot(100,this,SLOT(keyboard_delayed_operation()));
    servo.stop = 1;
    JOG.down = false;
#ifdef CONFIG_x86
    qDebug(__FUNCTION__);
#endif
}
void DC_Motor_PC::JOG_operation(){
    static u8 up_tmp = 0;
    static u8 down_tmp = 0;
    static u32 _100_msec_counter_up = 0;
    static u32 _100_msec_counter_down = 0;


    if(JOG.up){
        switch(up_tmp){
        case 0:
            speed_correction(0);
            up_tmp++;
            break;
        case 1:
            servo.start = 1;
            up_tmp++;
            break;
        case 2:
            _100_msec_counter_up++;    //100msec
            double spd = 0;
            spd = ((ui.doubleSpinBox_upward_max_speed->value()) * 1.0 * _100_msec_counter_up) / (ui.spinBox_upward_ramp_time->value() * 10.0);
            if(spd > ui.doubleSpinBox_upward_max_speed->value()){
                spd = ui.doubleSpinBox_upward_max_speed->value();
            }
            JOG.speed = spd;
            if(ui.radioButton_protect_specimen_on->isChecked()){
                if(qAbs(fuzpid->load_value) >= (ui.doubleSpinBox_protect_specimen->value())){
                    JOG.error = true;
                    servo.stop = 1;
                    up_tmp = 0;
                    _100_msec_counter_up = 0;
                    speed_correction(0);
                    QMessageBox msgBox;
                    msgBox.setWindowTitle(trUtf8("SPECIMEN PROTECTION"));
                    msgBox.setText(trUtf8("<font size = 10>Please select the checkbox OFF and tune the load"));
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    msgBox.exec();
                }
                else{
                    speed_correction(JOG.speed);
                }
            }
            else{
                speed_correction(JOG.speed);
            }
            break;
        }
    }
    else{
        up_tmp = 0;
        _100_msec_counter_up = 0;
    }

    if(JOG.down){
        switch(down_tmp){
        case 0:
            speed_correction(0);
            down_tmp++;
            break;
        case 1:
            servo.start = 1;
            down_tmp++;
            break;
        case 2:
            _100_msec_counter_down++;    //100msec
            double spd = 0;
            spd = ((ui.doubleSpinBox_downward_max_speed->value()) * 1.0 * _100_msec_counter_down) / (ui.spinBox_downward_ramp_time->value() * 10.0);
            if(spd > ui.doubleSpinBox_downward_max_speed->value()){
                spd = ui.doubleSpinBox_downward_max_speed->value();
            }
            JOG.speed = spd;
            if(ui.radioButton_protect_specimen_on->isChecked()){
                if(qAbs(fuzpid->load_value) >= (ui.doubleSpinBox_protect_specimen->value())){
                    JOG.error = true;
                    servo.stop = 1;
                    down_tmp = 0;
                    _100_msec_counter_down = 0;
                    speed_correction(0);
                    QMessageBox msgBox;
                    msgBox.setWindowTitle(trUtf8("SPECIMEN PROTECTION"));
                    msgBox.setText(trUtf8("<font size = 10>Please select the checkbox OFF and tune the load"));
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    msgBox.exec();
                }
                else{
                    speed_correction(JOG.speed);
                }
            }
            else{
                speed_correction(JOG.speed);
            }
            break;
        }
    }
    else{
        down_tmp = 0;
        _100_msec_counter_down = 0;
    }
}
void DC_Motor_PC::save_as_LOW(){
    if(prevent_double_click()) return;

    JOG.low.speed_up = ui.doubleSpinBox_upward_max_speed->value();
    JOG.low.speed_down = ui.doubleSpinBox_downward_max_speed->value();
    JOG.low.ramp_up = ui.spinBox_upward_ramp_time->value();
    JOG.low.ramp_down = ui.spinBox_downward_ramp_time->value();
}
void DC_Motor_PC::save_as_MID(){
    if(prevent_double_click()) return;

    JOG.mid.speed_up = ui.doubleSpinBox_upward_max_speed->value();
    JOG.mid.speed_down = ui.doubleSpinBox_downward_max_speed->value();
    JOG.mid.ramp_up = ui.spinBox_upward_ramp_time->value();
    JOG.mid.ramp_down = ui.spinBox_downward_ramp_time->value();
}
void DC_Motor_PC::save_as_HIGH(){
    if(prevent_double_click()) return;

    JOG.high.speed_up = ui.doubleSpinBox_upward_max_speed->value();
    JOG.high.speed_down = ui.doubleSpinBox_downward_max_speed->value();
    JOG.high.ramp_up = ui.spinBox_upward_ramp_time->value();
    JOG.high.ramp_down = ui.spinBox_downward_ramp_time->value();
}
void DC_Motor_PC::set_LOW(){
    if(prevent_double_click()) return;

    ui.doubleSpinBox_upward_max_speed->setValue(JOG.low.speed_up);
    ui.doubleSpinBox_downward_max_speed->setValue(JOG.low.speed_down);
    ui.spinBox_upward_ramp_time->setValue(JOG.low.ramp_up);
    ui.spinBox_downward_ramp_time->setValue(JOG.low.ramp_down);
}
void DC_Motor_PC::set_MID(){
    if(prevent_double_click()) return;

    ui.doubleSpinBox_upward_max_speed->setValue(JOG.mid.speed_up);
    ui.doubleSpinBox_downward_max_speed->setValue(JOG.mid.speed_down);
    ui.spinBox_upward_ramp_time->setValue(JOG.mid.ramp_up);
    ui.spinBox_downward_ramp_time->setValue(JOG.mid.ramp_down);
}
void DC_Motor_PC::set_HIGH(){
    if(prevent_double_click()) return;

    ui.doubleSpinBox_upward_max_speed->setValue(JOG.high.speed_up);
    ui.doubleSpinBox_downward_max_speed->setValue(JOG.high.speed_down);
    ui.spinBox_upward_ramp_time->setValue(JOG.high.ramp_up);
    ui.spinBox_downward_ramp_time->setValue(JOG.high.ramp_down);
}
