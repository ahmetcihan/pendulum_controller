#include "dc_motor_pc.h"

void DC_Motor_PC::calculate_area_cylindrical(void){

   if(ui.checkBox_precalculated_area->isChecked()){
        calculated_area = ui.doubleSpinBox_precalculated_area->value();
    }
    else{
        if(test_type == IDT){
            calculated_area = M_PI * (ui.doubleSpinBox_specimen_cyl_l->value() * ui.doubleSpinBox_specimen_cyl_d->value())/2.0;
        }
        else if(test_type == SCB){
            calculated_area = (ui.doubleSpinBox_specimen_cyl_d->value()/2 - ui.doubleSpinBox_specimen_scb_notch->value()) * ui.doubleSpinBox_specimen_scb_thickness->value();
        }
        else{
            calculated_area = M_PI * (ui.doubleSpinBox_specimen_cyl_d->value() * ui.doubleSpinBox_specimen_cyl_d->value())/4.0;
        }
    }
    ui.label_area->setText(QString::number(calculated_area,'f',3));
    ui.label_calculated_area->setText(QString::number(calculated_area,'f',3));
    pace_rate_handler_mpa();
}
void DC_Motor_PC::specimen_protection_handler(void){
    if(ui.radioButton_protect_specimen_off->isChecked()){
        JOG.error = false;
        ui.pushButton_speed_up->click();
        ui.pushButton_speed_down->click();
    }
}
void DC_Motor_PC::go_load(void){

    go_load_bit = true;
    ui.pushButton_start_test->setDisabled(1);
    ui.pushButton_stop_motor->setEnabled(1);

    ui.pushButton_start_test->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:start_button_passive.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_stop_motor->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:stop_button.jpg);"
                                                "border-width: 0px ;");
#ifdef CONFIG_x86
    qDebug(__FUNCTION__);
#endif
}
void DC_Motor_PC::go_load_operation(void){
    static float start_load = 0;

    if(go_load_bit == true){
        switch (go_load_tmp) {
        case 0:
            if(fuzpid->LS_up_error == 0){
                fuzpid->step_motor_command = STEPPER_COMMAND_RUN_UP;
                go_load_tmp++;
                speed_correction(parameters[test_type].go_to_load_speed);
                start_load = fuzpid->load_value;
            }
            break;
        case 1:
            go_load_tmp++;
            break;
        case 2:
            if(fuzpid->load_value > (start_load+(float)0.2)){
                fuzpid->step_stop();
                go_load_tmp = 0;
                go_load_bit = false;
                ui.pushButton_start_test->setEnabled(1);
                ui.pushButton_stop_motor->setDisabled(1);
                ui.pushButton_start_test->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                            "border-image: url(:start_button.jpg);"
                                                            "border-width: 0px ;");
                ui.pushButton_stop_motor->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                            "border-image: url(:stop_button_passive.jpg);"
                                                            "border-width: 0px ;");
            }
            break;
        default:
            break;
        }
    }
}
void DC_Motor_PC::return_home(void){

    return_home_bit = true;
    ui.pushButton_start_test->setDisabled(1);
    ui.pushButton_stop_motor->setEnabled(1);
    ui.pushButton_start_test->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:start_button_passive.jpg);"
                                                "border-width: 0px ;");
    ui.pushButton_stop_motor->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:stop_button.jpg);"
                                                "border-width: 0px ;");
#ifdef CONFIG_x86
    qDebug(__FUNCTION__);
#endif
}
void DC_Motor_PC::return_home_operation(void){
    #define approximation_speed 50
    static float initial_load = 0;

    if(return_home_bit == true){
        switch (return_home_tmp) {
        case 0:
            if(fuzpid->displacement_value <= 0){
                return_home_bit = false;
                ui.pushButton_start_test->setEnabled(1);
                ui.pushButton_stop_motor->setDisabled(1);
                ui.pushButton_start_test->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                            "border-image: url(:start_button.jpg);"
                                                            "border-width: 0px ;");
                ui.pushButton_stop_motor->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                            "border-image: url(:stop_button_passive.jpg);"
                                                            "border-width: 0px ;");
            }
            else{
                if(fuzpid->LS_down_error == 0){
                    fuzpid->step_motor_command = STEPPER_COMMAND_RUN_DOWN;
                    return_home_tmp++;
                    speed_correction(approximation_speed);
                }
            }
            break;
        case 1:
            fuzpid->error.go_home_load = false;
            ui.label_alert_status->setText(trUtf8("NO ALERT"));
            initial_load = fuzpid->load_value;
            return_home_tmp++;
            break;
        case 2:
            if(fuzpid->displacement_value > (float)1){
                speed_correction(parameters[test_type].return_home_speed);
            }
            else{
                speed_correction(approximation_speed);
            }
            if(fuzpid->displacement_value <= (float)0.02){
                fuzpid->step_stop();
                return_home_tmp = 0;
                return_home_bit = false;
                ui.pushButton_start_test->setEnabled(1);
                ui.pushButton_stop_motor->setDisabled(1);
                ui.pushButton_start_test->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                            "border-image: url(:start_button.jpg);"
                                                            "border-width: 0px ;");
                ui.pushButton_stop_motor->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                            "border-image: url(:stop_button_passive.jpg);"
                                                            "border-width: 0px ;");
            }
            if(fuzpid->load_value > initial_load + 0.2){    //load error
                fuzpid->step_stop();
                return_home_tmp = 0;
                return_home_bit = false;
                ui.pushButton_start_test->setEnabled(1);
                ui.pushButton_stop_motor->setDisabled(1);
                ui.pushButton_start_test->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                            "border-image: url(:start_button.jpg);"
                                                            "border-width: 0px ;");
                ui.pushButton_stop_motor->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                            "border-image: url(:stop_button_passive.jpg);"
                                                            "border-width: 0px ;");
                fuzpid->error.go_home_load = true;
                ui.label_alert_status->setText(trUtf8("GO HOME Load Error!"));
            }
            break;
        default:
            break;
        }
    }
}
void DC_Motor_PC::displacement_rate_handler(void){
    if(test_type != LOAD_CONTROL){
        ui.label_main_speed->setText(trUtf8("Pace : %1 %2/min").arg(ui.doubleSpinBox_displacement_rate->value()).arg(unit_system.length_label));
        ui.label_speed_cbr->setText(trUtf8("Speed : %1 %2/min").arg(ui.doubleSpinBox_displacement_rate->value()).arg(unit_system.length_label));
        ui.label_speed_marshall->setText(trUtf8("Speed : %1 %2/min").arg(ui.doubleSpinBox_displacement_rate->value()).arg(unit_system.length_label));
    }
}
void DC_Motor_PC::pace_rate_handler_kn(){
    if(test_type == LOAD_CONTROL){
        if(forbid_pace_recalculate_timer.elapsed() > 1000){ // to avoid recalculation mistakes due to math rounding
            forbid_pace_recalculate_timer.restart();
            if(ui.label_calculated_area->text().toDouble() > 0){
                switch(unit_system.index){
                case KN_MM:     //1 Pa = 1 N/m2
                    ui.doubleSpinBox_pace_rate_mpa->setValue((1000 * ui.doubleSpinBox_pace_rate->value()) / ui.label_calculated_area->text().toDouble());
                    break;
                case LBF_IN:    //1 psi = 1 lbf/in2
                    ui.doubleSpinBox_pace_rate_mpa->setValue((ui.doubleSpinBox_pace_rate->value()) / ui.label_calculated_area->text().toDouble());
                    break;
                case KGF_CM:    //kgf/cm2
                    ui.doubleSpinBox_pace_rate_mpa->setValue((ui.doubleSpinBox_pace_rate->value()) / ui.label_calculated_area->text().toDouble());
                    break;
                }
            }
        }
        ui.label_main_speed->setText(trUtf8("Pace : %1 %2/s").arg(ui.doubleSpinBox_pace_rate->value()).arg(unit_system.load_label));
    }
}
void DC_Motor_PC::pace_rate_handler_mpa(){
    if(forbid_pace_recalculate_timer.elapsed() > 1000){
        forbid_pace_recalculate_timer.restart();
        switch(unit_system.index){
        case KN_MM:     //1 Pa = 1 N/m2
            ui.doubleSpinBox_pace_rate->setValue((ui.doubleSpinBox_pace_rate_mpa->value() * ui.label_calculated_area->text().toDouble())/1000.0);
            break;
        case LBF_IN:    //1 psi = 1 lbf/in2
            ui.doubleSpinBox_pace_rate->setValue(ui.doubleSpinBox_pace_rate_mpa->value() * ui.label_calculated_area->text().toDouble());
            break;
        case KGF_CM:    //kgf/cm2
            ui.doubleSpinBox_pace_rate->setValue(ui.doubleSpinBox_pace_rate_mpa->value() * ui.label_calculated_area->text().toDouble());
            break;
        }
    }
}
void DC_Motor_PC::stress_calculation(){

    if(test_type == CBR){
        switch(unit_system.index){
        case KN_MM:     //1 Pa = 1 N/m2
            fuzpid->stress  = (fuzpid->load_value * 1000) / (ui.doubleSpinBox_area_cbr->value());
            break;
        case LBF_IN:    //1 psi = 1 lbf/in2
            fuzpid->stress  = (fuzpid->load_value) / (ui.doubleSpinBox_area_cbr->value());
            break;
        case KGF_CM:    //kgf/cm2
            fuzpid->stress  = (fuzpid->load_value) / (ui.doubleSpinBox_area_cbr->value());
            break;
        }
    }
    else if(test_type == MARSHALL){
        float area;
        area = M_PI * (briquette.d * briquette.d)/4.0;
        fuzpid->stress  = (fuzpid->load_value * 1000) / (area);
    }
    else{
        switch(unit_system.index){
        case KN_MM:     //1 Pa = 1 N/m2
            fuzpid->stress  = (fuzpid->load_value * 1000) / (calculated_area);
            break;
        case LBF_IN:    //1 psi = 1 lbf/in2
            fuzpid->stress  = (fuzpid->load_value) / (calculated_area);
            break;
        case KGF_CM:    //kgf/cm2
            fuzpid->stress  = (fuzpid->load_value) / (calculated_area);
            break;
        }
    }
}
void DC_Motor_PC::motor_go_test_direction(void){
    static u8 tmp = 0;
    static bool running_after_pause = false;

    if(fuzpid->test_status == TEST_PAUSED){
        running_after_pause = true;
    }
    switch(tmp){
    case 0:
        if((fuzpid->error.max_load == false)&&(fuzpid->error.max_displacement == false)){
            if(fuzpid->error.go_home_load){
                fuzpid->error.go_home_load = false;
                ui.label_alert_status->setText(trUtf8("NO ALERT"));
            }
            if(running_after_pause == false){
                reset_plot_load();
                marshall.latest_x = 0;
            }
            results.starting_date_time = QDate::currentDate().toString("dd.MM.yyyy") + " " + QTime::currentTime().toString();
            QTimer::singleShot(150,this,SLOT(motor_go_test_direction()));
            tmp++;
        }
        break;
    case 1:
        QTimer::singleShot(150,this,SLOT(motor_go_test_direction()));
        tmp++;

        PLOT_first_in = true;
        cbr.points_detected = false;
        cbr.step = 0;
        ui.label_cbr_step_target->setText(trUtf8("Next step:1 - %1 %2").arg(cbr_point_value[0]->value()).arg(unit_system.length_label));
        ui.label_cbr_step_load->setText(trUtf8("Step 0 load: 0 %1").arg(unit_system.load_label));
        ui.label_test_status_cbr->setText(trUtf8("TEST is RUNNING.."));

        ui.label_test_status->setText(trUtf8("TEST is RUNNING.."));
        ui.pushButton_tare_load->setDisabled(1);
        ui.pushButton_tare_displacement->setDisabled(1);
        ui.pushButton_tare_ch3->setDisabled(1);
        ui.pushButton_tare_ch4->setDisabled(1);
        ui.pushButton_start_test->setDisabled(1);
        ui.pushButton_stop_motor->setEnabled(1);
        ui.pushButton_pause_test->setEnabled(1);

        fuzpid->relay_start_stop = RELAY_ON;

        admin_authorization = false;
        ui.label_main_calibration->setStyleSheet("color : rgb(96,96,96); border: none;");
        ui.label_main_admin->setStyleSheet("color : rgb(96,96,96); border: none;");
        ui.pushButton_go_main_screen->setDisabled(1);
        ui.pushButton_go_main_screen->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                    "border-image: url(:go_main_screen_button_passive.jpg);"
                                                    "border-width: 0px ;");

        ui.pushButton_start_test->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                    "border-image: url(:start_button_passive.jpg);"
                                                    "border-width: 0px ;");
        ui.pushButton_pause_test->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                    "border-image: url(:pause_button.jpg);"
                                                    "border-width: 0px ;");
        ui.pushButton_stop_motor->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                    "border-image: url(:stop_button.jpg);"
                                                    "border-width: 0px ;");
        fuzpid->step_motor_command = STEPPER_COMMAND_RUN_UP;
        break;
    case 2:
        QTimer::singleShot(150,this,SLOT(motor_go_test_direction()));
        speed_correction(ui.doubleSpinBox_test_start_speed->value());
        tmp++;
        break;
    case 3:
        fuzpid->test_status  = TEST_RUNNING;
        tmp = 0;
        if(running_after_pause == false){
            current_test_no++;
            if(current_test_no > 999) current_test_no = 0;
            ui.label_test_no_main->setText(trUtf8("Test No : %1").arg(current_test_no));
            ui.label_test_no_main_cbr->setText(trUtf8("CBR Test No : %1").arg(current_test_no));
            ui.label_test_no_main_marshall->setText(trUtf8("Test No : %1").arg(current_test_no));

            results.starting_date_time = QDate::currentDate().toString("dd.MM.yyyy") + " " + QTime::currentTime().toString();
            ui.spinBox_test_numerator->setValue(ui.spinBox_test_numerator->value() + 1);
            start_date_filename_arg = "(" + QString::number(QDate::currentDate().day()) + "_"
                                            + QString::number(QDate::currentDate().month()) + "_"
                                            + QString::number(QDate::currentDate().year()) + "-"
                                            + QString::number(QTime::currentTime().hour()) + "_"
                                            + QString::number(QTime::currentTime().minute()) + "_"
                                            + QString::number(QTime::currentTime().second()) + ")";

            if(ui.radioButton_record_raw_data_on->isChecked()){
                excel_file.setFileName(QString("data_logs/excel_log_%1").arg(current_test_no) + start_date_filename_arg + ".csv");
                excel_file.open(QIODevice::WriteOnly);

                QTextStream datum(&excel_file);
                datum << trUtf8("Time (s)") << "," << trUtf8("LOAD (%1)").arg(unit_system.load_label) << "," << trUtf8("STRESS (%1)").arg(unit_system.stress_label) << "\n";
            }
        }
        running_after_pause = false;
        break;
    }
#ifdef CONFIG_x86
    qDebug(__FUNCTION__);
#endif
}
void DC_Motor_PC::pause_test(void){
    if(prevent_double_click()) return;
    if(fuzpid->test_status == TEST_RUNNING){
        fuzpid->test_status = TEST_PAUSED;
        ui.pushButton_start_test->setEnabled(1);
        ui.pushButton_start_test->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                    "border-image: url(:start_button.jpg);"
                                                    "border-width: 0px ;");
        ui.label_test_status->setText(trUtf8("TEST is PAUSED"));
        ui.label_test_status_cbr->setText(trUtf8("TEST is PAUSED"));
        fuzpid->step_stop();
    }
}
void DC_Motor_PC::motor_stop(void){
    if(return_home_bit == false){
        if(go_load_bit == false){
            fuzpid->test_finished = true;
        }
        else{
            fuzpid->step_stop();
            go_load_tmp = 0;
            go_load_bit = false;
            ui.pushButton_start_test->setEnabled(1);
            ui.pushButton_stop_motor->setDisabled(1);
            ui.pushButton_start_test->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                        "border-image: url(:start_button.jpg);"
                                                        "border-width: 0px ;");
            ui.pushButton_stop_motor->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                        "border-image: url(:stop_button_passive.jpg);"
                                                        "border-width: 0px ;");
        }
    }
    else{
        fuzpid->step_stop();
        return_home_tmp = 0;
        return_home_bit = false;
        ui.pushButton_start_test->setEnabled(1);
        ui.pushButton_stop_motor->setDisabled(1);
        ui.pushButton_start_test->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                    "border-image: url(:start_button.jpg);"
                                                    "border-width: 0px ;");
        ui.pushButton_stop_motor->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                    "border-image: url(:stop_button_passive.jpg);"
                                                    "border-width: 0px ;");
    }
    fuzpid->run_cbr = false;
}
void DC_Motor_PC::relay_on(void){
    fuzpid->relay_start_stop = RELAY_ON;
#ifdef CONFIG_x86
    qDebug(__FUNCTION__);
#endif
}
void DC_Motor_PC::relay_off(void){
    fuzpid->relay_start_stop = RELAY_OFF;
#ifdef CONFIG_x86
    qDebug(__FUNCTION__);
#endif
}
