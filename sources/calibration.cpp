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
    if(text == "ENC"){
        current_cal_channel = 6;
        for(u8 i = 0; i < 8; i++){
            doubleSpinBox_cal_point_value[i]->setDecimals(3);
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
    case 6: //enc
        ui.groupBox_cal_value->setTitle(trUtf8("VALUE (%1)").arg(unit_system.length_label));
        break;
    }
}
void DC_Motor_PC::send_calibration_values(void){
    for(u8 i = 0; i < 8; i++){
        fuzpid->cal[current_cal_channel].real_val[i] = spinBox_cal_raw_value[i]->value();
        fuzpid->cal[current_cal_channel].assigned_val[i] = doubleSpinBox_cal_point_value[i]->value();
    }
    fuzpid->cal[current_cal_channel].point_no = ui.comboBox_cal_point_number->currentIndex() + 2;

#ifdef CONFIG_x86
    qDebug(__FUNCTION__);
#endif
    write_calibration();
    fuzpid->slope_calculation(current_cal_channel);
}
void fuzzy_pid::slope_calculation(u8 no){
    u8 validation_1 = 0xFF;
    u8 validation_2 = 0xFF;

    if(cal[no].point_no > 8) cal[no].point_no = 2;
    for(u8 i = 0; i < (cal[no].point_no - 1); i++){
        if(cal[no].real_val[i] < cal[no].real_val[i+1]){
            validation_1 = validation_1 & 0xFF;
        }
        else{
            validation_1 = validation_1 & 0x00;
        }
    }
    if(validation_1 == 0xFF){
        qDebug() << "calibration is ascending";
        cal[no].type = ASCENDING;
    }
    else{
        for(u8 i = 0; i < (cal[no].point_no - 1); i++){
            if(cal[no].real_val[i] > cal[no].real_val[i+1]){
                validation_2 = validation_2 & 0xFF;
            }
            else{
                validation_2 = validation_2 & 0x00;
            }
        }
        if(validation_2 == 0xFF){
            qDebug() << "calibration is descending";
            cal[no].type = DESCENDING;
        }
        else{
            qDebug() << "calibration is faulty";
            cal[no].type = ASCENDING;
        }
    }

    for(u8 i= 0; i < (cal[no].point_no - 1); i++){
        cal[no].slope[i] = ((1.0*(double)(cal[no].assigned_val[i+1]-cal[no].assigned_val[i]))/
                (1.0*(double)(cal[no].real_val[i+1]-cal[no].real_val[i])));
        //if(no == 0) qDebug() << QString("slope_%1").arg(i) << cal[no].slope[i];
    }
    cal[no].tare_val = 0;

}
double fuzzy_pid::evaluate_calibrated_values_ascending(u8 no){
    double value = 0;
    double aux = 0;
    s32 tared = 0;

    switch (no) {
    case 0:
    case 1:
    case 2:
        tared = to_gui.signed_raw[0];
        break;
    case 3:
        tared = to_gui.signed_raw[1];
        break;
    case 4:
        tared = to_gui.signed_raw[2];
        break;
    case 5:
        tared = to_gui.signed_raw[3];
        break;
    case 6:
        tared = to_gui.signed_raw[4];
        break;
    }

    switch (cal[no].point_no) {
    case 8:
        if (tared <= cal[no].real_val[1]){
            aux  = (tared - cal[no].real_val[0]);
            value  = cal[no].slope[0]*(aux) + cal[no].assigned_val[0];
        }
        else if ((tared <= cal[no].real_val[2])){
            aux  = (tared - cal[no].real_val[1]);
            value  = cal[no].slope[1]*(aux) + cal[no].assigned_val[1];
        }
        else if ((tared <= cal[no].real_val[3])){
            aux  = (tared - cal[no].real_val[2]);
            value  = cal[no].slope[2]*(aux) + cal[no].assigned_val[2];
        }
        else if ((tared <= cal[no].real_val[4])){
            aux  = (tared - cal[no].real_val[3]);
            value  = cal[no].slope[3]*(aux) + cal[no].assigned_val[3];
        }
        else if ((tared <= cal[no].real_val[5])){
            aux  = (tared - cal[no].real_val[4]);
            value  = cal[no].slope[4]*(aux) + cal[no].assigned_val[4];
        }
        else if ((tared <= cal[no].real_val[6])){
            aux  = (tared - cal[no].real_val[5]);
            value  = cal[no].slope[5]*(aux) + cal[no].assigned_val[5];
        }
        else if ((tared > cal[no].real_val[6])){
            aux  = (tared - cal[no].real_val[6]);
            value  = cal[no].slope[6]*(aux) + cal[no].assigned_val[6];
        }
        break;
    case 7:
        if (tared <= cal[no].real_val[1]){
            aux  = (tared - cal[no].real_val[0]);
            value  = cal[no].slope[0]*(aux) + cal[no].assigned_val[0];
        }
        else if ((tared <= cal[no].real_val[2])){
            aux  = (tared - cal[no].real_val[1]);
            value  = cal[no].slope[1]*(aux) + cal[no].assigned_val[1];
        }
        else if ((tared <= cal[no].real_val[3])){
            aux  = (tared - cal[no].real_val[2]);
            value  = cal[no].slope[2]*(aux) + cal[no].assigned_val[2];
        }
        else if ((tared <= cal[no].real_val[4])){
            aux  = (tared - cal[no].real_val[3]);
            value  = cal[no].slope[3]*(aux) + cal[no].assigned_val[3];
        }
        else if ((tared <= cal[no].real_val[5])){
            aux  = (tared - cal[no].real_val[4]);
            value  = cal[no].slope[4]*(aux) + cal[no].assigned_val[4];
        }
        else if (tared > cal[no].real_val[5]){
            aux  = (tared - cal[no].real_val[5]);
            value  = cal[no].slope[5]*(aux) + cal[no].assigned_val[5];
        }
        break;
    case 6:
        if (tared <= cal[no].real_val[1]){
            aux  = (tared - cal[no].real_val[0]);
            value  = cal[no].slope[0]*(aux) + cal[no].assigned_val[0];
        }
        else if ((tared <= cal[no].real_val[2])){
            aux  = (tared - cal[no].real_val[1]);
            value  = cal[no].slope[1]*(aux) + cal[no].assigned_val[1];
        }
        else if ((tared <= cal[no].real_val[3])){
            aux  = (tared - cal[no].real_val[2]);
            value  = cal[no].slope[2]*(aux) + cal[no].assigned_val[2];
        }
        else if ((tared <= cal[no].real_val[4])){
            aux  = (tared - cal[no].real_val[3]);
            value  = cal[no].slope[3]*(aux) + cal[no].assigned_val[3];
        }
        else if (tared > cal[no].real_val[4]){
            aux  = (tared - cal[no].real_val[4]);
            value  = cal[no].slope[4]*(aux) + cal[no].assigned_val[4];
        }
        break;
    case 5:
        if (tared <= cal[no].real_val[1]){
            aux  = (tared - cal[no].real_val[0]);
            value  = cal[no].slope[0]*(aux) + cal[no].assigned_val[0];
        }
        else if ((tared <= cal[no].real_val[2])){
            aux  = (tared - cal[no].real_val[1]);
            value  = cal[no].slope[1]*(aux) + cal[no].assigned_val[1];
        }
        else if ((tared <= cal[no].real_val[3])){
            aux  = (tared - cal[no].real_val[2]);
            value  = cal[no].slope[2]*(aux) + cal[no].assigned_val[2];
        }
        else if (tared > cal[no].real_val[3]){
            aux  = (tared - cal[no].real_val[3]);
            value  = cal[no].slope[3]*(aux) + cal[no].assigned_val[3];
        }
        break;
    case 4:
        if (tared <= cal[no].real_val[1]){
            aux  = (tared - cal[no].real_val[0]);
            value  = cal[no].slope[0]*(aux) + cal[no].assigned_val[0];
        }
        else if ((tared <= cal[no].real_val[2])){
            aux  = (tared - cal[no].real_val[1]);
            value  = cal[no].slope[1]*(aux) + cal[no].assigned_val[1];
        }
        else if (tared > cal[no].real_val[2]){
            aux  = (tared - cal[no].real_val[2]);
            value  = cal[no].slope[2]*(aux) + cal[no].assigned_val[2];
        }
        break;
    case 3:
        if (tared <= cal[no].real_val[1]){
            aux  = (tared - cal[no].real_val[0]);
            value  = cal[no].slope[0]*(aux) + cal[no].assigned_val[0];
        }
        else if (tared > cal[no].real_val[1]){
            aux  = (tared - cal[no].real_val[1]);
            value  = cal[no].slope[1]*(aux) + cal[no].assigned_val[1];
        }
        break;
    case 2:
        aux  = (tared - cal[no].real_val[0]);
        value  = cal[no].slope[0]*(aux) + cal[no].assigned_val[0];
        break;
    }

    cal[no].absolute_calibrated = value;
    return (cal[no].absolute_calibrated - cal[no].tare_val);
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
