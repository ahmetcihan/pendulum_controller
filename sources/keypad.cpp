#include "dc_motor_pc.h"

QString DC_Motor_PC::getKeyText(QString txt){
    Keyboard k;
    k.setStyleSheet(currentStyleSheet);
    k.setSpinBox(txt);
    k.exec();
    const QString &s = k.getKeyboardText();
    return s;
}
void DC_Motor_PC::checkSpin(QWidget *newwidget){
    QSpinBox *spin = qobject_cast<QSpinBox *> (newwidget);
    if (!spin)
        return;

    QString s = getKeyText("");
    if (!s.isEmpty())
        spin->setValue(s.toInt());
}
void DC_Motor_PC::checkDoubleSpin(QWidget *newwidget){
    QDoubleSpinBox *spin = qobject_cast<QDoubleSpinBox *> (newwidget);
    if (!spin)
        return;

    QString s = getKeyText("");
    if (!s.isEmpty())
        spin->setValue(s.toDouble());
}
void DC_Motor_PC::checkLineEdit(QWidget *newwidget){
    QLineEdit *lin = qobject_cast<QLineEdit *> (newwidget);
    if (!lin)
        return;

    QString s = getKeyText("");
    if (!s.isEmpty())
        lin->setText(s);
}
void DC_Motor_PC::keyboard_delayed_operation(void){
    QApplication *app = (QApplication *) QApplication::instance();
    connect(app,SIGNAL(focusChanged(QWidget*,QWidget*)),this,SLOT(focusChanged(QWidget*,QWidget*)));
    if(do_not_change_focus == false){
        ui.pushButton_focus->setFocus();
    }
}
void DC_Motor_PC::focusChanged(QWidget *, QWidget *newwidget){
    QApplication *app = (QApplication *) QApplication::instance();
    disconnect(app,SIGNAL(focusChanged(QWidget*,QWidget*)),this,SLOT(focusChanged(QWidget*,QWidget*)));

    checkSpin(newwidget);
    checkDoubleSpin(newwidget);
    checkLineEdit(newwidget);

    QTimer::singleShot(100,this,SLOT(keyboard_delayed_operation()));
    data_changed = true;
}

