#include "dc_motor_pc.h"

void DC_Motor_PC::fill_after_language_change(){

    unit_system_handler();

    ui.comboBox_pid->setItemText(0,trUtf8("CBR"));
    ui.comboBox_pid->setItemText(1,trUtf8("MARSHALL"));
    ui.comboBox_pid->setItemText(2,trUtf8("IDT"));
    ui.comboBox_pid->setItemText(3,trUtf8("SCB"));
    ui.comboBox_pid->setItemText(4,trUtf8("TRIAXIAL"));
    ui.comboBox_pid->setItemText(5,trUtf8("UU"));
    ui.comboBox_pid->setItemText(6,trUtf8("CU-CD"));
    ui.comboBox_pid->setItemText(7,trUtf8("UNIAXIAL"));
    ui.comboBox_pid->setItemText(8,trUtf8("LOAD CTRL"));
    ui.comboBox_pid->setItemText(9,trUtf8("DISPL CTRL"));

    if(fuzpid->error.max_load == true){
        ui.label_alert_status->setText(trUtf8("MAX LOAD ERROR!!"));
    }
    else if(fuzpid->error.max_displacement == true){
        ui.label_alert_status->setText(trUtf8("MAX DISPL. ERROR!!"));
    }
    else{
        ui.label_alert_status->setText(trUtf8("NO ALERT"));
    }

    ui.label_test_no_main_cbr->setText(trUtf8("CBR Test No : %1").arg(current_test_no));
    ui.label_test_no_main_marshall->setText(trUtf8("Test No : %1").arg(current_test_no));
    ui.label_test_status_cbr->setText(trUtf8("TEST STOPPED.."));

#ifdef CONFIG_x86
qDebug(__FUNCTION__);
#endif
}
void DC_Motor_PC::language_load(void){
    QFont system_font;
    static u8 do_once = 1;

    u8 old_language = language_index;

    QString lang = "en";
    system_font.setFamily("Bitstream Vera Sans");

    if(ui.radioButton_english->isChecked()){
        language_index = 0;
        lang = "en";

        QFont font = ui.tabWidget->font();
        font.setPointSize(18);
        ui.tabWidget->setFont(font);
    }
    else if(ui.radioButton_turkish->isChecked()){
        language_index = 1;
        lang = "tr";

        QFont font = ui.tabWidget->font();
        font.setPointSize(17);
        ui.tabWidget->setFont(font);
    }
    else if(ui.radioButton_french->isChecked()){
        language_index = 2;
        lang = "fr";

        QFont font = ui.tabWidget->font();
        font.setPointSize(17);
        ui.tabWidget->setFont(font);
    }
    else if(ui.radioButton_spanish->isChecked()){
        language_index = 3;
        lang = "sp";

        QFont font = ui.tabWidget->font();
        font.setPointSize(17);
        ui.tabWidget->setFont(font);
    }
    else if(ui.radioButton_russian->isChecked()){
        language_index = 4;
        lang = "ru";
        system_font.setFamily("Amerika");

        QFont font = ui.tabWidget->font();
        font.setPointSize(15);
        ui.tabWidget->setFont(font);
    }
#ifdef CONFIG_x86
    QString filename = QString("/home/ahmet/Desktop/QT-Projects/multiplex/multiplex_%1.qm").arg(lang);
#else
    QString filename = QString("/usr/local/bin/multiplex_%1.qm").arg(lang);
#endif

    if (QFile::exists(filename)) {
        QCoreApplication *a = QApplication::instance();
        if (translator) {
            a->removeTranslator(translator);
            translator->deleteLater();
        }
        translator = new QTranslator(this);
        bool loaded = translator->load(filename);
        qDebug("translating to '%s' %s", qPrintable(lang), loaded ? "succeeded" : "failed");
        a->installTranslator(translator);
        ui.retranslateUi(this);
    }
    data_changed = true;
    QTimer::singleShot(1000,this,SLOT(fill_after_language_change()));

#ifndef CONFIG_x86
    if(do_once == 1){
        do_once = 0;
        QApplication::setFont(system_font);
        QString styleSheet = QString("font-family:%1;").arg(system_font.family());
        this->setStyleSheet(styleSheet);
        set_gradient();
    }
    else{
        if(((old_language < 4)&&(language_index == 4))||((old_language == 4)&&(language_index < 4))){
            QApplication::setFont(system_font);
            QString styleSheet = QString("font-family:%1;").arg(system_font.family());
            this->setStyleSheet(styleSheet);
            set_gradient();
        }
    }
#endif
}

