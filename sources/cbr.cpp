#include "dc_motor_pc.h"

void DC_Motor_PC::cbr_zero_suppression_exceeded(void){
    static u8 tmp = 0;

    switch(tmp){
    case 0:
        ui.pushButton_tare_displacement->setEnabled(1);
        ui.pushButton_tare_displacement->click();
        ui.pushButton_tare_displacement->setDisabled(1);
        QTimer::singleShot(500,this,SLOT(cbr_zero_suppression_exceeded()));
        tmp = 1;
        break;
    case 1:
        load_graphic_timer->start();
        draw_load_graphic();
        fuzpid->run_cbr = true;
        tmp = 0;
        break;
    }
}
void DC_Motor_PC::cbr_points_detection(double l_val, double d_val){
    static bool _2_54_detected = false;
    static bool _5_08_detected = false;
    static double d_2_54_values[2] = {0};
    static double l_2_54_values[2] = {0};
    static double d_5_08_values[2] = {0};
    static double l_5_08_values[2] = {0};

    if(cbr.points_detected == false){
        if(_2_54_detected == false){
            d_2_54_values[0] = d_2_54_values[1];
            d_2_54_values[1] = d_val;
            l_2_54_values[0] = l_2_54_values[1];
            l_2_54_values[1] = l_val;
            //if standart is en 13.2 - 20 kn (bs)
            //if standart is astm 6.9 - 10 Mpa
            switch (unit_system.index){
            case KN_MM:
                if(d_val >= 2.54){
                    _2_54_detected = true;
                    cbr.p_2_54 = (((2.54 - d_2_54_values[1])*(l_2_54_values[1] - l_2_54_values[0]))/(d_2_54_values[1] - d_2_54_values[0]) + l_2_54_values[1]);
                    switch(cbr.standard){
                    case ASTM_STANDARD_CBR:
                        cbr.p_2_54 =  (cbr.p_2_54 * 1000) / (ui.doubleSpinBox_area_cbr->value());
                        cbr.p_2_54 = 100 * cbr.p_2_54 / 6.9;
                        break;
                    case EN_STANDARD_CBR:
                        cbr.p_2_54 = 100 * cbr.p_2_54 / 13.2;
                        break;
                    case BS_STANDARD_CBR:
                        cbr.p_2_54 = 100 * cbr.p_2_54 / 13.2;
                        break;
                    }
                }
                break;
            case LBF_IN:
                if(d_val >= 0.1){
                    _2_54_detected = true;
                    cbr.p_2_54 = (((0.1 - d_2_54_values[1])*(l_2_54_values[1] - l_2_54_values[0]))/(d_2_54_values[1] - d_2_54_values[0]) + l_2_54_values[1]);
                    switch(cbr.standard){
                    case ASTM_STANDARD_CBR:
                        cbr.p_2_54 =  (cbr.p_2_54) / (ui.doubleSpinBox_area_cbr->value());
                        cbr.p_2_54 = 100 * cbr.p_2_54 / 1000;
                        break;
                    case EN_STANDARD_CBR:
                        cbr.p_2_54 = 100 * cbr.p_2_54 / 2967.48;
                        break;
                    case BS_STANDARD_CBR:
                        cbr.p_2_54 = 100 * cbr.p_2_54 / 2967.48;
                        break;
                    }
                }
                break;
            case KGF_CM:
                if(d_val >= 0.254){
                    _2_54_detected = true;
                    cbr.p_2_54 = (((0.254 - d_2_54_values[1])*(l_2_54_values[1] - l_2_54_values[0]))/(d_2_54_values[1] - d_2_54_values[0]) + l_2_54_values[1]);
                    switch(cbr.standard){
                    case ASTM_STANDARD_CBR:
                        cbr.p_2_54 =  (cbr.p_2_54) / (ui.doubleSpinBox_area_cbr->value());
                        cbr.p_2_54 = 100 * cbr.p_2_54 / 70.36;
                        break;
                    case EN_STANDARD_CBR:
                        cbr.p_2_54 = 100 * cbr.p_2_54 / 1346;
                        break;
                    case BS_STANDARD_CBR:
                        cbr.p_2_54 = 100 * cbr.p_2_54 / 1346;
                        break;
                    }
                }
                break;
            default:
                break;
            }
        }
        if(_5_08_detected == false){
            d_5_08_values[0] = d_5_08_values[1];
            d_5_08_values[1] = d_val;
            l_5_08_values[0] = l_5_08_values[1];
            l_5_08_values[1] = l_val;
            //if standart is en 20 kn (bs)
            //if standart is astm 10 Mpa
            switch (unit_system.index){
            case KN_MM:
                if(d_val >= 5.08){
                    _5_08_detected = true;
                    cbr.p_5_08 = (((5.08 - d_5_08_values[1])*(l_5_08_values[1] - l_5_08_values[0]))/(d_5_08_values[1] - d_5_08_values[0]) + l_5_08_values[1]);
                    switch(cbr.standard){
                    case ASTM_STANDARD_CBR:
                        cbr.p_5_08 =  (cbr.p_5_08 * 1000) / (ui.doubleSpinBox_area_cbr->value());
                        cbr.p_5_08 = 100 * cbr.p_5_08 / 10;
                        break;
                    case EN_STANDARD_CBR:
                        cbr.p_5_08 = 100 * cbr.p_5_08 / 20;
                        break;
                    case BS_STANDARD_CBR:
                        cbr.p_5_08 = 100 * cbr.p_5_08 / 20;
                        break;
                    }
                }
                break;
            case LBF_IN:
                if(d_val >= 0.2){
                    _5_08_detected = true;
                    cbr.p_5_08 = (((0.2 - d_5_08_values[1])*(l_5_08_values[1] - l_5_08_values[0]))/(d_5_08_values[1] - d_5_08_values[0]) + l_5_08_values[1]);
                    switch(cbr.standard){
                    case ASTM_STANDARD_CBR:
                        cbr.p_2_54 =  (cbr.p_2_54) / (ui.doubleSpinBox_area_cbr->value());
                        cbr.p_2_54 = 100 * cbr.p_2_54 / 1500;
                        break;
                    case EN_STANDARD_CBR:
                        cbr.p_2_54 = 100 * cbr.p_2_54 / 4496.17;
                        break;
                    case BS_STANDARD_CBR:
                        cbr.p_2_54 = 100 * cbr.p_2_54 / 4496.17;
                        break;
                    }
                }
                break;
            case KGF_CM:
                if(d_val >= 0.508){
                    _5_08_detected = true;
                    cbr.p_5_08 = (((0.508 - d_5_08_values[1])*(l_5_08_values[1] - l_5_08_values[0]))/(d_5_08_values[1] - d_5_08_values[0]) + l_5_08_values[1]);
                    switch(cbr.standard){
                    case ASTM_STANDARD_CBR:
                        cbr.p_2_54 =  (cbr.p_2_54) / (ui.doubleSpinBox_area_cbr->value());
                        cbr.p_2_54 = 100 * cbr.p_2_54 / 101.97;
                        break;
                    case EN_STANDARD_CBR:
                        cbr.p_2_54 = 100 * cbr.p_2_54 / 2039;
                        break;
                    case BS_STANDARD_CBR:
                        cbr.p_2_54 = 100 * cbr.p_2_54 / 2039;
                        break;
                    }
                }
                break;
            default:
                break;
            }
        }
        if((_2_54_detected == true)&&(_5_08_detected == true)){
            cbr.points_detected = true;
        }
    }
    else{
        d_2_54_values[0] = 0;
        d_2_54_values[1] = 0;
        d_5_08_values[0] = 0;
        d_5_08_values[1] = 0;
        l_2_54_values[0] = 0;
        l_2_54_values[1] = 0;
        l_5_08_values[0] = 0;
        l_5_08_values[1] = 0;
        _2_54_detected = false;
        _5_08_detected = false;
    }
}
void DC_Motor_PC::cbr_process(double l_val, double d_val){
    if(d_val >= cbr_point_value[cbr.step]->value()){
        switch (load_calibration_channel) {
        case LOAD_1:
            label_point[cbr.step]->setText(QString::number(l_val,'f',dp.load_1));
            break;
        case LOAD_2:
            label_point[cbr.step]->setText(QString::number(l_val,'f',dp.load_2));
            break;
        case LOAD_3:
            label_point[cbr.step]->setText(QString::number(l_val,'f',dp.load_3));
            break;
        default:
            break;
        }

        ui.label_cbr_step_target->setText(QString(trUtf8("Next step:%1 - %2 %3").arg(cbr.step+2).arg(cbr_point_value[cbr.step+1]->value()).arg(unit_system.length_label)));
        ui.label_cbr_step_load->setText(QString(trUtf8("Step %1 load: %2 %3").arg(cbr.step + 1).arg(label_point[cbr.step]->text()).arg(unit_system.load_label)));

        cbr.step++;
        if(cbr.step > ui.spinBox_test_latest_point->value()){
            fuzpid->test_finished = true;
            fuzpid->run_cbr = false;
        }
    }
    cbr_points_detection(l_val,d_val);
}
void DC_Motor_PC::cbr_load_standard_values(void){
    switch (cbr.standard) {
    case ASTM_STANDARD_CBR:
        ui.spinBox_test_latest_point->setValue(11);
        switch (unit_system.index) {
        case KN_MM:
            for(u8 i = 0; i < 11; i++){
                cbr_point_value[i]->setValue(astm_array[i]);
            }
            break;
        case LBF_IN:
            for(u8 i = 0; i < 11; i++){
                cbr_point_value[i]->setValue(astm_array[i] * 0.0393700787);
            }
            break;
        case KGF_CM:
            for(u8 i = 0; i < 11; i++){
                cbr_point_value[i]->setValue(astm_array[i] * 0.1);
            }
            break;
        default:
            break;
        }
        for(u8 i = 11; i < 40; i++){
            cbr_point_value[i]->setValue(0);
        }
        break;
    case EN_STANDARD_CBR:
        ui.spinBox_test_latest_point->setValue(15);
        switch (unit_system.index) {
        case KN_MM:
            for(u8 i = 0; i < 15; i++){
                cbr_point_value[i]->setValue(en_array[i]);
            }
            break;
        case LBF_IN:
            for(u8 i = 0; i < 15; i++){
                cbr_point_value[i]->setValue(en_array[i] * 0.0393700787);
            }
            break;
        case KGF_CM:
            for(u8 i = 0; i < 15; i++){
                cbr_point_value[i]->setValue(en_array[i] * 0.1);
            }
            break;
        default:
            break;
        }
        for(u8 i = 15; i < 40; i++){
            cbr_point_value[i]->setValue(0);
        }
        break;
    case BS_STANDARD_CBR:
        ui.spinBox_test_latest_point->setValue(30);
        switch (unit_system.index) {
        case KN_MM:
            for(u8 i = 0; i < 30; i++){
                cbr_point_value[i]->setValue(bs_array[i]);
            }
            break;
        case LBF_IN:
            for(u8 i = 0; i < 30; i++){
                cbr_point_value[i]->setValue(bs_array[i] * 0.0393700787);
            }
            break;
        case KGF_CM:
            for(u8 i = 0; i < 30; i++){
                cbr_point_value[i]->setValue(bs_array[i] * 0.1);
            }
            break;
        default:
            break;
        }
        for(u8 i = 30; i < 40; i++){
            cbr_point_value[i]->setValue(0);
        }
        break;
    default:
        break;
    }
    for(u8 i = 0; i < 40; i++){
        label_point[i]->setText("----.---");
    }
}
void DC_Motor_PC::cbr_export_log_to_file(void){
    cbr.log_file.setFileName(QString("test_results/cbr%1").arg(current_test_no) + start_date_filename_arg + ".txt");
    cbr.log_file.open(QIODevice::WriteOnly);
    QTextStream gui_settings(&cbr.log_file);

    gui_settings << trUtf8("*******************************************\n");
    gui_settings << trUtf8("===============  UTEST  ===================\n");
    gui_settings << trUtf8("================  CBR  ====================\n");
    gui_settings << trUtf8("*******************************************\n");

    gui_settings << trUtf8("Start date & time : ") << results.starting_date_time  << "\n";
    gui_settings << trUtf8("Finish time       : ") << results.completion_date_time << "\n";
    gui_settings << trUtf8("Last Point        : ") << ui.spinBox_test_latest_point->value() << "\n";

    for(u8 i = 0; i < ui.spinBox_test_latest_point->value() ; i++){
        gui_settings << QString("P%1 : ").arg(i + 1)
                     << cbr_point_value[i]->value() << " " << unit_system.length_label << "     " << label_point[i]->text() << " " << unit_system.load_label << "\n";
    }
    cbr.log_file.close();
}
void DC_Motor_PC::cbr_show_log(void){
    if(prevent_double_click()) return;

    QDir mydir("test_results");
    mydir.setNameFilters(QStringList() << QString("cbr%1*.txt").arg(ui.spinBox_test_no->value()));
    QStringList mylist = mydir.entryList();
    QString file_name = "";

    if(mylist.length() != 0){
        file_name = mylist[0];
        if(file_name.split("(")[0] == QString("cbr%1").arg(ui.spinBox_test_no->value())){
            file_name = mylist[0];
        }
        else{
            file_name = "";
        }
    }
    else{
        file_name = "";
    }


    QFile cons_file;
    cons_file.setFileName("test_results/"+file_name);

    QString cons_text = "";

    QMessageBox msgBox;
    int line = 0;
    msgBox.setWindowTitle(QString(trUtf8("CBR Points for Test No : %1")).arg(ui.spinBox_test_no->value()));
    if(cons_file.open(QIODevice::ReadOnly)){
        QTextStream in(&cons_file);
        while(!in.atEnd()){
            QString line_str = in.readLine();
            if(line > 3){
                cons_text.append(line_str);
                if(line % 2 == 0)
                    cons_text.append("\n");
                else
                    cons_text.append("  ---  ");
            }
            line++;
        }
    }
    msgBox.setText(cons_text);

    msgBox.setDefaultButton(QMessageBox::Ok);
    QSpacerItem* horizontalSpacer = new QSpacerItem(720, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());//    msgBox.setStandardButtons(QMessageBox::Yes);
    cons_file.close();
    msgBox.exec();

}
void DC_Motor_PC::cbr_standard_type_handler(void){
    if(ui.radioButton_standard_astm_cbr->isChecked()){
        cbr.standard = ASTM_STANDARD_CBR;
        ui.label_main_standard_cbr->setText(trUtf8("Standard: ASTM/AASHTO"));
    }
    else if(ui.radioButton_standard_en_cbr->isChecked()){
        cbr.standard = EN_STANDARD_CBR;
        ui.label_main_standard_cbr->setText(trUtf8("Standard: EN"));
    }
    else if(ui.radioButton_standard_bs_cbr->isChecked()){
        cbr.standard = BS_STANDARD_CBR;
        ui.label_main_standard_cbr->setText(trUtf8("Standard: BS 1377"));
    }
    data_changed = true;
}

