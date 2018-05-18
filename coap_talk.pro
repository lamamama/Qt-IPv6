#-------------------------------------------------
#
# Project created by QtCreator 2017-03-31T21:04:07
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION,5 ): QT += widgets printsupport

TARGET = coap_talk
TEMPLATE = app


SOURCES += main.cpp\
    mysplashscreen.cpp \
    qcustomplot/qcustomplot.cpp \
    main_dialog.cpp \
    measure_timer.cpp \
    curve_show.cpp \
    control.cpp \
    cantcoap.cpp

HEADERS  += \
    mysplashscreen.h \
    qcustomplot/qcustomplot.h \
    main_dialog.h \
    measure_timer.h \
    curve_show.h \
    control.h \
    cantcoap.h \
    dbg.h \
    sysdep.h \
    connect.h

FORMS    += \
    main_dialog.ui \
    curve_show.ui \
    control.ui

QT += network
QT += printsupport
QT += sql

RESOURCES += \
    resource.qrc

QMAKE_CXXFLAGS += -std=c++0x #对于Qt4.7版本



