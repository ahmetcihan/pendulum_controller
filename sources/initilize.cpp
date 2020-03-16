#include "dc_motor_pc.h"

void DC_Motor_PC::init_CHANNEL_SELECTIONs(void){
    connect(ui.checkBox_ch3,SIGNAL(stateChanged(int)),this,SLOT(channel_activations_handler()));
    connect(ui.checkBox_ch4,SIGNAL(stateChanged(int)),this,SLOT(channel_activations_handler()));
    connect(ui.lineEdit_ch3_name,SIGNAL(textChanged(QString)),this,SLOT(ch3_name_handler()));
    connect(ui.lineEdit_ch4_name,SIGNAL(textChanged(QString)),this,SLOT(ch4_name_handler()));

}
void DC_Motor_PC::init_RADIOBUTTONs(void){
    connect(this->ui.radioButton_standard_astm_cbr,SIGNAL(clicked()),this,SLOT(cbr_standard_type_handler()));
    connect(this->ui.radioButton_standard_en_cbr,SIGNAL(clicked()),this,SLOT(cbr_standard_type_handler()));
    connect(this->ui.radioButton_standard_bs_cbr,SIGNAL(clicked()),this,SLOT(cbr_standard_type_handler()));

    connect(this->ui.radioButton_load_1,SIGNAL(clicked()),this,SLOT(load_cal_channel_handler()));
    connect(this->ui.radioButton_load_2,SIGNAL(clicked()),this,SLOT(load_cal_channel_handler()));
    connect(this->ui.radioButton_load_3,SIGNAL(clicked()),this,SLOT(load_cal_channel_handler()));

    connect(this->ui.radioButton_auto,SIGNAL(clicked()),this,SLOT(control_mode_handler()));
    connect(this->ui.radioButton_man,SIGNAL(clicked()),this,SLOT(control_mode_handler()));

    connect(this->ui.radioButton_thickness_correction_on,SIGNAL(clicked()),this,SLOT(marshall_thickness_correction_handler()));
    connect(this->ui.radioButton_thickness_correction_off,SIGNAL(clicked()),this,SLOT(marshall_thickness_correction_handler()));

    connect(this->ui.radioButton_standard_astm_marshall,SIGNAL(clicked()),this,SLOT(marshall_standard_type_handler()));
    connect(this->ui.radioButton_standard_en_marshall,SIGNAL(clicked()),this,SLOT(marshall_standard_type_handler()));

    connect(this->ui.radioButton_cbr,SIGNAL(clicked()),this,SLOT(test_type_handler()));
    connect(this->ui.radioButton_marshall,SIGNAL(clicked()),this,SLOT(test_type_handler()));
    connect(this->ui.radioButton_idt,SIGNAL(clicked()),this,SLOT(test_type_handler()));
    connect(this->ui.radioButton_scb,SIGNAL(clicked()),this,SLOT(test_type_handler()));
    connect(this->ui.radioButton_triaxial,SIGNAL(clicked()),this,SLOT(test_type_handler()));
    connect(this->ui.radioButton_uu,SIGNAL(clicked()),this,SLOT(test_type_handler()));
    connect(this->ui.radioButton_cu_cd,SIGNAL(clicked()),this,SLOT(test_type_handler()));
    connect(this->ui.radioButton_uniaxal,SIGNAL(clicked()),this,SLOT(test_type_handler()));
    connect(this->ui.radioButton_load_control,SIGNAL(clicked()),this,SLOT(test_type_handler()));
    connect(this->ui.radioButton_displacement_control,SIGNAL(clicked()),this,SLOT(test_type_handler()));

    connect(this->ui.radioButton_auto_zoom,SIGNAL(clicked()),this,SLOT(graphic_type_handler()));
    connect(this->ui.radioButton_fixed_axes,SIGNAL(clicked()),this,SLOT(graphic_type_handler()));

    connect(this->ui.radioButton_ch3_kn,SIGNAL(clicked()),this,SLOT(ch3_unit_handler()));
    connect(this->ui.radioButton_ch3_mm,SIGNAL(clicked()),this,SLOT(ch3_unit_handler()));
    connect(this->ui.radioButton_ch3_unitless,SIGNAL(clicked()),this,SLOT(ch3_unit_handler()));
    connect(this->ui.radioButton_ch4_kn,SIGNAL(clicked()),this,SLOT(ch4_unit_handler()));
    connect(this->ui.radioButton_ch4_mm,SIGNAL(clicked()),this,SLOT(ch4_unit_handler()));
    connect(this->ui.radioButton_ch4_unitless,SIGNAL(clicked()),this,SLOT(ch4_unit_handler()));

    connect(this->ui.radioButton_v_axis_load,SIGNAL(clicked()),this,SLOT(graphic_v_axis_handler()));
    connect(this->ui.radioButton_v_axis_stress,SIGNAL(clicked()),this,SLOT(graphic_v_axis_handler()));
    connect(this->ui.radioButton_v_axis_displacement,SIGNAL(clicked()),this,SLOT(graphic_v_axis_handler()));
    connect(this->ui.radioButton_h_axis_time,SIGNAL(clicked()),this,SLOT(graphic_h_axis_handler()));
    connect(this->ui.radioButton_h_axis_displacement,SIGNAL(clicked()),this,SLOT(graphic_h_axis_handler()));

    connect(this->ui.radioButton_protect_specimen_on,SIGNAL(clicked()),this,SLOT(specimen_protection_handler()));
    connect(this->ui.radioButton_protect_specimen_off,SIGNAL(clicked()),this,SLOT(specimen_protection_handler()));
}
void DC_Motor_PC::init_PUSHBUTTONs(void){
    connect(ui.pushButton_load_standard_values,SIGNAL(pressed()),this,SLOT(cbr_load_standard_values()));
    connect(ui.pushButton_show_cbr_points,SIGNAL(pressed()),this,SLOT(cbr_show_log()));

    connect(ui.pushButton_speed_up,SIGNAL(pressed()),this,SLOT(handle_JOG_up_button()));
    connect(ui.pushButton_speed_up,SIGNAL(released()),this,SLOT(release_JOG_up_button()));
    connect(ui.pushButton_speed_down,SIGNAL(pressed()),this,SLOT(handle_JOG_down_button()));
    connect(ui.pushButton_speed_down,SIGNAL(released()),this,SLOT(release_JOG_down_button()));
    connect(ui.pushButton_speed_up_2,SIGNAL(pressed()),this,SLOT(handle_JOG_up_button()));
    connect(ui.pushButton_speed_up_2,SIGNAL(released()),this,SLOT(release_JOG_up_button()));
    connect(ui.pushButton_speed_down_2,SIGNAL(pressed()),this,SLOT(handle_JOG_down_button()));
    connect(ui.pushButton_speed_down_2,SIGNAL(released()),this,SLOT(release_JOG_down_button()));

    connect(ui.pushButton_save_as_low,SIGNAL(pressed()),this,SLOT(save_as_LOW()));
    connect(ui.pushButton_save_as_mid,SIGNAL(pressed()),this,SLOT(save_as_MID()));
    connect(ui.pushButton_save_as_high,SIGNAL(pressed()),this,SLOT(save_as_HIGH()));
    connect(ui.pushButton_set_low,SIGNAL(pressed()),this,SLOT(set_LOW()));
    connect(ui.pushButton_set_mid,SIGNAL(pressed()),this,SLOT(set_MID()));
    connect(ui.pushButton_set_high,SIGNAL(pressed()),this,SLOT(set_HIGH()));

    connect(ui.pushButton_load_factory_settings,SIGNAL(pressed()),this,SLOT(load_factory_settings()));
    connect(ui.pushButton_copy_logo_from_usb,SIGNAL(pressed()),this,SLOT(copy_logo_files()));
    connect(ui.pushButton_usb_copy,SIGNAL(pressed()),this,SLOT(copy_data_to_usb()));
    connect(ui.pushButton_delete_storage,SIGNAL(pressed()),this,SLOT(clean_storage_confirmation()));

    connect(ui.pushButton_copy_calibration_from_usb,SIGNAL(pressed()),this,SLOT(copy_calibration_from_usb()));
    connect(ui.pushButton_copy_factory_settings_from_usb,SIGNAL(pressed()),this,SLOT(copy_factory_settings_from_usb()));
    connect(ui.pushButton_copy_settings_from_usb,SIGNAL(pressed()),this,SLOT(copy_settings_from_usb()));

    connect(ui.pushButton_copy_calibration_to_usb,SIGNAL(pressed()),this,SLOT(copy_calibration_to_usb()));
    connect(ui.pushButton_copy_factory_settings_to_usb,SIGNAL(pressed()),this,SLOT(copy_factory_settings_to_usb()));
    connect(ui.pushButton_copy_settings_to_usb,SIGNAL(pressed()),this,SLOT(copy_settings_to_usb()));

    connect(this->ui.pushButton_get_test_results,SIGNAL(pressed()),this,SLOT(read_test_results_from_file()));

    connect(this->ui.pushButton_stop_motor,SIGNAL(pressed()),this,SLOT(motor_stop()));
    connect(this->ui.pushButton_start_test,SIGNAL(pressed()),this,SLOT(motor_go_test_direction()));
    connect(this->ui.pushButton_pause_test,SIGNAL(pressed()),this,SLOT(pause_test()));
    connect(ui.pushButton_get_storage_status,SIGNAL(pressed()),this,SLOT(get_storage_status()));

    connect(ui.pushButton_step_response,SIGNAL(released()),this,SLOT(step_response_handle()));

    connect(this->ui.pushButton_set_datetime,SIGNAL(pressed()),this,SLOT(set_datetime(void)));
    connect(this->ui.pushButton_start, SIGNAL(pressed()),this, SLOT (relay_on()));
    connect(this->ui.pushButton_stop, SIGNAL(pressed()),this, SLOT (relay_off()));

    connect(this->ui.pushButton_refresh,SIGNAL(pressed()),this,SLOT(refresh_button_handler()));

    connect(this->ui.pushButton_save_ip,SIGNAL(pressed()),this,SLOT(save_ip()));

    connect(this->ui.pushButton_jog, SIGNAL(pressed()),this, SLOT (show_jog_widget()));

    connect(this->ui.pushButton_result_left,SIGNAL(pressed()),this,SLOT(result_no_left()));
    connect(this->ui.pushButton_result_right,SIGNAL(pressed()),this,SLOT(result_no_right()));

    ui.pushButton_jog->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:jog_little_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_start_test->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:start_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_pause_test->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:pause_button_passive.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_stop_motor->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:stop_button_passive.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_refresh->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:refresh_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_refresh->hide();

    ui.pushButton_speed_up->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:sari_up.png);"
                                                "border-width: 0px ;");
    ui.pushButton_speed_down->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:sari_down.png);"
                                                "border-width: 0px ;");
    ui.pushButton_speed_up_2->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:sari_up.png);"
                                                "border-width: 0px ;");
    ui.pushButton_speed_down_2->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:sari_down.png);"
                                                "border-width: 0px ;");

    ui.pushButton_go_test->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:test_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_go_parameters->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:parameters_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_go_results->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:results_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_go_settings->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:settings_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_go_calibration->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:calibration_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_go_admin->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:admin_button.jpg);"
                                                "border-width: 0px ;");

    ui.pushButton_settings_company->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:company_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_settings_date_time->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:date_time_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_settings_usb_storage->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:usb_storage_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_settings_unit_system->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:unit_systems_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_settings_language->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:language_button.jpg);"
                                                "border-width: 0px ;");

    ui.pushButton_admin_gains->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:gains_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_admin_channels->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:channels_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_admin_pid->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:pid_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_admin_files->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:files_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_admin_speed_tune->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:speed_tune_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_admin_device->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:device_button.jpg);"
                                                "border-width: 0px ;");

    ui.pushButton_parameters_test_type->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:test_type_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_parameters_specimen->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:specimen_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_parameters_test_parameters->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:test_parameters_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_parameters_graph->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:graph_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_parameters_jog->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:jog_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_parameters_cbr_parameters->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:cbr_parameters_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_parameters_marshall_parameters->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:marshall_parameters_button.jpg);"
                                                "border-width: 0px ;");

    ui.pushButton_go_main_screen->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:go_main_screen_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_go_main_screen_3->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:go_main_screen_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_go_main_screen_5->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:go_main_screen_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_go_parameters_screen->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:go_main_screen_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_go_settings_screen->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:go_main_screen_button.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_go_admin_screen->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:go_main_screen_button.jpg);"
                                                "border-width: 0px ;");


    signalMapper_main_screen = new QSignalMapper(this);
    signalMapper_main_screen->setMapping(this->ui.pushButton_go_main_screen,    0);
    signalMapper_main_screen->setMapping(this->ui.pushButton_go_main_screen_3,  0);
    signalMapper_main_screen->setMapping(this->ui.pushButton_go_main_screen_5,  0);
    signalMapper_main_screen->setMapping(this->ui.pushButton_go_test,           1);
    signalMapper_main_screen->setMapping(this->ui.pushButton_go_parameters,     2);
    signalMapper_main_screen->setMapping(this->ui.pushButton_go_results,        3);
    signalMapper_main_screen->setMapping(this->ui.pushButton_go_settings,       4);
    signalMapper_main_screen->setMapping(this->ui.pushButton_go_calibration,    5);
    signalMapper_main_screen->setMapping(this->ui.pushButton_go_admin,          6);
    connect(this->ui.pushButton_go_main_screen,     SIGNAL(pressed()),signalMapper_main_screen, SLOT (map()));
    connect(this->ui.pushButton_go_main_screen_3,   SIGNAL(pressed()),signalMapper_main_screen, SLOT (map()));
    connect(this->ui.pushButton_go_main_screen_5,   SIGNAL(pressed()),signalMapper_main_screen, SLOT (map()));
    connect(this->ui.pushButton_go_test,            SIGNAL(pressed()),signalMapper_main_screen, SLOT (map()));
    connect(this->ui.pushButton_go_parameters,      SIGNAL(pressed()),signalMapper_main_screen, SLOT (map()));
    connect(this->ui.pushButton_go_results,         SIGNAL(pressed()),signalMapper_main_screen, SLOT (map()));
    connect(this->ui.pushButton_go_settings,        SIGNAL(pressed()),signalMapper_main_screen, SLOT (map()));
    connect(this->ui.pushButton_go_calibration,     SIGNAL(pressed()),signalMapper_main_screen, SLOT (map()));
    connect(this->ui.pushButton_go_admin,           SIGNAL(pressed()),signalMapper_main_screen, SLOT (map()));
    connect(signalMapper_main_screen, SIGNAL(mapped(int)),this,SLOT(main_screen_signalmapper_handler(int)));

    signalMapper_settings_screen = new QSignalMapper(this);
    signalMapper_settings_screen->setMapping(this->ui.pushButton_go_settings_screen,    0);
    signalMapper_settings_screen->setMapping(this->ui.pushButton_settings_company,      1);
    signalMapper_settings_screen->setMapping(this->ui.pushButton_settings_date_time,    2);
    signalMapper_settings_screen->setMapping(this->ui.pushButton_settings_usb_storage,  3);
    signalMapper_settings_screen->setMapping(this->ui.pushButton_settings_unit_system,  4);
    signalMapper_settings_screen->setMapping(this->ui.pushButton_settings_language,     5);
    connect(this->ui.pushButton_go_settings_screen,     SIGNAL(pressed()),signalMapper_settings_screen, SLOT (map()));
    connect(this->ui.pushButton_settings_company,       SIGNAL(pressed()),signalMapper_settings_screen, SLOT (map()));
    connect(this->ui.pushButton_settings_date_time,     SIGNAL(pressed()),signalMapper_settings_screen, SLOT (map()));
    connect(this->ui.pushButton_settings_usb_storage,   SIGNAL(pressed()),signalMapper_settings_screen, SLOT (map()));
    connect(this->ui.pushButton_settings_unit_system,   SIGNAL(pressed()),signalMapper_settings_screen, SLOT (map()));
    connect(this->ui.pushButton_settings_language,      SIGNAL(pressed()),signalMapper_settings_screen, SLOT (map()));
    connect(signalMapper_settings_screen, SIGNAL(mapped(int)),this,SLOT(settings_screen_signalmapper_handler(int)));

    signalMapper_admin_screen = new QSignalMapper(this);
    signalMapper_admin_screen->setMapping(this->ui.pushButton_go_admin_screen,  0);
    signalMapper_admin_screen->setMapping(this->ui.pushButton_admin_gains,      1);
    signalMapper_admin_screen->setMapping(this->ui.pushButton_admin_channels,   2);
    signalMapper_admin_screen->setMapping(this->ui.pushButton_admin_pid,        3);
    signalMapper_admin_screen->setMapping(this->ui.pushButton_admin_files,      4);
    signalMapper_admin_screen->setMapping(this->ui.pushButton_admin_speed_tune, 5);
    signalMapper_admin_screen->setMapping(this->ui.pushButton_admin_device,     6);
    connect(this->ui.pushButton_go_admin_screen,    SIGNAL(pressed()),signalMapper_admin_screen, SLOT (map()));
    connect(this->ui.pushButton_admin_gains,        SIGNAL(pressed()),signalMapper_admin_screen, SLOT (map()));
    connect(this->ui.pushButton_admin_channels,     SIGNAL(pressed()),signalMapper_admin_screen, SLOT (map()));
    connect(this->ui.pushButton_admin_pid,          SIGNAL(pressed()),signalMapper_admin_screen, SLOT (map()));
    connect(this->ui.pushButton_admin_files,        SIGNAL(pressed()),signalMapper_admin_screen, SLOT (map()));
    connect(this->ui.pushButton_admin_speed_tune,   SIGNAL(pressed()),signalMapper_admin_screen, SLOT (map()));
    connect(this->ui.pushButton_admin_device,       SIGNAL(pressed()),signalMapper_admin_screen, SLOT (map()));
    connect(signalMapper_admin_screen, SIGNAL(mapped(int)),this,SLOT(admin_screen_signalmapper_handler(int)));

    signalMapper_parameters_screen = new QSignalMapper(this);
    signalMapper_parameters_screen->setMapping(this->ui.pushButton_go_parameters_screen,            0);
    signalMapper_parameters_screen->setMapping(this->ui.pushButton_parameters_test_type,            1);
    signalMapper_parameters_screen->setMapping(this->ui.pushButton_parameters_specimen,             2);
    signalMapper_parameters_screen->setMapping(this->ui.pushButton_parameters_test_parameters,      3);
    signalMapper_parameters_screen->setMapping(this->ui.pushButton_parameters_jog,                  4);
    signalMapper_parameters_screen->setMapping(this->ui.pushButton_parameters_graph,                5);
    signalMapper_parameters_screen->setMapping(this->ui.pushButton_parameters_cbr_parameters,       6);
    signalMapper_parameters_screen->setMapping(this->ui.pushButton_parameters_marshall_parameters,  7);
    connect(this->ui.pushButton_go_parameters_screen,       SIGNAL(pressed()),signalMapper_parameters_screen, SLOT (map()));
    connect(this->ui.pushButton_parameters_test_type,       SIGNAL(pressed()),signalMapper_parameters_screen, SLOT (map()));
    connect(this->ui.pushButton_parameters_specimen,        SIGNAL(pressed()),signalMapper_parameters_screen, SLOT (map()));
    connect(this->ui.pushButton_parameters_test_parameters, SIGNAL(pressed()),signalMapper_parameters_screen, SLOT (map()));
    connect(this->ui.pushButton_parameters_jog,           SIGNAL(pressed()),signalMapper_parameters_screen, SLOT (map()));
    connect(this->ui.pushButton_parameters_graph,           SIGNAL(pressed()),signalMapper_parameters_screen, SLOT (map()));
    connect(this->ui.pushButton_parameters_marshall_parameters,           SIGNAL(pressed()),signalMapper_parameters_screen, SLOT (map()));
    connect(this->ui.pushButton_parameters_cbr_parameters,           SIGNAL(pressed()),signalMapper_parameters_screen, SLOT (map()));
    connect(signalMapper_parameters_screen, SIGNAL(mapped(int)),this,SLOT(parameters_screen_signalmapper_handler(int)));
}
void DC_Motor_PC::init_CALIBRATIONs(void){
    ui.comboBox_channel_no->addItem(QStringLiteral("LOAD_1"));
    ui.comboBox_channel_no->addItem(QStringLiteral("LOAD_2"));
    ui.comboBox_channel_no->addItem(QStringLiteral("LOAD_3"));
    ui.comboBox_channel_no->addItem(QStringLiteral("DISP"));
    ui.comboBox_channel_no->addItem(QStringLiteral("CH3"));
    ui.comboBox_channel_no->addItem(QStringLiteral("CH4"));
    ui.comboBox_channel_no->addItem(QStringLiteral("ENC"));
    ui.comboBox_channel_no->setCurrentIndex(0);
    current_cal_channel = 0;
    connect(this->ui.comboBox_channel_no,SIGNAL(currentIndexChanged( const QString)),this,SLOT(cal_channel_change(const QString)));

    ui.comboBox_cal_point_number->addItem(QStringLiteral("2"));
    ui.comboBox_cal_point_number->addItem(QStringLiteral("3"));
    ui.comboBox_cal_point_number->addItem(QStringLiteral("4"));
    ui.comboBox_cal_point_number->addItem(QStringLiteral("5"));
    ui.comboBox_cal_point_number->addItem(QStringLiteral("6"));
    ui.comboBox_cal_point_number->addItem(QStringLiteral("7"));
    ui.comboBox_cal_point_number->addItem(QStringLiteral("8"));
    ui.comboBox_cal_point_number->setCurrentIndex(6);

    for(u8 i = 0; i < 8; i++){
        doubleSpinBox_cal_point_value[i]->setDecimals(ui.spinBox_dp_load_1->value());
    }

    for(u8 i = 0; i < 8; i++){
        spinBox_cal_raw_value[i]->setValue(fuzpid->cal[current_cal_channel].real_val[i]);
        doubleSpinBox_cal_point_value[i]->setValue(fuzpid->cal[current_cal_channel].assigned_val[i]);
    }
    ui.comboBox_cal_point_number->setCurrentIndex(fuzpid->cal[current_cal_channel].point_no - 2);

}
void DC_Motor_PC::init_SPECIMENs(void){
    connect(this->ui.doubleSpinBox_specimen_cyl_d,SIGNAL(valueChanged(double)),this,SLOT(calculate_area_cylindrical()));
    connect(this->ui.doubleSpinBox_specimen_cyl_l,SIGNAL(valueChanged(double)),this,SLOT(calculate_area_cylindrical()));
    connect(this->ui.doubleSpinBox_specimen_scb_notch,SIGNAL(valueChanged(double)),this,SLOT(calculate_area_cylindrical()));
    connect(this->ui.doubleSpinBox_specimen_scb_thickness,SIGNAL(valueChanged(double)),this,SLOT(calculate_area_cylindrical()));

    QPixmap pixmap;
    pixmap.load(QStringLiteral(":cylinder.jpg"));
    ui.label_specimen_picture->setPixmap(pixmap);

    pixmap.load(QStringLiteral(":briquette_narrow.jpg"));
    ui.label_specimen_picture_marshall->setPixmap(pixmap);

}
void DC_Motor_PC::init_PIDs(void){
    ui.comboBox_pid->addItem(trUtf8("CBR"));
    ui.comboBox_pid->addItem(trUtf8("MARSHALL"));
    ui.comboBox_pid->addItem(trUtf8("IDT"));
    ui.comboBox_pid->addItem(trUtf8("SCB"));
    ui.comboBox_pid->addItem(trUtf8("TRIAXIAL"));
    ui.comboBox_pid->addItem(trUtf8("UU"));
    ui.comboBox_pid->addItem(trUtf8("CU-CD"));
    ui.comboBox_pid->addItem(trUtf8("UNIAXIAL"));
    ui.comboBox_pid->addItem(trUtf8("LOAD CTRL"));
    ui.comboBox_pid->addItem(trUtf8("DISPL CTRL"));

    ui.comboBox_pid->setCurrentIndex(0);
    connect(this->ui.comboBox_pid,SIGNAL(currentIndexChanged( int )),this,SLOT(pid_combobox_handle(int)));
    connect(this->ui.pushButton_save_pid,SIGNAL(clicked()),this,SLOT(save_pid_values()));

}
void DC_Motor_PC::init_PIXMAPs(void){
    QPixmap pixmap;
    pixmap.load(QStringLiteral("ADDRESS_LOGO.jpg"));           // load picture
    this->ui.label_utest_address_logo->setPixmap(pixmap);

    pixmap.load(QStringLiteral(":usb_to_device.jpg"));           // load picture
    this->ui.label_copy_from_usb->setPixmap(pixmap);

    pixmap.load(QStringLiteral(":device_to_usb.jpg"));           // load picture
    this->ui.label_copy_to_usb->setPixmap(pixmap);

}
void DC_Motor_PC::init_TIMERs(void){
    _100_msec_timer = new QTimer(this);
    command_send_protection_wait_timer= new QTimer(this);
    gain_send_timer = new QTimer(this);
    load_graphic_timer = new QTimer(this);
    pace_graphic_timer = new QTimer(this);

    _100_msec_timer->setInterval(100);
    _100_msec_timer->start();
    command_send_protection_wait_timer->setInterval(100);
    gain_send_timer->setInterval(100);
    load_graphic_timer->setInterval(200);
    pace_graphic_timer->setInterval(50);

    connect(_100_msec_timer, SIGNAL(timeout()),this,SLOT(_100_msec_handler()));
    connect(sserver, SIGNAL(dataReady(QString)), SLOT(handleTcpMessage(QString)));
    connect(command_send_protection_wait_timer, SIGNAL(timeout()),this,SLOT(command_sending_protection()));
    connect(gain_send_timer, SIGNAL(timeout()),fuzpid,SLOT(send_gains()));
    connect(load_graphic_timer, SIGNAL(timeout()),this,SLOT(draw_load_graphic()));
    connect(pace_graphic_timer, SIGNAL(timeout()),this,SLOT(draw_pace_graphic()));

}
void DC_Motor_PC::init_DIAMETERs(void){
    ui.comboBox_diameter->addItem(QStringLiteral("101.6"));
    ui.comboBox_diameter->addItem(QStringLiteral("152.4"));
    ui.comboBox_diameter->setCurrentIndex(marshall.diameter_number);
    connect(this->ui.comboBox_diameter,SIGNAL(currentIndexChanged( int )),this,SLOT(marshall_diameter_selection_handle(int)));

}
void DC_Motor_PC::init_GUI(void){

#ifdef CONFIG_x86
   set_gradient();
#endif

    current_tab_index = TAB_TEST;
    fuzpid->from_gui.current_tab_index = TAB_TEST;

    control_mode = AUTO;
    ui.label_auto_man->setText(QStringLiteral("AUTO"));
    fuzpid->test_status  = TEST_STOPPED;
    ui.label_test_status->setText(trUtf8("TEST STOPPED.."));
    ui.label_test_status_cbr->setText(trUtf8("TEST STOPPED.."));
    PLOT_first_in = true;
    step_response_status = false;
    fuzpid->tared_val = 0;
    ui.label_alert_status->setText(trUtf8("NO ALERT"));
    ui.label_alert_status_cbr->setText(trUtf8("NO ALERT"));
    ui.label_test_no_main->setText(trUtf8("Test No : %1").arg(current_test_no));
    ui.label_test_no_main_cbr->setText(trUtf8("CBR Test No : %1").arg(current_test_no));
    ui.label_test_no_main_marshall->setText(trUtf8("Test No : %1").arg(current_test_no));

    ui.radioButton_auto->setChecked(1);
    ui.radioButton_record_raw_data_off->setChecked(1);
    ui.pushButton_stop_motor->setDisabled(1);
    ui.pushButton_pause_test->setDisabled(1);

    fuzpid->cal[0].tare_val = 0;
    fuzpid->cal[1].tare_val = 0;
    fuzpid->cal[2].tare_val = 0;
    fuzpid->cal[3].tare_val = 0;
    fuzpid->cal[4].tare_val = 0;
    fuzpid->cal[5].tare_val = 0;
    fuzpid->cal[6].tare_val = 0;

    switch (zoom_radiobutton_no) {
    case FIXED_AXES:
        ui.radioButton_fixed_axes->setChecked(1);
        break;
    case AUTO_ZOOM:
        ui.radioButton_auto_zoom->setChecked(1);
        break;
    default:
        break;
    }
    switch (v_axis_radiobutton_no) {
    case V_AXIS_LOAD:
        ui.radioButton_v_axis_load->setChecked(1);
        plot_ch[0].plot->setAxisTitle(plot_ch[0].plot->yLeft, trUtf8("Load (%1)").arg(unit_system.load_label));
        break;
    case V_AXIS_STRESS:
        ui.radioButton_v_axis_stress->setChecked(1);
        plot_ch[0].plot->setAxisTitle(plot_ch[0].plot->yLeft, trUtf8("Stress (%1)").arg(unit_system.stress_label));
        break;
    case V_AXIS_DISPLACEMENT:
        ui.radioButton_v_axis_displacement->setChecked(1);
        plot_ch[0].plot->setAxisTitle(plot_ch[0].plot->yLeft, trUtf8("Displacement (%1)").arg(unit_system.length_label));
        break;
    default:
        break;
    }
    switch (h_axis_radiobutton_no) {
    case H_AXIS_TIME:
        ui.radioButton_h_axis_time->setChecked(1);
        break;
    case H_AXIS_DISPLACEMENT:
        ui.radioButton_h_axis_displacement->setChecked(1);
        break;
    }

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
    default:
        ui.radioButton_cbr->setChecked(1);
        test_type = CBR;
        test_type_name = trUtf8("CBR");
        break;
    }
    fuzpid->from_gui.test_type = test_type;
    main_screen_arrangements();

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

    ui.label_test_type_specimen->setText(test_type_name);

    switch (marshall.thickness_correction) {
    case 1:
        ui.radioButton_thickness_correction_on->setChecked(1);
        break;
    case 0:
        ui.radioButton_thickness_correction_off->setChecked(1);
        break;
    }

    channel_activations_handler();
    pid_combobox_handle(0);
    marshall_diameter_selection_handle(marshall.diameter_number);

    switch (ch3.unit) {
    case 1:
        ui.radioButton_ch3_kn->setChecked(1);
        ui.label_main_ch3_unit->setText(unit_system.load_label);
        break;
    case 2:
        ui.radioButton_ch3_mm->setChecked(1);
        ui.label_main_ch3_unit->setText(unit_system.length_label);
        break;
    case 3:
        ui.radioButton_ch3_unitless->setChecked(1);
        ui.label_main_ch3_unit->setText(QStringLiteral("  "));
        break;
    default:
        break;
    }
    switch (ch4.unit) {
    case 1:
        ui.radioButton_ch4_kn->setChecked(1);
        ui.label_main_ch4_unit->setText(unit_system.load_label);
        break;
    case 2:
        ui.radioButton_ch4_mm->setChecked(1);
        ui.label_main_ch4_unit->setText(unit_system.length_label);
        break;
    case 3:
        ui.radioButton_ch4_unitless->setChecked(1);
        ui.label_main_ch4_unit->setText(QStringLiteral("  "));
        break;
    default:
        break;
    }

    ui.tabWidget->setCurrentIndex(0);
    ui.tabWidget_parameters->setCurrentIndex(0);
    ui.tabWidget_admin->setCurrentIndex(0);
    ui.tabWidget_gains->setCurrentIndex(0);
    ui.tabWidget_pid->setCurrentIndex(0);
    connect(ui.tabWidget,SIGNAL(currentChanged(int)),this,SLOT(tab_change_handler(int)));
    connect(ui.tabWidget_admin,SIGNAL(currentChanged(int)),this,SLOT(tab_admin_change_handler(int)));

    connect(this->ui.doubleSpinBox_displacement_rate,SIGNAL(valueChanged(double)),this,SLOT(displacement_rate_handler()));
    connect(this->ui.doubleSpinBox_pace_rate,SIGNAL(valueChanged(double)),this,SLOT(pace_rate_handler_kn()));
    connect(this->ui.doubleSpinBox_pace_rate_mpa,SIGNAL(valueChanged(double)),this,SLOT(pace_rate_handler_mpa()));

    connect(this->ui.spinBox_dp_load_1,SIGNAL(valueChanged(int)),this,SLOT(dp_load_change_handler()));
    connect(this->ui.spinBox_dp_load_2,SIGNAL(valueChanged(int)),this,SLOT(dp_load_change_handler()));
    connect(this->ui.spinBox_dp_load_3,SIGNAL(valueChanged(int)),this,SLOT(dp_load_change_handler()));
    connect(this->ui.spinBox_dp_stress,SIGNAL(valueChanged(int)),this,SLOT(dp_stress_change_handler()));
    connect(this->ui.spinBox_dp_ch3,SIGNAL(valueChanged(int)),this,SLOT(dp_ch3_change_handler()));
    connect(this->ui.spinBox_dp_ch4,SIGNAL(valueChanged(int)),this,SLOT(dp_ch4_change_handler()));
    connect(this->ui.spinBox_dp_displacement,SIGNAL(valueChanged(int)),this,SLOT(dp_displacement_change_handler()));

    connect(this->ui.lineEdit_admin_password,SIGNAL(textChanged(QString)),this,SLOT(admin_authorization_handler()));
    connect(this->ui.doubleSpinBox_briquette_t,SIGNAL(valueChanged(double)),this,SLOT(marshall_correction_factor_calculator()));
    marshall_correction_factor_calculator();

    admin_authorization_handler();
    calculate_area_cylindrical();
    ch3_name_handler();
    ch4_name_handler();
    dp_load_change_handler();
    dp_stress_change_handler();
    dp_ch3_change_handler();
    dp_ch4_change_handler();
    dp_displacement_change_handler();
    graphic_v_axis_handler();
    graphic_h_axis_handler();

    ui.spinBox_test_no->setValue(current_test_no);
    read_test_results_from_file();
}
