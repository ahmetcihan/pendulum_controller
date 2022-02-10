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
    data.resize(15);

    send_data_order(data.data(),"CONV",0,3);
    data[4] = relay_auto_man;
    data[5] = relay_start_stop;
    data[6] = step_motor_command;

    data[7] = (dcMotorPc->ui.spinBox_go_pos->value() / 256) % 256;
    data[8] = (dcMotorPc->ui.spinBox_go_pos->value()) % 256;

    if(step_motor_in_test == 0){
        data[9] = ((dcMotorPc->step_motor_speed / 65536) % 256);
        data[10] = ((dcMotorPc->step_motor_speed / 256) % 256);
        data[11] = ((dcMotorPc->step_motor_speed) % 256);
    }
    else{
        data[9] = ((dcMotorPc->ui.spinBox_step_motor_speed->value() / 65536) % 256);
        data[10] = ((dcMotorPc->ui.spinBox_step_motor_speed->value() / 256) % 256);
        data[11] = ((dcMotorPc->ui.spinBox_step_motor_speed->value()) % 256);
    }

    data[12] = TMC_command;

    EOL(data.data(),13);

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
        else if(usart_signalmapper_no < 54){
            fuzpid->send_channel_polarity(usart_signalmapper_no - 50, 0);
        }
        else if(usart_signalmapper_no < 58){
            fuzpid->send_channel_polarity(usart_signalmapper_no - 54, 1);
        }
        else if(usart_signalmapper_no == 60){
            hold_calibration_values();
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
void fuzzy_pid::send_all_parameters(void){
    static u8 tmp = 0;
    QByteArray data;
    data.resize(72);

    switch(tmp){
    case 0:
        command_silencer = true;
        QTimer::singleShot(150,this,SLOT(send_all_parameters()));
        tmp++;
        break;
    case 1:
        QTimer::singleShot(500,this,SLOT(send_all_parameters()));
        tmp++;
        send_data_order(data.data(),"PRMT",0,3);

        char_to_f.u32_val = dcMotorPc->ui.doubleSpinBox_headshake_speed->value();
        data[4] = char_to_f.u8_val[0];
        data[5] = char_to_f.u8_val[1];
        data[6] = char_to_f.u8_val[2];
        data[7] = char_to_f.u8_val[3];

        char_to_f.float_val = dcMotorPc->ui.doubleSpinBox_headshake_direction_timer->value();
        data[8] = char_to_f.u8_val[0];
        data[9] = char_to_f.u8_val[1];
        data[10] = char_to_f.u8_val[2];
        data[11] = char_to_f.u8_val[3];

        char_to_f.float_val = dcMotorPc->ui.doubleSpinBox_pendulum_mid_point->value();
        data[12] = char_to_f.u8_val[0];
        data[13] = char_to_f.u8_val[1];
        data[14] = char_to_f.u8_val[2];
        data[15] = char_to_f.u8_val[3];

        char_to_f.float_val = dcMotorPc->ui.doubleSpinBox_pendulum_top_boundary->value();
        data[16] = char_to_f.u8_val[0];
        data[17] = char_to_f.u8_val[1];
        data[18] = char_to_f.u8_val[2];
        data[19] = char_to_f.u8_val[3];

        data[20] = 0;

        char_to_f.float_val = 0;
        data[21] = char_to_f.u8_val[0];
        data[22] = char_to_f.u8_val[1];
        data[23] = char_to_f.u8_val[2];
        data[24] = char_to_f.u8_val[3];

        char_to_f.u32_val = dcMotorPc->speed_correction(dcMotorPc->ui.doubleSpinBox_step_second_speed->value());
        data[25] = char_to_f.u8_val[0];
        data[26] = char_to_f.u8_val[1];
        data[27] = char_to_f.u8_val[2];
        data[28] = char_to_f.u8_val[3];

        data[29] = dcMotorPc->ui.spinBox_step_transition_time->value();

        char_to_f.float_val = dcMotorPc->ui.doubleSpinBox_pendulum_kp->value();
        data[30] = char_to_f.u8_val[0];
        data[31] = char_to_f.u8_val[1];
        data[32] = char_to_f.u8_val[2];
        data[33] = char_to_f.u8_val[3];
        char_to_f.float_val = dcMotorPc->ui.doubleSpinBox_pendulum_ki->value();
        data[34] = char_to_f.u8_val[0];
        data[35] = char_to_f.u8_val[1];
        data[36] = char_to_f.u8_val[2];
        data[37] = char_to_f.u8_val[3];
        char_to_f.float_val = dcMotorPc->ui.doubleSpinBox_pendulum_kd->value();
        data[38] = char_to_f.u8_val[0];
        data[39] = char_to_f.u8_val[1];
        data[40] = char_to_f.u8_val[2];
        data[41] = char_to_f.u8_val[3];

        char_to_f.float_val = dcMotorPc->ui.doubleSpinBox_pendulum_kp_down->value();
        data[42] = char_to_f.u8_val[0];
        data[43] = char_to_f.u8_val[1];
        data[44] = char_to_f.u8_val[2];
        data[45] = char_to_f.u8_val[3];
        char_to_f.float_val = dcMotorPc->ui.doubleSpinBox_pendulum_ki_down->value();
        data[46] = char_to_f.u8_val[0];
        data[47] = char_to_f.u8_val[1];
        data[48] = char_to_f.u8_val[2];
        data[49] = char_to_f.u8_val[3];
        char_to_f.float_val = dcMotorPc->ui.doubleSpinBox_pendulum_kd_down->value();
        data[50] = char_to_f.u8_val[0];
        data[51] = char_to_f.u8_val[1];
        data[52] = char_to_f.u8_val[2];
        data[53] = char_to_f.u8_val[3];

        char_to_f.float_val = dcMotorPc->ui.doubleSpinBox_lqr_k1->value();
        data[54] = char_to_f.u8_val[0];
        data[55] = char_to_f.u8_val[1];
        data[56] = char_to_f.u8_val[2];
        data[57] = char_to_f.u8_val[3];
        char_to_f.float_val = dcMotorPc->ui.doubleSpinBox_lqr_k2->value();
        data[58] = char_to_f.u8_val[0];
        data[59] = char_to_f.u8_val[1];
        data[60] = char_to_f.u8_val[2];
        data[61] = char_to_f.u8_val[3];
        char_to_f.float_val = dcMotorPc->ui.doubleSpinBox_lqr_k3->value();
        data[62] = char_to_f.u8_val[0];
        data[63] = char_to_f.u8_val[1];
        data[64] = char_to_f.u8_val[2];
        data[65] = char_to_f.u8_val[3];
        char_to_f.float_val = dcMotorPc->ui.doubleSpinBox_lqr_k4->value();
        data[66] = char_to_f.u8_val[0];
        data[67] = char_to_f.u8_val[1];
        data[68] = char_to_f.u8_val[2];
        data[69] = char_to_f.u8_val[3];

        EOL(data.data(),70);

        pSerial->write(data);
        break;
    case 2:
        tmp = 0;
        command_silencer = false;
        break;
    }

#ifdef CONFIG_x86
    qDebug(__FUNCTION__);
#endif

}
void fuzzy_pid::send_mid_up(void){
    static u8 tmp = 0;
    QByteArray data;
    data.resize(8);

    switch(tmp){
    case 0:
        command_silencer = true;
        QTimer::singleShot(150,this,SLOT(send_mid_up()));
        tmp++;
        break;
    case 1:
        QTimer::singleShot(200,this,SLOT(send_mid_up()));
        tmp++;
        send_data_order(data.data(),"MIDUP",0,5);

        EOL(data.data(),6);

        pSerial->write(data);
        break;
    case 2:
        tmp = 0;
        command_silencer = false;
        break;
    }
#ifdef CONFIG_x86
    qDebug(__FUNCTION__);
#endif
}
void fuzzy_pid::send_mid_down(void){
    static u8 tmp = 0;
    QByteArray data;
    data.resize(8);

    switch(tmp){
    case 0:
        command_silencer = true;
        QTimer::singleShot(150,this,SLOT(send_mid_down()));
        tmp++;
        break;
    case 1:
        QTimer::singleShot(200,this,SLOT(send_mid_down()));
        tmp++;
        send_data_order(data.data(),"MIDDN",0,5);

        EOL(data.data(),6);

        pSerial->write(data);
        break;
    case 2:
        tmp = 0;
        command_silencer = false;
        break;
    }
#ifdef CONFIG_x86
    qDebug(__FUNCTION__);
#endif
}

void fuzzy_pid::send_clear_encoder(void){
    static u8 tmp = 0;
    QByteArray data;
    data.resize(9);

    switch(tmp){
    case 0:
        command_silencer = true;
        QTimer::singleShot(150,this,SLOT(send_clear_encoder()));
        tmp++;
        break;
    case 1:
        QTimer::singleShot(200,this,SLOT(send_clear_encoder()));
        tmp++;
        send_data_order(data.data(),"CLRENC",0,6);

        EOL(data.data(),7);

        pSerial->write(data);
        break;
    case 2:
        tmp = 0;
        command_silencer = false;
        break;
    }

#ifdef CONFIG_x86
    qDebug(__FUNCTION__);
#endif

}

void fuzzy_pid::send_channel_polarity(u8 channel, u8 polarity){
    QByteArray data;
    data.resize(10);

    send_data_order(data.data(),"PLRT",0,3);
    data[4] = 0x30 + channel;
    data[5] = 0x30 + polarity ;

    EOL(data.data(),6);

    ch_polarity[channel] = polarity;

    pSerial->write(data);
#ifdef CONFIG_x86
    qDebug(__FUNCTION__);
#endif

}
void fuzzy_pid::send_gains_and_polarities(void){
    static u8 tmp = 0;

    switch(tmp){
    case 0:
        command_silencer = true;
        QTimer::singleShot(150,this,SLOT(send_gains_and_polarities()));
        tmp++;
        break;
    case 1:
        QTimer::singleShot(150,this,SLOT(send_gains_and_polarities()));
        tmp++;
        break;
    case 2:
        QTimer::singleShot(150,this,SLOT(send_gains_and_polarities()));
        send_gain(gain[0]);
        tmp++;
        break;
    case 3:
        QTimer::singleShot(150,this,SLOT(send_gains_and_polarities()));
        send_gain(11 + gain[1]);
        tmp++;
        break;
    case 4:
        QTimer::singleShot(150,this,SLOT(send_gains_and_polarities()));
        send_gain(22 + gain[2]);
        tmp++;
        break;
    case 5:
        QTimer::singleShot(150,this,SLOT(send_gains_and_polarities()));
        send_gain(33 + gain[3]);
        tmp++;
        break;
    case 6:
        QTimer::singleShot(150,this,SLOT(send_gains_and_polarities()));
        send_channel_polarity(0,ch_polarity[0]);
        tmp++;
        break;
    case 7:
        QTimer::singleShot(150,this,SLOT(send_gains_and_polarities()));
        send_channel_polarity(1,ch_polarity[1]);
        tmp++;
        break;
    case 8:
        QTimer::singleShot(150,this,SLOT(send_gains_and_polarities()));
        send_channel_polarity(2,ch_polarity[2]);
        tmp++;
        break;
    case 9:
        QTimer::singleShot(150,this,SLOT(send_gains_and_polarities()));
        send_channel_polarity(3,ch_polarity[3]);
        tmp++;
        break;
    case 10:
        QTimer::singleShot(150,this,SLOT(send_gains_and_polarities()));
        send_calibration(0);
        tmp++;
        break;
    case 11:
        QTimer::singleShot(150,this,SLOT(send_gains_and_polarities()));
        send_calibration(1);
        tmp++;
        break;
    case 12:
        QTimer::singleShot(150,this,SLOT(send_gains_and_polarities()));
        send_calibration(2);
        tmp++;
        break;
    case 13:
        QTimer::singleShot(150,this,SLOT(send_gains_and_polarities()));
        send_calibration(3);
        tmp++;
        break;
    case 14:
        QTimer::singleShot(150,this,SLOT(send_gains_and_polarities()));
        send_all_parameters();
        tmp++;
        break;
    case 15:
        tmp = 0;
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
    signalMapper_usart->setMapping(this->ui.pushButton_set_polarity_load_unipolar,          50);
    signalMapper_usart->setMapping(this->ui.pushButton_set_polarity_displacement_unipolar,  51);
    signalMapper_usart->setMapping(this->ui.pushButton_set_polarity_extensometer_unipolar,  52);
    signalMapper_usart->setMapping(this->ui.pushButton_set_polarity_ch4_unipolar,           53);
    signalMapper_usart->setMapping(this->ui.pushButton_set_polarity_load_bipolar,           54);
    signalMapper_usart->setMapping(this->ui.pushButton_set_polarity_displacement_bipolar,   55);
    signalMapper_usart->setMapping(this->ui.pushButton_set_polarity_extensometer_bipolar,   56);
    signalMapper_usart->setMapping(this->ui.pushButton_set_polarity_ch4_bipolar,            57);
    signalMapper_usart->setMapping(this->ui.pushButton_save_all_cal_data,       60);

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
    connect(this->ui.pushButton_set_polarity_load_unipolar,         SIGNAL(pressed()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_polarity_displacement_unipolar, SIGNAL(pressed()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_polarity_extensometer_unipolar, SIGNAL(pressed()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_polarity_ch4_unipolar,          SIGNAL(pressed()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_polarity_load_bipolar,          SIGNAL(pressed()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_polarity_displacement_bipolar,  SIGNAL(pressed()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_polarity_extensometer_bipolar,  SIGNAL(pressed()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_set_polarity_ch4_bipolar,           SIGNAL(pressed()),signalMapper_usart, SLOT (map()));
    connect(this->ui.pushButton_save_all_cal_data,          SIGNAL(pressed()),signalMapper_usart, SLOT (map()));

    connect(signalMapper_usart, SIGNAL(mapped(int)),this,SLOT(usart_signalmapper_handler(int)));

}
void DC_Motor_PC::usart_signalmapper_handler(int i){
    usart_signalmapper_no = i;
    command_send_protection_wait_timer->start();
    fuzpid->command_silencer = true;
    data_changed = true;
}
