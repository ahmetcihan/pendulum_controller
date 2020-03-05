#include "dc_motor_pc.h"

void DC_Motor_PC::copy_data_to_usb(){
    if(prevent_double_click()) return;
    static u8 tmp = 0;
    static QLabel deneme_label;
    QFile usb_flash_drive("/dev/sda1");

    switch(tmp){
    case 0:
        if(usb_flash_drive.exists()){
            deneme_label.setGeometry(150,220,500,100);
            deneme_label.setFont(QFont("Ubuntu",16,QFont::Black,true));
            deneme_label.setText(QString(trUtf8("Please wait.. \n"
                                                "mount /dev/sda1 /media/usb")));
            deneme_label.show();
            QProcess::execute("mount /dev/sda1 /media/usb");
            QTimer::singleShot(5000,this,SLOT(copy_data_to_usb()));
            tmp++;
        }
        else{
            deneme_label.setGeometry(150,220,500,100);
            deneme_label.setFont(QFont("Ubuntu",16,QFont::Black,true));
            deneme_label.setText(QString(trUtf8("USB Flash Drive is not detected\n"
                                                "Please format to FAT32")));
            deneme_label.show();
            QTimer::singleShot(5000,this,SLOT(copy_data_to_usb()));
            tmp = 4;
        }
        break;
    case 1:
        deneme_label.setText(QString(trUtf8("Please wait.. \n"
                                            "Copying Test Results")));
        QProcess::execute("cp -r /usr/local/bin/test_results /media/usb");
        QTimer::singleShot(5000,this,SLOT(copy_data_to_usb()));
        tmp++;
        break;
    case 2:
        deneme_label.setText(QString(trUtf8("Please wait.. \n"
                                            "Copying Data Logs")));
        QProcess::execute("cp -r /usr/local/bin/data_logs /media/usb");
        QTimer::singleShot(5000,this,SLOT(copy_data_to_usb()));
        tmp++;
        break;
    case 3:
        deneme_label.setText(QString(trUtf8("Please wait.. \n"
                                            "umount /media/usb")));
        QProcess::execute("umount /media/usb");
        QTimer::singleShot(5000,this,SLOT(copy_data_to_usb()));
        tmp++;
        break;
    case 4:
        deneme_label.hide();
        tmp = 0;
        break;
    }
}
void DC_Motor_PC::copy_logo_files(){
    if(prevent_double_click()) return;
    static u8 tmp = 0;
    static QLabel deneme_label;
    QPixmap pixmap;
    QFile address_logo_file("/media/usb/ADDRESS_LOGO.jpg");
    QFile splash_file("/media/usb/SPLASH.rgb565");
    QFile usb_flash_drive("/dev/sda1");

    switch(tmp){
    case 0:
        if(usb_flash_drive.exists()){
            deneme_label.setGeometry(150,220,500,100);
            deneme_label.setFont(QFont("Ubuntu",16,QFont::Black,true));
            deneme_label.setText(QString(trUtf8("Please wait.. \n"
                                                "mount /dev/sda1 /media/usb")));
            deneme_label.show();
            QProcess::execute("mount /dev/sda1 /media/usb");
            QTimer::singleShot(5000,this,SLOT(copy_logo_files()));
            tmp++;
        }
        else{
            deneme_label.setGeometry(150,220,500,100);
            deneme_label.setFont(QFont("Ubuntu",16,QFont::Black,true));
            deneme_label.setText(QString(trUtf8("USB Flash Drive is not detected\n"
                                                "Please format to FAT32")));
            deneme_label.show();
            QTimer::singleShot(5000,this,SLOT(copy_logo_files()));
            tmp = 6;
        }
        break;
    case 1:
        if(address_logo_file.exists()){
            deneme_label.setText(trUtf8("copying company logo and address.."));
            QProcess::execute("cp /media/usb/ADDRESS_LOGO.jpg /usr/local/bin/");
            tmp++;
        }
        else{
            deneme_label.setText(trUtf8("ADDRESS_LOGO.jpg file doesn't exist.."));
            tmp = 5;
        }
        QTimer::singleShot(1000,this,SLOT(copy_logo_files()));
        break;
    case 2:
        pixmap.load("ADDRESS_LOGO.jpg");           // load picture
        this->ui.label_utest_address_logo->setPixmap(pixmap);
        QTimer::singleShot(1000,this,SLOT(copy_logo_files()));
        tmp++;
        break;
    case 3:
        if(splash_file.exists()){
            deneme_label.setText(trUtf8("copying splash logo.."));
            QProcess::execute("flash_eraseall /dev/mtd4");
            tmp++;
        }
        else{
            deneme_label.setText(trUtf8("SPLASH.rgb565 file doesn't exist.."));
            tmp = 5;
        }
        QTimer::singleShot(1000,this,SLOT(copy_logo_files()));
        break;
    case 4:
        QProcess::execute("cp /media/usb/SPLASH.rgb565 /dev/mtd4");
        QTimer::singleShot(4000,this,SLOT(copy_logo_files()));
        tmp++;
        break;
    case 5:
        deneme_label.setText(QString(trUtf8("Please wait.. \n"
                                            "umount /media/usb")));
        QProcess::execute("umount /media/usb");
        QTimer::singleShot(5000,this,SLOT(copy_logo_files()));
        tmp++;
        break;
    case 6:
        deneme_label.hide();
        tmp = 0;
        break;
    }
}
void DC_Motor_PC::copy_calibration_from_usb(){
    if(prevent_double_click()) return;
    static u8 tmp = 0;
    static QLabel deneme_label;
    QFile calibration_file("/media/usb/calibration.ini");
    QFile usb_flash_drive("/dev/sda1");

    switch(tmp){
    case 0:
        if(usb_flash_drive.exists()){
            deneme_label.setGeometry(150,220,500,100);
            deneme_label.setFont(QFont("Ubuntu",16,QFont::Black,true));
            deneme_label.setText(QString(trUtf8("Please wait.. \n"
                                                "mount /dev/sda1 /media/usb")));
            deneme_label.show();
            QProcess::execute("mount /dev/sda1 /media/usb");
            QTimer::singleShot(5000,this,SLOT(copy_calibration_from_usb()));
            tmp++;
        }
        else{
            deneme_label.setGeometry(150,220,500,100);
            deneme_label.setFont(QFont("Ubuntu",16,QFont::Black,true));
            deneme_label.setText(QString(trUtf8("USB Flash Drive is not detected\n"
                                                "Please format to FAT32")));
            deneme_label.show();
            QTimer::singleShot(5000,this,SLOT(copy_calibration_from_usb()));
            tmp = 3;
        }
        break;
    case 1:
        if(calibration_file.exists()){
            deneme_label.setText(trUtf8("copying calibration.ini"));
            QProcess::execute("cp /media/usb/calibration.ini /usr/local/bin/test_results/");
        }
        else{
            deneme_label.setText(trUtf8("calibration.ini file doesn't exist.."));
        }
        QTimer::singleShot(2000,this,SLOT(copy_calibration_from_usb()));
        tmp++;
        break;
    case 2:
        deneme_label.setText(QString(trUtf8("Please wait.. \n"
                                            "umount /media/usb")));
        QProcess::execute("umount /media/usb");
        QTimer::singleShot(5000,this,SLOT(copy_calibration_from_usb()));
        tmp++;
        break;
    case 3:
        deneme_label.hide();
        tmp = 0;
        break;
    }
}
void DC_Motor_PC::copy_calibration_to_usb(){
    if(prevent_double_click()) return;
    static u8 tmp = 0;
    static QLabel deneme_label;
    QFile usb_flash_drive("/dev/sda1");

    switch(tmp){
    case 0:
        if(usb_flash_drive.exists()){
            deneme_label.setGeometry(150,220,500,100);
            deneme_label.setFont(QFont("Ubuntu",16,QFont::Black,true));
            deneme_label.setText(QString(trUtf8("Please wait.. \n"
                                                "mount /dev/sda1 /media/usb")));
            deneme_label.show();
            QProcess::execute("mount /dev/sda1 /media/usb");
            QTimer::singleShot(5000,this,SLOT(copy_calibration_to_usb()));
            tmp++;
        }
        else{
            deneme_label.setGeometry(150,220,500,100);
            deneme_label.setFont(QFont("Ubuntu",16,QFont::Black,true));
            deneme_label.setText(QString(trUtf8("USB Flash Drive is not detected\n"
                                                "Please format to FAT32")));
            deneme_label.show();
            QTimer::singleShot(5000,this,SLOT(copy_calibration_to_usb()));
            tmp = 3;
        }
        break;
    case 1:
        deneme_label.setText(trUtf8("copying calibration.ini"));
        QProcess::execute("cp /usr/local/bin/test_results/calibration.ini /media/usb/");
        QTimer::singleShot(2000,this,SLOT(copy_calibration_to_usb()));
        tmp++;
        break;
    case 2:
        deneme_label.setText(QString(trUtf8("Please wait.. \n"
                                            "umount /media/usb")));
        QProcess::execute("umount /media/usb");
        QTimer::singleShot(5000,this,SLOT(copy_calibration_to_usb()));
        tmp++;
        break;
    case 3:
        deneme_label.hide();
        tmp = 0;
        break;
    }
}
void DC_Motor_PC::copy_factory_settings_from_usb(){
    if(prevent_double_click()) return;
    static u8 tmp = 0;
    static QLabel deneme_label;
    QFile factory_settings_file("/media/usb/factory_settings.ini");
    QFile usb_flash_drive("/dev/sda1");

    switch(tmp){
    case 0:
        if(usb_flash_drive.exists()){
            deneme_label.setGeometry(150,220,500,100);
            deneme_label.setFont(QFont("Ubuntu",16,QFont::Black,true));
            deneme_label.setText(QString(trUtf8("Please wait.. \n"
                                                "mount /dev/sda1 /media/usb")));
            deneme_label.show();
            QProcess::execute("mount /dev/sda1 /media/usb");
            QTimer::singleShot(5000,this,SLOT(copy_factory_settings_from_usb()));
            tmp++;
        }
        else{
            deneme_label.setGeometry(150,220,500,100);
            deneme_label.setFont(QFont("Ubuntu",16,QFont::Black,true));
            deneme_label.setText(QString(trUtf8("USB Flash Drive is not detected\n"
                                                "Please format to FAT32")));
            deneme_label.show();
            QTimer::singleShot(2000,this,SLOT(copy_factory_settings_from_usb()));
            tmp = 3;
        }
        break;
    case 1:
        if(factory_settings_file.exists()){
            deneme_label.setText(trUtf8("copying factory_settings.ini"));
            QProcess::execute("cp /media/usb/factory_settings.ini /usr/local/bin/");
        }
        else{
            deneme_label.setText(trUtf8("factory_settings.ini file doesn't exist.."));
        }
        QTimer::singleShot(2000,this,SLOT(copy_factory_settings_from_usb()));
        tmp++;
        break;
    case 2:
        deneme_label.setText(QString(trUtf8("Please wait.. \n"
                                            "umount /media/usb")));
        QProcess::execute("umount /media/usb");
        QTimer::singleShot(5000,this,SLOT(copy_factory_settings_from_usb()));
        tmp++;
        break;
    case 3:
        deneme_label.hide();
        tmp = 0;
        break;
    }
}
void DC_Motor_PC::copy_factory_settings_to_usb(){
    if(prevent_double_click()) return;
    static u8 tmp = 0;
    static QLabel deneme_label;
    QFile usb_flash_drive("/dev/sda1");

    switch(tmp){
    case 0:
        if(usb_flash_drive.exists()){
            deneme_label.setGeometry(150,220,500,100);
            deneme_label.setFont(QFont("Ubuntu",16,QFont::Black,true));
            deneme_label.setText(QString(trUtf8("Please wait.. \n"
                                                "mount /dev/sda1 /media/usb")));
            deneme_label.show();
            QProcess::execute("mount /dev/sda1 /media/usb");
            QTimer::singleShot(5000,this,SLOT(copy_factory_settings_to_usb()));
            tmp++;
        }
        else{
            deneme_label.setGeometry(150,220,500,100);
            deneme_label.setFont(QFont("Ubuntu",16,QFont::Black,true));
            deneme_label.setText(QString(trUtf8("USB Flash Drive is not detected\n"
                                                "Please format to FAT32")));
            deneme_label.show();
            QTimer::singleShot(2000,this,SLOT(copy_factory_settings_to_usb()));
            tmp = 3;
        }
        break;
    case 1:
        deneme_label.setText(trUtf8("copying factory_settings.ini"));
        QProcess::execute("cp /usr/local/bin/factory_settings.ini /media/usb/");
        QTimer::singleShot(2000,this,SLOT(copy_factory_settings_to_usb()));
        tmp++;
        break;
    case 2:
        deneme_label.setText(QString(trUtf8("Please wait.. \n"
                                            "umount /media/usb")));
        QProcess::execute("umount /media/usb");
        QTimer::singleShot(5000,this,SLOT(copy_factory_settings_to_usb()));
        tmp++;
        break;
    case 3:
        deneme_label.hide();
        tmp = 0;
        break;
    }
}
void DC_Motor_PC::copy_settings_from_usb(){
    if(prevent_double_click()) return;
    static u8 tmp = 0;
    static QLabel deneme_label;
    QFile settings_file("/media/usb/settings.ini");
    QFile usb_flash_drive("/dev/sda1");

    switch(tmp){
    case 0:
        if(usb_flash_drive.exists()){
            deneme_label.setGeometry(150,220,500,100);
            deneme_label.setFont(QFont("Ubuntu",16,QFont::Black,true));
            deneme_label.setText(QString(trUtf8("Please wait.. \n"
                                                "mount /dev/sda1 /media/usb")));
            deneme_label.show();
            QProcess::execute("mount /dev/sda1 /media/usb");
            QTimer::singleShot(5000,this,SLOT(copy_settings_from_usb()));
            tmp++;
        }
        else{
            deneme_label.setGeometry(150,220,500,100);
            deneme_label.setFont(QFont("Ubuntu",16,QFont::Black,true));
            deneme_label.setText(QString(trUtf8("USB Flash Drive is not detected\n"
                                                "Please format to FAT32")));
            deneme_label.show();
            QTimer::singleShot(2000,this,SLOT(copy_settings_from_usb()));
            tmp = 3;
        }
        break;
    case 1:
        if(settings_file.exists()){
            deneme_label.setText(trUtf8("copying settings.ini"));
            QProcess::execute("cp /media/usb/settings.ini /usr/local/bin/");
        }
        else{
            deneme_label.setText(trUtf8("settings.ini file doesn't exist.."));
        }
        QTimer::singleShot(2000,this,SLOT(copy_settings_from_usb()));
        tmp++;
        break;
    case 2:
        deneme_label.setText(QString(trUtf8("Please wait.. \n"
                                            "umount /media/usb")));
        QProcess::execute("umount /media/usb");
        QTimer::singleShot(5000,this,SLOT(copy_settings_from_usb()));
        tmp++;
        break;
    case 3:
        deneme_label.hide();
        tmp = 0;
        break;
    }
}
void DC_Motor_PC::copy_settings_to_usb(){
    if(prevent_double_click()) return;
    static u8 tmp = 0;
    static QLabel deneme_label;
    QFile usb_flash_drive("/dev/sda1");

    switch(tmp){
    case 0:
        if(usb_flash_drive.exists()){
            deneme_label.setGeometry(150,220,500,100);
            deneme_label.setFont(QFont("Ubuntu",16,QFont::Black,true));
            deneme_label.setText(QString(trUtf8("Please wait.. \n"
                                                "mount /dev/sda1 /media/usb")));
            deneme_label.show();
            QProcess::execute("mount /dev/sda1 /media/usb");
            QTimer::singleShot(5000,this,SLOT(copy_settings_to_usb()));
            tmp++;
        }
        else{
            deneme_label.setGeometry(150,220,500,100);
            deneme_label.setFont(QFont("Ubuntu",16,QFont::Black,true));
            deneme_label.setText(QString(trUtf8("USB Flash Drive is not detected\n"
                                                "Please format to FAT32")));
            deneme_label.show();
            QTimer::singleShot(2000,this,SLOT(copy_settings_to_usb()));
            tmp = 3;
        }
        break;
    case 1:
        deneme_label.setText(trUtf8("copying settings.ini"));
        QProcess::execute("cp /usr/local/bin/settings.ini /media/usb/");
        QTimer::singleShot(2000,this,SLOT(copy_settings_to_usb()));
        tmp++;
        break;
    case 2:
        deneme_label.setText(QString(trUtf8("Please wait.. \n"
                                            "umount /media/usb")));
        QProcess::execute("umount /media/usb");
        QTimer::singleShot(5000,this,SLOT(copy_settings_to_usb()));
        tmp++;
        break;
    case 3:
        deneme_label.hide();
        tmp = 0;
        break;
    }
}
