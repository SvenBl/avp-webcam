#-------------------------------------------------
#
# Project created by QtCreator 2014-07-15T19:38:24
#
#-------------------------------------------------

INCLUDEPATH += ../videoengine

SOURCES +=  ../videoengine/videoengine.cpp \
            ../videoengine/videowidget.cpp \
    ../videoengine/videoformat.cpp
HEADERS  += ../videoengine/videoengine.h \
            ../videoengine/videowidget.h \
            ../videoengine/cvmattoqimage.h \
            ../videoengine/videoprocessor.h \
    ../videoengine/videoformat.h

include(../videoengine/opencv.pri)
