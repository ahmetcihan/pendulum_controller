#include "dc_motor_pc.h"
#include <QDebug>
#include <QApplication>

DC_Motor_PC::DC_Motor_PC(QWidget *parent)
    : QWidget(parent),
      rcon(new RemoteConsole(8945, this)),
      sserver(new SettingsServer),
      dataStream(new RemoteDataStream)
{
    ui.setupUi(this);
    fuzpid = new fuzzy_pid(this);
    QApplication *app = (QApplication *) QApplication::instance();
    connect(app,SIGNAL(focusChanged(QWidget*,QWidget*)),this,SLOT(focusChanged(QWidget*,QWidget*)));
    forbid_pace_recalculate_timer.addSecs(1);
    block_double_click.addSecs(1);

#ifdef CONFIG_ARM
    QCursor pCursor;
    pCursor.setPos(0,0);
    ui.tabWidget->setCursor(Qt::BlankCursor);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    ui.tabWidget->removeTab(TAB_RESERVED);

    QProcess::execute("mkdir /usr/local/bin/test_results");
    QProcess::execute("mkdir /usr/local/bin/data_logs");
    QProcess::execute("mkdir /media/usb");
    QTimer::singleShot(1000,this,SLOT(eth0_address_set()));
#endif

#ifdef MATLAB_RECORDINGS
    matlab_log.setFileName(QString("matlab_log.csv"));
    matlab_log.open(QIODevice::WriteOnly);
#endif
    //fuzzy_screen = new QWidget();
    //fuzzy_screen_ui.setupUi(fuzzy_screen);
    //fuzzy_screen->show();
    //bessel_filter_coeffs();
    rearrange_object_names();
    cal_points_signalmapper();
    usart_signalmapper();

    correctly_opened = false;
    readAppSettings();
    read_calibration();

    serial_port_setup();
    set_plotter();

    assign_type_parameters();

    init_SPECIMENs();
    init_PIDs();
    init_DIAMETERs();
    init_TIMERs();
    init_CALIBRATIONs();
    init_PUSHBUTTONs();
    init_RADIOBUTTONs();
    init_CHANNEL_SELECTIONs();
    init_PIXMAPs();
    init_GUI();

    connect(fuzpid,SIGNAL(cohen_coon_kp(QString)),this->ui.label_step_resp_CC_KP,SLOT(setText(QString)));
    connect(fuzpid,SIGNAL(cohen_coon_ki(QString)),this->ui.label_step_resp_CC_Ki,SLOT(setText(QString)));
    connect(fuzpid,SIGNAL(cohen_coon_kd(QString)),this->ui.label_step_resp_CC_Kd,SLOT(setText(QString)));
    connect(fuzpid,SIGNAL(let_step_response()),this,SLOT(step_response_handle()));

    connect(fuzpid,SIGNAL(zero_suppression_exceeded()),this,SLOT(cbr_zero_suppression_exceeded()));
    connect(fuzpid,SIGNAL(zero_suppression_exceeded_marshall()),this,SLOT(marshall_zero_suppression_exceeded()));
    connect(fuzpid,SIGNAL(cbr_process(double,double)),this,SLOT(cbr_process(double,double)));

    servo.start = 0;
    servo.stop = 0;
    servo.down = 0;
    servo.up = 0;
    JOG.speed = 0;
    JOG.up = false;
    JOG.down = false;
    JOG.error = false;
    do_not_change_focus = false;
    return_home_bit = false;
    go_load_bit = false;
    return_home_tmp = 0;
    go_load_tmp = 0;
    cbr.points_detected = false;
    cbr.step = 0;

    marshall.slope_line = new QwtPlotCurve();
    marshall.shifting_slope_line = new QwtPlotCurve();
    marshall.stability_marker = new QwtPlotMarker();
    marshall.stability_symbol = new QwtSymbol(QwtSymbol::Star2,QBrush(Qt::red),QPen(Qt::red,0),QSize(10,10));
    marshall.x_axis_line = new QwtPlotMarker();
    marshall.y_axis_line = new QwtPlotMarker();

#ifdef CONFIG_x86
    ui.tabWidget->setTabEnabled(TAB_CALIBRATION,1);
    ui.tabWidget->setTabEnabled(TAB_ADMIN,1);
#endif
    connect(ui.listWidget_parameters,SIGNAL(currentRowChanged(int)),ui.tabWidget_parameters,SLOT(setCurrentIndex(int)));
    ui.listWidget_parameters->setCurrentRow(0);
    ui.tabWidget_parameters->setCurrentIndex(0);
    ui.tabWidget_settings->setCurrentIndex(0);
    ui.tabWidget_admin->setCurrentIndex(0);

    translator = new QTranslator(this);
    connect(this->ui.radioButton_english,SIGNAL(clicked()),this,SLOT(language_load()));
    connect(this->ui.radioButton_turkish,SIGNAL(clicked()),this,SLOT(language_load()));
    connect(this->ui.radioButton_french,SIGNAL(clicked()),this,SLOT(language_load()));
    connect(this->ui.radioButton_spanish,SIGNAL(clicked()),this,SLOT(language_load()));
    connect(this->ui.radioButton_russian,SIGNAL(clicked()),this,SLOT(language_load()));
    language_load();

    connect(this->ui.radioButton_unit_kN,SIGNAL(clicked()),this,SLOT(unit_system_handler()));
    connect(this->ui.radioButton_unit_lbf,SIGNAL(clicked()),this,SLOT(unit_system_handler()));
    connect(this->ui.radioButton_unit_kgf,SIGNAL(clicked()),this,SLOT(unit_system_handler()));

    QTimer::singleShot(20000,this,SLOT(check_the_storage()));

}
void DC_Motor_PC::check_the_storage(void){
    QProcess p;
    p.start("df /usr/local/bin");
    p.waitForFinished();

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
        ui.progressBar_storage->setValue(100 - (vals[3].trimmed().toInt()/130)); //assumes that maximum size is 13000 kByte
        if(vals[3].trimmed().toInt() < 2600){   //2600
            QMessageBox msgBox;
            msgBox.setWindowTitle(trUtf8("Checking Storage.."));
            msgBox.setText(trUtf8("<font size = 10>Please backup your files and clean the storage!!"));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
    }

    qDebug(__FUNCTION__);
}
void DC_Motor_PC::save_and_reboot(void){
    static u8 tmp = 0;

    switch (tmp) {
    case 0:
        writeAppSettings();
        QTimer::singleShot(1000,this,SLOT(save_and_reboot()));
        tmp++;
        break;
    case 1:
#ifdef CONFIG_x86
        qDebug() << "reboot";
        this->close();
#else
        QProcess::execute("reboot");
#endif
        break;
    default:
        break;
    }
}
void DC_Motor_PC::show_jog_widget(void){
    if(prevent_double_click()) return;

    ui.pushButton_jog->setDisabled(1);

    QWidget *mywidget = new QWidget(this->ui.tab_main_test);
    mywidget->setGeometry(250,247,540,95);
    mywidget->show();

    QPushButton *close_button = new QPushButton(mywidget);
    connect(close_button, SIGNAL(pressed()), mywidget, SLOT(close()));
    connect(close_button, SIGNAL(pressed()), mywidget, SLOT(deleteLater()));
    connect(close_button, SIGNAL(pressed()), this, SLOT(close_jog_widget()));
    close_button->setGeometry(437,10,75,75);
//    close_button->setText("CLOSE");
//    close_button->setFont(QFont("Ubuntu",16,QFont::Normal,false));
    close_button->setStyleSheet("min-width: 50px; min-height: 50px;"
                            "border-image: url(:sari_close.png);"
                            "border-width: 0px ;");
    close_button->show();

    QPushButton *up_button = new QPushButton(mywidget);
    QPushButton *down_button = new QPushButton(mywidget);
    up_button->setGeometry(25,10,75,75);
    up_button->setStyleSheet("min-width: 50px; min-height: 50px;"
                            "border-image: url(:sari_up.png);"
                            "border-width: 0px ;");
    up_button->show();
    down_button->setGeometry(128,10,75,75);
    down_button->setStyleSheet("min-width: 50px; min-height: 50px;"
                            "border-image: url(:sari_down.png);"
                            "border-width: 0px ;");
    down_button->show();
    connect(up_button, SIGNAL(pressed()), this, SLOT(handle_JOG_up_button()));
    connect(up_button, SIGNAL(released()), this, SLOT(release_JOG_up_button()));
    connect(down_button, SIGNAL(pressed()), this, SLOT(handle_JOG_down_button()));
    connect(down_button, SIGNAL(released()), this, SLOT(release_JOG_down_button()));

    QPushButton *return_home_button = new QPushButton(mywidget);
    return_home_button->setGeometry(231,10,75,75);
    return_home_button->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:sari_home.png);"
                                                "border-width: 0px ;");
    return_home_button->show();
    connect(return_home_button,SIGNAL(pressed()),this,SLOT(return_home()));

    QPushButton *go_load_button = new QPushButton(mywidget);
    go_load_button->setGeometry(334,10,75,75);
    go_load_button->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                "border-image: url(:sari_to_load.png);"
                                                "border-width: 0px ;");
    go_load_button->show();
    connect(go_load_button,SIGNAL(pressed()),this,SLOT(go_load()));


}
void DC_Motor_PC::close_jog_widget(void){
    ui.pushButton_jog->setEnabled(1);
}
void DC_Motor_PC::refresh_button_handler(void){
    if(return_home_bit){
        ui.pushButton_start_test->setDisabled(1);
        ui.pushButton_start_test->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                    "border-image: url(:sari_start-silver.png);"
                                                    "border-width: 0px ;");
    }
    ui.pushButton_start_test->show();
    ui.pushButton_refresh->hide();
    fuzpid->test_status = TEST_STOPPED;
    fuzpid->peak_stress = 0;
    fuzpid->peak_load = 0;

    ui.pushButton_tare_load->setEnabled(1);
    ui.pushButton_tare_displacement->setEnabled(1);
    ui.pushButton_tare_ch3->setEnabled(1);
    ui.pushButton_tare_ch4->setEnabled(1);

    ui.tabWidget->setTabEnabled(TAB_PARAMETERS,1);
    ui.tabWidget->setTabEnabled(TAB_TEST_RESULTS,1);
    ui.tabWidget->setTabEnabled(TAB_SETTINGS,1);
}
int DC_Motor_PC::prevent_double_click(void){
    if(block_double_click.elapsed() < DOUBLE_CLICK_BLOCK_TIME){
        block_double_click.restart();
        return 1;
    }
    block_double_click.restart();
    return 0;
}
void DC_Motor_PC::load_factory_settings(){
    static u8 tmp = 0;
    static u8 timer;
    QMessageBox msgBox;
    static QLabel deneme_label;

    switch (tmp) {
    case 0:
        msgBox.setWindowTitle(trUtf8("Load default parameters"));
        msgBox.setText(trUtf8("<font size = 10>The device will reboot. Are you sure?"));
        msgBox.setStandardButtons(QMessageBox::Yes);
        msgBox.addButton(QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        msgBox.setButtonText(QMessageBox::Yes, trUtf8("Yes"));
        msgBox.setButtonText(QMessageBox::No, trUtf8("No"));
        if(msgBox.exec() == QMessageBox::Yes){
            QProcess::execute("cp /usr/local/bin/factory_settings.ini /usr/local/bin/settings.ini");
            tmp++;
            QTimer::singleShot(500,this,SLOT(load_factory_settings()));
            timer = 20;
        }
        else{
        }
        break;
    case 1:
        QTimer::singleShot(500,this,SLOT(load_factory_settings()));

        deneme_label.setGeometry(150,220,500,100);
        deneme_label.setFont(QFont("Ubuntu",16,QFont::Black,true));
        deneme_label.setText(QString(trUtf8("Please do not power off or unplug your machine\nThe device will reboot after %1 secs")).arg(timer/2));
        deneme_label.show();

        (timer > 0) ? timer-- : tmp++;
        break;
    case 2:
        QProcess::execute("reboot");
        break;
    default:
        break;
    }
}
void DC_Motor_PC::admin_authorization_handler(){
    if(ui.lineEdit_admin_password->text() == "88378"){
        ui.tabWidget->setTabEnabled(TAB_CALIBRATION,1);
        ui.tabWidget->setTabEnabled(TAB_ADMIN,1);
    }
    else{
        if(ui.lineEdit_admin_password->text() != "*****"){
            ui.tabWidget->setTabEnabled(TAB_CALIBRATION,0);
            ui.tabWidget->setTabEnabled(TAB_ADMIN,0);
        }
    }
    ui.lineEdit_admin_password->setText("*****");
}
void DC_Motor_PC::main_screen_arrangements(void){
    if(test_type == CBR){
        ui.listWidget_parameters->item(8)->setHidden(1);    //marshall  - OFF
        ui.listWidget_parameters->item(7)->setHidden(0);    //cbr       - ON
        ui.listWidget_parameters->item(1)->setHidden(1);    //specimen  - OFF
        ui.groupBox_main_ch3->hide();
        ui.groupBox_main_ch4->hide();
        ui.groupBox_main_par_0->hide();
        ui.groupBox_main_par_1->hide();
        disconnect(ui.checkBox_ch3,SIGNAL(stateChanged(int)),this,SLOT(channel_activations_handler()));
        disconnect(ui.checkBox_ch4,SIGNAL(stateChanged(int)),this,SLOT(channel_activations_handler()));
        ui.groupBox_main_par_cbr->setParent(ui.tab_main_test);
        ui.groupBox_main_par_cbr->setGeometry(5,209,240,201);
        ui.groupBox_main_par_cbr->setVisible(1);
        ui.groupBox_stress->setHidden(0);
        ui.groupBox_main_load->move(5,73);
        ui.groupBox_main_displacement->move(5,141);
        ui.groupBox_main_par_marshall->setParent(ui.tab_main_reserve);
        ui.groupBox_main_par_marshall->setGeometry(5,215,240,201);
        ui.groupBox_main_par_marshall->setVisible(1);
    }
    else if(test_type == MARSHALL){
        ui.listWidget_parameters->item(8)->setHidden(0);    //marshall  - ON
        ui.listWidget_parameters->item(7)->setHidden(1);    //cbr       - OFF
        ui.listWidget_parameters->item(1)->setHidden(1);    //specimen  - OFF
        ui.groupBox_main_ch3->hide();
        ui.groupBox_main_ch4->hide();
        ui.groupBox_main_par_0->hide();
        ui.groupBox_main_par_1->hide();
        disconnect(ui.checkBox_ch3,SIGNAL(stateChanged(int)),this,SLOT(channel_activations_handler()));
        disconnect(ui.checkBox_ch4,SIGNAL(stateChanged(int)),this,SLOT(channel_activations_handler()));
        ui.groupBox_stress->setHidden(1);
        ui.groupBox_main_load->move(5,5);
        ui.groupBox_main_displacement->move(5,73);
        ui.groupBox_main_par_cbr->setParent(ui.tab_main_reserve);
        ui.groupBox_main_par_cbr->setGeometry(5,5,240,201);
        ui.groupBox_main_par_cbr->setVisible(1);
        ui.groupBox_main_par_marshall->setParent(ui.tab_main_test);
        ui.groupBox_main_par_marshall->setGeometry(5,141,240,201);
        ui.groupBox_main_par_marshall->setVisible(1);
    }
    else{
        ui.listWidget_parameters->item(8)->setHidden(1);    //marshall  - OFF
        ui.listWidget_parameters->item(7)->setHidden(1);    //cbr       - OFF
        ui.listWidget_parameters->item(1)->setHidden(0);    //specimen  - ON
        ui.groupBox_stress->show();
        ui.groupBox_main_load->move(5,73);
        ui.groupBox_main_displacement->move(5,141);

        ui.groupBox_main_ch3->show();
        ui.groupBox_main_ch4->show();
        ui.groupBox_main_par_0->show();
        ui.groupBox_main_par_1->show();
        connect(ui.checkBox_ch3,SIGNAL(stateChanged(int)),this,SLOT(channel_activations_handler()));
        connect(ui.checkBox_ch4,SIGNAL(stateChanged(int)),this,SLOT(channel_activations_handler()));
        ui.groupBox_main_par_cbr->setParent(ui.tab_main_reserve);
        ui.groupBox_main_par_cbr->setGeometry(5,5,240,201);
        ui.groupBox_main_par_cbr->setVisible(1);
        ui.groupBox_main_par_marshall->setParent(ui.tab_main_reserve);
        ui.groupBox_main_par_marshall->setGeometry(5,215,240,201);
        ui.groupBox_main_par_marshall->setVisible(1);
        channel_activations_handler();
        ch3_name_handler();
        ch4_name_handler();
    }

    //pace rate groupbox
    if(test_type == LOAD_CONTROL){
        ui.groupBox_speed_displacement->hide();
        ui.groupBox_speed_load->setParent(ui.tab_test_type);
        ui.groupBox_speed_load->setGeometry(310,230,295,165);
        ui.groupBox_speed_load->setVisible(1);
        ui.label_main_speed->setText(trUtf8("Pace : %1 %2/s").arg(ui.doubleSpinBox_pace_rate->value()).arg(unit_system.load_label));
        ui.label_autotuning_base_unit->setText(trUtf8("Base (%1/s)").arg(unit_system.load_label));
        ui.label_autotuning_step_unit->setText(trUtf8("Step (%1/s)").arg(unit_system.load_label));
        plot_ch[1].plot->setAxisTitle(plot_ch[1].plot->yLeft, trUtf8("Pace Rate (%1/s)").arg(unit_system.load_label));
        ui.label_pid_channel->setText(trUtf8("PID channel : Load"));
    }
    else{
        ui.groupBox_speed_displacement->show();
        ui.groupBox_speed_load->setParent(ui.tab_main_reserve);
        ui.label_main_speed->setText(QString(trUtf8("Pace : %1 %2/min")).arg(ui.doubleSpinBox_displacement_rate->value()).arg(unit_system.length_label));
        ui.label_autotuning_base_unit->setText(trUtf8("Base (%1/min)").arg(unit_system.length_label));
        ui.label_autotuning_step_unit->setText(trUtf8("Step (%1/min)").arg(unit_system.length_label));
        plot_ch[1].plot->setAxisTitle(plot_ch[1].plot->yLeft, trUtf8("Pace Rate (%1/s)").arg(unit_system.length_label));
        ui.label_pid_channel->setText(trUtf8("PID channel : Displacement"));
    }
    if(test_type == SCB){
        ui.groupBox_specimen_scb->show();
    }
    else{
        ui.groupBox_specimen_scb->hide();
    }

}
void DC_Motor_PC::test_type_handler(){
    writeAppSettings();

    if(ui.radioButton_cbr->isChecked()){
        test_type_name = trUtf8("CBR");
        test_type = CBR;
    }
    else if(ui.radioButton_marshall->isChecked()){
        test_type_name = trUtf8("MARSHALL");
        test_type = MARSHALL;
    }
    else if(ui.radioButton_idt->isChecked()){
        test_type_name = trUtf8("IDT");
        test_type = IDT;
    }
    else if(ui.radioButton_scb->isChecked()){
        test_type_name = trUtf8("SCB");
        test_type = SCB;
    }
    else if(ui.radioButton_triaxial->isChecked()){
        test_type_name = trUtf8("TRIAXIAL");
        test_type = TRIAXIAL;
    }
    else if(ui.radioButton_uu->isChecked()){
        test_type_name = trUtf8("UU");
        test_type = UU;
    }
    else if(ui.radioButton_cu_cd->isChecked()){
        test_type_name = trUtf8("CU-CD");
        test_type = CU_CD;
    }
    else if(ui.radioButton_uniaxal->isChecked()){
        test_type_name = trUtf8("UNIAXIAL");
        test_type = UNIAXIAL;
    }
    else if(ui.radioButton_load_control->isChecked()){
        test_type_name = trUtf8("LOAD CTRL.");
        test_type = LOAD_CONTROL;
    }
    else if(ui.radioButton_displacement_control->isChecked()){
        test_type_name = trUtf8("DISPL. CTRL.");
        test_type = DISPLACEMENT_CONTROL;
    }

    ui.label_test_type_specimen->setText(test_type_name);
    fuzpid->from_gui.test_type = test_type;
    assign_type_parameters();
    main_screen_arrangements();
    data_changed = true;
}
void DC_Motor_PC::control_mode_handler(){
    if(ui.radioButton_auto->isChecked()){
        control_mode = AUTO;
        fuzpid->relay_auto_man = RELAY_ON;
        ui.label_auto_man->setText(trUtf8("AUTO"));
    }
    if(ui.radioButton_man->isChecked()){
        control_mode = MAN;
        fuzpid->relay_auto_man = RELAY_OFF;
        ui.label_auto_man->setText(trUtf8("MAN"));
    }
}
void DC_Motor_PC::test_finishing(){
    double strength;

    if(fuzpid->to_gui.test_finished){
        fuzpid->to_gui.test_finished = false;

        //load_graphic_timer->stop();
        QTimer::singleShot(500,this->load_graphic_timer,SLOT(stop()));
        ui.pushButton_start_test->setEnabled(1);
        step_response_status = false;
        pace_graphic_timer->stop();
        ui.pushButton_step_response->setText(trUtf8("START AUTOTUNING"));
        ui.label_test_status->setText(trUtf8("TEST is FINISHED.."));
        ui.label_test_status_cbr->setText(trUtf8("TEST is FINISHED.."));
        excel_file.close();

        ui.pushButton_start_test->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                    "border-image: url(:sari_start.png);"
                                                    "border-width: 0px ;");
        ui.pushButton_pause_test->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                    "border-image: url(:sari_pause-silver.png);"
                                                    "border-width: 0px ;");
        ui.pushButton_stop_motor->setStyleSheet("min-width: 50px; min-height: 50px;"
                                                    "border-image: url(:sari_stop-silver.png);"
                                                    "border-width: 0px ;");
        ui.pushButton_stop_motor->setDisabled(1);
        ui.pushButton_pause_test->setDisabled(1);

        if(fuzpid->test_status == TEST_JUST_FINISHED){
            ui.pushButton_refresh->show();
            ui.pushButton_start_test->hide();
            plot_ch[0].zoomer->setZoomBase(QRectF(plot_ch[0].min_value_x,plot_ch[0].max_value_x, plot_ch[0].min_value_y,plot_ch[0].max_value_y));

            switch (test_type) {
            case MARSHALL:
                marshall_calculations();
                break;
            case IDT:
                strength = (2000*fuzpid->peak_load)/(M_PI * parameters[test_type].cylinder.d * parameters[test_type].cylinder.l);
                results.strength = QString::number(strength,'f',3);
               break;
            }
        }
        prepare_test_results();

        if(test_type != MARSHALL){
            write_test_results_to_file(current_test_no);
        }
        ui.label_test_date_time_main->setText(results.starting_date_time + "-" + results.completion_date_time.section(" ",1));
        ui.label_test_date_time_main_cbr->setText(results.starting_date_time + "-" + results.completion_date_time.section(" ",1));
        ui.label_test_date_time_main_marshall->setText(results.starting_date_time + "-" + results.completion_date_time.section(" ",1));
        cbr_export_log_to_file();
    }
}
void DC_Motor_PC::error_messagebox(void){
    QMessageBox msgBox;
    msgBox.setWindowTitle(trUtf8("!!ERROR!!"));
    msgBox.setText(trUtf8("<font size = 10>Max Load Error!! Device is stopped.."));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}
void DC_Motor_PC::update_gui(){
    static u32 local_counter = 0;
    local_counter++;

    if(current_tab_index == TAB_ADMIN){
        ui.label_adc_channel_1->setText(QString::number(fuzpid->to_gui.signed_raw[0]));
        ui.label_adc_channel_2->setText(QString::number(fuzpid->to_gui.signed_raw[1]));
        ui.label_adc_channel_3->setText(QString::number(fuzpid->to_gui.signed_raw[2]));
        ui.label_adc_channel_4->setText(QString::number(fuzpid->to_gui.signed_raw[3]));
        for(u8 i = 0; i < 4; i++){ //except for encoder
            switch(fuzpid->to_gui.gain[i]){
            case 0:
                label_gain[i]->setText(QStringLiteral("+/- 10 V"));
                break;
            case 1:
                label_gain[i]->setText(QStringLiteral("+/- 5 V"));
                break;
            case 2:
                label_gain[i]->setText(QStringLiteral("+/- 2.5 V"));
                break;
            case 3:
                label_gain[i]->setText(QStringLiteral("+/- 1 V"));
                break;
            case 4:
                label_gain[i]->setText(QStringLiteral("+/- 500 mV"));
                break;
            case 5:
                label_gain[i]->setText(QStringLiteral("+/- 250 mV"));
                break;
            case 6:
                label_gain[i]->setText(QStringLiteral("+/- 125 mV"));
                break;
            case 7:
                label_gain[i]->setText(QStringLiteral("+/- 50 mV"));
                break;
            case 8:
                label_gain[i]->setText(QStringLiteral("+/- 25 mV"));
                break;
            case 9:
                label_gain[i]->setText(QStringLiteral("+/- 10 mV"));
                break;
            case 10:
                label_gain[i]->setText(QStringLiteral("+/- 5 mV"));
                break;
            }
        }
        switch (load_calibration_channel) {
        case LOAD_1:
            ui.label_pid_load_pace->setText(trUtf8("Load : ") + QString::number(fuzpid->load_value,'f',dp.load_1) + trUtf8("\tPace : ") + QString::number(fuzpid->current_pace_rate,'f',3));
            break;
        case LOAD_2:
            ui.label_pid_load_pace->setText(trUtf8("Load : ") + QString::number(fuzpid->load_value,'f',dp.load_2) + trUtf8("\tPace : ") + QString::number(fuzpid->current_pace_rate,'f',3));
            break;
        case LOAD_3:
            ui.label_pid_load_pace->setText(trUtf8("Load : ") + QString::number(fuzpid->load_value,'f',dp.load_3) + trUtf8("\tPace : ") + QString::number(fuzpid->current_pace_rate,'f',3));
            break;
        default:
            break;
        }
        ui.label_pid_load_pace_2->setText(trUtf8("Displacement : ") + QString::number(fuzpid->displacement_value,'f',dp.displacement) + trUtf8("\tPace : ") + QString::number(fuzpid->current_displacement_rate,'f',3));

    }
    if(current_tab_index == TAB_SETTINGS){
        if((local_counter %10) == 0){
            ui.label_current_time->setText(QTime::currentTime().toString());
            ui.label_current_date->setText(QDate::currentDate().toString("dd.MM.yyyy"));
        }
    }
    if(current_tab_index == TAB_PARAMETERS){
        switch (load_calibration_channel) {
        case LOAD_1:
            ui.label_load->setText(QString::number(fuzpid->load_value,'f',dp.load_1));
            break;
        case LOAD_2:
            ui.label_load->setText(QString::number(fuzpid->load_value,'f',dp.load_2));
            break;
        case LOAD_3:
            ui.label_load->setText(QString::number(fuzpid->load_value,'f',dp.load_3));
            break;
        default:
            break;
        }
        ui.label_displacement->setText(QString::number(fuzpid->displacement_value,'f',dp.displacement));
    }
    if(current_tab_index == TAB_CALIBRATION){
        double aux;
        aux = fuzpid->evaluate_calibrated_values_ascending(current_cal_channel);

        switch (current_cal_channel) {  //TODO:assign calculated values
            case 0:
                ui.label_calibrated->setText(QString::number(aux,'f',dp.load_1));
                current_channel_data = fuzpid->to_gui.signed_raw[0];
                break;
            case 1:
                ui.label_calibrated->setText(QString::number(aux,'f',dp.load_2));
                current_channel_data = fuzpid->to_gui.signed_raw[0];
                break;
            case 2:
                ui.label_calibrated->setText(QString::number(aux,'f',dp.load_3));
                current_channel_data = fuzpid->to_gui.signed_raw[0];
                break;
            case 3:
                ui.label_calibrated->setText(QString::number(aux,'f',dp.displacement));
                current_channel_data = fuzpid->to_gui.signed_raw[1];
                break;
            case 4:
                ui.label_calibrated->setText(QString::number(aux,'f',ch3.dp));
                current_channel_data = fuzpid->to_gui.signed_raw[2];
                break;
            case 5:
                ui.label_calibrated->setText(QString::number(aux,'f',ch4.dp));
                current_channel_data = fuzpid->to_gui.signed_raw[3];
                break;
            case 6:
                ui.label_calibrated->setText(QString::number(aux,'f',3));
                current_channel_data = fuzpid->to_gui.signed_raw[4];
                break;
            default:
                break;
        }

        ui.label_adc_channel_raw_data->setText(QString::number(current_channel_data));
    }

    if(current_tab_index == TAB_TEST){
        if(fuzpid->test_status == TEST_JUST_FINISHED){
            switch (load_calibration_channel) {
            case LOAD_1:
                ui.label_main_load->setText(QString::number(fuzpid->peak_load,'f',dp.load_1));
                break;
            case LOAD_2:
                ui.label_main_load->setText(QString::number(fuzpid->peak_load,'f',dp.load_2));
                break;
            case LOAD_3:
                ui.label_main_load->setText(QString::number(fuzpid->peak_load,'f',dp.load_3));
                break;
            default:
                break;
            }
            ui.label_main_stress->setText(QString::number(fuzpid->peak_stress,'f',dp.stress));
        }
        else{
            switch (load_calibration_channel) {
            case LOAD_1:
                ui.label_main_load->setText(QString::number(fuzpid->load_value,'f',dp.load_1));
                break;
            case LOAD_2:
                ui.label_main_load->setText(QString::number(fuzpid->load_value,'f',dp.load_2));
                break;
            case LOAD_3:
                ui.label_main_load->setText(QString::number(fuzpid->load_value,'f',dp.load_3));
                break;
            default:
                break;
            }
            ui.label_main_stress->setText(QString::number(fuzpid->stress,'f',dp.stress));
        }
        ui.label_main_displacement->setText(QString::number(((1.0*fuzpid->displacement_value)),'f',dp.displacement));
        ui.label_main_ch3->setText(QString::number(((1.0*fuzpid->ch3_value)),'f',ch3.dp));
        ui.label_main_ch4->setText(QString::number(((1.0*fuzpid->ch4_value)),'f',ch4.dp));

        if(fuzpid->test_label_status){
            fuzpid->test_label_status = false;
            ui.label_test_status->setText(trUtf8("THRESHOLD is EXCEEDED.."));
            ui.label_test_status_cbr->setText(trUtf8("THRESHOLD is EXCEEDED.."));
        }

        stress_calculation();

        if((fuzpid->test_status == TEST_RUNNING)||(fuzpid->test_status == TEST_PAUSED)){
            if(fuzpid->stress > fuzpid->peak_stress){
                fuzpid->peak_stress = fuzpid->stress;
            }
        }
    }

    if(current_tab_index != TAB_CALIBRATION){
        static u8 error_counter = 0;
        if(fuzpid->error.max_load == false){
            if(fuzpid->load_value >= (ui.doubleSpinBox_max_load->value())){
                error_counter++;
                if(error_counter > 10){
                    error_counter = 0;
                    fuzpid->hard_stop = true;
                    fuzpid->error.max_load = true;
                    QTimer::singleShot(500,this,SLOT(error_messagebox()));
                    //ui.label_alert_status->setText(trUtf8("MAX LOAD ERROR!!"));
                }
            }
            else{
                error_counter = 0;
            }
        }
        else{
            if(fuzpid->load_value < (ui.doubleSpinBox_max_load->value())){
                fuzpid->error.max_load = false;
                error_counter = 0;
            }
        }
        if(fuzpid->error.max_displacement == false){
            if(fuzpid->displacement_value >= (ui.doubleSpinBox_max_displacement->value())){
                fuzpid->hard_stop = true;
                fuzpid->error.max_displacement = true;
            }
        }
        else{
            if(fuzpid->displacement_value < (ui.doubleSpinBox_max_displacement->value())){
                fuzpid->error.max_displacement = false;
            }
        }
        if(fuzpid->error.max_load == true){
            ui.label_alert_status->setText(trUtf8("MAX LOAD ERROR!!"));
            ui.label_alert_status_cbr->setText(trUtf8("MAX LOAD ERROR!!"));
        }
        else if(fuzpid->error.max_displacement == true){
            ui.label_alert_status->setText(trUtf8("MAX DISPL. ERROR!!"));
            ui.label_alert_status_cbr->setText(trUtf8("MAX DISPL. ERROR!!"));
        }
        else{
            ui.label_alert_status->setText(trUtf8("NO ALERT"));
            ui.label_alert_status_cbr->setText(trUtf8("NO ALERT"));
        }
    }
    test_finishing();
}
void DC_Motor_PC::_100_msec_handler(){
    static bool do_at_the_opening = true;

    if(do_at_the_opening){
        if(fuzpid->communication_established){
            gain_send_timer->start();
            fuzpid->command_silencer = true;
            do_at_the_opening = false;
        }
    }

    return_home_operation();
    go_load_operation();
    update_gui();
    if((fuzpid->test_status == TEST_RUNNING)||(fuzpid->test_status == TEST_PAUSED)){
        excel_recording();
    }
    else{   //TEST_STOPPED
        if(JOG.error == false){
            JOG_operation();
        }
    }
    ethernet_periodic_sending();
}
void DC_Motor_PC::set_datetime(){
    if(prevent_double_click()) return;
    QString year = QString::number(ui.spinBox_year->value());
    QString month = QString::number(ui.spinBox_month->value());
    QString day = QString::number(ui.spinBox_day->value());
    QString hour = QString::number(ui.spinBox_hour->value());
    QString min = QString::number(ui.spinBox_min->value());
    QString secs = QString::number(ui.spinBox_secs->value());

    if(ui.spinBox_month->value() < 10)  month = QString("0%1").arg(month);
    if(ui.spinBox_day->value() < 10)    day = QString("0%1").arg(day);
    if(ui.spinBox_hour->value() < 10)   hour = QString("0%1").arg(hour);
    if(ui.spinBox_min->value() < 10)    min = QString("0%1").arg(min);
    if(ui.spinBox_secs->value() < 10)   secs = QString("0%1").arg(secs);

    QString str = QString("%1.%2.%3-%4:%5:%6").arg(year).arg(month).arg(day).arg(hour).arg(min).arg(secs);
    QProcess::execute(QString("date %1").arg(str));
    /* set hw clock */
    QProcess::execute("hwclock -w"); //TODO: report RTC errors

}
void DC_Motor_PC::tab_change_handler(int i){
    static u8 last_index = 0;

    if(data_changed){
        data_changed = false;
        if(correctly_opened == true){
            writeAppSettings();
        }
    }
    ui.pushButton_focus->setFocus();
    current_tab_index = i;
    fuzpid->from_gui.current_tab_index = i;

    if(i == TAB_CALIBRATION){
        fuzpid->relay_auto_man = RELAY_OFF;
    }
    else{
        if(control_mode == AUTO){
            fuzpid->relay_auto_man = RELAY_ON;
        }
        else{
            fuzpid->relay_auto_man = RELAY_OFF;
        }
        if(i == TAB_TEST_RESULTS){
            ui.spinBox_test_no->setValue(current_test_no);
            read_test_results_from_file();
        }
        if(i == TAB_ADMIN){
            if(ui.tabWidget_admin->currentIndex() == 2){
                fuzpid->relay_auto_man = RELAY_ON;
            }
        }
    }
    if(last_index == TAB_CALIBRATION){
        ui.pushButton_stop->click();
    }
    last_index = i;
}
void DC_Motor_PC::tab_admin_change_handler(int i){
    if(i == 2){ //this is for autotunning
        fuzpid->relay_auto_man = RELAY_ON;
    }
    else{
        if(control_mode == AUTO){
            fuzpid->relay_auto_man = RELAY_ON;
        }
        else{
            fuzpid->relay_auto_man = RELAY_OFF;
        }
    }
}
void DC_Motor_PC::set_gradient(void){
    QFile file;

    file.setFileName(":/qss/qmc2-black-fast/qmc2-black-0.10.qss");

    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    setStyleSheet(styleSheet);
    currentStyleSheet = styleSheet;
}
DC_Motor_PC::~DC_Motor_PC(){
#ifdef CONFIG_x86
    if(correctly_opened == true){
        writeAppSettings();
    }
#endif
    excel_file.close();
#ifdef MATLAB_RECORDINGS
    matlab_log.close();
#endif

}
void DC_Motor_PC::closeEvent(QCloseEvent *ev){
    ev->accept();
    //fuzzy_screen->close();
}
void DC_Motor_PC::main_screen_signalmapper_handler(int i){
    if(prevent_double_click()) return;

    ui.tabWidget->setCurrentIndex(i);
//    if(i < 5){
//        ui.tabWidget->setCurrentIndex(i);
//    }
//    else{
//        if(admin_authorization == false){
//            pass = new password(this);
//            pass->setGeometry(200,90,400,200);
//            connect(pass,SIGNAL(emit_text_value(QString)),this,SLOT(admin_authorization_handler(QString)));
//            pass->exec();
//        }
//        else{
//            ui.tabWidget->setCurrentIndex(i);
//        }
//    }
}
void DC_Motor_PC::settings_screen_signalmapper_handler(int i){
    if(prevent_double_click()) return;

    if(i == 0){
        if(ui.tabWidget_settings->currentIndex() == 0){
            ui.tabWidget->setCurrentIndex(0);
        }
        else{
            ui.tabWidget_settings->setCurrentIndex(0);
        }
    }
    else{
        ui.tabWidget_settings->setCurrentIndex(i);
    }
}
void DC_Motor_PC::admin_screen_signalmapper_handler(int i){
    if(prevent_double_click()) return;

    if(i == 0){
        if(ui.tabWidget_admin->currentIndex() == 0){
            ui.tabWidget->setCurrentIndex(0);
        }
        else{
            ui.tabWidget_admin->setCurrentIndex(0);
        }
    }
    else{
        ui.tabWidget_admin->setCurrentIndex(i);
    }
}
