#include "dc_motor_pc.h"

void fuzzy_pid::send_gain(int _gain){
    QByteArray data;
    data.resize(10);

    send_data_order(data.data(),"GAIN",0,3);
    if(_gain <= 10){         //load ch
        data[4] = 0x30 + (u8)(_gain);
        data[5] = 0x30 ;
        gain[0] = (u8)(_gain);
    }
    else if(_gain <= 21){    //ch2
        data[4] = 0x30 + (u8)(_gain - 11);
        data[5] = 0x31 ;
        gain[1] = (u8)(_gain - 11);
    }
    else if(_gain <= 32){    //ch3
        data[4] = 0x30 + (u8)(_gain - 22);
        data[5] = 0x32 ;
        gain[2] = (u8)(_gain - 22);
    }
    else if(_gain <= 43){    //ch4
        data[4] = 0x30 + (u8)(_gain - 33);
        data[5] = 0x33 ;
        gain[3] = (u8)(_gain - 33);
    }

    EOL(data.data(),6);

    pSerial->write(data);
#ifdef CONFIG_x86
    qDebug(__FUNCTION__);
#endif
}
void fuzzy_pid::send_calibration(u8 ch){
    u8 no,i;
    QByteArray data;
    data.resize(75);

    if(ch == 0){
        no = dcMotorPc->load_calibration_channel;
    }
    else{
        no = ch + 2;
    }

    send_data_order(data.data(),"CALSEND",0,6);

    data[7] = ch;
    data[8] = cal[no].point_no;

    for (i = 0; i < 8; i++){
        char_to_f.int_val = cal[no].real_val[i] * RAW_DATA_DIVIDER;

        data[9 + 4*i] = char_to_f.u8_val[0];
        data[10 + 4*i] = char_to_f.u8_val[1];
        data[11 + 4*i] = char_to_f.u8_val[2];
        data[12 + 4*i] = char_to_f.u8_val[3];
    }
    //last index is 40
    for (i = 0; i < 8; i++){
        char_to_f.float_val = cal[no].assigned_val[i];

        data[41 + 4*i] = char_to_f.u8_val[0];
        data[42 + 4*i] = char_to_f.u8_val[1];
        data[43 + 4*i] = char_to_f.u8_val[2];
        data[44 + 4*i] = char_to_f.u8_val[3];
    }
    //last index is 72

    EOL(data.data(),73);

    pSerial->write(data);
#ifdef CONFIG_x86
    qDebug(__FUNCTION__);
#endif
}

void fuzzy_pid::tare_channel(u8 channel){
    u8 chn = 0;

    switch(channel){
    case 0: //load
        switch(dcMotorPc->load_calibration_channel){
        case LOAD_1:
            chn = 0;
            break;
        case LOAD_2:
            chn = 1;
            break;
        case LOAD_3:
            chn = 2;
            break;
        }
        break;
    case 1: //displacement
        chn = 3;
        break;
    case 2: //ch3
        chn = 4;
        break;
    case 3: //ch4
        chn = 5;
        break;
    case 4: //encoder
        chn = 6;
        break;
    }

    cal[chn].tare_val = cal[chn].absolute_calibrated;
#ifdef CONFIG_x86
    qDebug(__FUNCTION__);
#endif
}
