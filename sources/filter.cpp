#include "dc_motor_pc.h"

double fuzzy_pid::classic_MA(double* raw_signal,u8 filter_coefficient){
    static double running_average[16];
    double processed_value;
    u8 j;

    running_average[filter_coefficient-1] = *raw_signal;
    processed_value = *raw_signal;
    for (j = 0; j < (filter_coefficient-1); j++){
        processed_value += running_average[j];
        running_average[j] = running_average[j+1];
    }
    processed_value = (processed_value)/(filter_coefficient);

    return processed_value;
}
double fuzzy_pid::WMA(double *raw_signal, u8 filter_coefficient){
    static double weighted_average = 0;

    weighted_average -= weighted_average/filter_coefficient;
    weighted_average += *raw_signal/filter_coefficient;

    return weighted_average;
}
double fuzzy_pid::EMA(double *raw_signal, u8 filter_coefficient){
    double EMA = 0;
    static double past_EMA = 0;
    double alpha = (double)2/(filter_coefficient+1);

    EMA = (*raw_signal)*alpha + past_EMA*(1-alpha);
    past_EMA = EMA;

    return EMA;
}
double fuzzy_pid::EMA_displacement(double *raw_signal, u8 filter_coefficient){
    double EMA = 0;
    static double past_EMA = 0;
    double alpha = (double)2/(filter_coefficient+1);

    EMA = (*raw_signal)*alpha + past_EMA*(1-alpha);
    past_EMA = EMA;

    return EMA;
}

double fuzzy_pid::IIR_Filter(double* raw_signal,u8 filter_coefficient){
    static double running_average[16];
    static double last_output[16];
    double processed_value;
    double iir_output;
    u8 j;
    double max_val;
    double min_val;

    running_average[filter_coefficient-1] = *raw_signal;
    processed_value = *raw_signal;
    max_val = *raw_signal;
    min_val = *raw_signal;
    for (j = 0; j < (filter_coefficient-1); j++){
        processed_value += running_average[j];
        if(max_val < running_average[j])    max_val = running_average[j];
        if(min_val > running_average[j])    min_val = running_average[j];
        running_average[j] = running_average[j+1];
    }
    processed_value = (processed_value - (max_val+min_val))/(filter_coefficient-2);

    last_output[filter_coefficient-1] = processed_value;
    iir_output = processed_value;
    for (j = 0; j < filter_coefficient-1; j++){
        iir_output += last_output[j];
        last_output[j] = last_output[j+1];
    }
    iir_output = (iir_output)/(filter_coefficient);

    return iir_output;
}
void fuzzy_pid::bessel_filter_coeffs(void){
    double samplerate = 400;
    double cutoff = 20;

    double QcRaw  = (2.0 * M_PI * cutoff) / (1.0*samplerate);
    double QcWarp = 1.0* qTan(QcRaw); // Warp cutoff frequency

    double gain = 1.0 / (1.0 + M_SQRT2/QcWarp + 2.0/(QcWarp*QcWarp));

    by[2] = (1.0 - M_SQRT2/QcWarp + 2.0/(QcWarp*QcWarp)) * gain;
    by[1] = (2.0 - 4.0/(QcWarp*QcWarp)) * gain;
    by[0] = 1.0;
    ax[0] = 1.0 * gain;
    ax[1] = 2.0 * gain;
    ax[2] = 1.0 * gain;
}
double fuzzy_pid::bessel_filter(double input){
    static double xv[3] = {0};
    static double yv[3] = {0};

    xv[2] = xv[1];
    xv[1] = xv[0];
    xv[0] = input;
    yv[2] = yv[1];
    yv[1] = yv[0];
    yv[0] =   (ax[0] * xv[0] + ax[1] * xv[1] + ax[2] * xv[2]
                - by[1] * yv[0]
                - by[2] * yv[1]);

    return yv[0];
}
