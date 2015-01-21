#-------------------------------------------------
#
# Project created by QtCreator 2014-07-15T19:38:24
#
#-------------------------------------------------

QT       += core gui widgets



TARGET = ColorKeyer
TEMPLATE = app


SOURCES += main.cpp\
        videoplayer.cpp \
    colorkeyerhsv.cpp

HEADERS  += videoplayer.h \
    colorkeyerhsv.h

FORMS    += videoplayer.ui

include(../videoengine/videoengine.pri)
