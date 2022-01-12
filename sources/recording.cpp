#include "dc_motor_pc.h"

void DC_Motor_PC::excel_recording(void){
    static u32 local_counter = 0;
    QTextStream datum(&excel_file);
    static QTime record_elapsed;
    static double time_counter = 0;


    if(ui.radioButton_record_raw_data_on->isChecked()){
        if(fuzpid->run_pid){
            time_counter = time_counter + record_elapsed.elapsed();
            record_elapsed.restart();
            if(local_counter % ((u32)(10*ui.doubleSpinBox_record_interval->value())) == 0){
                datum << QString::number((time_counter/1000),'f',1) << "," << fuzpid->load_value << "," << fuzpid->stress << "\n";
            }
        }
        else{
            time_counter = 0;
            record_elapsed.restart();
        }
        local_counter++;
    }
}
void DC_Motor_PC::save_type_parameters(){

    parameters[test_type].max_load = ui.doubleSpinBox_max_load->value();
    parameters[test_type].max_displacement = ui.doubleSpinBox_max_displacement->value();
    parameters[test_type].zero_suppression = ui.doubleSpinBox_zero_suppression->value();
    parameters[test_type].load_threshold = ui.doubleSpinBox_load_threshold->value();
    parameters[test_type].break_percentage = ui.spinBox_break_percentage->value();
    parameters[test_type].displacement_rate = ui.doubleSpinBox_displacement_rate->value();
    parameters[test_type].cylinder.d = ui.doubleSpinBox_specimen_cyl_d->value();
    parameters[test_type].cylinder.l = ui.doubleSpinBox_specimen_cyl_l->value();
    parameters[test_type].start_speed = ui.doubleSpinBox_test_start_speed->value();
    parameters[test_type].return_home_speed = ui.doubleSpinBox_return_home_speed->value();
    parameters[test_type].go_to_load_speed = ui.doubleSpinBox_go_to_load_speed->value();
    parameters[test_type].precalculated_area_status = ui.checkBox_precalculated_area->isChecked();
    parameters[test_type].precalculated_area = ui.doubleSpinBox_precalculated_area->value();
    briquette.t = ui.doubleSpinBox_briquette_t->value();

    guideline_spacing = ui.spinBox_guideline_spacing->value();
    pace_rate = ui.doubleSpinBox_pace_rate->value();
    pace_mPa = ui.doubleSpinBox_pace_rate_mpa->value();
    ch3.status= ui.checkBox_ch3->isChecked();
    ch4.status= ui.checkBox_ch4->isChecked();
    ch3.label = ui.lineEdit_ch3_name->text();
    ch4.label = ui.lineEdit_ch4_name->text();
    dp.load_1 = ui.spinBox_dp_load_1->value();
    dp.load_2 = ui.spinBox_dp_load_2->value();
    dp.load_3 = ui.spinBox_dp_load_3->value();
    dp.stress = ui.spinBox_dp_stress->value();
    dp.displacement = ui.spinBox_dp_displacement->value();
    ch3.dp = ui.spinBox_dp_ch3->value();
    ch4.dp = ui.spinBox_dp_ch4->value();

}
void DC_Motor_PC::assign_type_parameters(){

    ui.doubleSpinBox_max_load->setValue(parameters[test_type].max_load);
    ui.doubleSpinBox_max_displacement->setValue(parameters[test_type].max_displacement);
    ui.doubleSpinBox_zero_suppression->setValue(parameters[test_type].zero_suppression);
    ui.doubleSpinBox_load_threshold->setValue(parameters[test_type].load_threshold);
    ui.spinBox_break_percentage->setValue(parameters[test_type].break_percentage);
    ui.doubleSpinBox_test_start_speed->setValue(parameters[test_type].start_speed);
    ui.doubleSpinBox_displacement_rate->setValue(parameters[test_type].displacement_rate);
    ui.doubleSpinBox_specimen_cyl_d->setValue(parameters[test_type].cylinder.d);
    ui.doubleSpinBox_specimen_cyl_l->setValue(parameters[test_type].cylinder.l);
    ui.doubleSpinBox_return_home_speed->setValue(parameters[test_type].return_home_speed);
    ui.doubleSpinBox_go_to_load_speed->setValue(parameters[test_type].go_to_load_speed);
    ui.checkBox_precalculated_area->setChecked(parameters[test_type].precalculated_area_status);
    ui.doubleSpinBox_precalculated_area->setValue(parameters[test_type].precalculated_area);
    ui.doubleSpinBox_briquette_t->setValue(briquette.t);

    ui.doubleSpinBox_pace_rate->setValue(pace_rate);
    ui.doubleSpinBox_pace_rate_mpa->setValue(pace_mPa);
    ui.spinBox_guideline_spacing->setValue(guideline_spacing);
    ui.checkBox_ch3->setChecked(ch3.status);
    ui.checkBox_ch4->setChecked(ch4.status);
    ui.lineEdit_ch3_name->setText(ch3.label);
    ui.lineEdit_ch4_name->setText(ch4.label);
    ui.spinBox_dp_load_1->setValue(dp.load_1);
    ui.spinBox_dp_load_2->setValue(dp.load_2);
    ui.spinBox_dp_load_3->setValue(dp.load_3);
    ui.spinBox_dp_stress->setValue(dp.stress);
    ui.spinBox_dp_displacement->setValue(dp.displacement);
    ui.spinBox_dp_ch3->setValue(ch3.dp);
    ui.spinBox_dp_ch4->setValue(ch4.dp);

}
void DC_Motor_PC::writeAppSettings(){

    save_type_parameters();

    QSettings settings("settings.ini",QSettings::IniFormat);

    settings.beginGroup(QStringLiteral("common"));
    settings.setValue(QStringLiteral("check"),"viva zapata");
    settings.setValue(QStringLiteral("zoom_radiobutton_no"),zoom_radiobutton_no);
    settings.setValue(QStringLiteral("v_axis_radiobutton_no"),v_axis_radiobutton_no);
    settings.setValue(QStringLiteral("h_axis_radiobutton_no"),h_axis_radiobutton_no);
    settings.setValue(QStringLiteral("x_neg_val"),ui.spinBox_zoom_val_neg_x->value());
    settings.setValue(QStringLiteral("x_pos_val"),ui.spinBox_zoom_val_pos_x->value());
    settings.setValue(QStringLiteral("y_neg_val"),ui.spinBox_zoom_val_neg_y->value());
    settings.setValue(QStringLiteral("y_pos_val"),ui.spinBox_zoom_val_pos_y->value());
    settings.setValue(QStringLiteral("current_test_no"),current_test_no);
    settings.setValue(QStringLiteral("record_interval"),ui.doubleSpinBox_record_interval->value());
    settings.setValue(QStringLiteral("upward_max_speed"),ui.doubleSpinBox_upward_max_speed->value());
    settings.setValue(QStringLiteral("upward_ramp_time"),ui.spinBox_upward_ramp_time->value());
    settings.setValue(QStringLiteral("downward_max_speed"),ui.doubleSpinBox_downward_max_speed->value());
    settings.setValue(QStringLiteral("downward_ramp_time"),ui.spinBox_downward_ramp_time->value());
    settings.setValue(QStringLiteral("motor_RPM"),ui.doubleSpinBox_motor_rpm->value());
    settings.setValue(QStringLiteral("displacement_speed"),ui.doubleSpinBox_displ_speed->value());
    settings.setValue(QStringLiteral("protect_specimen"),ui.doubleSpinBox_protect_specimen->value());
    settings.setValue(QStringLiteral("ip_address"),ui.lineEdit_ip_address->text());
    settings.setValue(QStringLiteral("netmask"),ui.lineEdit_ip_mask->text());
    settings.setValue(QStringLiteral("gateway"),ui.lineEdit_ip_gw->text());
    settings.setValue(QStringLiteral("language_index"),language_index);
    settings.setValue(QStringLiteral("unit_system_index"),unit_system.index);
    settings.setValue(QStringLiteral("low_speed_up"),JOG.low.speed_up);
    settings.setValue(QStringLiteral("low_speed_down"),JOG.low.speed_down);
    settings.setValue(QStringLiteral("low_speed_up_ramp"),JOG.low.ramp_up);
    settings.setValue(QStringLiteral("low_speed_down_ramp"),JOG.low.ramp_down);
    settings.setValue(QStringLiteral("mid_speed_up"),JOG.mid.speed_up);
    settings.setValue(QStringLiteral("mid_speed_down"),JOG.mid.speed_down);
    settings.setValue(QStringLiteral("mid_speed_up_ramp"),JOG.mid.ramp_up);
    settings.setValue(QStringLiteral("mid_speed_down_ramp"),JOG.mid.ramp_down);
    settings.setValue(QStringLiteral("high_speed_up"),JOG.high.speed_up);
    settings.setValue(QStringLiteral("high_speed_down"),JOG.high.speed_down);
    settings.setValue(QStringLiteral("high_speed_up_ramp"),JOG.high.ramp_up);
    settings.setValue(QStringLiteral("high_speed_down_ramp"),JOG.high.ramp_down);
    settings.setValue(QStringLiteral("step_first_speed"),ui.doubleSpinBox_step_first_speed->value());
    settings.setValue(QStringLiteral("step_second_speed"),ui.doubleSpinBox_step_second_speed->value());
    settings.setValue(QStringLiteral("step_transition_time"),ui.spinBox_step_transition_time->value());
    settings.setValue(QStringLiteral("load_calibration_channel"),load_calibration_channel);
    settings.setValue(QStringLiteral("pace_rate"),pace_rate);
    settings.setValue(QStringLiteral("pace_mPa"),pace_mPa);
    settings.setValue(QStringLiteral("dp_load_1"),dp.load_1);
    settings.setValue(QStringLiteral("dp_load_2"),dp.load_2);
    settings.setValue(QStringLiteral("dp_load_3"),dp.load_3);
    settings.setValue(QStringLiteral("dp_stress"),dp.stress);
    settings.setValue(QStringLiteral("dp_displacement"),dp.displacement);
    settings.setValue(QStringLiteral("dp_ch3"),ch3.dp);
    settings.setValue(QStringLiteral("dp_ch4"),ch4.dp);
    settings.setValue(QStringLiteral("ch3_status"),ch3.status);
    settings.setValue(QStringLiteral("ch4_status"),ch4.status);
    settings.setValue(QStringLiteral("ch3_label"),ch3.label);
    settings.setValue(QStringLiteral("ch4_label"),ch4.label);
    settings.setValue(QStringLiteral("test_type"),test_type);
    settings.setValue(QStringLiteral("ch3_unit"),ch3.unit);
    settings.setValue(QStringLiteral("ch4_unit"),ch4.unit);
    settings.setValue(QStringLiteral("guideline_spacing"),guideline_spacing);
    settings.setValue(QStringLiteral("standard_cbr"),cbr.standard);
    settings.setValue(QStringLiteral("latest_test_point"),ui.spinBox_test_latest_point->value());
    settings.setValue(QStringLiteral("area_cbr"),ui.doubleSpinBox_area_cbr->value());
    settings.setValue(QStringLiteral("briquette_t"),briquette.t);
    settings.setValue(QStringLiteral("standard_type_marshall"),marshall.standard);
    settings.setValue(QStringLiteral("thickness_correction"),marshall.thickness_correction);
    settings.setValue(QStringLiteral("diameter_number"),marshall.diameter_number);
    settings.setValue(QStringLiteral("scb_notch"),ui.doubleSpinBox_specimen_scb_notch->value());
    settings.setValue(QStringLiteral("scb_thickness"),ui.doubleSpinBox_specimen_scb_thickness->value());
    settings.setValue(QStringLiteral("headshake_speed"),ui.doubleSpinBox_headshake_speed->value());
    settings.setValue(QStringLiteral("headshake_direction_timer"),ui.doubleSpinBox_headshake_direction_timer->value());
    settings.setValue(QStringLiteral("pendulum_mid_point"),ui.doubleSpinBox_pendulum_mid_point->value());
    settings.setValue(QStringLiteral("pendulum_top_boundary"),ui.doubleSpinBox_pendulum_top_boundary->value());
    settings.setValue(QStringLiteral("pendulum_speed_multiplier"),ui.doubleSpinBox_pendulum_speed_multiplier->value());
    settings.setValue(QStringLiteral("pendulum_kp"),ui.doubleSpinBox_pendulum_kp->value());
    settings.setValue(QStringLiteral("pendulum_ki"),ui.doubleSpinBox_pendulum_ki->value());
    settings.setValue(QStringLiteral("pendulum_kd"),ui.doubleSpinBox_pendulum_kd->value());
    settings.setValue(QStringLiteral("pendulum_tolerance"),ui.doubleSpinBox_pendulum_tolerance->value());

    for(u8 j = 0; j < 10; j++){
        settings.setValue(QStringLiteral("pid_kp_%1").arg(j),pid[j].Kp);
        settings.setValue(QStringLiteral("pid_ki_%1").arg(j),pid[j].Ki);
        settings.setValue(QStringLiteral("pid_kd_%1").arg(j),pid[j].Kd);
    }
    for(u8 j = 0; j < 40; j++){
        settings.setValue(QStringLiteral("cbr_point_%1").arg(j),cbr_point_value[j]->value());
    }

    for(u8 j = 0; j < 4; j++){
        settings.setValue(QString("gain_%1").arg(j),fuzpid->gain[j]);
        settings.setValue(QString("polarity_%1").arg(j),fuzpid->ch_polarity[j]);
    }
    settings.endGroup();

    settings.beginGroup(QStringLiteral("parameters"));
    for(u8 j = 0; j < 10; j++){
        settings.setValue(QStringLiteral("max_load_%1").arg(j),parameters[j].max_load);
        settings.setValue(QStringLiteral("max_displacement_%1").arg(j),parameters[j].max_displacement);
        settings.setValue(QStringLiteral("zero_suppression_%1").arg(j),parameters[j].zero_suppression);
        settings.setValue(QStringLiteral("load_threshold_%1").arg(j),parameters[j].load_threshold);
        settings.setValue(QStringLiteral("break_percentage_%1").arg(j),parameters[j].break_percentage);
        settings.setValue(QStringLiteral("displacement_rate_%1").arg(j),parameters[j].displacement_rate);
        settings.setValue(QStringLiteral("cylindrical_d_%1").arg(j),parameters[j].cylinder.d);
        settings.setValue(QStringLiteral("cylindrical_l_%1").arg(j),parameters[j].cylinder.l);
        settings.setValue(QStringLiteral("start_speed_%1").arg(j),parameters[j].start_speed);
        settings.setValue(QStringLiteral("return_home_speed_%1").arg(j),parameters[j].return_home_speed);
        settings.setValue(QStringLiteral("go_to_load_speed_%1").arg(j),parameters[j].go_to_load_speed);
        settings.setValue(QStringLiteral("precalculated_area_status_%1").arg(j),parameters[j].precalculated_area_status);
        settings.setValue(QStringLiteral("precalculated_area_%1").arg(j),parameters[j].precalculated_area);
    }
    settings.endGroup();
    settings.sync();
    QProcess::execute("sync()");

    QTimer::singleShot(500,this,SLOT(create_backup_file()));
}
void DC_Motor_PC::create_backup_file(){
    QProcess::execute("cp /usr/local/bin/settings.ini /usr/local/bin/backup_settings.ini");
    qDebug(__FUNCTION__);
}
void DC_Motor_PC::load_backup_file(){
    static u8 tmp = 0;
    static QLabel deneme_label;

    switch (tmp) {
    case 0:
        QProcess::execute("sync()");
        QTimer::singleShot(500,this,SLOT(load_backup_file()));
        tmp++;
        break;
    case 1:
        QProcess::execute("cp /usr/local/bin/backup_settings.ini /usr/local/bin/settings.ini");
        QTimer::singleShot(500,this,SLOT(load_backup_file()));
        tmp++;
        break;
    case 2:
        deneme_label.setGeometry(150,220,500,100);
        deneme_label.setFont(QFont("Ubuntu",16,QFont::Black,true));
        deneme_label.setText(QString(trUtf8("Please reboot to fix the error")));
        deneme_label.show();

        //QProcess::execute("reboot");
        break;
    default:
        break;
    }
    qDebug(__FUNCTION__);
}
void DC_Motor_PC::load_temporary_settings(void){
    zoom_radiobutton_no 	= 1;
    ui.spinBox_zoom_val_neg_x->setValue(0);
    ui.spinBox_zoom_val_pos_x->setValue(1);
    ui.spinBox_zoom_val_neg_y->setValue(0);
    ui.spinBox_zoom_val_pos_y->setValue(1);
    current_test_no 		= 1;
    v_axis_radiobutton_no = V_AXIS_LOAD;
    h_axis_radiobutton_no = H_AXIS_TIME;
    ui.doubleSpinBox_record_interval->setValue(1);
    ui.lineEdit_ip_address->setText("192.168.1.177");
    ui.lineEdit_ip_mask->setText("255.255.255.0");
    ui.lineEdit_ip_gw->setText("192.168.1.1");
    language_index = 0;
    ui.radioButton_english->setChecked(1);
    unit_system.index = 0;
    ui.radioButton_unit_kN->setChecked(1);
    dp.load_1 = 2;
    dp.load_2 = 2;
    dp.load_3 = 2;
    dp.stress = 3;
    dp.displacement = 2;
    ch3.dp = 0;
    ch4.dp = 0;
    pace_rate = 13.5;
    pace_mPa = 1;
    guideline_spacing = 1;
    ch3.status= 1;
    ch4.status= 1;
    ch3.label = "ch3";
    ch4.label = "ch4";
    ch3.unit = 0;
    ch4.unit = 0;
    test_type = CBR;
    fuzpid->from_gui.test_type = CBR;
    for(u8 j = 0; j < 4; j++){
        fuzpid->gain[j] = 0;
        fuzpid->ch_polarity[j] = 0;
    }
    for(u8 j = 0; j < 10; j++){
        pid[j].Kp = 0;
        pid[j].Ki = 0;
        pid[j].Kd = 0;
    }
    for(u8 j = 0; j < 10; j++){
        parameters[j].max_load = 100;
        parameters[j].max_displacement = 100;
        parameters[j].zero_suppression = 10;
        parameters[j].load_threshold = 20;
        parameters[j].break_percentage = 20;
        parameters[j].displacement_rate = 1.234;
        parameters[j].cylinder.d = 1;
        parameters[j].cylinder.l = 0;
        parameters[j].start_speed = 1;
        parameters[j].return_home_speed = 1;
        parameters[j].go_to_load_speed = 1;
    }
}
void DC_Motor_PC::readAppSettings(){
    QSettings settings("settings.ini",QSettings::IniFormat);

    if(settings.value("common/check") == "viva zapata"){
        qDebug("viva zapata!!");
        correctly_opened = true;
    }
    else{
        correctly_opened = false;
        qDebug("yamuldu lo :( ");
        load_temporary_settings();
        load_backup_file();
        return;
    }
    //commons
    zoom_radiobutton_no 	= settings.value("common/zoom_radiobutton_no").toInt();
    v_axis_radiobutton_no 	= settings.value("common/v_axis_radiobutton_no").toInt();
    h_axis_radiobutton_no 	= settings.value("common/h_axis_radiobutton_no").toInt();
    ui.spinBox_zoom_val_neg_x->setValue(settings.value("common/x_neg_val").toInt());
    ui.spinBox_zoom_val_pos_x->setValue(settings.value("common/x_pos_val").toInt());
    ui.spinBox_zoom_val_neg_y->setValue(settings.value("common/y_neg_val").toInt());
    ui.spinBox_zoom_val_pos_y->setValue(settings.value("common/y_pos_val").toInt());
    current_test_no 		= settings.value("common/current_test_no").toInt();
    ui.doubleSpinBox_record_interval->setValue(settings.value("common/record_interval").toDouble());
    ui.doubleSpinBox_upward_max_speed->setValue(settings.value("common/upward_max_speed").toDouble());
    ui.spinBox_upward_ramp_time->setValue(settings.value("common/upward_ramp_time").toInt());
    ui.doubleSpinBox_downward_max_speed->setValue(settings.value("common/downward_max_speed").toDouble());
    ui.spinBox_downward_ramp_time->setValue(settings.value("common/downward_ramp_time").toInt());
    ui.doubleSpinBox_motor_rpm->setValue(settings.value("common/motor_RPM").toDouble());
    ui.doubleSpinBox_displ_speed->setValue(settings.value("common/displacement_speed").toDouble());
    ui.doubleSpinBox_protect_specimen->setValue(settings.value("common/protect_specimen").toDouble());
    ui.lineEdit_ip_address->setText(settings.value("common/ip_address").toString());
    ui.lineEdit_ip_mask->setText(settings.value("common/netmask").toString());
    ui.lineEdit_ip_gw->setText(settings.value("common/gateway").toString());
    language_index = settings.value("common/language_index").toInt();
    switch (language_index) {
    case 0:
        ui.radioButton_english->setChecked(1);
        break;
    case 1:
        ui.radioButton_turkish->setChecked(1);
        break;
    case 2:
        ui.radioButton_french->setChecked(1);
        break;
    case 3:
        ui.radioButton_spanish->setChecked(1);
        break;
    case 4:
        ui.radioButton_russian->setChecked(1);
        break;
    default:
        break;
    }
    unit_system.index = settings.value("common/unit_system_index").toInt();
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
    cbr.standard = settings.value("common/standard_cbr").toInt();
    ui.spinBox_test_latest_point->setValue(settings.value("common/latest_test_point").toInt());
    ui.doubleSpinBox_area_cbr->setValue(settings.value("common/area_cbr").toDouble());
    briquette.t 		= settings.value("common/briquette_t").toDouble();
    marshall.standard = settings.value(QStringLiteral("common/standard_type_marshall")).toInt();
    marshall.thickness_correction = settings.value(QStringLiteral("common/thickness_correction")).toInt();
    marshall.diameter_number 	= settings.value("common/diameter_number").toDouble();

    JOG.low.speed_up = settings.value("common/low_speed_up").toDouble();
    JOG.low.speed_down = settings.value("common/low_speed_down").toDouble();
    JOG.low.ramp_up = settings.value("common/low_speed_up_ramp").toDouble();
    JOG.low.ramp_down = settings.value("common/low_speed_down_ramp").toDouble();
    JOG.mid.speed_up = settings.value("common/mid_speed_up").toDouble();
    JOG.mid.speed_down = settings.value("common/mid_speed_down").toDouble();
    JOG.mid.ramp_up = settings.value("common/mid_speed_up_ramp").toDouble();
    JOG.mid.ramp_down = settings.value("common/mid_speed_down_ramp").toDouble();
    JOG.high.speed_up = settings.value("common/high_speed_up").toDouble();
    JOG.high.speed_down = settings.value("common/high_speed_down").toDouble();
    JOG.high.ramp_up = settings.value("common/high_speed_up_ramp").toDouble();
    JOG.high.ramp_down = settings.value("common/high_speed_down_ramp").toDouble();

    ui.doubleSpinBox_step_first_speed->setValue(settings.value(QStringLiteral("common/step_first_speed")).toDouble());
    ui.doubleSpinBox_step_second_speed->setValue(settings.value(QStringLiteral("common/step_second_speed")).toDouble());
    ui.spinBox_step_transition_time->setValue(settings.value(QStringLiteral("common/step_transition_time")).toInt());

    guideline_spacing = settings.value(QStringLiteral("common/guideline_spacing")).toInt();
    ch3.status= settings.value(QStringLiteral("common/ch3_status")).toBool();
    ch4.status= settings.value(QStringLiteral("common/ch4_status")).toBool();
    ch3.label = settings.value(QStringLiteral("common/ch3_label")).toString();
    ch4.label = settings.value(QStringLiteral("common/ch4_label")).toString();
    test_type = settings.value(QStringLiteral("common/test_type")).toInt();
    ch3.unit = settings.value(QStringLiteral("common/ch3_unit")).toInt();
    ch4.unit = settings.value(QStringLiteral("common/ch4_unit")).toInt();
    pace_rate = settings.value(QStringLiteral("common/pace_rate")).toDouble();
    pace_mPa = settings.value(QStringLiteral("common/pace_mPa")).toDouble();
    dp.load_1 = settings.value(QStringLiteral("common/dp_load_1")).toInt();
    dp.load_2 = settings.value(QStringLiteral("common/dp_load_2")).toInt();
    dp.load_3 = settings.value(QStringLiteral("common/dp_load_3")).toInt();
    dp.stress = settings.value(QStringLiteral("common/dp_stress")).toInt();
    dp.displacement = settings.value(QStringLiteral("common/dp_displacement")).toInt();
    ch3.dp = settings.value(QStringLiteral("common/dp_ch3")).toInt();
    ch4.dp = settings.value(QStringLiteral("common/dp_ch4")).toInt();

    ui.doubleSpinBox_specimen_scb_notch->setValue(settings.value(QStringLiteral("common/scb_notch")).toDouble());
    ui.doubleSpinBox_specimen_scb_thickness->setValue(settings.value(QStringLiteral("common/scb_thickness")).toDouble());

    ui.doubleSpinBox_headshake_speed->setValue(settings.value(QStringLiteral("common/headshake_speed")).toDouble());
    ui.doubleSpinBox_headshake_direction_timer->setValue(settings.value(QStringLiteral("common/headshake_direction_timer")).toDouble());
    ui.doubleSpinBox_pendulum_mid_point->setValue(settings.value(QStringLiteral("common/pendulum_mid_point")).toDouble());
    ui.doubleSpinBox_pendulum_top_boundary->setValue(settings.value(QStringLiteral("common/pendulum_top_boundary")).toDouble());
    ui.doubleSpinBox_pendulum_speed_multiplier->setValue(settings.value(QStringLiteral("common/pendulum_speed_multiplier")).toDouble());
    ui.doubleSpinBox_pendulum_kp->setValue(settings.value(QStringLiteral("common/pendulum_kp")).toDouble());
    ui.doubleSpinBox_pendulum_ki->setValue(settings.value(QStringLiteral("common/pendulum_ki")).toDouble());
    ui.doubleSpinBox_pendulum_kd->setValue(settings.value(QStringLiteral("common/pendulum_kd")).toDouble());
    ui.doubleSpinBox_pendulum_tolerance->setValue(settings.value(QStringLiteral("common/pendulum_tolerance")).toDouble());

    for(u8 j = 0; j < 10; j++){
        pid[j].Kp = settings.value(QStringLiteral("common/pid_kp_%1").arg(j)).toDouble();
        pid[j].Ki = settings.value(QStringLiteral("common/pid_ki_%1").arg(j)).toDouble();
        pid[j].Kd = settings.value(QStringLiteral("common/pid_kd_%1").arg(j)).toDouble();
    }

    for(u8 j = 0; j < 40; j++){
        cbr_point_value[j]->setValue(settings.value(QStringLiteral("common/cbr_point_%1").arg(j)).toDouble());
    }

    load_calibration_channel = settings.value("common/load_calibration_channel").toInt();
    switch (load_calibration_channel) {
    case LOAD_1:
        ui.radioButton_load_1->setChecked(1);
        break;
    case LOAD_2:
        ui.radioButton_load_2->setChecked(1);
        break;
    case LOAD_3:
        ui.radioButton_load_3->setChecked(1);
        break;
    default:
        ui.radioButton_load_1->setChecked(1);
        break;
    }

    for(u8 j = 0; j < 4; j++){
        fuzpid->gain[j] = settings.value(QString("common/gain_%1").arg(j)).toInt();
        fuzpid->ch_polarity[j] = settings.value(QString("common/polarity_%1").arg(j)).toInt();
    }
    // commons end

    //parameters
    for(u8 j = 0; j < 10; j++){
        parameters[j].max_load = settings.value(QStringLiteral("parameters/max_load_%1").arg(j)).toDouble();
        parameters[j].max_displacement = settings.value(QStringLiteral("parameters/max_displacement_%1").arg(j)).toDouble();
        parameters[j].zero_suppression = settings.value(QStringLiteral("parameters/zero_suppression_%1").arg(j)).toDouble();
        parameters[j].load_threshold = settings.value(QStringLiteral("parameters/load_threshold_%1").arg(j)).toDouble();
        parameters[j].break_percentage = settings.value(QStringLiteral("parameters/break_percentage_%1").arg(j)).toInt();
        parameters[j].displacement_rate = settings.value(QStringLiteral("parameters/displacement_rate_%1").arg(j)).toDouble();
        parameters[j].cylinder.d = settings.value(QStringLiteral("parameters/cylindrical_d_%1").arg(j)).toDouble();
        parameters[j].cylinder.l = settings.value(QStringLiteral("parameters/cylindrical_l_%1").arg(j)).toDouble();
        parameters[j].start_speed = settings.value(QStringLiteral("parameters/start_speed_%1").arg(j)).toDouble();
        parameters[j].return_home_speed = settings.value(QStringLiteral("parameters/return_home_speed_%1").arg(j)).toDouble();
        parameters[j].go_to_load_speed = settings.value(QStringLiteral("parameters/go_to_load_speed_%1").arg(j)).toDouble();
        parameters[j].precalculated_area_status = settings.value(QStringLiteral("parameters/precalculated_area_status_%1").arg(j)).toInt();
        parameters[j].precalculated_area = settings.value(QStringLiteral("parameters/precalculated_area_%1").arg(j)).toDouble();
    }

    fuzpid->from_gui.test_type = test_type;
}
void DC_Motor_PC::get_storage_status(void){
    if(prevent_double_click()) return;
#ifdef CONFIG_x86
    qDebug(__FUNCTION__);
#endif
    QProcess p;
    p.start("df /usr/local/bin");
    p.waitForFinished();
    //qDebug() << "outputs" << p.readAllStandardOutput();

    QStringList lines = QString::fromUtf8(p.readAllStandardOutput()).split("\n");

    foreach (QString line, lines) {
        if (line.isEmpty())
            continue;
        if (line.contains("Filesystem"))
            continue;
        QStringList vals = line.split(" ", QString::SkipEmptyParts);
        if (vals.size() == 1)
            vals = line.split("\t", QString::SkipEmptyParts);
        if (vals.size() < 6)
            continue;
        //qDebug() << "returned : " << vals[3].trimmed().toInt();
        //ui.label_storage->setText(vals[3].trimmed());
        ui.progressBar_storage->setValue(100 - (vals[3].trimmed().toInt()/130));
    }
}
void DC_Motor_PC::clean_storage(){
    QProcess::execute("mv /usr/local/bin/test_results/calibration.ini /usr/local/bin");
    QProcess::execute("find /usr/local/bin/data_logs -type f -exec rm {} ;");
    QProcess::execute("find /usr/local/bin/test_results -type f -exec rm {} ;");
    QProcess::execute("find /media/usb/test_results -type f -exec rm {} ;");
    QProcess::execute("find /media/usb/consolidation_logs -type f -exec rm {} ;");
    QProcess::execute("mv /usr/local/bin/calibration.ini /usr/local/bin/test_results ");
    current_test_no = 0;
}
void DC_Motor_PC::clean_storage_confirmation(){
    if(prevent_double_click()) return;
    QMessageBox msgBox;
    msgBox.setWindowTitle(trUtf8("Clean Data Logs"));
    msgBox.setText(trUtf8("<font size = 10>Are you sure?"));
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, trUtf8("Yes"));
    msgBox.setButtonText(QMessageBox::No, trUtf8("No"));
    if(msgBox.exec() == QMessageBox::Yes){
        clean_storage();
    }
}
void DC_Motor_PC::write_calibration(void){
    QSettings calibration("test_results/calibration.ini",QSettings::IniFormat);

    for(u8 j = 0; j < 6; j++){
        calibration.beginGroup(calibration_channel_names[j]);
        calibration.setValue(QStringLiteral("Points"),fuzpid->cal[j].point_no);
        for(u8 i = 0; i < 8; i++){
            calibration.setValue(QStringLiteral("Raw_%1").arg(i), fuzpid->cal[j].real_val[i]);
        }
        for(u8 i = 0; i < 8; i++){
            calibration.setValue(QStringLiteral("Value_%1").arg(i), fuzpid->cal[j].assigned_val[i]);
        }
        calibration.endGroup();
    }
    calibration.sync();
}
void DC_Motor_PC::read_calibration(void){
    QSettings calibration("test_results/calibration.ini",QSettings::IniFormat);

    for(u8 j = 0; j < 6; j++){
        fuzpid->cal[j].point_no = calibration.value(calibration_channel_names[j] + QStringLiteral("/Points")).toInt();
        for(u8 i = 0; i < 8; i++){
            fuzpid->cal[j].real_val[i] = calibration.value(calibration_channel_names[j] + QStringLiteral("/Raw_%1").arg(i)).toDouble();
            fuzpid->cal[j].assigned_val[i] = calibration.value(calibration_channel_names[j] + QStringLiteral("/Value_%1").arg(i)).toDouble();
        }
    }
}
