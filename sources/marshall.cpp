#include "dc_motor_pc.h"

void DC_Motor_PC::marshall_diameter_selection_handle(int val){
    marshall.diameter_number = val;
    if(marshall.diameter_number == 0){
        switch(unit_system.index){
        case KN_MM:
            briquette.d = 101.6;
            ui.doubleSpinBox_briquette_t->setMinimum(25.4);
            ui.doubleSpinBox_briquette_t->setMaximum(76.2);
            break;
        case LBF_IN:
            briquette.d = 4;
            ui.doubleSpinBox_briquette_t->setMinimum(1);
            ui.doubleSpinBox_briquette_t->setMaximum(3);
            break;
        case KGF_CM:
            briquette.d = 10.16;
            ui.doubleSpinBox_briquette_t->setMinimum(2.54);
            ui.doubleSpinBox_briquette_t->setMaximum(7.62);
            break;
        }
    }
    else{
        switch(unit_system.index){
        case KN_MM:
            briquette.d = 152.4;
            ui.doubleSpinBox_briquette_t->setMinimum(88.9);
            ui.doubleSpinBox_briquette_t->setMaximum(101.6);
            break;
        case LBF_IN:
            briquette.d = 6;
            ui.doubleSpinBox_briquette_t->setMinimum(3.5);
            ui.doubleSpinBox_briquette_t->setMaximum(4);
            break;
        case KGF_CM:
            briquette.d = 15.24;
            ui.doubleSpinBox_briquette_t->setMinimum(8.89);
            ui.doubleSpinBox_briquette_t->setMaximum(10.16);
            break;
        }
    }
    data_changed = true;
}
void DC_Motor_PC::marshall_zero_suppression_exceeded(void){
    static u8 tmp = 0;

    switch(tmp){
    case 0:
        ui.pushButton_tare_displacement->setEnabled(1);
        ui.pushButton_tare_displacement->click();
        ui.pushButton_tare_displacement->setDisabled(1);
        QTimer::singleShot(1500,this,SLOT(marshall_zero_suppression_exceeded()));
        tmp = 1;
        break;
    case 1:
        load_graphic_timer->start();
        draw_load_graphic();
        tmp = 0;
        break;
    }

}
void DC_Motor_PC::marshall_thickness_correction_handler(){
    if(ui.radioButton_thickness_correction_on->isChecked()){
        marshall.thickness_correction = 1;
    }
    else if(ui.radioButton_thickness_correction_off->isChecked()){
        marshall.thickness_correction = 0;
    }
    data_changed = true;
}
void DC_Motor_PC::marshall_correction_factor_calculator(void){

    if(unit_system.index != KN_MM){
        marshall.correction_multiplier = 1;
        ui.label_correction_multiplier->setText(trUtf8("Correction factor is ") + QString::number(marshall.correction_multiplier,'f',3));
        return;
    }

    if(test_type == MARSHALL){
        if(marshall.standard == EN_STANDARD_MARSHALL){
            marshall.correction_multiplier = 5.24 * (exp(-0.0258 * ui.doubleSpinBox_briquette_t->value()));
            ui.label_correction_multiplier->setText(trUtf8("Correction factor is ") + QString::number(marshall.correction_multiplier,'f',3));
        }
        else{
            if(marshall.diameter_number == 0){   //4 inch
                if(ui.doubleSpinBox_briquette_t->value() == 76.2){
                    marshall.correction_multiplier = 0.76;
                    ui.label_correction_multiplier->setText(trUtf8("Correction factor is ") + QString::number(marshall.correction_multiplier,'f',3));
                }
                else{
                    for(u8 i = 0; i < 33; i++){
                        if(ui.doubleSpinBox_briquette_t->value() <= (double)_4_inch_thickness_array[i]){
                            marshall.correction_multiplier = ((ui.doubleSpinBox_briquette_t->value() - _4_inch_thickness_array[i]) * (_4_inch_correction_factor[i] - _4_inch_correction_factor[i-1]))/(_4_inch_thickness_array[i] - _4_inch_thickness_array[i-1]) + _4_inch_correction_factor[i];
                            ui.label_correction_multiplier->setText(trUtf8("Correction factor is ") + QString::number(marshall.correction_multiplier,'f',3));
                            break;
                        }
                    }
                }
            }
            else{   //6 inch
                if(ui.doubleSpinBox_briquette_t->value() == 101.6){
                    marshall.correction_multiplier = 0.9;
                    ui.label_correction_multiplier->setText(trUtf8("Correction factor is ") + QString::number(marshall.correction_multiplier,'f',3));
                }
                else{
                    for(u8 i = 0; i < 9; i++){
                        if(ui.doubleSpinBox_briquette_t->value() <= (double)_6_inch_thickness_array[i]){
                            marshall.correction_multiplier = ((ui.doubleSpinBox_briquette_t->value() - _6_inch_thickness_array[i]) * (_6_inch_correction_factor[i] - _6_inch_correction_factor[i-1]))/(_6_inch_thickness_array[i] - _6_inch_thickness_array[i-1]) + _6_inch_correction_factor[i];
                            ui.label_correction_multiplier->setText(trUtf8("Correction factor is ") + QString::number(marshall.correction_multiplier,'f',3));
                            break;
                        }
                    }
                }
            }
        }
    }

}
void DC_Motor_PC::marshall_calculations(void){
    static u8 tmp = 0;
    static bool inconvenient_test = false;
    static double max_load = 0;
    static double disp_at_max_load = 0;
    static double slope_line_x[4];
    static double slope_line_y[4];
    static double shifting_slope_line_x[4];
    static double shifting_slope_line_y[4];
    static double max_slope = 0;
    static double max_slope_x[2];
    static double max_slope_y[2];

    struct _junction{
        double last_x;
        double last_y;
        double next_to_last_x;
        double next_to_last_y;
    };
    _junction junction_slope,junction_curve;

    static double x_axis_cut_point = 0;
    static double x_at_line_end = 0;
    double x_max = 0;
    double y_max = 0;
    double slope = 0;

    double y_slope;
    u32 overlap_first_index = 0;
    u32 overlap_last_index = 0;
    u32 latest_x_index = 0;

    double m_slope = 0;
    double m_curve = 0;
    double a = 0;
    double c = 0;
    double b = 0;
    double d = 0;
    static double intersection_x = 0;
    static double intersection_y = 0;

    switch(tmp){
    case 0:
        inconvenient_test = false;
        if(marshall.latest_x == 0){
            tmp = 5;
            inconvenient_test = true;
            QTimer::singleShot(1000,this,SLOT(marshall_calculations()));
            break;
        }
        max_load = 0;
        max_slope = 0;
        for(u32 i = 0; i < marshall.latest_x ; i++){
            if(plot_ch[0].yval[i] > max_load){
                max_load = plot_ch[0].yval[i];
                disp_at_max_load = plot_ch[0].xval[i];
                //disp_at_max_load = i;
            }

            if(i > 2){
                slope = (plot_ch[0].yval[i] - plot_ch[0].yval[i-1]) / (plot_ch[0].xval[i] - plot_ch[0].xval[i-1]);
                //qDebug() << "slope" << slope;
                if(slope > max_slope){
                    max_slope = slope;
                    max_slope_x[0] = plot_ch[0].xval[i-1];
                    max_slope_x[1] = plot_ch[0].xval[i];
                    max_slope_y[0] = plot_ch[0].yval[i-1];
                    max_slope_y[1] = plot_ch[0].yval[i];
                }
            }
        }

//        qDebug() << "max slope is" << max_slope;
//        qDebug() << "max load is" << max_load;
//        qDebug() << "displacement at max load is" << disp_at_max_load;

        x_axis_cut_point = ((0 - max_slope_y[0]) * (max_slope_x[0] - max_slope_x[1])) / (max_slope_y[0] - max_slope_y[1]) + max_slope_x[0];

        //line_and is 2 kN bigger than max_load
        x_at_line_end = ((max_load+2 - max_slope_y[0]) * (max_slope_x[0] - max_slope_x[1])) / (max_slope_y[0] - max_slope_y[1]) + max_slope_x[0];

        //reveal the slope line
        slope_line_x[0] = x_axis_cut_point;
        slope_line_y[0] = 0;
        slope_line_x[1] = max_slope_x[0];
        slope_line_y[1] = max_slope_y[0];
        slope_line_x[2] = max_slope_x[1];
        slope_line_y[2] = max_slope_y[1];
        slope_line_x[3] = x_at_line_end;
        slope_line_y[3] = max_load+2;

        tmp++;
        QTimer::singleShot(1000,this,SLOT(marshall_calculations()));
        break;
    case 1:
        marshall.slope_line->setRenderHint(QwtPlotItem::RenderAntialiased);
        marshall.slope_line->setPen(QPen(Qt::red,3,Qt::SolidLine));
        marshall.slope_line->attach(plot_ch[0].plot);
        marshall.slope_line->setRawSamples(slope_line_x,slope_line_y,4);
        //refind the axis
        y_max = plot_ch[0].plot->axisInterval(0).maxValue();
        x_max = plot_ch[0].xval[marshall.latest_x-1];
        if(x_axis_cut_point < 0){
            plot_ch[0].plot->setAxisScale(plot_ch[0].plot->xBottom, x_axis_cut_point-1, x_max);
        }
        else{
            plot_ch[0].plot->setAxisScale(plot_ch[0].plot->xBottom, -1, x_max);
        }
        plot_ch[0].plot->setAxisScale(plot_ch[0].plot->yLeft, -1, y_max);
        plot_ch[0].plot->replot();
        tmp++;
        QTimer::singleShot(1000,this,SLOT(marshall_calculations()));
        break;
    case 2:
        //  ...a horizontal line at y = 0...
        marshall.x_axis_line->setLabel(QString::fromLatin1("x = 0"));
        marshall.x_axis_line->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
        marshall.x_axis_line->setLineStyle(QwtPlotMarker::HLine);
        marshall.x_axis_line->setYValue(0.0);
        marshall.x_axis_line->attach(plot_ch[0].plot);

        //  ...a vertical line at x = 0...
        marshall.y_axis_line->setLabel(QString::fromLatin1("y = 0"));
        marshall.y_axis_line->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
        marshall.y_axis_line->setLineStyle(QwtPlotMarker::VLine);
        marshall.y_axis_line->setXValue(0.0);
        marshall.y_axis_line->attach(plot_ch[0].plot);

        plot_ch[0].plot->replot();

        tmp++;
        QTimer::singleShot(1000,this,SLOT(marshall_calculations()));
        break;
    case 3:
        //locate the shifted slope line
        for(u8 i = 0; i < 4; i++){
            shifting_slope_line_x[i] = slope_line_x[i] + 1.5;
            shifting_slope_line_y[i] = slope_line_y[i];
        }
        marshall.shifting_slope_line->setRenderHint(QwtPlotItem::RenderAntialiased);
        marshall.shifting_slope_line->setPen(QPen(Qt::green,3,Qt::SolidLine));
        marshall.shifting_slope_line->attach(plot_ch[0].plot);
        marshall.shifting_slope_line->setRawSamples(shifting_slope_line_x,shifting_slope_line_y,4);
        plot_ch[0].plot->replot();
        tmp++;
        QTimer::singleShot(1000,this,SLOT(marshall_calculations()));
        break;
    case 4:
        for(u32 i = 0; i < marshall.latest_x ; i++){
            if(shifting_slope_line_x[0] > plot_ch[0].xval[i]){
                overlap_first_index = i;
            }
        }

        for(u32 i = marshall.latest_x; i > 0 ; i--){
            if(shifting_slope_line_x[3] < plot_ch[0].xval[i]){
                overlap_last_index = i;
                //qDebug() << "overlap_last_index" << overlap_last_index;
            }
        }
        //qDebug() << "graph_lastest_x" << graph_lastest_x << "channel[0].xval[graph_lastest_x-1]" << channel[0].xval[graph_lastest_x-1];

        //qDebug() << "overlap_first_index" << overlap_first_index << "overlap_last_index" << overlap_last_index;

        if(overlap_last_index <= overlap_first_index){
            inconvenient_test = true;
            tmp++;
            QTimer::singleShot(1000,this,SLOT(marshall_calculations()));
            break;
        }

        if(overlap_last_index == 0) overlap_last_index = marshall.latest_x-1;

        junction_slope.last_x = shifting_slope_line_x[3];
        junction_slope.last_y = shifting_slope_line_y[3];
        junction_slope.next_to_last_x = shifting_slope_line_x[2];
        junction_slope.next_to_last_y = shifting_slope_line_y[2];

        for(u32 i = overlap_last_index; i > overlap_first_index ; i--){
            y_slope = (plot_ch[0].xval[i] - junction_slope.next_to_last_x)*(junction_slope.next_to_last_y - junction_slope.last_y)/(junction_slope.next_to_last_x - junction_slope.last_x)
                    + junction_slope.next_to_last_y;

            if(plot_ch[0].yval[i] < y_slope){
                latest_x_index = i;
            }
        }

        junction_curve.last_x = plot_ch[0].xval[latest_x_index];
        junction_curve.last_y = plot_ch[0].yval[latest_x_index];
        junction_curve.next_to_last_x = plot_ch[0].xval[latest_x_index-1];
        junction_curve.next_to_last_y = plot_ch[0].yval[latest_x_index-1];

        //line and curve equations
        //y_slope = (x_slope - junction_slope_next_to_last_x)*(junction_slope_next_to_last_x - junction_slope_last_x)/(junction_slope_next_to_last_y - junction_slope_last_y) + junction_slope_next_to_last_y;
        //y_curve = (x_curve - junction_curve_next_to_last_x)*(junction_curve_next_to_last_x - junction_curve_last_x)/(junction_curve_next_to_last_y - junction_curve_last_y) + junction_curve_next_to_last_y;
        //y_slope = a*x + c
        //y_curve = b*x + d
        //intersection x = (d-c)/(a-b)
        //intersection y = a(intersection x)+c
        m_slope = (junction_slope.next_to_last_y - junction_slope.last_y)/(junction_slope.next_to_last_x - junction_slope.last_x);
        m_curve = (junction_curve.next_to_last_y - junction_curve.last_y)/(junction_curve.next_to_last_x - junction_curve.last_x);
        a = m_slope;
        c = junction_slope.next_to_last_y - m_slope*junction_slope.next_to_last_x;
        b = m_curve;
        d = junction_curve.next_to_last_y - m_curve*junction_curve.next_to_last_x;

        intersection_x = (d-c)/(a-b);
        intersection_y = a*intersection_x+c;

        marshall.stability_marker->setSymbol(marshall.stability_symbol);
        marshall.stability_marker->setLabel(QString::fromLatin1("Stability"));
        marshall.stability_marker->setLabelAlignment(Qt::AlignRight);
        marshall.stability_marker->setValue(intersection_x,intersection_y);
        marshall.stability_marker->attach(plot_ch[0].plot);
        plot_ch[0].plot->replot();

        tmp++;
        QTimer::singleShot(1000,this,SLOT(marshall_calculations()));
        break;
    case 5:
        if(inconvenient_test == true){
            QMessageBox msgBox;
            msgBox.setWindowTitle(trUtf8("ERROR!"));
            msgBox.setText(trUtf8("<font size = 10>inconvenient test data.."));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
        else{
            results.displacement_at_peak_load = QString::number(disp_at_max_load,'f',3);
            if(marshall.thickness_correction == 1){
                results.stability_tangent_offset = QString::number(marshall.correction_multiplier * intersection_y,'f',3);
                results.stability_peak_method = QString::number(marshall.correction_multiplier * max_load,'f',3);
                results.thickness_correction = "ON";
            }
            else{
                results.stability_tangent_offset = QString::number(intersection_y,'f',3);
                results.stability_peak_method = QString::number(max_load,'f',3);
                results.thickness_correction = "OFF";
            }
            results.flow_tangent_offset = QString::number(intersection_x - x_axis_cut_point,'f',3);
            results.flow_peak_method = QString::number(disp_at_max_load - x_axis_cut_point,'f',3);

            write_test_results_to_file(current_test_no);

        }
        tmp = 0;
        break;
    }
}
void DC_Motor_PC::marshall_standard_type_handler(void){
    if(ui.radioButton_standard_astm_marshall->isChecked()){
        marshall.standard = ASTM_STANDARD_MARSHALL;
    }
    else if(ui.radioButton_standard_en_marshall->isChecked()){
        marshall.standard = EN_STANDARD_MARSHALL;
    }
    data_changed = true;
}


