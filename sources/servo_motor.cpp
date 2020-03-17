#include "dc_motor_pc.h"

double DC_Motor_PC::speed_correction(double val){
    double multiplier = 1;

    if(val < ui.doubleSpinBox_region_1_limit->value()){
        multiplier = ui.doubleSpinBox_factor_1->value();
    }
    else if(val < ui.doubleSpinBox_region_2_limit->value()){
        multiplier = ui.doubleSpinBox_factor_2->value();
    }
    else{
        multiplier = ui.doubleSpinBox_factor_3->value();
    }
    //qDebug() << "servo speed" << fuzpid->fuzzy_raw_servo_speed << "val" << val;

    fuzpid->fuzzy_raw_servo_speed = multiplier *(1350000.0 / (((double) ui.doubleSpinBox_motor_rpm->value() / (double) ui.doubleSpinBox_displ_speed->value() * val)));
    fuzpid->send_fuzzy_raw_servo_speed = 1;
    return fuzpid->fuzzy_raw_servo_speed;
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
                reset_plot();
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
        ui.tabWidget->setTabEnabled(TAB_PARAMETERS,0);
        ui.tabWidget->setTabEnabled(TAB_TEST_RESULTS,0);
        ui.tabWidget->setTabEnabled(TAB_SETTINGS,0);
        ui.tabWidget->setTabEnabled(TAB_CALIBRATION,0);
        ui.tabWidget->setTabEnabled(TAB_ADMIN,0);

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
        servo.down = 1;
        break;
    case 2:
        QTimer::singleShot(150,this,SLOT(motor_go_test_direction()));
        speed_correction(ui.doubleSpinBox_test_start_speed->value());
        tmp++;
        break;
    case 3:
        servo.start = 1;
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
        servo.stop = 1;
    }
}
void DC_Motor_PC::motor_stop(void){
    if(return_home_bit == false){
        if(go_load_bit == false){
            fuzpid->test_finished = true;
        }
        else{
            servo.stop = 1;
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
        servo.stop = 1;
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
