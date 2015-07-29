HEADERS += glwidget.h \ 
    shader_loader.h
SOURCES += glwidget.cpp main.cpp \
    shader_loader.cpp

QT += opengl designer
CONFIG -= app_bundle
CONFIG += console c++11
INCLUDEPATH += "../include"
INCLUDEPATH += $$PWD

RESOURCES += shaders.qrc \
    textures.qrc

FORMS += \
    mainwindow.ui
