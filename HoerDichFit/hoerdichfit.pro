#-------------------------------------------------
#
# Project created by QtCreator 2014-07-15T19:38:24
#
#-------------------------------------------------

QT       += core gui widgets multimedia multimediawidgets



TARGET = HoerDichFit
TEMPLATE = app


SOURCES += main.cpp\
        videoplayer.cpp \
    colorkeyerhsv.cpp

HEADERS  += videoplayer.h \
    colorkeyerhsv.h

FORMS    += videoplayer.ui

include(../videoengine/videoengine.pri)
