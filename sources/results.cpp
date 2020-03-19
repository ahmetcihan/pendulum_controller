#include "dc_motor_pc.h"

void DC_Motor_PC::result_no_left(void){
    static u8 tmp = 0;
    switch(tmp){
    case 0:
        ui.spinBox_test_no->setValue(ui.spinBox_test_no->value() - 1);
        read_test_results_from_file();
        QTimer::singleShot(1000,this,SLOT(result_no_left()));
        ui.pushButton_result_left->setText("...");
        ui.pushButton_result_right->setText("...");
        ui.pushButton_result_left->setDisabled(1);
        ui.pushButton_result_right->setDisabled(1);
        tmp++;
        break;
    case 1:
        tmp = 0;
        ui.pushButton_result_left->setText("-");
        ui.pushButton_result_right->setText("+");
        ui.pushButton_result_left->setEnabled(1);
        ui.pushButton_result_right->setEnabled(1);
        break;
    }
}
void DC_Motor_PC::result_no_right(void){
    static u8 tmp = 0;
    switch(tmp){
    case 0:
        ui.spinBox_test_no->setValue(ui.spinBox_test_no->value() + 1);
        read_test_results_from_file();
        QTimer::singleShot(1000,this,SLOT(result_no_right()));
        ui.pushButton_result_left->setText("...");
        ui.pushButton_result_right->setText("...");
        ui.pushButton_result_left->setDisabled(1);
        ui.pushButton_result_right->setDisabled(1);
        tmp++;
        break;
    case 1:
        tmp = 0;
        ui.pushButton_result_left->setText("-");
        ui.pushButton_result_right->setText("+");
        ui.pushButton_result_left->setEnabled(1);
        ui.pushButton_result_right->setEnabled(1);
        break;
    }
}
void DC_Motor_PC::read_test_results_from_file(void){
    QDir mydir("test_results");
    mydir.setNameFilters(QStringList() << QString("test_%1*.txt").arg(ui.spinBox_test_no->value()));
    QStringList mylist = mydir.entryList();
    QString file_name = "";

    if(mylist.length() != 0){
        file_name = mylist[0];
        if(file_name.split("(")[0] == QString("test_%1").arg(ui.spinBox_test_no->value())){
            file_name = mylist[0];
        }
        else{
            file_name = "";
        }
    }
    else{
        file_name = "";
    }

    QFile test_res;
    test_res.setFileName("test_results/"+file_name);
    test_res.open(QIODevice::ReadOnly);
    QTextStream lines(&test_res);

    lines.readLine();
    lines.readLine();
    lines.readLine();
    ui.lineEdit_0->setText(lines.readLine().section(":",1));
    ui.lineEdit_1->setText(lines.readLine().section(":",1));
    ui.lineEdit_2->setText(lines.readLine().section(":",1));
    ui.lineEdit_3->setText(lines.readLine().section(":",1));
    ui.label_results_value_test_no->setText(lines.readLine().section(":",1));
    ui.label_results_value_starting_date_time->setText(lines.readLine().section(trUtf8("Starting Date & Time :"),1));
    ui.label_results_value_completion_data_time->setText(lines.readLine().section(trUtf8("Completion Date & Time :"),1));
    ui.spinBox_test_numerator->setValue(lines.readLine().section(":",1).toInt());
    ui.label_results_value_test_type->setText(lines.readLine().section(":",1));
    ui.label_results_value_test_speed->setText(lines.readLine().section(":",1));

    ui.label_results_name_area->hide();
    ui.label_results_value_area->hide();
    ui.label_results_name_cbr_2_54->hide();
    ui.label_results_value_cbr_2_54->hide();
    ui.label_results_name_cbr_5_08->hide();
    ui.label_results_value_cbr_5_08->hide();
    ui.label_results_name_dimensions->hide();
    ui.label_results_value_dimensions->hide();
    ui.label_results_name_displacement_at_peak_load->hide();
    ui.label_results_value_displacement_at_peak_load->hide();
    ui.label_results_name_flow_peak_method->hide();
    ui.label_results_value_flow_peak_method->hide();
    ui.label_results_name_flow_tangent_offset->hide();
    ui.label_results_value_flow_tangent_offset->hide();
    ui.label_results_name_peak_load->hide();
    ui.label_results_value_peak_load->hide();
    ui.label_results_name_peak_stress->hide();
    ui.label_results_value_peak_stress->hide();
    ui.label_results_name_stability_peak_method->hide();
    ui.label_results_value_stability_peak_method->hide();
    ui.label_results_name_stability_tangent_offset->hide();
    ui.label_results_value_stability_tangent_offset->hide();
    ui.label_results_name_standard->hide();
    ui.label_results_value_standard->hide();
    ui.label_results_name_strength->hide();
    ui.label_results_value_strength->hide();
    ui.label_results_name_thickness_correction->hide();
    ui.label_results_value_thickness_correction->hide();
    ui.pushButton_show_cbr_points->hide();
    ui.label_results_name_ch3->hide();
    ui.label_results_value_ch3->hide();
    ui.label_results_name_scb_notch->hide();
    ui.label_results_value_scb_notch->hide();
    ui.label_results_name_scb_thickness->hide();
    ui.label_results_value_scb_thickness->hide();
    ui.label_results_name_scb_max_displacement->hide();
    ui.label_results_value_scb_max_displacement->hide();

    if(ui.label_results_value_test_type->text() == "LOAD CTRL."){
        ui.label_results_name_test_speed->setText(trUtf8("Test Speed (%1/s) :").arg(unit_system.load_label));
    }
    else{
        ui.label_results_name_test_speed->setText(trUtf8("Test Speed (%1/min) :").arg(unit_system.length_label));
    }

    if(ui.label_results_value_test_type->text() == "CBR"){
        ui.label_results_name_peak_load->show();
        ui.label_results_value_peak_load->show();
        ui.label_results_name_peak_load->setGeometry(410,75,230,30);
        ui.label_results_value_peak_load->setGeometry(640,75,150,30);

        ui.label_results_name_peak_stress->show();
        ui.label_results_value_peak_stress->show();
        ui.label_results_name_peak_stress->setGeometry(410,110,230,30);
        ui.label_results_value_peak_stress->setGeometry(640,110,150,30);

        ui.label_results_name_cbr_2_54->show();
        ui.label_results_value_cbr_2_54->show();
        ui.label_results_name_cbr_2_54->setGeometry(410,145,230,30);
        ui.label_results_value_cbr_2_54->setGeometry(640,145,150,30);

        ui.label_results_name_cbr_5_08->show();
        ui.label_results_value_cbr_5_08->show();
        ui.label_results_name_cbr_5_08->setGeometry(410,180,230,30);
        ui.label_results_value_cbr_5_08->setGeometry(640,180,150,30);

        ui.label_results_name_standard->show();
        ui.label_results_value_standard->show();
        ui.label_results_name_standard->setGeometry(410,215,230,30);
        ui.label_results_value_standard->setGeometry(640,215,150,30);

        ui.label_results_name_area->show();
        ui.label_results_value_area->show();
        ui.label_results_name_area->setGeometry(410,250,230,30);
        ui.label_results_value_area->setGeometry(640,250,150,30);

        ui.pushButton_show_cbr_points->show();

        ui.label_results_value_peak_load->setText(lines.readLine().section(":",1));
        ui.label_results_value_peak_stress->setText(lines.readLine().section(":",1));
        ui.label_results_value_cbr_2_54->setText(lines.readLine().section(":",1));
        ui.label_results_value_cbr_5_08->setText(lines.readLine().section(":",1));
        ui.label_results_value_standard->setText(lines.readLine().section(":",1));
        ui.label_results_value_area->setText(lines.readLine().section(":",1));

    }
    else if(ui.label_results_value_test_type->text() == "MARSHALL"){
        ui.label_results_name_thickness_correction->show();
        ui.label_results_value_thickness_correction->show();
        ui.label_results_name_thickness_correction->setGeometry(5,320,250,30);
        ui.label_results_value_thickness_correction->setGeometry(255,320,130,30);

        ui.label_results_name_standard->show();
        ui.label_results_value_standard->show();
        ui.label_results_name_standard->setGeometry(410,75,230,30);
        ui.label_results_value_standard->setGeometry(640,75,150,30);

        ui.label_results_name_peak_load->show();
        ui.label_results_value_peak_load->show();
        ui.label_results_name_peak_load->setGeometry(410,110,230,30);
        ui.label_results_value_peak_load->setGeometry(640,110,150,30);

        ui.label_results_name_displacement_at_peak_load->show();
        ui.label_results_value_displacement_at_peak_load->show();
        ui.label_results_name_displacement_at_peak_load->setGeometry(410,145,230,30);
        ui.label_results_value_displacement_at_peak_load->setGeometry(640,145,150,30);

        ui.label_results_name_dimensions->show();
        ui.label_results_value_dimensions->show();
        ui.label_results_name_dimensions->setGeometry(410,180,230,30);
        ui.label_results_value_dimensions->setGeometry(640,180,150,30);

        ui.label_results_name_flow_peak_method->show();
        ui.label_results_value_flow_peak_method->show();
        ui.label_results_name_flow_peak_method->setGeometry(410,215,310,30);
        ui.label_results_value_flow_peak_method->setGeometry(720,215,70,30);

        ui.label_results_name_flow_tangent_offset->show();
        ui.label_results_value_flow_tangent_offset->show();
        ui.label_results_name_flow_tangent_offset->setGeometry(410,250,310,30);
        ui.label_results_value_flow_tangent_offset->setGeometry(720,250,70,30);

        ui.label_results_name_stability_peak_method->show();
        ui.label_results_value_stability_peak_method->show();
        ui.label_results_name_stability_peak_method->setGeometry(410,285,310,30);
        ui.label_results_value_stability_peak_method->setGeometry(720,285,70,30);

        ui.label_results_name_stability_tangent_offset->show();
        ui.label_results_value_stability_tangent_offset->show();
        ui.label_results_name_stability_tangent_offset->setGeometry(410,320,310,30);
        ui.label_results_value_stability_tangent_offset->setGeometry(720,320,70,30);

        ui.label_results_value_thickness_correction->setText(lines.readLine().section(":",1));
        ui.label_results_value_standard->setText(lines.readLine().section(":",1));
        ui.label_results_value_peak_load->setText(lines.readLine().section(":",1));
        ui.label_results_value_displacement_at_peak_load->setText(lines.readLine().section(":",1));
        ui.label_results_value_dimensions->setText(lines.readLine().section(":",1));
        ui.label_results_value_flow_peak_method->setText(lines.readLine().section(":",1));
        ui.label_results_value_flow_tangent_offset->setText(lines.readLine().section(":",1));
        ui.label_results_value_stability_peak_method->setText(lines.readLine().section(":",1));
        ui.label_results_value_stability_tangent_offset->setText(lines.readLine().section(":",1));
    }
    else if(ui.label_results_value_test_type->text() == "IDT"){
        ui.label_results_name_peak_load->show();
        ui.label_results_value_peak_load->show();
        ui.label_results_name_peak_load->setGeometry(410,75,230,30);
        ui.label_results_value_peak_load->setGeometry(640,75,150,30);

        ui.label_results_name_peak_stress->show();
        ui.label_results_value_peak_stress->show();
        ui.label_results_name_peak_stress->setGeometry(410,110,230,30);
        ui.label_results_value_peak_stress->setGeometry(640,110,150,30);

        ui.label_results_name_strength->show();
        ui.label_results_value_strength->show();
        ui.label_results_name_strength->setGeometry(410,145,230,30);
        ui.label_results_value_strength->setGeometry(640,145,150,30);

        ui.label_results_name_dimensions->show();
        ui.label_results_value_dimensions->show();
        ui.label_results_name_dimensions->setGeometry(410,180,230,30);
        ui.label_results_value_dimensions->setGeometry(640,180,150,30);

        ui.label_results_name_area->show();
        ui.label_results_value_area->show();
        ui.label_results_name_area->setGeometry(410,215,230,30);
        ui.label_results_value_area->setGeometry(640,215,150,30);

        ui.label_results_value_peak_load->setText(lines.readLine().section(":",1));
        ui.label_results_value_peak_stress->setText(lines.readLine().section(":",1));
        ui.label_results_value_strength->setText(lines.readLine().section(":",1));
        ui.label_results_value_dimensions->setText(lines.readLine().section(":",1));
        ui.label_results_value_area->setText(lines.readLine().section(":",1));
    }
    else if(ui.label_results_value_test_type->text() == "SCB"){
        ui.label_results_name_peak_load->show();
        ui.label_results_value_peak_load->show();
        ui.label_results_name_peak_load->setGeometry(410,75,230,30);
        ui.label_results_value_peak_load->setGeometry(640,75,150,30);

        ui.label_results_name_peak_stress->show();
        ui.label_results_value_peak_stress->show();
        ui.label_results_name_peak_stress->setGeometry(410,110,230,30);
        ui.label_results_value_peak_stress->setGeometry(640,110,150,30);

        ui.label_results_name_scb_notch->show();
        ui.label_results_value_scb_notch->show();
        ui.label_results_name_scb_notch->setGeometry(410,145,230,30);
        ui.label_results_value_scb_notch->setGeometry(640,145,150,30);

        ui.label_results_name_scb_thickness->show();
        ui.label_results_value_scb_thickness->show();
        ui.label_results_name_scb_thickness->setGeometry(410,180,230,30);
        ui.label_results_value_scb_thickness->setGeometry(640,180,150,30);

        ui.label_results_name_scb_max_displacement->show();
        ui.label_results_value_scb_max_displacement->show();
        ui.label_results_name_scb_max_displacement->setGeometry(410,215,230,30);
        ui.label_results_value_scb_max_displacement->setGeometry(640,215,150,30);

        ui.label_results_name_area->show();
        ui.label_results_value_area->show();
        ui.label_results_name_area->setGeometry(410,250,230,30);
        ui.label_results_value_area->setGeometry(640,250,150,30);

        ui.label_results_value_peak_load->setText(lines.readLine().section(":",1));
        ui.label_results_value_peak_stress->setText(lines.readLine().section(":",1));
        ui.label_results_value_scb_notch->setText(lines.readLine().section(":",1));
        ui.label_results_value_scb_thickness->setText(lines.readLine().section(":",1));
        ui.label_results_value_scb_max_displacement->setText(lines.readLine().section(":",1));
        ui.label_results_value_area->setText(lines.readLine().section(":",1));
    }
    else if((ui.label_results_value_test_type->text() == "TRIAXIAL")){

        ui.label_results_name_peak_load->show();
        ui.label_results_value_peak_load->show();
        ui.label_results_name_peak_load->setGeometry(410,75,230,30);
        ui.label_results_value_peak_load->setGeometry(640,75,150,30);

        ui.label_results_name_peak_stress->show();
        ui.label_results_value_peak_stress->show();
        ui.label_results_name_peak_stress->setGeometry(410,110,230,30);
        ui.label_results_value_peak_stress->setGeometry(640,110,150,30);

        ui.label_results_name_dimensions->show();
        ui.label_results_value_dimensions->show();
        ui.label_results_name_dimensions->setGeometry(410,145,230,30);
        ui.label_results_value_dimensions->setGeometry(640,145,150,30);

        ui.label_results_name_area->show();
        ui.label_results_value_area->show();
        ui.label_results_name_area->setGeometry(410,180,230,30);
        ui.label_results_value_area->setGeometry(640,180,150,30);

        ui.label_results_value_peak_load->setText(lines.readLine().section(":",1));
        ui.label_results_value_peak_stress->setText(lines.readLine().section(":",1));
        ui.label_results_value_dimensions->setText(lines.readLine().section(":",1));
        ui.label_results_value_area->setText(lines.readLine().section(":",1));
    }
    else if((ui.label_results_value_test_type->text() == "UU")||(ui.label_results_value_test_type->text() == "CU-CD")||(ui.label_results_value_test_type->text() == "UNIAXIAL")){

        ui.label_results_name_peak_load->show();
        ui.label_results_value_peak_load->show();
        ui.label_results_name_peak_load->setGeometry(410,75,230,30);
        ui.label_results_value_peak_load->setGeometry(640,75,150,30);

        ui.label_results_name_peak_stress->show();
        ui.label_results_value_peak_stress->show();
        ui.label_results_name_peak_stress->setGeometry(410,110,230,30);
        ui.label_results_value_peak_stress->setGeometry(640,110,150,30);

        ui.label_results_name_dimensions->show();
        ui.label_results_value_dimensions->show();
        ui.label_results_name_dimensions->setGeometry(410,145,230,30);
        ui.label_results_value_dimensions->setGeometry(640,145,150,30);

        ui.label_results_name_area->show();
        ui.label_results_value_area->show();
        ui.label_results_name_area->setGeometry(410,180,230,30);
        ui.label_results_value_area->setGeometry(640,180,150,30);

        ui.label_results_name_ch3->show();
        ui.label_results_value_ch3->show();
        ui.label_results_name_ch3->setGeometry(410,215,230,30);
        ui.label_results_value_ch3->setGeometry(640,215,150,30);

        ui.label_results_value_peak_load->setText(lines.readLine().section(":",1));
        ui.label_results_value_peak_stress->setText(lines.readLine().section(":",1));
        ui.label_results_value_dimensions->setText(lines.readLine().section(":",1));
        ui.label_results_value_area->setText(lines.readLine().section(":",1));
        QString str = lines.readLine();
        ui.label_results_name_ch3->setText(str.section(":",0,0));
        ui.label_results_value_ch3->setText(str.section(":",1));
    }
    else if((ui.label_results_value_test_type->text() == "DISPL. CTRL.")||(ui.label_results_value_test_type->text() == "LOAD CTRL.")){

        ui.label_results_name_peak_load->show();
        ui.label_results_value_peak_load->show();
        ui.label_results_name_peak_load->setGeometry(410,75,230,30);
        ui.label_results_value_peak_load->setGeometry(640,75,150,30);

        ui.label_results_name_peak_stress->show();
        ui.label_results_value_peak_stress->show();
        ui.label_results_name_peak_stress->setGeometry(410,110,230,30);
        ui.label_results_value_peak_stress->setGeometry(640,110,150,30);

        ui.label_results_name_dimensions->show();
        ui.label_results_value_dimensions->show();
        ui.label_results_name_dimensions->setGeometry(410,145,230,30);
        ui.label_results_value_dimensions->setGeometry(640,145,150,30);

        ui.label_results_name_area->show();
        ui.label_results_value_area->show();
        ui.label_results_name_area->setGeometry(410,180,230,30);
        ui.label_results_value_area->setGeometry(640,180,150,30);

        ui.label_results_value_peak_load->setText(lines.readLine().section(":",1));
        ui.label_results_value_peak_stress->setText(lines.readLine().section(":",1));
        ui.label_results_value_dimensions->setText(lines.readLine().section(":",1));
        ui.label_results_value_area->setText(lines.readLine().section(":",1));
    }

    ui.label_results_value_current_test_no->setText(QString::number(current_test_no));
    test_res.close();
}
void DC_Motor_PC::write_test_results_to_file(int no){
    QFile test_res;
    test_res.setFileName(QString("test_results/test_%1").arg(no) + start_date_filename_arg + ".txt");
    test_res.open(QIODevice::WriteOnly);
    QTextStream lines(&test_res);

    lines << trUtf8("*****************************************************\n");
    lines << trUtf8("==================== TEST RESULTS ===================\n");
    lines << trUtf8("*****************************************************\n");
    lines << trUtf8("Line 1 :") << ui.lineEdit_0->text() << "\n";
    lines << trUtf8("Line 2 :") << ui.lineEdit_1->text() << "\n";
    lines << trUtf8("Line 3 :") << ui.lineEdit_2->text() << "\n";
    lines << trUtf8("Line 4 :") << ui.lineEdit_3->text() << "\n";
    lines << trUtf8("Test No :") << no << "\n";
    lines << trUtf8("Starting Date & Time :") << results.starting_date_time << "\n";
    lines << trUtf8("Completion Date & Time :") << results.completion_date_time << "\n";
    lines << trUtf8("Numerator :") << ui.spinBox_test_numerator->value() << "\n";
    lines << trUtf8("Test Type :") << results.test_type << "\n";
    if(results.test_type == "LOAD CTRL."){
        lines << trUtf8("Test Speed (%1/s) :").arg(unit_system.load_label) << results.test_speed << "\n";
    }
    else{
        lines << trUtf8("Test Speed (%1/min) :").arg(unit_system.length_label) << results.test_speed << "\n";
    }

    if(results.test_type == "CBR"){
        lines << trUtf8("Peak Load (%1) :").arg(unit_system.load_label) << results.peak_load << "\n";
        lines << trUtf8("Peak Stress (%1) :").arg(unit_system.stress_label) << results.peak_stress << "\n";
        lines << trUtf8("CBR 2.54 :") << results.CBR_2_54 << "\n";
        lines << trUtf8("CBR 5.08 :") << results.CBR_5_08 << "\n";
        lines << trUtf8("Standard :") << results.standard << "\n";
        lines << trUtf8("Area (%1%2) :").arg(unit_system.length_label,"2") << results.area << "\n";
    }
    else if(results.test_type == "MARSHALL"){
        lines << trUtf8("Thickness correction :") << results.thickness_correction << "\n";
        lines << trUtf8("Standard :") << results.standard << "\n";
        lines << trUtf8("Peak Load (%1) :").arg(unit_system.load_label) << results.peak_load << "\n";
        lines << trUtf8("Peak l. Displ. (%1) :").arg(unit_system.length_label) << results.displacement_at_peak_load << "\n";
        lines << trUtf8("Dimensions (%1) :").arg(unit_system.length_label) << results.dimensions << "\n";
        lines << trUtf8("Flow (%1) [PEAK METHOD] :").arg(unit_system.length_label) << results.flow_peak_method << "\n";
        lines << trUtf8("Flow (%1) [TANG. OFFSET] :").arg(unit_system.length_label) << results.flow_tangent_offset << "\n";
        lines << trUtf8("Stability (%1) [PEAK METHOD] :").arg(unit_system.load_label) << results.stability_peak_method << "\n";
        lines << trUtf8("Stability (%1) [TANG. OFFSET] :").arg(unit_system.load_label) << results.stability_tangent_offset << "\n";
    }
    else if(results.test_type == "IDT"){
        lines << trUtf8("Peak Load (%1) :").arg(unit_system.load_label) << results.peak_load << "\n";
        lines << trUtf8("Peak Stress (%1) :").arg(unit_system.stress_label) << results.peak_stress << "\n";
        lines << trUtf8("Strength (%1) :").arg(unit_system.stress_label) << results.strength << "\n";
        lines << trUtf8("Dimensions (%1) :").arg(unit_system.length_label) << results.dimensions << "\n";
        lines << trUtf8("Area (%1%2) :").arg(unit_system.length_label,"2") << results.area << "\n";
    }
    else if(results.test_type == "SCB"){
        lines << trUtf8("Peak Load (%1) :").arg(unit_system.load_label) << results.peak_load << "\n";
        lines << trUtf8("Peak Stress (%1) :").arg(unit_system.stress_label) << results.peak_stress << "\n";
        lines << trUtf8("NOTCH (%1) :").arg(unit_system.length_label) << ui.doubleSpinBox_specimen_scb_notch->value() << "\n";
        lines << trUtf8("THICKNESS (%1) :").arg(unit_system.length_label) << ui.doubleSpinBox_specimen_scb_thickness->value() << "\n";
        lines << trUtf8("MAX DISPL. (%1) :").arg(unit_system.length_label) << results.max_displacement << "\n";
        lines << trUtf8("Area (%1%2) :").arg(unit_system.length_label,"2") << results.area << "\n";
    }
    else if((results.test_type == "TRIAXIAL")){
        lines << trUtf8("Peak Load (%1) :").arg(unit_system.load_label) << results.peak_load << "\n";
        lines << trUtf8("Peak Stress (%1) :").arg(unit_system.stress_label) << results.peak_stress << "\n";
        lines << trUtf8("Dimensions (%1) :").arg(unit_system.length_label) << results.dimensions << "\n";
        lines << trUtf8("Area (%1%2) :").arg(unit_system.length_label,"2") << results.area << "\n";
    }
    else if((results.test_type == "UU")||(results.test_type == "CU-CD")||(results.test_type == "UNIAXIAL")){
        lines << trUtf8("Peak Load (%1) :").arg(unit_system.load_label) << results.peak_load << "\n";
        lines << trUtf8("Peak Stress (%1) :").arg(unit_system.stress_label) << results.peak_stress << "\n";
        lines << trUtf8("Dimensions (%1) :").arg(unit_system.length_label) << results.dimensions << "\n";
        lines << trUtf8("Area (%1%2) :").arg(unit_system.length_label,"2") << results.area << "\n";
        lines << ch3.label + QString(" (%1) :").arg(ui.label_main_ch3_unit->text()) << ui.label_main_ch3->text() << "\n";
    }
    else if((results.test_type == "DISPL. CTRL.")||(results.test_type == "LOAD CTRL.")){
        lines << trUtf8("Peak Load (%1) :").arg(unit_system.load_label) << results.peak_load << "\n";
        lines << trUtf8("Peak Stress (%1) :").arg(unit_system.stress_label) << results.peak_stress << "\n";
        lines << trUtf8("Dimensions (%1) :").arg(unit_system.length_label) << results.dimensions << "\n";
        lines << trUtf8("Area (%1%2) :").arg(unit_system.length_label,"2") << results.area << "\n";
    }

    test_res.close();

}
void DC_Motor_PC::prepare_test_results(void){
    results.test_type = test_type_name;
    results.completion_date_time = QDate::currentDate().toString("dd.MM.yyyy") + " " + QTime::currentTime().toString();
    results.peak_load = QString::number(fuzpid->peak_load,'f',3);
    results.peak_stress = QString::number(fuzpid->peak_stress,'f',3);
    results.CBR_2_54 = QString::number(cbr.p_2_54,'f',3);
    results.CBR_5_08 = QString::number(cbr.p_5_08,'f',3);
    results.area = ui.label_area->text();
    results.max_displacement = QString::number(fuzpid->displacement_value);

    results.dimensions = QString::number(ui.doubleSpinBox_specimen_cyl_d->value())+"x" + QString::number(ui.doubleSpinBox_specimen_cyl_l->value());

    if(test_type_name == "LOAD CTRL."){
        results.test_speed = QString::number(ui.doubleSpinBox_pace_rate->value(),'f',3);
    }
    else{
        results.test_speed = QString::number(ui.doubleSpinBox_displacement_rate->value(),'f',3);
    }


    if(test_type_name == "CBR"){
        results.area = QString::number(ui.doubleSpinBox_area_cbr->value());
        switch(cbr.standard){
        case ASTM_STANDARD_CBR:
            results.standard = trUtf8("ASTM/AASHTO");
            break;
        case EN_STANDARD_CBR:
            results.standard = trUtf8("EN");
            break;
        case BS_STANDARD_CBR:
            results.standard = trUtf8("BS 1377");
            break;
        }
    }
    else if(test_type_name == "MARSHALL"){
        results.dimensions = QString::number(briquette.d)+"x" + QString::number(briquette.t);
        switch(marshall.standard){
        case ASTM_STANDARD_MARSHALL:
            results.standard = trUtf8("ASTM");
            break;
        case EN_STANDARD_MARSHALL:
            results.standard = trUtf8("EN");
            break;
        }
    }

}
