#ifndef DC_MOTOR_PC_H
#define DC_MOTOR_PC_H

#include <QtGui/QWidget>

#include <qextserialport.h>
#include <QFileDialog>
#include <QTimer>
#include <QPixmap>
#include <QDate>
#include <QTime>
#include <QTextStream>
#include <QList>
#include <QRadioButton>
#include <QProcess>
#include <QTabWidget>
#include <QTabBar>
#include <QProgressDialog>

#include <qsignalmapper.h>
#include <qwt_plot.h>
#include <qwt_symbol.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_zoomer.h>
#include <qwt_legend.h>

#include "ui_dc_motor_pc.h"
#include "keyboard.h"
#include "password.h"
#include "remoteconsole.h"
#include "settingsserver.h"
#include <math.h>
#include <QCloseEvent>
#include "fuzzy_pid.h"
#include <QMessageBox>
#include <QSettings>
#include <QTranslator>
#include <QFontDatabase>
#include <QSpacerItem>
#include <QGridLayout>

const QString calibration_channel_names[6] = {"LOAD_1",
                                            "LOAD_2",
                                            "LOAD_3",
                                            "DISPLACEMENT",
                                            "CHANNEL_3",
                                            "CHANNEL_4"
                                             };


const float astm_array[] = {0.64,1.27,1.91,2.54,3.18,3.81,4.45,5.08,7.62,10.16,12.7};
const float en_array[] = {0.50,1.00,1.50,2.00,2.50,3.00,3.50,4.00,4.50,5.00,5.50,6.00,6.50,7.00,7.50};
const float bs_array[] = {0.25,0.50,0.75,1.00,1.25,1.50,1.75,2.00,2.25,2.50,2.75,3.00,3.25,3.50,3.75,4.00,4.25,4.50,4.75,5.00,5.25,5.50,5.75,6.00,6.25,6.50,6.75,7.00,7.25,7.50};

const float _4_inch_thickness_array[] = {25.4,27,28.6,30.2,31.8,33.3,34.9,36.5,38.1,39.7,41.3,42.9,44.4,46,47.6,49.2,50.8,52.4,54,55.6,57.2,58.7,60.3,61.9,63.5,65.1,66.7,68.3,69.8,71.4,73,74.6,76.2};
const float _4_inch_correction_factor[] = {5.56,5,4.55,4.17,3.85,3.57,3.33,3.03,2.78,2.5,2.27,2.08,1.92,1.79,1.67,1.56,1.47,1.39,1.32,1.25,1.19,1.14,1.09,1.04,1,0.96,0.93,0.89,0.86,0.83,0.81,0.78,0.76};
const float _6_inch_thickness_array[] = {88.9,90.5,92.1,93.7,95.2,96.8,98.4,100,101.6};
const float _6_inch_correction_factor[] = {1.12,1.09,1.06,1.03,1,0.97,0.95,0.92,0.9};

class DC_Motor_PC : public QWidget
{
    Q_OBJECT

public:
#ifdef MATLAB_RECORDINGS
    QFile matlab_log;
#endif
    DC_Motor_PC(QWidget *parent = 0);
    ~DC_Motor_PC();
    void closeEvent(QCloseEvent *ev);
    Ui::DC_Motor_PCClass ui;
    fuzzy_pid *fuzpid;
    password *pass;

    QString readSettingEthernet(const QString &setting);
    QString getKeyText(QString txt);
    int setSettingEthernet(const QString &setting, const QString &value);
    void send_data_order(char *base_array,const char *array, u8 first_index, u8 last_index);
    void checkSpin(QWidget *newwidget);
    void checkDoubleSpin(QWidget *newwidget);
    void checkLineEdit(QWidget *newwidget);
    void prepare_test_results(void);
    void write_calibration(void);
    void read_calibration(void);
    void return_home_operation(void);
    void go_load_operation(void);

    struct _jog{
        struct {
            double speed_up;
            double speed_down;
            u8 ramp_up;
            u8 ramp_down;
        }
        low,mid,high;

        double speed;
        bool up;
        bool down;
        bool error;
    };
    _jog JOG;

    struct chan {
        QwtPlotCurve *cSin;
        QwtPlotCurve *cCos;
        QwtPlotCurve *cTan;
        QwtPlotCurve *cCot;
        QwtPlot *plot;
        QwtPlotGrid *grid;
        QwtPlotZoomer *zoomer;
        double max_value_x;
        double min_value_x;
        double max_value_y;
        double min_value_y;
        double xval[4004];
        double yval[4004];
        u32 counter;
    };
    chan plot_ch[2];

    struct test_res{
        QString test_type;
        QString test_speed;
        QString starting_date_time;
        QString completion_date_time;
        QString dimensions;
        QString max_displacement;
        QString area;
        QString peak_stress;
        QString peak_load;
        QString standard;
        QString thickness_correction;
        QString CBR_2_54;
        QString CBR_5_08;
        QString stability_peak_method;
        QString flow_peak_method;
        QString stability_tangent_offset;
        QString flow_tangent_offset;
        QString displacement_at_peak_load;
        QString strength;
    };
    test_res results;

    struct _pid {
        double Kp;
        double Ki;
        u32 Kd;
    };
    _pid pid[10];

    struct _cylinder{   //same as paving stone
        double l;
        double d;
    };

    struct _ch {
        QString label;
        u8 dp;
        u8 unit;
        bool status;
    };
    _ch ch3,ch4;

    struct _param{
        _cylinder cylinder;
        double max_load;
        double max_displacement;
        double zero_suppression;
        double load_threshold;
        double displacement_rate;
        double start_speed;
        double return_home_speed;
        double go_to_load_speed;
        double precalculated_area;
        u8 break_percentage;
        u8 precalculated_area_status;
    };
    _param parameters[10];

    struct _unit_system{
        QString load_label;
        QString length_label;
        QString stress_label;
        double load_multiplier;
        double length_multiplier;
        double stress_multiplier;
        double area_multiplier;
        u8 index;
    };
    struct _unit_system unit_system;

    struct _briquette{
        double d;
        double t;
    };
    struct _briquette briquette;

    struct _cbr{
        QFile log_file;
        double p_2_54;
        double p_5_08;
        u8 step;
        u8 standard;
        bool points_detected;
    };
    _cbr cbr;

    struct _marshall{
        QwtPlotCurve *slope_line;
        QwtPlotCurve *shifting_slope_line;
        QwtPlotMarker *stability_marker;
        QwtSymbol *stability_symbol;
        QwtPlotMarker *x_axis_line;
        QwtPlotMarker *y_axis_line;
        double correction_multiplier;
        u32 latest_x;
        u8 thickness_correction;
        u8 standard;
        u8 diameter_number;

    };
    _marshall marshall;

    struct _dp{
        u8 load_1;
        u8 load_2;
        u8 load_3;
        u8 stress;
        u8 displacement;
    };
    _dp dp;

    RemoteConsole *rcon;
    SettingsServer *sserver;
    RemoteDataStream *dataStream;
    QTimer *load_graphic_timer;
    QTimer *pace_graphic_timer;
    QTimer *_100_msec_timer;
    QTimer *command_send_protection_wait_timer;
    QTime   forbid_pace_recalculate_timer;
    QTime   block_double_click;
    QSignalMapper *signalMapper_cal;
    QSignalMapper *signalMapper_usart;

    QSignalMapper *signalMapper_main_screen;
    QSignalMapper *signalMapper_settings_screen;
    QSignalMapper *signalMapper_admin_screen;
    QSignalMapper *signalMapper_parameters_screen;

    QTranslator *translator;
    QList<QLabel*> label_gain;
    QList<QLabel*> label_point;
    QList<QSpinBox*> spinBox_cal_raw_value;
    QList<QDoubleSpinBox*> doubleSpinBox_cal_point_value;
    QList<QDoubleSpinBox*> cbr_point_value;
    QList<QPushButton*> pushButton_read_cal_point;
    QPushButton *zoom_out_button;
    QFile excel_file;
    QString currentStyleSheet;
    QString test_type_name;
    QString start_date_filename_arg;
    double calculated_area;
    double pace_rate;
    double pace_mPa;
    u32 current_test_no;
    s32 current_channel_data;
    u8 guideline_spacing;
    u8 test_type;
    u8 load_calibration_channel;
    u8 current_cal_channel;
    u8 usart_signalmapper_no;
    u8 return_home_tmp;
    u8 go_load_tmp;
    u8 current_tab_index;
    u8 zoom_radiobutton_no;
    u8 v_axis_radiobutton_no;
    u8 h_axis_radiobutton_no;
    u8 control_mode;
    u8 language_index;
    u32 step_motor_speed;
    bool blink;

    bool do_not_change_focus;
    bool return_home_bit;
    bool go_load_bit;
    bool step_response_status;
    bool PLOT_first_in;
    bool data_changed;
    bool correctly_opened;
    bool admin_authorization;

private:
    int prevent_double_click(void);
    static bool doublespinbox_name_compare(QDoubleSpinBox *p1, QDoubleSpinBox *p2);
    static bool spinbox_name_compare(QSpinBox *p1, QSpinBox *p2);
    static bool label_name_compare(QLabel *p1, QLabel *p2);
    static bool pushbutton_name_compare(QPushButton *p1, QPushButton *p2);
    void init_CHANNEL_SELECTIONs(void);
    void init_RADIOBUTTONs(void);
    void init_PUSHBUTTONs(void);
    void init_CALIBRATIONs(void);
    void init_SPECIMENs(void);
    void init_PIDs(void);
    void init_PIXMAPs(void);
    void init_TIMERs(void);
    void init_GUI(void);
    void init_DIAMETERs(void);
    void main_screen_arrangements(void);

public slots:
    void blink_op(void);
    u32 speed_correction(double val);
    void step_response_handle(void);
    void motor_stop(void);
    void return_home(void);
    void go_load(void);
    void stress_calculation(void);
    void write_test_results_to_file(int no);
    void error_messagebox (void);
    void wait_messagebox (void);
    void unit_system_recalculate(void);

private slots:
    void update_device(void);

    void specimen_protection_handler(void);
    void load_cal_channel_handler(void);

    void save_as_LOW(void);
    void save_as_MID(void);
    void save_as_HIGH(void);
    void set_LOW(void);
    void set_MID(void);
    void set_HIGH(void);

    void check_the_storage(void);
    void fill_after_language_change(void);
    void language_load(void);
    void unit_system_calibration_handler(void);
    void unit_system_handler(void);
    void result_no_left(void);
    void result_no_right(void);
    void show_jog_widget(void);
    void save_and_reboot(void);
    void close_jog_widget(void);
    void eth0_address_set(void);
    void save_ip(void);
    void handle_JOG_up_button(void);
    void release_JOG_up_button(void);
    void handle_JOG_down_button(void);
    void release_JOG_down_button(void);
    void JOG_operation(void);
    void create_backup_file(void);
    void load_backup_file(void);
    void reset_plot_load(void);
    void reset_plot_pace(void);
    void refresh_button_handler(void);
    void load_factory_settings(void);
    void read_test_results_from_file(void);
    void ethernet_periodic_sending(void);
    void save_type_parameters(void);
    void assign_type_parameters(void);
    void ch3_name_handler(void);
    void ch4_name_handler(void);
    void writeAppSettings(void);
    void readAppSettings(void);
    void load_temporary_settings(void);
    void channel_activations_handler();
    void control_mode_handler(void);
    void test_type_handler(void);
    void ch3_unit_handler(void);
    void ch4_unit_handler(void);
    void graphic_type_handler(void);
    void graphic_v_axis_handler(void);
    void graphic_h_axis_handler(void);
    void dp_load_change_handler(void);
    void dp_stress_change_handler(void);
    void dp_ch3_change_handler(void);
    void dp_ch4_change_handler(void);
    void dp_displacement_change_handler(void);
    void admin_authorization_handler(QString val);
    void displacement_rate_handler(void);
    void pace_rate_handler_kn(void);
    void pace_rate_handler_mpa(void);
    void relay_on(void);
    void relay_off(void);
    void save_pid_values(void);
    void pid_combobox_handle(int val);
    void reset_zoom(void);
    void draw_load_graphic(void);
    void draw_pace_graphic(void);
    void command_sending_protection(void);
    void get_storage_status(void);
    void keyboard_delayed_operation(void);
    void clean_storage(void);
    void clean_storage_confirmation(void);
    void handleTcpMessage(const QString &message);
    void cal_channel_change(const QString & text);
    void serial_port_setup(void);
    void cal_assignment(int);
    void hold_calibration_values(void);
    void cal_points_signalmapper(void);
    void usart_signalmapper(void);
    void rearrange_object_names(void);
    void load_plotter(QwtPlot *plot, QwtPlotCurve *cSin, QwtPlotCurve *cCos, QwtPlotCurve *cTan, QwtPlotCurve *cCot,
                        double value_x, double value_y,
                        double *max_value_x, double *min_value_x,
                        double *max_value_y, double *min_value_y,
                        u32 *counter,
                        double *xval, double *yval);
    void pace_plotter(QwtPlot *plot, QwtPlotCurve *cSin,
                        double value_x, double value_y,
                        double *max_value_x, double *min_value_x,
                        double *max_value_y, double *min_value_y,
                        u32 *counter,
                        double *xval, double *yval);
    void plot_prepare_load(QwtPlot *plot, QwtPlotCurve *cSin, QwtPlotCurve *cCos, QwtPlotCurve *cTan,QwtPlotCurve *cCot, QString str_y, QString str_x);
    void plot_prepare_pace(QwtPlot *plot, QwtPlotCurve *cSin, QString str_y, QString str_x);
    void set_gradient(void);
    void set_plotter(void);
    void EOL(char *base_array, u8 i);
    void motor_go_test_direction(void);
    void pause_test(void);
    void calculate_area_cylindrical(void);
    void test_finishing(void);
    void update_gui(void);
    void set_datetime(void);
    void focusChanged(QWidget*, QWidget*newwidget);
    void tab_change_handler(int i);
    void _100_msec_handler();
    void excel_recording(void);

    void marshall_standard_type_handler(void);
    void marshall_diameter_selection_handle(int val);
    void marshall_calculations(void);
    void marshall_zero_suppression_exceeded(void);
    void marshall_thickness_correction_handler(void);
    void marshall_correction_factor_calculator(void);

    void cbr_zero_suppression_exceeded(void);
    void cbr_process(double l_val, double d_val);
    void cbr_points_detection(double l_val, double d_val);
    void cbr_load_standard_values(void);
    void cbr_standard_type_handler(void);
    void cbr_show_log(void);
    void cbr_export_log_to_file(void);

    void copy_data_to_usb(void);
    void copy_logo_files(void);
    void copy_calibration_from_usb(void);
    void copy_calibration_to_usb(void);
    void copy_factory_settings_from_usb(void);
    void copy_factory_settings_to_usb(void);
    void copy_settings_from_usb(void);
    void copy_settings_to_usb(void);

    void usart_signalmapper_handler(int i);
    void main_screen_signalmapper_handler(int i);
    void settings_screen_signalmapper_handler(int i);
    void admin_screen_signalmapper_handler(int i);
    void parameters_screen_signalmapper_handler(int i);

};

#endif // DC_MOTOR_PC_H
