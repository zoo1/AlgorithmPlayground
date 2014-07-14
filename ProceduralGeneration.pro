#-------------------------------------------------
#
# Project created by QtCreator 2014-05-17T20:42:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProceduralGeneration
TEMPLATE = app


SOURCES += main.cpp\
        generation.cpp \
    map.cpp \
    room.cpp \
    hallway.cpp

HEADERS  += generation.h \
    map.h \
    room.h \
    hallway.h

FORMS    += generation.ui

OTHER_FILES +=
