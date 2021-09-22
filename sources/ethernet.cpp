#include "dc_motor_pc.h"

void DC_Motor_PC::eth0_address_set(){
    static u8 tmp = 0;

    switch(tmp){
    case 0:
        QProcess::execute("mkdir /var/run/");
        QTimer::singleShot(500,this,SLOT(eth0_address_set()));
        tmp++;
        break;
    case 1:
        QProcess::execute("/etc/init.d/networking restart");
        break;
    }
}
void DC_Motor_PC::save_ip(void){
    if(prevent_double_click()) return;

    static u8 tmp = 0;
    static QLabel wait_label;
    static QFile networking_file;
    QTextStream lines(&networking_file);


    switch(tmp){
    case 0:
        wait_label.setGeometry(260,278,300,80);
        wait_label.setText(QString(trUtf8("Please wait..")));
        wait_label.show();

        networking_file.setFileName(QString("/etc/network/interfaces"));
        networking_file.open(QIODevice::WriteOnly);

        lines << "auto lo\n";
        lines << "iface lo inet loopback\n";
        lines << "auto eth0\n";
        lines << "iface eth0 inet static\n";
        lines << "address " << ui.lineEdit_ip_address->text() << "\n";
        lines << "netmask " << ui.lineEdit_ip_mask->text() << "\n";
        lines << "gateway " << ui.lineEdit_ip_gw->text() << "\n";
        networking_file.close();

        QTimer::singleShot(500,this,SLOT(save_ip()));
        tmp++;
        break;
    case 1:
        QProcess::execute("mkdir /var/run/");
        QTimer::singleShot(500,this,SLOT(save_ip()));
        tmp++;
        break;
    case 2:
        QProcess::execute("/etc/init.d/networking restart");
        QTimer::singleShot(1000,this,SLOT(save_ip()));
        tmp++;
        break;
    case 3:
        wait_label.hide();
        tmp = 0;
        break;

    }
}
QString DC_Motor_PC::readSettingEthernet(const QString &setting){

    if (setting == "param.pace_rate")          return QString::number(ui.doubleSpinBox_pace_rate->value()); //bu load control için
    else if (setting == "param.displacement_rate")  return QString::number(ui.doubleSpinBox_displacement_rate->value());
    else if (setting == "param.test_start_speed")   return QString::number(ui.doubleSpinBox_test_start_speed->value());
    else if (setting == "param.return_home_speed")  return QString::number(ui.doubleSpinBox_return_home_speed->value());
    else if (setting == "param.go_to_load_speed")   return QString::number(ui.doubleSpinBox_go_to_load_speed->value());
    else if (setting == "param.max_load")           return QString::number(ui.doubleSpinBox_max_load->value());
    else if (setting == "param.max_displacement")   return QString::number(ui.doubleSpinBox_max_displacement->value());
    else if (setting == "param.failure_threshold")  return QString::number(ui.doubleSpinBox_load_threshold->value());
    else if (setting == "param.zero_suppression")   return QString::number(ui.doubleSpinBox_zero_suppression->value());
    else if (setting == "param.break_percentage")   return QString::number(ui.spinBox_break_percentage->value());
    else if (setting == "param.dp.load_1")            return QString::number(ui.spinBox_dp_load_1->value());
    else if (setting == "param.dp.load_2")            return QString::number(ui.spinBox_dp_load_2->value());
    else if (setting == "param.dp.load_3")            return QString::number(ui.spinBox_dp_load_3->value());
    else if (setting == "param.dp.stress")          return QString::number(ui.spinBox_dp_stress->value());
    else if (setting == "param.dp.displacement")    return QString::number(ui.spinBox_dp_displacement->value());
    else if (setting == "param.dp.ch3")    return QString::number(ui.spinBox_dp_ch3->value());
    else if (setting == "param.dp.ch4")    return QString::number(ui.spinBox_dp_ch4->value());
    else if (setting == "param.scb_notch")    return QString::number(ui.doubleSpinBox_specimen_scb_notch->value());
    else if (setting == "param.scb_thickness")    return QString::number(ui.doubleSpinBox_specimen_scb_thickness->value());
    else if (setting == "param.test_type") return (test_type_name);
    else if (setting == "param.unit_system") return (QString::number(unit_system.index));
    else if (setting == "param.precalculated_area")   return QString::number(ui.doubleSpinBox_precalculated_area->value());
    else if (setting == "param.precalculated_area_status")   return QString::number(ui.checkBox_precalculated_area->isChecked());

    else if (setting == "cbr.standard.cbr")return QString::number(cbr.standard);
    else if (setting == "cbr.latest_point")return QString::number(ui.spinBox_test_latest_point->value());
    else if (setting == "cbr.area")return QString::number(ui.doubleSpinBox_area_cbr->value());
    else if (setting == "cbr.points.1")return QString::number(cbr_point_value[0]->value());
    else if (setting == "cbr.points.2")return QString::number(cbr_point_value[1]->value());
    else if (setting == "cbr.points.3")return QString::number(cbr_point_value[2]->value());
    else if (setting == "cbr.points.4")return QString::number(cbr_point_value[3]->value());
    else if (setting == "cbr.points.5")return QString::number(cbr_point_value[4]->value());
    else if (setting == "cbr.points.6")return QString::number(cbr_point_value[5]->value());
    else if (setting == "cbr.points.7")return QString::number(cbr_point_value[6]->value());
    else if (setting == "cbr.points.8")return QString::number(cbr_point_value[7]->value());
    else if (setting == "cbr.points.9")return QString::number(cbr_point_value[8]->value());
    else if (setting == "cbr.points.10")return QString::number(cbr_point_value[9]->value());
    else if (setting == "cbr.points.11")return QString::number(cbr_point_value[10]->value());
    else if (setting == "cbr.points.12")return QString::number(cbr_point_value[11]->value());
    else if (setting == "cbr.points.13")return QString::number(cbr_point_value[12]->value());
    else if (setting == "cbr.points.14")return QString::number(cbr_point_value[13]->value());
    else if (setting == "cbr.points.15")return QString::number(cbr_point_value[14]->value());
    else if (setting == "cbr.points.16")return QString::number(cbr_point_value[15]->value());
    else if (setting == "cbr.points.17")return QString::number(cbr_point_value[16]->value());
    else if (setting == "cbr.points.18")return QString::number(cbr_point_value[17]->value());
    else if (setting == "cbr.points.19")return QString::number(cbr_point_value[18]->value());
    else if (setting == "cbr.points.20")return QString::number(cbr_point_value[29]->value());
    else if (setting == "cbr.points.21")return QString::number(cbr_point_value[20]->value());
    else if (setting == "cbr.points.22")return QString::number(cbr_point_value[21]->value());
    else if (setting == "cbr.points.23")return QString::number(cbr_point_value[22]->value());
    else if (setting == "cbr.points.24")return QString::number(cbr_point_value[23]->value());
    else if (setting == "cbr.points.25")return QString::number(cbr_point_value[24]->value());
    else if (setting == "cbr.points.26")return QString::number(cbr_point_value[25]->value());
    else if (setting == "cbr.points.27")return QString::number(cbr_point_value[26]->value());
    else if (setting == "cbr.points.28")return QString::number(cbr_point_value[27]->value());
    else if (setting == "cbr.points.29")return QString::number(cbr_point_value[28]->value());
    else if (setting == "cbr.points.30")return QString::number(cbr_point_value[29]->value());
    else if (setting == "cbr.points.31")return QString::number(cbr_point_value[30]->value());
    else if (setting == "cbr.points.32")return QString::number(cbr_point_value[31]->value());
    else if (setting == "cbr.points.33")return QString::number(cbr_point_value[32]->value());
    else if (setting == "cbr.points.34")return QString::number(cbr_point_value[33]->value());
    else if (setting == "cbr.points.35")return QString::number(cbr_point_value[34]->value());
    else if (setting == "cbr.points.36")return QString::number(cbr_point_value[35]->value());
    else if (setting == "cbr.points.37")return QString::number(cbr_point_value[36]->value());
    else if (setting == "cbr.points.38")return QString::number(cbr_point_value[37]->value());
    else if (setting == "cbr.points.39")return QString::number(cbr_point_value[38]->value());
    else if (setting == "cbr.points.40")return QString::number(cbr_point_value[39]->value());

    else if (setting == "specimen.cyl.d")   return QString::number(ui.doubleSpinBox_specimen_cyl_d->value());
    else if (setting == "specimen.cyl.l")   return QString::number(ui.doubleSpinBox_specimen_cyl_l->value());

    else if (setting == "marshall.thickness_correction")return QString::number(marshall.thickness_correction);
    else if (setting == "marshall.d")return QString::number(ui.comboBox_diameter->currentIndex());
    else if (setting == "marshall.t")return QString::number(ui.doubleSpinBox_briquette_t->value());

    else if (setting == "marshall.standard")return QString::number(marshall.standard);

    else if (setting == "cal.raw_1")        return QString::number(ui.spinBox_cal_raw_value_1->value());
    else if (setting == "cal.raw_2")        return QString::number(ui.spinBox_cal_raw_value_2->value());
    else if (setting == "cal.raw_3")        return QString::number(ui.spinBox_cal_raw_value_3->value());
    else if (setting == "cal.raw_4")        return QString::number(ui.spinBox_cal_raw_value_4->value());
    else if (setting == "cal.raw_5")        return QString::number(ui.spinBox_cal_raw_value_5->value());
    else if (setting == "cal.raw_6")        return QString::number(ui.spinBox_cal_raw_value_6->value());
    else if (setting == "cal.raw_7")        return QString::number(ui.spinBox_cal_raw_value_7->value());
    else if (setting == "cal.raw_8")        return QString::number(ui.spinBox_cal_raw_value_8->value());
    else if (setting == "cal.value_1")         return QString::number(ui.doubleSpinBox_cal_point_value_1->value());
    else if (setting == "cal.value_2")         return QString::number(ui.doubleSpinBox_cal_point_value_2->value());
    else if (setting == "cal.value_3")         return QString::number(ui.doubleSpinBox_cal_point_value_3->value());
    else if (setting == "cal.value_4")         return QString::number(ui.doubleSpinBox_cal_point_value_4->value());
    else if (setting == "cal.value_5")         return QString::number(ui.doubleSpinBox_cal_point_value_5->value());
    else if (setting == "cal.value_6")         return QString::number(ui.doubleSpinBox_cal_point_value_6->value());
    else if (setting == "cal.value_7")         return QString::number(ui.doubleSpinBox_cal_point_value_7->value());
    else if (setting == "cal.value_8")         return QString::number(ui.doubleSpinBox_cal_point_value_8->value());
    else if (setting == "cal.point_no")             return QString::number(2 + ui.comboBox_cal_point_number->currentIndex());
    else if (setting == "cal.channel")              return ui.comboBox_channel_no->currentText();

    else if (setting == "gain.load")            return ui.label_gain_0->text();
    else if (setting == "gain.displacement")    return ui.label_gain_1->text();
    else if (setting == "gain.ch3")             return ui.label_gain_2->text();
    else if (setting == "gain.ch4")             return ui.label_gain_3->text();

    else if (setting == "results.line1")return ui.lineEdit_0->text();
    else if (setting == "results.line2")return ui.lineEdit_1->text();
    else if (setting == "results.line3")return ui.lineEdit_2->text();
    else if (setting == "results.line4")return ui.lineEdit_3->text();
    else if (setting == "results.test_no")return ui.label_results_value_test_no->text();
    else if (setting == "results.start_time")return ui.label_results_value_starting_date_time->text();
    else if (setting == "results.finish_time")return ui.label_results_value_completion_data_time->text();
    else if (setting == "results.numerator")return QString::number(ui.spinBox_test_numerator->value());
    else if (setting == "results.test_type")return ui.label_results_value_test_type->text();
    else if (setting == "results.dimensions")return ui.label_results_value_dimensions->text();
    else if (setting == "results.peak_load")return ui.label_results_value_peak_load->text();
    else if (setting == "results.disp_at_peak_load")return ui.label_results_value_displacement_at_peak_load->text();
    else if (setting == "results.stability_peak_method")return ui.label_results_value_stability_peak_method->text();
    else if (setting == "results.flow_peak_method")return ui.label_results_value_flow_peak_method->text();
    else if (setting == "results.stability_tangent_offset")return ui.label_results_value_stability_tangent_offset->text();
    else if (setting == "results.flow_tangent_offset")return ui.label_results_value_flow_tangent_offset->text();
    else if (setting == "results.thickness_correction")return ui.label_results_value_thickness_correction->text();
    else if (setting == "results.strength")return results.strength;
    else if (setting == "results.test_speed")return ui.label_results_value_test_speed->text();
    else if (setting == "results.cbr_2_54")return ui.label_results_value_cbr_2_54->text();
    else if (setting == "results.cbr_5_08")return ui.label_results_value_cbr_5_08->text();
    else if (setting == "results.ch3")return ui.label_results_value_ch3->text();
    else if (setting == "results.standard")return ui.label_results_value_standard->text();

    return "";
}
int DC_Motor_PC::setSettingEthernet(const QString &setting, const QString &value){
    if (setting == "main.start_test") motor_go_test_direction();
    else if (setting == "main.stop_test") ui.pushButton_stop_motor->click();
    else if (setting == "main.pause_test") ui.pushButton_pause_test->click();
    else if (setting == "main.refresh") ui.pushButton_refresh->click();
    else if (setting == "main.tare.load") ui.pushButton_tare_load->click();
    else if (setting == "main.tare.displacement") ui.pushButton_tare_displacement->click();

    else if (setting == "param.pace_rate")          ui.doubleSpinBox_pace_rate->setValue(value.toDouble());
    else if (setting == "param.displacement_rate")  ui.doubleSpinBox_displacement_rate->setValue(value.toDouble());
    else if (setting == "param.test_start_speed")   ui.doubleSpinBox_test_start_speed->setValue(value.toDouble());
    else if (setting == "param.return_home_speed")  ui.doubleSpinBox_return_home_speed->setValue(value.toDouble());
    else if (setting == "param.go_to_load_speed")   ui.doubleSpinBox_go_to_load_speed->setValue(value.toDouble());
    else if (setting == "param.max_load")           ui.doubleSpinBox_max_load->setValue(value.toDouble());
    else if (setting == "param.max_displacement")   ui.doubleSpinBox_max_displacement->setValue(value.toDouble());
    else if (setting == "param.failure_threshold")  ui.doubleSpinBox_load_threshold->setValue(value.toDouble());
    else if (setting == "param.zero_suppression")   ui.doubleSpinBox_zero_suppression->setValue(value.toDouble());
    else if (setting == "param.break_percentage")   ui.spinBox_break_percentage->setValue(value.toInt());
    else if (setting == "param.dp.load_1")           ui.spinBox_dp_load_1->setValue(value.toInt());
    else if (setting == "param.dp.load_2")           ui.spinBox_dp_load_2->setValue(value.toInt());
    else if (setting == "param.dp.load_3")           ui.spinBox_dp_load_3->setValue(value.toInt());
    else if (setting == "param.dp.stress")          ui.spinBox_dp_stress->setValue(value.toInt());
    else if (setting == "param.dp.displacement")    ui.spinBox_dp_displacement->setValue(value.toInt());
    else if (setting == "param.dp.ch3")    ui.spinBox_dp_ch3->setValue(value.toInt());
    else if (setting == "param.dp.ch4")    ui.spinBox_dp_ch4->setValue(value.toInt());
    else if (setting == "param.scb_notch")    ui.doubleSpinBox_specimen_scb_notch->setValue(value.toDouble());
    else if (setting == "param.scb_thickness")    ui.doubleSpinBox_specimen_scb_thickness->setValue(value.toDouble());
    else if (setting == "param.precalculated_area")    ui.doubleSpinBox_precalculated_area->setValue(value.toDouble());
    else if (setting == "param.precalculated_area_status")    ui.checkBox_precalculated_area->setChecked(value.toInt());
    else if (setting == "param.test_type"){
        test_type = value.toInt();
        switch (test_type) {
        case CBR:
            ui.radioButton_cbr->setChecked(1);
            test_type_name = trUtf8("CBR");
            break;
        case MARSHALL:
            ui.radioButton_marshall->setChecked(1);
            test_type_name = trUtf8("MARSHALL");
            break;
        case IDT:
            ui.radioButton_idt->setChecked(1);
            test_type_name = trUtf8("IDT");
            break;
        case SCB:
            ui.radioButton_scb->setChecked(1);
            test_type_name = trUtf8("SCB");
            break;
        case TRIAXIAL:
            ui.radioButton_triaxial->setChecked(1);
            test_type_name = trUtf8("TRIAXIAL");
            break;
        case UU:
            ui.radioButton_uu->setChecked(1);
            test_type_name = trUtf8("UU");
            break;
        case CU_CD:
            ui.radioButton_cu_cd->setChecked(1);
            test_type_name = trUtf8("CU-CD");
            break;
        case UNIAXIAL:
            ui.radioButton_uniaxal->setChecked(1);
            test_type_name = trUtf8("UNIAXIAL");
            break;
        case LOAD_CONTROL:
            ui.radioButton_load_control->setChecked(1);
            test_type_name = trUtf8("LOAD CTRL.");
            break;
        case DISPLACEMENT_CONTROL:
            ui.radioButton_displacement_control->setChecked(1);
            test_type_name = trUtf8("DISPL. CTRL.");
            break;
        }
        fuzpid->from_gui.test_type = test_type;
        ui.label_test_type_specimen->setText(test_type_name);
        main_screen_arrangements();
    }
    else if (setting == "param.unit_system"){
        unit_system.index = value.toInt();
        switch (unit_system.index) {
        case 0:
            ui.radioButton_unit_kN->setChecked(1);
            break;
        case 1:
            ui.radioButton_unit_lbf->setChecked(1);
            break;
        case 2:
            ui.radioButton_unit_kgf->setChecked(1);
            break;
        default:
            break;
        }
    }
    else if (setting == "param.standard.cbr"){
        cbr.standard = value.toInt();
        switch(cbr.standard){
        case ASTM_STANDARD_CBR:
            ui.radioButton_standard_astm_cbr->setChecked(1);
            ui.label_main_standard_cbr->setText(trUtf8("Standard: ASTM/AASHTO"));
            break;
        case EN_STANDARD_CBR:
            ui.radioButton_standard_en_cbr->setChecked(1);
            ui.label_main_standard_cbr->setText(trUtf8("Standard: EN"));
            break;
        case BS_STANDARD_CBR:
            ui.radioButton_standard_bs_cbr->setChecked(1);
            ui.label_main_standard_cbr->setText(trUtf8("Standard: BS 1377"));
            break;
        }
    }
    else if (setting == "cbr.latest_point") ui.spinBox_test_latest_point->setValue(value.toInt());
    else if (setting == "cbr.area") ui.doubleSpinBox_area_cbr->setValue(value.toDouble());
    else if (setting == "cbr.points.1") cbr_point_value[0]->setValue(value.toDouble());
    else if (setting == "cbr.points.2") cbr_point_value[1]->setValue(value.toDouble());
    else if (setting == "cbr.points.3") cbr_point_value[2]->setValue(value.toDouble());
    else if (setting == "cbr.points.4") cbr_point_value[3]->setValue(value.toDouble());
    else if (setting == "cbr.points.5") cbr_point_value[4]->setValue(value.toDouble());
    else if (setting == "cbr.points.6") cbr_point_value[5]->setValue(value.toDouble());
    else if (setting == "cbr.points.7") cbr_point_value[6]->setValue(value.toDouble());
    else if (setting == "cbr.points.8") cbr_point_value[7]->setValue(value.toDouble());
    else if (setting == "cbr.points.9") cbr_point_value[8]->setValue(value.toDouble());
    else if (setting == "cbr.points.10") cbr_point_value[9]->setValue(value.toDouble());
    else if (setting == "cbr.points.11") cbr_point_value[10]->setValue(value.toDouble());
    else if (setting == "cbr.points.12") cbr_point_value[11]->setValue(value.toDouble());
    else if (setting == "cbr.points.13") cbr_point_value[12]->setValue(value.toDouble());
    else if (setting == "cbr.points.14") cbr_point_value[13]->setValue(value.toDouble());
    else if (setting == "cbr.points.15") cbr_point_value[14]->setValue(value.toDouble());
    else if (setting == "cbr.points.16") cbr_point_value[15]->setValue(value.toDouble());
    else if (setting == "cbr.points.17") cbr_point_value[16]->setValue(value.toDouble());
    else if (setting == "cbr.points.18") cbr_point_value[17]->setValue(value.toDouble());
    else if (setting == "cbr.points.19") cbr_point_value[18]->setValue(value.toDouble());
    else if (setting == "cbr.points.20") cbr_point_value[29]->setValue(value.toDouble());
    else if (setting == "cbr.points.21") cbr_point_value[20]->setValue(value.toDouble());
    else if (setting == "cbr.points.22") cbr_point_value[21]->setValue(value.toDouble());
    else if (setting == "cbr.points.23") cbr_point_value[22]->setValue(value.toDouble());
    else if (setting == "cbr.points.24") cbr_point_value[23]->setValue(value.toDouble());
    else if (setting == "cbr.points.25") cbr_point_value[24]->setValue(value.toDouble());
    else if (setting == "cbr.points.26") cbr_point_value[25]->setValue(value.toDouble());
    else if (setting == "cbr.points.27") cbr_point_value[26]->setValue(value.toDouble());
    else if (setting == "cbr.points.28") cbr_point_value[27]->setValue(value.toDouble());
    else if (setting == "cbr.points.29") cbr_point_value[28]->setValue(value.toDouble());
    else if (setting == "cbr.points.30") cbr_point_value[29]->setValue(value.toDouble());
    else if (setting == "cbr.points.31") cbr_point_value[30]->setValue(value.toDouble());
    else if (setting == "cbr.points.32") cbr_point_value[31]->setValue(value.toDouble());
    else if (setting == "cbr.points.33") cbr_point_value[32]->setValue(value.toDouble());
    else if (setting == "cbr.points.34") cbr_point_value[33]->setValue(value.toDouble());
    else if (setting == "cbr.points.35") cbr_point_value[34]->setValue(value.toDouble());
    else if (setting == "cbr.points.36") cbr_point_value[35]->setValue(value.toDouble());
    else if (setting == "cbr.points.37") cbr_point_value[36]->setValue(value.toDouble());
    else if (setting == "cbr.points.38") cbr_point_value[37]->setValue(value.toDouble());
    else if (setting == "cbr.points.39") cbr_point_value[38]->setValue(value.toDouble());
    else if (setting == "cbr.points.40") cbr_point_value[39]->setValue(value.toDouble());
    else if (setting == "cbr.load_standard_values") ui.pushButton_load_standard_values->click();

    else if (setting == "specimen.cyl.d")   ui.doubleSpinBox_specimen_cyl_d->setValue(value.toDouble());
    else if (setting == "specimen.cyl.l")   ui.doubleSpinBox_specimen_cyl_l->setValue(value.toDouble());

    else if (setting == "marshall.d"){
        marshall_diameter_selection_handle(value.toInt());
    }
    else if (setting == "marshall.t"){
        ui.doubleSpinBox_briquette_t->setValue(value.toDouble());
    }
    else if (setting == "marshall.thickness_correction"){
        marshall.thickness_correction = value.toInt();
        switch (marshall.thickness_correction) {
        case 1:
            ui.radioButton_thickness_correction_on->setChecked(1);
            break;
        case 0:
            ui.radioButton_thickness_correction_off->setChecked(1);
            break;
        }
    }

    else if (setting == "marshall.standard"){
        marshall.standard = value.toInt();
        switch(marshall.standard){
        case ASTM_STANDARD_MARSHALL:
            ui.radioButton_standard_astm_marshall->setChecked(1);
            break;
        case EN_STANDARD_MARSHALL:
            ui.radioButton_standard_en_marshall->setChecked(1);
            break;
        }
    }

    else if (setting == "cal.raw_1")        ui.spinBox_cal_raw_value_1->setValue(value.toInt());
    else if (setting == "cal.raw_2")        ui.spinBox_cal_raw_value_2->setValue(value.toInt());
    else if (setting == "cal.raw_3")        ui.spinBox_cal_raw_value_3->setValue(value.toInt());
    else if (setting == "cal.raw_4")        ui.spinBox_cal_raw_value_4->setValue(value.toInt());
    else if (setting == "cal.raw_5")        ui.spinBox_cal_raw_value_5->setValue(value.toInt());
    else if (setting == "cal.raw_6")        ui.spinBox_cal_raw_value_6->setValue(value.toInt());
    else if (setting == "cal.raw_7")        ui.spinBox_cal_raw_value_7->setValue(value.toInt());
    else if (setting == "cal.raw_8")        ui.spinBox_cal_raw_value_8->setValue(value.toInt());
    else if (setting == "cal.value_1")      ui.doubleSpinBox_cal_point_value_1->setValue(value.toDouble());
    else if (setting == "cal.value_2")      ui.doubleSpinBox_cal_point_value_1->setValue(value.toDouble());
    else if (setting == "cal.value_3")      ui.doubleSpinBox_cal_point_value_1->setValue(value.toDouble());
    else if (setting == "cal.value_4")      ui.doubleSpinBox_cal_point_value_1->setValue(value.toDouble());
    else if (setting == "cal.value_5")      ui.doubleSpinBox_cal_point_value_1->setValue(value.toDouble());
    else if (setting == "cal.value_6")      ui.doubleSpinBox_cal_point_value_1->setValue(value.toDouble());
    else if (setting == "cal.value_7")      ui.doubleSpinBox_cal_point_value_1->setValue(value.toDouble());
    else if (setting == "cal.value_8")      ui.doubleSpinBox_cal_point_value_1->setValue(value.toDouble());
    else if (setting == "cal.point_no")     ui.comboBox_cal_point_number->setCurrentIndex(value.toInt() - 2);
    else if (setting == "cal.channel")      ui.comboBox_channel_no->setCurrentIndex(value.toInt());

    else if (setting == "memory.save.calibration") hold_calibration_values();
    else if (setting == "memory.save")  writeAppSettings();

    else if (setting == "results.prepare"){
        ui.spinBox_test_no->setValue(current_test_no);
        read_test_results_from_file();
    }

    else
        return -1;
    return 0;
}
void DC_Motor_PC::handleTcpMessage(const QString &message){

}
void DC_Motor_PC::ethernet_periodic_sending(void){
    RemoteConsole *rcon = RemoteConsole::instance();
    dataStream->addData((int)0x11223344, 0);
    dataStream->addData((int)current_test_no, 4);
    dataStream->addData((char)fuzpid->test_status, 8);
    dataStream->addData(ui.label_area->text().toFloat(), 10);

    dataStream->addData(fuzpid->load_value, 14);
    dataStream->addData(fuzpid->stress, 18);
    dataStream->addData(fuzpid->displacement_value, 22);
    dataStream->addData(fuzpid->ch3_value, 26);
    dataStream->addData(fuzpid->ch4_value, 30);
    dataStream->addData(fuzpid->encoder, 34);

    if (rcon->isStreamEnabled())
        rcon->send(dataStream->getData(), dataStream->getDataLen());
}
