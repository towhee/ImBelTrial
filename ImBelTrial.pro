#-------------------------------------------------
#
# Project created by QtCreator 2013-03-28T14:11:32
#
#-------------------------------------------------

QT       += core gui xml


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImBelTrial
TEMPLATE = app

CONFIG += console

SOURCES += main.cpp\
           mainwindow.cpp \
           dommodel.cpp \
           domitem.cpp

HEADERS  += mainwindow.h\
            ui_mainwindow.h \
            dommodel.h \
            domitem.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    ImBel.xml
