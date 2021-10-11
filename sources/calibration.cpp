#include "dc_motor_pc.h"

void DC_Motor_PC::cal_assignment(int index){
    spinBox_cal_raw_value[index-1]->setValue(current_channel_data);
}
void DC_Motor_PC::cal_channel_change(const QString & text){
    if(text == "LOAD_1"){
        current_cal_channel = 0;
        for(u8 i = 0; i < 8; i++){
            doubleSpinBox_cal_point_value[i]->setDecimals(ui.spinBox_dp_load_1->value());
        }
    }
    if(text == "LOAD_2"){
        current_cal_channel = 1;
        for(u8 i = 0; i < 8; i++){
            doubleSpinBox_cal_point_value[i]->setDecimals(ui.spinBox_dp_load_2->value());
        }
    }
    if(text == "LOAD_3"){
        current_cal_channel = 2;
        for(u8 i = 0; i < 8; i++){
            doubleSpinBox_cal_point_value[i]->setDecimals(ui.spinBox_dp_load_3->value());
        }
    }
    if(text == "DISP"){
        current_cal_channel = 3;
        for(u8 i = 0; i < 8; i++){
            doubleSpinBox_cal_point_value[i]->setDecimals(ui.spinBox_dp_displacement->value());
        }
    }
    if(text == "CH3"){
        current_cal_channel = 4;
        for(u8 i = 0; i < 8; i++){
            doubleSpinBox_cal_point_value[i]->setDecimals(ui.spinBox_dp_ch3->value());
        }
    }
    if(text == "CH4"){
        current_cal_channel = 5;
        for(u8 i = 0; i < 8; i++){
            doubleSpinBox_cal_point_value[i]->setDecimals(ui.spinBox_dp_ch4->value());
        }
    }

    for(u8 i = 0; i < 8; i++){
        spinBox_cal_raw_value[i]->setValue(fuzpid->cal[current_cal_channel].real_val[i]);
        doubleSpinBox_cal_point_value[i]->setValue(fuzpid->cal[current_cal_channel].assigned_val[i]);
    }
    ui.comboBox_cal_point_number->setCurrentIndex(fuzpid->cal[current_cal_channel].point_no - 2);

    switch(current_cal_channel){
    case 0: //load
    case 1: //load
    case 2: //load
        ui.groupBox_cal_value->setTitle(trUtf8("VALUE (%1)").arg(unit_system.load_label));
        break;
    case 3: //displacement
        ui.groupBox_cal_value->setTitle(trUtf8("VALUE (%1)").arg(ui.label_main_displacement_unit->text()));
        break;
    case 4: //ch3
        ui.groupBox_cal_value->setTitle(trUtf8("VALUE (%1)").arg(ui.label_main_ch3_unit->text()));
        break;
    case 5: //ch4
        ui.groupBox_cal_value->setTitle(trUtf8("VALUE (%1)").arg(ui.label_main_ch4_unit->text()));
        break;
    }
}
void DC_Motor_PC::hold_calibration_values(void){
    for(u8 i = 0; i < 8; i++){
        fuzpid->cal[current_cal_channel].real_val[i] = spinBox_cal_raw_value[i]->value();
        fuzpid->cal[current_cal_channel].assigned_val[i] = doubleSpinBox_cal_point_value[i]->value();
    }
    fuzpid->cal[current_cal_channel].point_no = ui.comboBox_cal_point_number->currentIndex() + 2;

#ifdef CONFIG_x86
    qDebug(__FUNCTION__);
#endif
    write_calibration();

    switch(current_cal_channel){
    case 0: //load
    case 1: //load
    case 2: //load
        fuzpid->send_calibration(0);
        break;
    case 3: //displacement
        fuzpid->send_calibration(1);
        break;
    case 4: //ch3
        fuzpid->send_calibration(2);
        break;
    case 5: //ch4
        fuzpid->send_calibration(3);
        break;
    }
}
void DC_Motor_PC::cal_points_signalmapper(void){
    signalMapper_cal = new QSignalMapper(this);
    for(char i = 0; i < 8; i++){
        signalMapper_cal->setMapping(this->pushButton_read_cal_point[i], i+1);
    }
    for(char i = 0; i < 8; i++){
        connect(this->pushButton_read_cal_point[i], SIGNAL(pressed()),signalMapper_cal, SLOT (map()));
    }
    connect(signalMapper_cal, SIGNAL(mapped(int)),this,SLOT(cal_assignment(int)));
}
