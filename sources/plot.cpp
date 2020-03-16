#include "dc_motor_pc.h"

void DC_Motor_PC::reset_zoom(void){
    if(prevent_double_click()) return;

    plot_ch[0].zoomer->setZoomStack(plot_ch[0].zoomer->zoomStack(),0);
    zoom_out_button->hide();
}
void DC_Motor_PC::reset_plot(void){
    static double ccos_y[2] = {0};
    static double ccos_x[2] = {0};
    static double ctan_y[2] = {0};
    static double ctan_x[2] = {0};
    static double ccot_y[2] = {0};
    static double ccot_x[2] = {0};

    for(u8 i = 0; i < 2; i++){
        ccos_x[i] = 0;
        ccos_y[i] = 0;
        ctan_x[i] = 0;
        ctan_y[i] = 0;
        ccot_x[i] = 0;
        ccot_y[i] = 0;
        plot_ch[i].max_value_x = 0;
        plot_ch[i].min_value_x = 0xFFFFFF;
        plot_ch[i].max_value_y = 0;
        plot_ch[i].min_value_y = 0xFFFFFF;
        plot_ch[i].counter = 0;
        for(u32 j = 0; j < 1200; j++){
            plot_ch[i].xval[j] = 0;
            plot_ch[i].yval[j] = 0;
        }
    }
    plot_ch[0].cSin->setRawSamples(plot_ch[0].xval,plot_ch[0].yval,1200);
    plot_ch[0].cCos->setRawSamples(ccos_x,ccos_y,2);
    plot_ch[0].cTan->setRawSamples(ctan_x,ctan_y,2);
    plot_ch[0].cCot->setRawSamples(ccot_x,ccot_y,2);

    //marshall plots
    marshall.slope_line->detach();
    marshall.shifting_slope_line->detach();
    marshall.stability_marker->detach();
    marshall.x_axis_line->detach();
    marshall.y_axis_line->detach();

    plot_ch[0].plot->replot();

}
void DC_Motor_PC::set_plotter(void){
    plot_ch[0].plot = new QwtPlot(ui.widget_graphic);
    plot_ch[0].cSin = new QwtPlotCurve();
    plot_ch[0].cCos = new QwtPlotCurve();
    plot_ch[0].cTan = new QwtPlotCurve();
    plot_ch[0].cCot = new QwtPlotCurve();

    plot_ch[1].plot = new QwtPlot(this->ui.widget_pace_graphic);
    plot_ch[1].cSin = new QwtPlotCurve();

    for(u8 i = 0; i < 2; i++){
        plot_ch[i].max_value_x = 0;
        plot_ch[i].min_value_x = 0xFFFFFF;
        plot_ch[i].max_value_y = 0;
        plot_ch[i].min_value_y = 0xFFFFFF;
        plot_ch[i].counter = 0;
        for(u32 j = 0; j < 1200; j++){
            plot_ch[i].xval[j] = 0;
            plot_ch[i].yval[j] = 0;
        }
        plot_ch[i].grid = new QwtPlotGrid();
        plot_ch[i].grid->enableXMin(true);
        plot_ch[i].grid->enableYMin(true);
        plot_ch[i].grid->setMajPen(QPen(Qt::black, 0, Qt::DotLine));
        plot_ch[i].grid->setMinPen(QPen(Qt::gray, 0 , Qt::DotLine));
        plot_ch[i].grid->attach(plot_ch[i].plot);
    }

    plot_prepare_load(plot_ch[0].plot,plot_ch[0].cSin,plot_ch[0].cCos,plot_ch[0].cTan,plot_ch[0].cCot, trUtf8("Load (%1)").arg(unit_system.load_label),trUtf8("Time (sec)"));
    plot_prepare_pace(plot_ch[1].plot,plot_ch[1].cSin, trUtf8("Pace Rate (%1/s)").arg(unit_system.load_label),trUtf8("Time (sec)"));

    plot_ch[0].zoomer = new QwtPlotZoomer(plot_ch[0].plot->canvas());
    plot_ch[0].zoomer->setZoomBase(QRectF(-10,-10,20,20));
    plot_ch[0].zoomer->setRubberBand(QwtPicker::RectRubberBand);
    plot_ch[0].zoomer->setRubberBandPen(QColor(Qt::black));
    plot_ch[0].zoomer->setTrackerMode(QwtPicker::AlwaysOff);
    plot_ch[0].zoomer->setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton);
    connect(plot_ch[0].zoomer,SIGNAL(zoomed(QRectF)),this->zoom_out_button,SLOT(show()));
}
void DC_Motor_PC::plot_prepare_load(QwtPlot *plot, QwtPlotCurve *cSin, QwtPlotCurve *cCos,QwtPlotCurve *cTan,QwtPlotCurve *cCot, QString str_y, QString str_x){

    plot->setGeometry(0,0,540,380);
    plot->setAxisTitle(plot->xBottom, str_x );
    plot->setAxisScale(plot->xBottom, 0.0, 1.0);
    plot->setAxisTitle(plot->yLeft, str_y);
    plot->setAxisScale(plot->yLeft, 0, 1.0);

    plot->canvas()->setLineWidth(0);
    plot->canvas()->setBorderRadius(0);
    plot->canvas()->setFrameStyle( QFrame::Box | QFrame::Plain );

    QString eSheet("background: white;");
    plot->canvas()->setStyleSheet(eSheet);

    QString bSheet("background: rgb(0,206,209);");
    plot->setStyleSheet(bSheet);

    zoom_out_button = new QPushButton(plot->canvas());
    zoom_out_button->setGeometry(370,10,100,70);
    zoom_out_button->setText("ZOOM\nOUT");
    zoom_out_button->setFont(QFont("Ubuntu",16));
    zoom_out_button->hide();

    connect(zoom_out_button,SIGNAL(pressed()),this,SLOT(reset_zoom()));

    // Insert new curves
    cSin->setRenderHint(QwtPlotItem::RenderAntialiased);
    cSin->setPen(QPen(Qt::blue,4,Qt::SolidLine));
    cSin->attach(plot);

    // Insert new curves
    cCos->setRenderHint(QwtPlotItem::RenderAntialiased);
    cCos->setPen(QPen(Qt::red,2,Qt::DashDotDotLine));
    cCos->attach(plot);

    // Insert new curves
    cTan->setRenderHint(QwtPlotItem::RenderAntialiased);
    cTan->setPen(QPen(Qt::red,2,Qt::DashDotDotLine));
    cTan->attach(plot);

    // Insert new curves
    cCot->setRenderHint(QwtPlotItem::RenderAntialiased);
    cCot->setPen(QPen(Qt::black,1,Qt::DashDotDotLine));
    cCot->attach(plot);

    /*
    //  ...a horizontal line at y = 0...
    QwtPlotMarker *mY_ch1 = new QwtPlotMarker();
    mY_ch1->setLabel(QString::fromLatin1("y = 0"));
    mY_ch1->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    mY_ch1->setLineStyle(QwtPlotMarker::HLine);
    mY_ch1->setYValue(0.0);
    mY_ch1->attach(plot);
    */
//    QwtLegend *legend = new QwtLegend;
//    legend->setFrameStyle(QFrame::Box|QFrame::Sunken);
//    plot->insertLegend(legend, QwtPlot::RightLegend);

}
void DC_Motor_PC::plot_prepare_pace(QwtPlot *plot, QwtPlotCurve *cSin, QString str_y, QString str_x){

    plot->setGeometry(0,0,291,221);
    plot->setAxisFont(plot->yLeft,QFont("Ubuntu",8));
    plot->setAxisFont(plot->xBottom,QFont("Ubuntu",8));
    plot->setAxisTitle(plot->xBottom, str_x );
    plot->setAxisScale(plot->xBottom, 0.0, 1.0);
    plot->setAxisTitle(plot->yLeft, str_y);
    plot->setAxisScale(plot->yLeft, 0, 1.0);

    plot->setAutoFillBackground(false);
    plot->setPalette(QPalette(QColor(165,193,228)));

    plot->canvas()->setLineWidth(1);
    plot->canvas()->setBorderRadius(15);
    plot->canvas()->setFrameStyle( QFrame::Box | QFrame::Plain );
    plot->setCanvasBackground(Qt::white);

    // Insert new curves
    cSin->setRenderHint(QwtPlotItem::RenderAntialiased);
    cSin->setPen(QPen(Qt::blue,3));
    cSin->attach(plot);

    //  ...a horizontal line at y = 0...
    QwtPlotMarker *mY_ch1 = new QwtPlotMarker();
    mY_ch1->setLabel(QString::fromLatin1("y = 0"));
    mY_ch1->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    mY_ch1->setLineStyle(QwtPlotMarker::HLine);
    mY_ch1->setYValue(0.0);
    mY_ch1->attach(plot);
}
void DC_Motor_PC::load_plotter(QwtPlot *plot, QwtPlotCurve *cSin, QwtPlotCurve *cCos, QwtPlotCurve *cTan, QwtPlotCurve *cCot,
        double value_x, double value_y,
        double *max_value_x, double *min_value_x,
        double *max_value_y, double *min_value_y,
        u32 *counter,
        double *xval, double *yval)
{
    double guideline_target;
    static int cycle = 0;
    static bool guideline_on = false;
    static double first_load = 0;
    static double caught_y = 0;
    static double caught_x = 0;
    static double ccos_y[2] = {0};
    static double ccos_x[2] = {0};
    static double ctan_y[2] = {0};
    static double ctan_x[2] = {0};
    static double ccot_y[2] = {0};
    static double ccot_x[2] = {0};

    guideline_target = (v_axis_radiobutton_no == V_AXIS_LOAD) ? ui.doubleSpinBox_pace_rate->value() : ui.doubleSpinBox_pace_rate_mpa->value();

    qDebug() << "test type" << test_type << " guideline" << guideline_on;
    if(PLOT_first_in){
        PLOT_first_in = false;
        cycle = 0;
        guideline_on = false;
    }
    else{
        cycle++;
        if(cycle == 11){
            if(test_type == LOAD_CONTROL){
                if(h_axis_radiobutton_no == H_AXIS_TIME){
                    if(v_axis_radiobutton_no != V_AXIS_DISPLACEMENT){
                        guideline_on = true;
                    }
                }
            }
            caught_y = value_y;
            caught_x = (double)(*counter)/5;
            first_load = caught_y - guideline_target*caught_x;
        }
        if(h_axis_radiobutton_no == H_AXIS_TIME){
            xval[(*counter)] = value_x/5;     //200 msec loop
        }
        else if(h_axis_radiobutton_no == H_AXIS_DISPLACEMENT){
            xval[(*counter)] = value_x;
        }
        yval[(*counter)] = value_y;

        if(xval[(*counter)] > *max_value_x){
            (*max_value_x) = xval[(*counter)];
        }
        if(xval[(*counter)] < (*min_value_x)){
            (*min_value_x) = xval[(*counter)];
        }
        if(yval[(*counter)] > *max_value_y){
           (*max_value_y) = yval[(*counter)];
        }
        if(yval[(*counter)] < (*min_value_y)){
           (*min_value_y) = yval[(*counter)];
        }

        if(zoom_radiobutton_no == FIXED_AXES){
            plot->setAxisScale(plot->xBottom,ui.spinBox_zoom_val_neg_x->value(),ui.spinBox_zoom_val_pos_x->value());
            plot->setAxisScale(plot->yLeft,ui.spinBox_zoom_val_neg_y->value(),ui.spinBox_zoom_val_pos_y->value());

            if(guideline_on){
                ccos_y[0] = first_load;
                ccos_x[0] = 0;
                ccos_y[1] = first_load + (guideline_target + guideline_target*0.01*ui.spinBox_guideline_spacing->value())*ui.spinBox_zoom_val_pos_x->value();
                ccos_x[1] = ui.spinBox_zoom_val_pos_x->value();

                ctan_y[0] = first_load;
                ctan_x[0] = 0;
                ctan_y[1] = first_load + (guideline_target - guideline_target*0.01*ui.spinBox_guideline_spacing->value())*ui.spinBox_zoom_val_pos_x->value();
                ctan_x[1] = ui.spinBox_zoom_val_pos_x->value();

                ccot_y[0] = first_load;
                ccot_x[0] = 0;
                ccot_y[1] = first_load + guideline_target*ui.spinBox_zoom_val_pos_x->value();
                ccot_x[1] = ui.spinBox_zoom_val_pos_x->value();
            }
        }
        else{
            if(*min_value_x >= 0){
                plot->setAxisScale(plot->xBottom, (*min_value_x) - ((*min_value_x) * 0.2), (*max_value_x) + ((*max_value_x) * 0.2));
            }
            else{
                plot->setAxisScale(plot->xBottom, (*min_value_x) + ((*min_value_x) * 0.2), (*max_value_x) + ((*max_value_x) * 0.2));
            }
            if(*min_value_y >= 0){
                plot->setAxisScale(plot->yLeft, (*min_value_y) - ((*min_value_y) * 0.2), (*max_value_y) + ((*max_value_y) * 0.2));
            }
            else{
                plot->setAxisScale(plot->yLeft, (*min_value_y) + ((*min_value_y) * 0.2), (*max_value_y) + ((*max_value_y) * 0.2));
            }

            if(guideline_on){
                // Top line
                ccos_y[0] = first_load;
                ccos_x[0] = 0;
                ccos_y[1] = first_load + (guideline_target + guideline_target*0.01*ui.spinBox_guideline_spacing->value())*((double)(*counter)/5 + caught_x);
                ccos_x[1] = ((double)(*counter)/5 + caught_x);
                // Bottom line
                ctan_y[0] = first_load;
                ctan_x[0] = 0;
                ctan_x[1] = ((double)(*counter)/5 + caught_x);
                ctan_y[1] = first_load + (guideline_target - guideline_target*0.01*ui.spinBox_guideline_spacing->value())*((double)(*counter)/5 + caught_x);
                //mid-line
                ccot_y[0] = first_load;
                ccot_x[0] = 0;
                ccot_y[1] = first_load + guideline_target*((double)(*counter)/5 + caught_x);
                ccot_x[1] = ((double)(*counter)/5 + caught_x);
            }
        }
        cSin->setRawSamples(xval,yval,(*counter));
        if(control_mode == AUTO){
            if(guideline_on){
                cCos->setRawSamples(ccos_x,ccos_y,2);
                cTan->setRawSamples(ctan_x,ctan_y,2);
                cCot->setRawSamples(ccot_x,ccot_y,2);
            }
            else{
                cCos->setRawSamples(0,0,0);
                cTan->setRawSamples(0,0,0);
                cCot->setRawSamples(0,0,0);
            }
        }

        plot->replot();
        if((*counter) < 1998){
            (*counter) = 1 + *counter;
        }
        if(test_type == MARSHALL){
            marshall.latest_x = *counter;
        }
    }

}
void DC_Motor_PC::pace_plotter(QwtPlot *plot, QwtPlotCurve *cSin,
        double value_x, double value_y,
        double *max_value_x, double *min_value_x,
        double *max_value_y, double *min_value_y,
        u32 *counter,
        double *xval, double *yval)
{
    xval[(*counter)] = value_x/20;  //50 msec loop
    yval[(*counter)] = value_y;

    if(xval[(*counter)] > *max_value_x){
        (*max_value_x) = xval[(*counter)];
    }
    if(xval[(*counter)] < (*min_value_x)){
        (*min_value_x) = xval[(*counter)];
    }

    if(yval[(*counter)] > *max_value_y){
        (*max_value_y) = yval[(*counter)];
    }
    if(yval[(*counter)] < (*min_value_y)){
        (*min_value_y) = yval[(*counter)];
    }

    cSin->setRawSamples(xval,yval,(*counter));

    if(*min_value_x >= 0){
        plot->setAxisScale(plot->xBottom, (*min_value_x) - (((*min_value_x) * 2)/10), (*max_value_x) + (((*max_value_x) * 2)/10));
    }
    else{
        plot->setAxisScale(plot->xBottom, (*min_value_x) + (((*min_value_x) * 2)/10), (*max_value_x) + (((*max_value_x) * 2)/10));
    }
    if(*min_value_y >= 0){
        plot->setAxisScale(plot->yLeft, (*min_value_y) - (((*min_value_y) * 2)/10), (*max_value_y) + (((*max_value_y) * 2)/10));
    }
    else{
        plot->setAxisScale(plot->yLeft, (*min_value_y) + (((*min_value_y) * 2)/10), (*max_value_y) + (((*max_value_y) * 2)/10));
    }
    plot->replot();
    (*counter) = 1 + *counter;
}
void DC_Motor_PC::draw_load_graphic(void){
    double vertical_axis = 0;
    double horizontal_axis = 0;

    if(v_axis_radiobutton_no == V_AXIS_LOAD){
        vertical_axis = fuzpid->load_value;
    }
    else if (v_axis_radiobutton_no == V_AXIS_STRESS){
        vertical_axis = fuzpid->stress;
    }
    else if (v_axis_radiobutton_no == V_AXIS_DISPLACEMENT){
        vertical_axis = fuzpid->displacement_value;
    }

    if(h_axis_radiobutton_no == H_AXIS_TIME){
        horizontal_axis = (double)plot_ch[0].counter;
    }
    else if(h_axis_radiobutton_no == H_AXIS_DISPLACEMENT){
        horizontal_axis = fuzpid->displacement_value;
    }

    load_plotter(plot_ch[0].plot,plot_ch[0].cSin,plot_ch[0].cCos,plot_ch[0].cTan,plot_ch[0].cCot,
            horizontal_axis,vertical_axis,
            &plot_ch[0].max_value_x,&plot_ch[0].min_value_x,
            &plot_ch[0].max_value_y,&plot_ch[0].min_value_y,
            &plot_ch[0].counter,
            plot_ch[0].xval,plot_ch[0].yval);
}
void DC_Motor_PC::draw_pace_graphic(void){
    float pace = 0;

    if(test_type == LOAD_CONTROL){
        pace = fuzpid->current_pace_rate;
    }
    else{
        pace = fuzpid->current_displacement_rate;
    }

    pace_plotter(plot_ch[1].plot,plot_ch[1].cSin,
            (double)plot_ch[1].counter,pace,
            &plot_ch[1].max_value_x,&plot_ch[1].min_value_x,
            &plot_ch[1].max_value_y,&plot_ch[1].min_value_y,
            &plot_ch[1].counter,
            plot_ch[1].xval,plot_ch[1].yval);
}

void DC_Motor_PC::graphic_type_handler(){
    if(ui.radioButton_fixed_axes->isChecked()){
        zoom_radiobutton_no = FIXED_AXES;
    }
    else if(ui.radioButton_auto_zoom->isChecked()){
        zoom_radiobutton_no = AUTO_ZOOM;
    }
    data_changed = true;
}
void DC_Motor_PC::graphic_v_axis_handler(){

    if(ui.radioButton_v_axis_load->isChecked()){
        v_axis_radiobutton_no = V_AXIS_LOAD;
        plot_ch[0].plot->setAxisTitle(plot_ch[0].plot->yLeft, trUtf8("Load (%1)").arg(unit_system.load_label));
    }
    else if(ui.radioButton_v_axis_stress->isChecked()){
        v_axis_radiobutton_no = V_AXIS_STRESS;
        plot_ch[0].plot->setAxisTitle(plot_ch[0].plot->yLeft, trUtf8("Stress (%1)").arg(unit_system.stress_label));
    }
    else if(ui.radioButton_v_axis_displacement->isChecked()){
        v_axis_radiobutton_no = V_AXIS_DISPLACEMENT;
        plot_ch[0].plot->setAxisTitle(plot_ch[0].plot->yLeft, trUtf8("Displacement (%1)").arg(unit_system.length_label));
    }
    data_changed = true;
}
void DC_Motor_PC::graphic_h_axis_handler(){

    if(ui.radioButton_h_axis_time->isChecked()){
        h_axis_radiobutton_no = H_AXIS_TIME;
        plot_ch[0].plot->setAxisTitle(plot_ch[0].plot->xBottom, trUtf8("Time (sec)") );
    }
    else if(ui.radioButton_h_axis_displacement->isChecked()){
        h_axis_radiobutton_no = H_AXIS_DISPLACEMENT;
        plot_ch[0].plot->setAxisTitle(plot_ch[0].plot->xBottom, trUtf8("Displacement (%1)").arg(unit_system.length_label) );
    }
    data_changed = true;
}
