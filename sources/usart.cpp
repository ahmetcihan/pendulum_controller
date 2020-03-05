#include "dc_motor_pc.h"

void DC_Motor_PC::EOL(char *base_array, u8 i){
    base_array[i] 	= 0x0D;
    base_array[i+1] = 0x0A;
}
void fuzzy_pid::EOL(char *base_array, u8 i){
    base_array[i] 	= 0x0D;
    base_array[i+1] = 0x0A;
}
void fuzzy_pid::always_send(void){
    QByteArray data;
    data.resize(16);

    send_data_order(data.data(),"CONV",0,3);
    data[4] = send_fuzzy_raw_servo_speed;
    data[5] = (((u32)fuzzy_raw_servo_speed) / 65536) % 256;
    data[6] = (((u32)fuzzy_raw_servo_speed) / 256) % 256;
    data[7] = (((u32)fuzzy_raw_servo_speed) / 1) % 256;

    data[8] = relay_auto_man;
    data[9] = relay_start_stop;

    data[10] = dcMotorPc->servo.stop;
    data[11] = dcMotorPc->servo.start;
    data[12] = dcMotorPc->servo.down;
    data[13] = dcMotorPc->servo.up;

    EOL(data.data(),14);

    send_fuzzy_raw_servo_speed = 0;
    dcMotorPc->servo.start = 0;
    dcMotorPc->servo.stop = 0;
    dcMotorPc->servo.down = 0;
    dcMotorPc->servo.up = 0;

    pSerial->write(data);
}

void DC_Motor_PC::send_data_order(char *base_array, const char *array, u8 first_index, u8 last_index){
    u8 i;

    for(i = first_index; i < last_index+1; i++){
        base_array[i] = array[i];
    }
}
void DC_Motor_PC::command_sending_protection(void){
    static u8 tmp = 0;

    switch(tmp){
    case 0:
        fuzpid->read_parameters();
        tmp++;
        break;
    case 1:
        if(usart_signalmapper_no < 44){
            fuzpid->send_gain(usart_signalmapper_no);
        }
        else if(usart_signalmapper_no < 49){
            fuzpid->tare_channel(usart_signalmapper_no - 44);
        }
        else if(usart_signalmapper_no == 50){
            send_calibration_values();
        }
        tmp++;
        break;
    case 2:
        fuzpid->read_parameters();
        tmp++;
        break;
    case 3:
        command_send_protection_wait_timer->stop();
        fuzpid->command_silencer = false;
        tmp = 0;
        break;
    }
}
void fuzzy_pid::send_gains(void){
    static u8 tmp = 0;

    switch(tmp){
    case 0:
        tmp++;
        break;
    case 1:
        tmp++;
        break;
    case 2:
        send_gain(gain[0]);    //gain_load
        tmp++;
        break;
    case 3:
        send_gain(11 + gain[1]);    //gain_ch2
        tmp++;
        break;
    case 4:
        send_gain(22 + gain[2]);    //gain_ch3
        tmp++;
        break;
    case 5:
        send_gain(33 + gain[3]);    //gain_ch4
        tmp++;
        break;
    case 6:
        tmp = 0;
        dcMotorPc->gain_send_timer->stop();
        command_silencer = false;
        break;
    }
}
void DC_Motor_PC::serial_port_setup(void){
#ifdef CONFIG_x86
    fuzpid->pSerial = new QextSerialPort("/dev/ttyUSB0");
#else
    fuzpid->pSerial = new QextSerialPort("/dev/ttyS0");
#endif
    fuzpid->pSerial->setQueryMode(QextSerialPort::EventDriven);

    fuzpid->pSerial->setBaudRate(BAUD115200);
    fuzpid->pSerial->setFlowControl(FLOW_OFF);
    fuzpid->pSerial->setParity(PAR_NONE);
    fuzpid->pSerial->setDataBits(DATA_8);
    fuzpid->pSerial->setStopBits(STOP_1);
    fuzpid->pSerial->open(QIODevice::ReadWrite | QIODevice::Unbuffered);
    fuzpid->pSerial->setTimeout(0);
    qDebug("is open: %d", fuzpid->pSerial->isOpen());
    fuzpid->pSerial->reset();
}

void DC_Motor_PC::usart_signalmapper(void){
    signalMapper_usart = new QSignalMapper(this);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_load_0,         0);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_load_1,         1);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_load_2,         2);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_load_3,         3);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_load_4,         4);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_load_5,         5);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_load_6,         6);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_load_7,         7);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_load_8,         8);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_load_9,         9);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_load_10,        10);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_displacement_0, 11);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_displacement_1, 12);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_displacement_2, 13);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_displacement_3, 14);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_displacement_4, 15);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_displacement_5, 16);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_displacement_6, 17);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_displacement_7, 18);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_displacement_8, 19);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_displacement_9, 20);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_displacement_10,21);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_extensometer_0, 22);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_extensometer_1, 23);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_extensometer_2, 24);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_extensometer_3, 25);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_extensometer_4, 26);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_extensometer_5, 27);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_extensometer_6, 28);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_extensometer_7, 29);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_extensometer_8, 30);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_extensometer_9, 31);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_extensometer_10, 32);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_ch4_0,          33);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_ch4_1,          34);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_ch4_2,          35);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_ch4_3,          36);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_ch4_4,          37);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_ch4_5,          38);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_ch4_6,          39);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_ch4_7,          40);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_ch4_8,          41);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_ch4_9,          42);
    signalMapper_usart->setMapping(this->ui.pushButton_set_gain_ch4_10,         43);
    signalMapper_usart->setMapping(this->ui.pushButton_tare_load,               44);
    signalMapper_usart->setMapping(this->ui.pushButton_tare_displacement,       45);
    signalMapper_usart->setMapping(this->ui.pushButton_tare_ch3,                46);
    signalMapper_usart->setMapping(this->ui.pushButton_tare_ch4,                47);
    signalMapper_usart->setMapping(this->ui.pushButton_save_all_cal_data,       50);

    connect(this->ui.pushButton_set_gain_load_0,            SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_load_1,            SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_load_2,            SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_load_3,            SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_load_4,            SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_load_5,            SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_load_6,            SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_load_7,            SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_load_8,            SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_load_9,            SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_load_10,           SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_displacement_0,    SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_displacement_1,    SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_displacement_2,    SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_displacement_3,    SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_displacement_4,    SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_displacement_5,    SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_displacement_6,    SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_displacement_7,    SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_displacement_8,    SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_displacement_9,    SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_displacement_10,   SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_extensometer_0,    SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_extensometer_1,    SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_extensometer_2,    SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_extensometer_3,    SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_extensometer_4,    SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_extensometer_5,    SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_extensometer_6,    SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_extensometer_7,    SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_extensometer_8,    SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_extensometer_9,    SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_extensometer_10,   SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_ch4_0,             SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_ch4_1,             SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_ch4_2,             SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_ch4_3,             SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_ch4_4,             SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_ch4_5,             SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_ch4_6,             SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_ch4_7,             SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_ch4_8,             SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_ch4_9,             SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_gain_ch4_10,            SIGNAL(clicked()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_tare_load,                  SIGNAL(pressed()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_tare_displacement,          SIGNAL(pressed()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_tare_ch3,                   SIGNAL(pressed()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_tare_ch4,                   SIGNAL(pressed()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_save_all_cal_data,          SIGNAL(pressed()),signalMapper_usart, SLOT (map()));

    connect(signalMapper_usart, SIGNAL(mapped(int)),this,SLOT(usart_signalmapper_handler(int)));

}
void DC_Motor_PC::usart_signalmapper_handler(int i){
    usart_signalmapper_no = i;
    command_send_protection_wait_timer->start();
    fuzpid->command_silencer = true;
}
