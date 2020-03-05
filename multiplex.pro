TEMPLATE = app
TARGET = multiplex
QT += core gui network concurrent
CONFIG += qt

QMAKE_CXXFLAGS_RELEASE = -O2

HEADERS += dc_motor_pc.h \
	keyboard.h \
	remoteconsole.h \
	settingsserver.h \
	fuzzy_pid.h \
    macros.h

SOURCES += sources/plot.cpp \
	sources/process.cpp \
	sources/adc.cpp \
	sources/usart.cpp \
	sources/calibration.cpp \
	main.cpp \
	dc_motor_pc.cpp \
	sources/servo_motor.cpp \
	sources/filter.cpp \
	keyboard.cpp \
	remoteconsole.cpp \
	settingsserver.cpp \
	fuzzy_pid.cpp \
	sources/PID.cpp \
	sources/ethernet.cpp \
	sources/recording.cpp \
	sources/autotuning.cpp \
	sources/initilize.cpp \
    sources/channels.cpp \
    sources/usb.cpp \
    sources/names.cpp \
    sources/keypad.cpp \
    sources/language.cpp \
    sources/results.cpp \
    sources/unit_system.cpp \
    sources/jog.cpp \
    sources/cbr.cpp \
    sources/marshall.cpp

FORMS += dc_motor_pc.ui \
	keyboard.ui \
	fuzzy_pid.ui

include(/home/ahmet/Desktop/qextserialport/src/qextserialport.pri)

x86 {
	INCLUDEPATH += /home/ahmet/Desktop/qwt_x86/src
	LIBS += -L/home/ahmet/Desktop/qwt_x86/lib -lqwt
	DEFINES += CONFIG_x86
}

arm {
	INCLUDEPATH += /home/ahmet/Desktop/qwt-6.0.1/src
	LIBS += -L/home/ahmet/Desktop/qwt-6.0.1/lib -lqwt
	DEFINES += CONFIG_ARM
}

TRANSLATIONS = multiplex_tr.ts multiplex_en.ts multiplex_fr.ts multiplex_sp.ts multiplex_ru.ts

RESOURCES += \
	resources.qrc

OTHER_FILES +=
