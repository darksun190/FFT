#-------------------------------------------------
#
# Project created by QtCreator 2013-08-19T17:29:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_FFT
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
INCLUDEPATH += C:\Qt\SP++3.0\include

LIBS            += C:\Qt\libs\libsp_xmlread.a
INCLUDEPATH     += C:\Qt\include

OTHER_FILES += \
    readme.txt
