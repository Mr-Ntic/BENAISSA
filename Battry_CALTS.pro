#-------------------------------------------------
#
# Project created by QtCreator 2016-03-26T16:27:36
#
#-------------------------------------------------

QT       += core gui serialport
RC_FILE = myapp.rc
DESTDIR = $$PWD
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Battry_CALTS
TEMPLATE = app


SOURCES += main.cpp\
        b_calts.cpp \
        about.cpp \
        select_t.cpp \
        QCP/qcustomplot.cpp

HEADERS  += b_calts.h \
    about.h \
    select_t.h \
    QCP/qcustomplot.h

FORMS    += b_calts.ui \
    about.ui \
    select_t.ui

RESOURCES += \
    resource_file.qrc
