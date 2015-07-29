HEADERS += glwidget.h
SOURCES += glwidget.cpp main.cpp

QT += opengl
QT += widgets
CONFIG -= app_bundle
CONFIG += console
INCLUDEPATH += "../include"

RESOURCES += \
    images.qrc

DISTFILES += \
    vertex.vert
