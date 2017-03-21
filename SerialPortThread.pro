#-------------------------------------------------
#
# Project created by QtCreator 2017-03-07T00:30:49
#
#-------------------------------------------------
#需要在这里加serialport,此配置文件需要根据实际项目编写
QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EthSerialTest
TEMPLATE = app


SOURCES += main.cpp\
        ethmainwindow.cpp \
    serialportthread.cpp

HEADERS  += ethmainwindow.h \
    serialportthread.h

FORMS    += ethmainwindow.ui
