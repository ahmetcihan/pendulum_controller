#include "dc_motor_pc.h"

bool DC_Motor_PC::pushbutton_name_compare(QPushButton *p1, QPushButton *p2){
    if ( p1 && p2 )
        return p1->objectName() < p2->objectName();
    return false;
}
bool DC_Motor_PC::label_name_compare(QLabel *p1, QLabel *p2){
    if ( p1 && p2 )
        return p1->objectName() < p2->objectName();
    return false;
}
bool DC_Motor_PC::spinbox_name_compare(QSpinBox *p1, QSpinBox *p2){
    if ( p1 && p2 )
        return p1->objectName() < p2->objectName();
    return false;
}
bool DC_Motor_PC::doublespinbox_name_compare(QDoubleSpinBox *p1, QDoubleSpinBox *p2){
    if ( p1 && p2 )
        return p1->objectName() < p2->objectName();
    return false;
}
void DC_Motor_PC::rearrange_object_names(void){
    label_gain = this->findChildren<QLabel*>(QRegExp("label_gain"));
    qSort(label_gain.begin(),label_gain.end(),label_name_compare);

    label_point = this->findChildren<QLabel*>(QRegExp("label_point_"));
    qSort(label_point.begin(),label_point.end(),label_name_compare);

    spinBox_cal_raw_value = this->findChildren<QSpinBox*>(QRegExp("spinBox_cal_raw_value_"));
    qSort(spinBox_cal_raw_value.begin(),spinBox_cal_raw_value.end(),spinbox_name_compare);

    doubleSpinBox_cal_point_value = this->findChildren<QDoubleSpinBox*>(QRegExp("doubleSpinBox_cal_point_value"));
    qSort(doubleSpinBox_cal_point_value.begin(),doubleSpinBox_cal_point_value.end(),doublespinbox_name_compare);

    cbr_point_value = this->findChildren<QDoubleSpinBox*>(QRegExp("doubleSpinBox_point_"));
    qSort(cbr_point_value.begin(),cbr_point_value.end(),doublespinbox_name_compare);

    pushButton_read_cal_point = this->findChildren<QPushButton*>(QRegExp("pushButton_read_cal_point_"));
    qSort(pushButton_read_cal_point.begin(),pushButton_read_cal_point.end(),pushbutton_name_compare);

}

