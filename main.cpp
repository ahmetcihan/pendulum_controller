#include "dc_motor_pc.h"
#include <QDir>
#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    QDir::setCurrent(a.applicationDirPath());
    if (!QFile::exists("settings.ini")) {
        QFile f("settings.ini");
        f.open(QIODevice::ReadOnly);
        QByteArray ba = f.readAll();
        f.close();
        f.setFileName("settings.ini");
        f.open(QIODevice::WriteOnly);
        f.write(ba);
        f.close();
    }
    DC_Motor_PC w;
    w.show();
    return a.exec();
}
