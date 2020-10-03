#ifndef FUZZY_PID_H
#define FUZZY_PID_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>
#include <QDebug>
#include <QThread>
#include <QtConcurrentRun>
#include <qextserialport.h>
#include "macros.h"

namespace Ui {
    class fuzzy_pid;
}
class DC_Motor_PC;

class fuzzy_pid : public QWidget
{
    Q_OBJECT

public:
    explicit fuzzy_pid(DC_Motor_PC *master, QWidget *parent = 0);
    ~fuzzy_pid();
    QextSerialPort *pSerial;
    void send_data_order(char *base_array,const char *array, u8 first_index, u8 last_index);

    double ax[3], by[3];
    double fuzzy_raw_servo_speed;
    double usart_pace_rate;
    double usart_displacement_rate;
    double tared_val;
    float load_value;
    float displacement_value;
    float ch3_value;
    float ch4_value;
    float encoder;
    float stress;
    float max_load_value;
    float peak_load;
    float peak_stress;
    float current_pace_rate;
    float current_displacement_rate;
    u8 ch_polarity[4];
    u8 gain[4];
    u8 breaking_case;
    u8 test_status;
    u8 relay_auto_man;
    u8 relay_start_stop;
    bool send_fuzzy_raw_servo_speed;
    bool command_silencer;
    bool test_finished;
    bool test_label_status;
    bool run_pid;
    bool run_cbr;
    bool hard_stop;
    bool step_response_first_in;
    bool communication_established;
    bool PID_first_in;

    struct _cal{
        double slope[7];
        double assigned_val[8];
        float tare_val;
        float absolute_calibrated;
        int real_val[8];
        u8 point_no;
        char type;
    };
    _cal cal[7];

    struct _from_gui{
        u8 current_tab_index;
        u8 test_type;
    };
    _from_gui from_gui;

    struct _error{
        bool max_load;
        bool max_displacement;
        bool go_home_load;
    };
    _error error;

    struct _to_gui{
        u8 ch_polarity[5];
        u8 gain[5];
        s32 signed_raw[5];
        bool test_finished;
    };
    struct _to_gui to_gui;

signals:
    void cohen_coon_kp(QString);
    void cohen_coon_ki(QString);
    void cohen_coon_kd(QString);
    void let_step_response(void);
    void zero_suppression_exceeded_cbr(void);
    void zero_suppression_exceeded_marshall(void);
    void cbr_process(double l_val, double d_val);

public slots:
    void read_parameters(void);
    double IIR_Filter(double* raw_signal,u8 filter_coefficient);
    double classic_MA(double *raw_signal,u8 filter_coefficient);
    double WMA(double *raw_signal,u8 filter_coefficient);
    double EMA(double *raw_signal,u8 filter_coefficient);
    double EMA_displacement(double *raw_signal,u8 filter_coefficient);
    double discrete_PID_dac(void);
    void always_send(void);
    void fuzpid_thread_handler(void);
    void bessel_filter_coeffs(void);
    double bessel_filter(double input);
    void send_gain(int _gain);
    void send_gains(void);
    void tare_channel(u8 channel);
    void channel_polarity(u8 channel, u8 polar);
    void slope_calculation(u8 no);
    double evaluate_calibrated_values_ascending(u8 no);

private:
    Ui::fuzzy_pid *ui;
    DC_Motor_PC *dcMotorPc;

    QTimer* thread_timer;
    QTime   pid_delta_t_dac;

private slots:
    u8 read_data_order(QByteArray base_array,const char *array, u8 first_index, u8 last_index);
    u32 crc_chk(u8* data, u8 length);

    void EOL(char *base_array, u8 i);
    void step_response(void);

};

#endif // FUZZY_PID_H
