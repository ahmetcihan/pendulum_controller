#include "dc_motor_pc.h"

void DC_Motor_PC::ch3_name_handler(){
    ui.groupBox_main_ch3->setTitle(ui.lineEdit_ch3_name->text());
}
void DC_Motor_PC::ch4_name_handler(){
    ui.groupBox_main_ch4->setTitle(ui.lineEdit_ch4_name->text());
}
void DC_Motor_PC::ch3_unit_handler(){
    if(ui.radioButton_ch3_kn->isChecked()){
        ch3.unit = 1;
        ui.label_main_ch3_unit->setText(ui.radioButton_ch3_kn->text());
    }
    else if(ui.radioButton_ch3_mm->isChecked()){
        ch3.unit = 2;
        ui.label_main_ch3_unit->setText(ui.radioButton_ch3_mm->text());
    }
    else if(ui.radioButton_ch3_unitless->isChecked()){
        ch3.unit = 3;
        ui.label_main_ch3_unit->setText(QStringLiteral("  "));
    }
    data_changed = true;
}
void DC_Motor_PC::ch4_unit_handler(){
    if(ui.radioButton_ch4_kn->isChecked()){
        ch4.unit = 1;
        ui.label_main_ch4_unit->setText(ui.radioButton_ch4_kn->text());
    }
    else if(ui.radioButton_ch4_mm->isChecked()){
        ch4.unit = 2;
        ui.label_main_ch4_unit->setText(ui.radioButton_ch4_mm->text());
    }
    else if(ui.radioButton_ch4_unitless->isChecked()){
        ch4.unit = 3;
        ui.label_main_ch4_unit->setText(QStringLiteral("  "));
    }
    data_changed = true;
}
void DC_Motor_PC::dp_load_change_handler(){
    u8 load_dp;

    switch (load_calibration_channel) {
    case LOAD_1:
        load_dp = ui.spinBox_dp_load_1->value();
        break;
    case LOAD_2:
        load_dp = ui.spinBox_dp_load_2->value();
        break;
    case LOAD_3:
        load_dp = ui.spinBox_dp_load_3->value();
        break;
    default:
        break;
    }

    ui.doubleSpinBox_max_load->setDecimals(load_dp);
    ui.doubleSpinBox_load_threshold->setDecimals(load_dp);
    ui.doubleSpinBox_zero_suppression->setDecimals(load_dp);
    ui.doubleSpinBox_protect_specimen->setDecimals(load_dp);

    ui.comboBox_channel_no->setCurrentIndex(0);

}
void DC_Motor_PC::dp_stress_change_handler(void){
    dp.stress = ui.spinBox_dp_stress->value();
}
void DC_Motor_PC::dp_ch3_change_handler(void){
    ch3.dp = ui.spinBox_dp_ch3->value();
}
void DC_Motor_PC::dp_ch4_change_handler(void){
    ch4.dp = ui.spinBox_dp_ch4->value();
}
void DC_Motor_PC::dp_displacement_change_handler(void){
    dp.displacement = ui.spinBox_dp_displacement->value();
    ui.doubleSpinBox_max_displacement->setDecimals(ui.spinBox_dp_displacement->value());

}
void DC_Motor_PC::channel_activations_handler(){
    if(ui.checkBox_ch3->isChecked() == TRUE){
        ui.groupBox_main_ch3->show();
        if(ui.checkBox_ch4->isChecked() == TRUE){   //all ok
            ui.groupBox_main_ch4->show();
            ui.groupBox_main_ch3->move(SCREEN_POS_2);
            ui.groupBox_main_ch4->move(SCREEN_POS_3);
            ui.groupBox_main_par_0->move(SCREEN_POS_4);
        }
        else{                                       // ch4
            ui.groupBox_main_ch4->hide();
            ui.groupBox_main_ch3->move(SCREEN_POS_2);
            ui.groupBox_main_par_0->move(SCREEN_POS_3);
        }
    }
    else {
        ui.groupBox_main_ch3->hide();
        if(ui.checkBox_ch4->isChecked() == TRUE){   //ch3
            ui.groupBox_main_ch4->show();
            ui.groupBox_main_ch4->move(SCREEN_POS_2);
            ui.groupBox_main_par_0->move(SCREEN_POS_3);
        }
        else{                                       //ch3 -ch4
            ui.groupBox_main_ch4->hide();
            ui.groupBox_main_par_0->move(SCREEN_POS_2);
        }
    }
}
void DC_Motor_PC::load_cal_channel_handler(void){
    if(ui.radioButton_load_1->isChecked()){
        load_calibration_channel = LOAD_1;
    }
    else if(ui.radioButton_load_2->isChecked()){
        load_calibration_channel = LOAD_2;
    }
    else if(ui.radioButton_load_3->isChecked()){
        load_calibration_channel = LOAD_3;
    }

}
