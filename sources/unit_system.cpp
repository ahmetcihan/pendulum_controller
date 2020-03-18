#include "dc_motor_pc.h"

void DC_Motor_PC::unit_system_calibration_handler(void){
    static u8 tmp = 0;

    switch(tmp){
    case 0:
        QTimer::singleShot(100,this,SLOT(unit_system_calibration_handler()));
        tmp++;
        break;
    case 1:
        for(u8 i = 0; i < 8; i++){
            fuzpid->cal[0].assigned_val[i] = fuzpid->cal[0].assigned_val[i] * unit_system.load_multiplier;
            fuzpid->cal[1].assigned_val[i] = fuzpid->cal[1].assigned_val[i] * unit_system.load_multiplier;
            fuzpid->cal[2].assigned_val[i] = fuzpid->cal[2].assigned_val[i] * unit_system.load_multiplier;
            fuzpid->cal[3].assigned_val[i] = fuzpid->cal[3].assigned_val[i] * unit_system.length_multiplier;
        }
        QTimer::singleShot(100,this,SLOT(unit_system_calibration_handler()));
        tmp++;
        break;
    case 2:
        fuzpid->slope_calculation(0);
        ui.comboBox_channel_no->setCurrentIndex(1);
        QTimer::singleShot(100,this,SLOT(unit_system_calibration_handler()));
        tmp++;
        break;
    case 3:
        ui.comboBox_channel_no->setCurrentIndex(2);
        fuzpid->slope_calculation(1);
        QTimer::singleShot(100,this,SLOT(unit_system_calibration_handler()));
        tmp++;
        break;
    case 4:
        fuzpid->slope_calculation(2);
        ui.comboBox_channel_no->setCurrentIndex(3);
        QTimer::singleShot(100,this,SLOT(unit_system_calibration_handler()));
        tmp++;
        break;
    case 5:
        ui.comboBox_channel_no->setCurrentIndex(0);
        fuzpid->slope_calculation(3);
        QTimer::singleShot(100,this,SLOT(unit_system_calibration_handler()));
        tmp++;
        break;
    case 6:
        write_calibration();
        QTimer::singleShot(100,this,SLOT(unit_system_calibration_handler()));
        tmp++;
        break;
    case 7:
        tmp = 0;
        break;
    }
}
void DC_Motor_PC::unit_system_handler(void){
    static u8 first_in = 1;

    u8 old_index = unit_system.index;

    if(ui.radioButton_unit_kN->isChecked()){
        unit_system.index = KN_MM;
        unit_system.load_label = trUtf8("kN");
        unit_system.stress_label = trUtf8("MPa");
        unit_system.length_label = trUtf8("mm");
    }
    if(ui.radioButton_unit_lbf->isChecked()){
        unit_system.index = LBF_IN;
        unit_system.load_label = trUtf8("lbf");
        unit_system.stress_label = trUtf8("psi");
        unit_system.length_label = trUtf8("in");
    }
    if(ui.radioButton_unit_kgf->isChecked()){
        unit_system.index = KGF_CM;
        unit_system.load_label = trUtf8("kgf");
        unit_system.stress_label = trUtf8("kgf/cm2");
        unit_system.length_label = trUtf8("cm");
    }
    //admin->channels
    ui.radioButton_ch3_kn->setText(unit_system.load_label);
    ui.radioButton_ch4_kn->setText(unit_system.load_label);
    ui.radioButton_ch3_mm->setText(unit_system.length_label);
    ui.radioButton_ch4_mm->setText(unit_system.length_label);

    //main
    ui.label_main_load_unit->setText(QString("%1").arg(unit_system.load_label));
    ui.label_main_stress_unit->setText(QString("%1").arg(unit_system.stress_label));
    ui.label_main_displacement_unit->setText(QString("%1").arg(unit_system.length_label));
    ch3_unit_handler();
    ch4_unit_handler();
    graphic_v_axis_handler();
    graphic_h_axis_handler();

    ui.label_main_area_unit->setText(QString("%1%2").arg(unit_system.length_label,"2"));

    plot_ch[1].plot->setAxisTitle(plot_ch[1].plot->yLeft, trUtf8("Pace Rate (%1/s)").arg(unit_system.length_label));
    //parameters->test_type
    ui.label_unit_pace_load->setText(trUtf8("%1/s").arg(unit_system.load_label));
    ui.label_unit_pace_stress->setText(trUtf8("%1/s").arg(unit_system.stress_label));
    ui.label_unit_displacement_speed->setText(trUtf8("%1/min").arg(unit_system.length_label));

    ui.label_cbr_step_target->setText(trUtf8("Next step:1 - %1 %2").arg(cbr_point_value[0]->value()).arg(unit_system.length_label));
    ui.label_cbr_step_load->setText(trUtf8("Step 0 load: 0 %1").arg(unit_system.load_label));

    //parameters->specimen
    QList<QLabel*> label_unit_specimen = this->findChildren<QLabel*>(QRegExp("label_unit_specimen"));;
    qSort(label_unit_specimen.begin(),label_unit_specimen.end(),label_name_compare);

    for(u8 i = 0; i < label_unit_specimen.length(); i++){
        label_unit_specimen[i]->setText(unit_system.length_label);
    }
    ui.label_unit_area_2->setText(QString("%1%2").arg(unit_system.length_label,"2"));

    ui.label_unit_briquette_d->setText(unit_system.length_label);
    ui.label_unit_briquette_t->setText(unit_system.length_label);

    //others
    ui.label_unit_area_cbr->setText(QString("%1%2").arg(unit_system.length_label,"2"));

    //parameters->parameters
    ui.label_unit_test_start_speed->setText(trUtf8("%1/min").arg(unit_system.length_label));
    ui.label_unit_return_home_speed->setText(trUtf8("%1/min").arg(unit_system.length_label));
    ui.label_unit_go_to_load_speed->setText(trUtf8("%1/min").arg(unit_system.length_label));
    ui.label_unit_max_displacement->setText(unit_system.length_label);
    ui.label_unit_max_load->setText(unit_system.load_label);
    ui.label_unit_failure_threshold->setText(unit_system.load_label);
    ui.label_unit_zero_suppression->setText(unit_system.load_label);

    //parameters->JOG
    ui.label_unit_up_max_speed->setText(trUtf8("%1/min").arg(unit_system.length_label));
    ui.label_unit_down_max_speed->setText(trUtf8("%1/min").arg(unit_system.length_label));
    ui.label_unit_jog_load->setText(unit_system.load_label);
    ui.label_unit_jog_displacement->setText(unit_system.length_label);
    ui.label_unit_specimen_protection->setText(unit_system.load_label);

    //results
    ui.label_results_name_peak_load->setText(trUtf8("PEAK LOAD (%1)").arg(unit_system.load_label));
    ui.label_results_name_peak_stress->setText(trUtf8("PEAK STRESS (%1)").arg(unit_system.stress_label));
    ui.label_results_name_dimensions->setText(trUtf8("DIMENSIONS (%1)").arg(unit_system.length_label));
    ui.label_results_name_area->setText(trUtf8("AREA (%1%2)").arg(unit_system.length_label,"2"));
    ui.label_results_name_test_speed->setText(trUtf8("TEST SPEED (%1/min)").arg(unit_system.length_label));
    ui.label_results_name_scb_notch->setText(trUtf8("NOTCH (%1)").arg(unit_system.length_label));
    ui.label_results_name_scb_thickness->setText(trUtf8("THICKNESS (%1)").arg(unit_system.length_label));
    ui.label_results_name_scb_max_displacement->setText(trUtf8("DISPLACEMENT (%1)").arg(unit_system.length_label));

    switch (unit_system.index){
    case KN_MM:
        ui.label_results_name_cbr_2_54->setText("CBR 2.54 mm");
        ui.label_results_name_cbr_5_08->setText("CBR 5.08 mm");
        break;
    case LBF_IN:
        ui.label_results_name_cbr_2_54->setText("CBR 0.1 in");
        ui.label_results_name_cbr_5_08->setText("CBR 0.2 in");
        break;
    case KGF_CM:
        ui.label_results_name_cbr_2_54->setText("CBR 0.254 cm");
        ui.label_results_name_cbr_5_08->setText("CBR 0.508 cm");
        break;
    default:
        break;
    }

    //calibration
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
    case 6: //encoder
        ui.groupBox_cal_value->setTitle(trUtf8("VALUE (%1)").arg(unit_system.length_label));
        break;
    }
    //cbr
    ui.label_unit_points_displacement->setText(trUtf8("Dis.(%1)").arg(unit_system.length_label));
    ui.label_unit_points_displacement_2->setText(trUtf8("Dis.(%1)").arg(unit_system.length_label));
    ui.label_unit_points_displacement_3->setText(trUtf8("Dis.(%1)").arg(unit_system.length_label));
    ui.label_unit_points_displacement_4->setText(trUtf8("Dis.(%1)").arg(unit_system.length_label));
    ui.label_unit_points_load->setText(trUtf8("Load(%1)").arg(unit_system.load_label));
    ui.label_unit_points_load_2->setText(trUtf8("Load(%1)").arg(unit_system.load_label));
    ui.label_unit_points_load_3->setText(trUtf8("Load(%1)").arg(unit_system.load_label));
    ui.label_unit_points_load_4->setText(trUtf8("Load(%1)").arg(unit_system.load_label));

    //admin
    ui.label_unit_disp_speed->setText(trUtf8("%1/min").arg(unit_system.length_label));
    ui.label_unit_disp_speed_2->setText(trUtf8("%1/min").arg(unit_system.length_label));
    ui.label_unit_disp_speed_3->setText(trUtf8("%1/min").arg(unit_system.length_label));

    //multipliers
    unit_system.load_multiplier = 1;
    unit_system.length_multiplier = 1;
    unit_system.stress_multiplier = 1;
    unit_system.area_multiplier = 1;

    if(old_index == KN_MM){
        if(unit_system.index == LBF_IN){
            unit_system.load_multiplier = 224.808943;       //kN->lbf
            unit_system.stress_multiplier = 145.037738;     //MPa->psi
            unit_system.length_multiplier = 0.0393700787;   //mm->in
            unit_system.area_multiplier = 0.0015500031;     //mm2->in2
        }
        else if(unit_system.index == KGF_CM){
            unit_system.load_multiplier = 101.971621;       //kN->kgf
            unit_system.stress_multiplier = 10.197162;      //MPa->kgf/cm2
            unit_system.length_multiplier = 0.1;            //mm->cm
            unit_system.area_multiplier = 0.01;             //mm2->cm2
        }
    }
    else if(old_index == LBF_IN){
        if(unit_system.index == KN_MM){
            unit_system.load_multiplier = 0.00444822162;    //lbf->kN
            unit_system.stress_multiplier = 0.00689475729;  //psi->MPa
            unit_system.length_multiplier = 25.4;           //in->mm
            unit_system.area_multiplier = 645.16;           //in2->mm2
        }
        else if(unit_system.index == KGF_CM){
            unit_system.load_multiplier = 0.45359237;       //lbf->kgf
            unit_system.stress_multiplier = 0.070306958;    //psi->kgf/cm2
            unit_system.length_multiplier = 2.54;           //in->cm
            unit_system.area_multiplier = 6.4516;           //in2->cm2
        }
    }
    else if(old_index == KGF_CM){
        if(unit_system.index == KN_MM){
            unit_system.load_multiplier = 0.00980665;       //kgf->kN
            unit_system.stress_multiplier = 0.0980665;      //kgf/cm2->MPa
            unit_system.length_multiplier = 10;             //cm->mm
            unit_system.area_multiplier = 100;              //cm2->mm2
        }
        else if(unit_system.index == LBF_IN){
            unit_system.load_multiplier = 2.20462262;       //kgf->lbf
            unit_system.stress_multiplier = 14.2233433;     //kgf/cm2->psi
            unit_system.length_multiplier = 0.393700787;    //cm->in
            unit_system.area_multiplier = 0.15500031;       //cm2->in2
        }
    }
    //parameters->test_type
    ui.doubleSpinBox_pace_rate->setValue(ui.doubleSpinBox_pace_rate->value() * unit_system.load_multiplier);
    ui.doubleSpinBox_pace_rate_mpa->setValue(ui.doubleSpinBox_pace_rate_mpa->value() * unit_system.stress_multiplier);
    ui.doubleSpinBox_displacement_rate->setValue(ui.doubleSpinBox_displacement_rate->value() * unit_system.length_multiplier);

    if(test_type == LOAD_CONTROL){
        ui.label_main_speed->setText(trUtf8("Pace : %1 %2/s").arg(ui.doubleSpinBox_pace_rate->value()).arg(unit_system.load_label));
        ui.label_autotuning_base_unit->setText(trUtf8("Base (%1/s)").arg(unit_system.load_label));
        ui.label_autotuning_step_unit->setText(trUtf8("Step (%1/s)").arg(unit_system.load_label));
    }
    else{
        ui.label_main_speed->setText(QString(trUtf8("Pace : %1 %2/min")).arg(ui.doubleSpinBox_displacement_rate->value()).arg(unit_system.length_label));
        ui.label_autotuning_base_unit->setText(trUtf8("Base (%1/min)").arg(unit_system.length_label));
        ui.label_autotuning_step_unit->setText(trUtf8("Step (%1/min)").arg(unit_system.length_label));
    }
    ui.label_speed_cbr->setText(QString(trUtf8("Speed : %1 %2/min")).arg(ui.doubleSpinBox_displacement_rate->value()).arg(unit_system.length_label));
    ui.label_speed_marshall->setText(QString(trUtf8("Speed : %1 %2/min")).arg(ui.doubleSpinBox_displacement_rate->value()).arg(unit_system.length_label));

    //parameters->specimen
    ui.doubleSpinBox_specimen_cyl_d->setValue(ui.doubleSpinBox_specimen_cyl_d->value() * unit_system.length_multiplier);
    ui.doubleSpinBox_specimen_cyl_l->setValue(ui.doubleSpinBox_specimen_cyl_l->value() * unit_system.length_multiplier);

    //parameters->parameters
    ui.doubleSpinBox_test_start_speed->setValue(ui.doubleSpinBox_test_start_speed->value() * unit_system.length_multiplier);
    ui.doubleSpinBox_return_home_speed->setValue(ui.doubleSpinBox_return_home_speed->value() * unit_system.length_multiplier);
    ui.doubleSpinBox_go_to_load_speed->setValue(ui.doubleSpinBox_go_to_load_speed->value() * unit_system.length_multiplier);
    ui.doubleSpinBox_max_displacement->setValue(ui.doubleSpinBox_max_displacement->value() * unit_system.length_multiplier);
    ui.doubleSpinBox_max_load->setValue(ui.doubleSpinBox_max_load->value() * unit_system.load_multiplier);
    ui.doubleSpinBox_load_threshold->setValue(ui.doubleSpinBox_load_threshold->value() * unit_system.load_multiplier);
    ui.doubleSpinBox_zero_suppression->setValue(ui.doubleSpinBox_zero_suppression->value() * unit_system.load_multiplier);

    //parameters->jog
    ui.doubleSpinBox_upward_max_speed->setValue(ui.doubleSpinBox_upward_max_speed->value() * unit_system.length_multiplier);
    ui.doubleSpinBox_downward_max_speed->setValue(ui.doubleSpinBox_downward_max_speed->value() * unit_system.length_multiplier);
    ui.doubleSpinBox_protect_specimen->setValue(ui.doubleSpinBox_protect_specimen->value() * unit_system.load_multiplier);

    //admin->pid
    ui.doubleSpinBox_step_first_speed->setValue(ui.doubleSpinBox_step_first_speed->value() * unit_system.length_multiplier);
    ui.doubleSpinBox_step_second_speed->setValue(ui.doubleSpinBox_step_second_speed->value() * unit_system.length_multiplier);

    //admin->speed_tune
    ui.doubleSpinBox_displ_speed->setValue(ui.doubleSpinBox_displ_speed->value() * unit_system.length_multiplier);
    ui.doubleSpinBox_region_1_limit->setValue(ui.doubleSpinBox_region_1_limit->value() * unit_system.length_multiplier);
    ui.doubleSpinBox_region_2_limit->setValue(ui.doubleSpinBox_region_2_limit->value() * unit_system.length_multiplier);

    ui.doubleSpinBox_briquette_t->setValue(ui.doubleSpinBox_briquette_t->value() * unit_system.length_multiplier);

    //others
    ui.doubleSpinBox_area_cbr->setValue(ui.doubleSpinBox_area_cbr->value() * unit_system.area_multiplier);

    //saved parameters
    for(u8 i= 0; i < 10; i++){
        if(i != test_type){
            parameters[i].max_load = parameters[i].max_load * unit_system.load_multiplier;
            parameters[i].load_threshold = parameters[i].load_threshold * unit_system.load_multiplier;
            parameters[i].zero_suppression = parameters[i].zero_suppression * unit_system.load_multiplier;
            parameters[i].displacement_rate = parameters[i].displacement_rate * unit_system.length_multiplier;
            parameters[i].max_displacement = parameters[i].max_displacement * unit_system.length_multiplier;
            parameters[i].start_speed = parameters[i].start_speed * unit_system.length_multiplier;
            parameters[i].cylinder.d = parameters[i].cylinder.d * unit_system.length_multiplier;
            parameters[i].cylinder.l = parameters[i].cylinder.l * unit_system.length_multiplier;
            parameters[i].return_home_speed = parameters[i].return_home_speed * unit_system.length_multiplier;
            parameters[i].go_to_load_speed = parameters[i].go_to_load_speed * unit_system.length_multiplier;
        }
    }


    test_type_handler();
    cbr_standard_type_handler();
    QTimer::singleShot(1001,this,SLOT(pace_rate_handler_kn()));
    unit_system_calibration_handler();

    switch(unit_system.index){
    case KN_MM:
        ui.comboBox_diameter->setItemText(0,"101.6");
        ui.comboBox_diameter->setItemText(1,"152.4");
        break;
    case LBF_IN:
        ui.comboBox_diameter->setItemText(0,"4");
        ui.comboBox_diameter->setItemText(1,"6");
        break;
    case KGF_CM:
        ui.comboBox_diameter->setItemText(0,"10.16");
        ui.comboBox_diameter->setItemText(1,"15.24");
        break;
    }

    marshall_diameter_selection_handle(marshall.diameter_number);

    //QTimer::singleShot(1000,this,SLOT(unit_system_recalculate()));

    if(first_in == 1){
        first_in = 0;
    }
    else{
        wait_messagebox();
    }
#ifdef CONFIG_x86
qDebug(__FUNCTION__);
#endif
}
void DC_Motor_PC::unit_system_recalculate(void){
    ui.doubleSpinBox_test_start_speed->setValue(ui.doubleSpinBox_test_start_speed->value() * unit_system.length_multiplier);
    ui.doubleSpinBox_max_displacement->setValue(ui.doubleSpinBox_max_displacement->value() * unit_system.length_multiplier);
    ui.doubleSpinBox_max_load->setValue(ui.doubleSpinBox_max_load->value() * unit_system.load_multiplier);
    ui.doubleSpinBox_load_threshold->setValue(ui.doubleSpinBox_load_threshold->value() * unit_system.load_multiplier);
    ui.doubleSpinBox_zero_suppression->setValue(ui.doubleSpinBox_zero_suppression->value() * unit_system.load_multiplier);

    //parameters->test_type
    ui.doubleSpinBox_pace_rate->setValue(ui.doubleSpinBox_pace_rate->value() * unit_system.load_multiplier);
    ui.doubleSpinBox_pace_rate_mpa->setValue(ui.doubleSpinBox_pace_rate_mpa->value() * unit_system.stress_multiplier);
    ui.doubleSpinBox_displacement_rate->setValue(ui.doubleSpinBox_displacement_rate->value() * unit_system.length_multiplier);

    ui.label_main_speed->setText(QString(trUtf8("Pace : %1 %2/min")).arg(ui.doubleSpinBox_displacement_rate->value()).arg(unit_system.length_label));
    ui.label_speed_cbr->setText(QString(trUtf8("Speed : %1 %2/min")).arg(ui.doubleSpinBox_displacement_rate->value()).arg(unit_system.length_label));
    ui.label_speed_marshall->setText(QString(trUtf8("Speed : %1 %2/min")).arg(ui.doubleSpinBox_displacement_rate->value()).arg(unit_system.length_label));

    ui.doubleSpinBox_specimen_cyl_d->setValue(ui.doubleSpinBox_specimen_cyl_d->value() * unit_system.length_multiplier);
    ui.doubleSpinBox_specimen_cyl_l->setValue(ui.doubleSpinBox_specimen_cyl_l->value() * unit_system.length_multiplier);
    ui.doubleSpinBox_specimen_scb_notch->setValue(ui.doubleSpinBox_specimen_scb_notch->value() * unit_system.length_multiplier);
    ui.doubleSpinBox_specimen_scb_thickness->setValue(ui.doubleSpinBox_specimen_scb_thickness->value() * unit_system.length_multiplier);

//    qDebug() << "gad damn multiplier" << unit_system.length_multiplier;
//    qDebug() << "gad damn value" << ui.doubleSpinBox_test_start_speed->value() * unit_system.length_multiplier;
//    ui.doubleSpinBox_test_start_speed->setValue(ui.doubleSpinBox_test_start_speed->value() * unit_system.length_multiplier);

}
void DC_Motor_PC::wait_messagebox(void){
    static QLabel deneme_label;
    static u8 timer = 30;

    deneme_label.setGeometry(150,220,500,100);
    deneme_label.setFont(QFont("Ubuntu",16,QFont::Black,true));
    deneme_label.setText(QString(trUtf8("Please wait %1 secs")).arg(timer/2));

    if(timer > 0){
        timer--;
        QTimer::singleShot(500,this,SLOT(wait_messagebox()));
        deneme_label.show();
    }
    else{
        deneme_label.hide();
        timer = 30;
    }


}
