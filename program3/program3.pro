HEADERS += glwidget.h \ 
    cube.h \
    shader_loader.h \
    skybox.h \
    ground.h \
    house.h \
    texture_assets.h \
    pyramid.h \
    tree.h \
    object_loader.h \
    grass.h \
    well.h \
    barrel.h \
    allien.h
SOURCES += glwidget.cpp main.cpp \
    cube.cpp \
    shader_loader.cpp \
    skybox.cpp \
    ground.cpp \
    house.cpp \
    texture_assets.cpp \
    pyramid.cpp \
    object_loader.cpp \
    tree.cpp \
    ../include/tinyobjloader/tiny_obj_loader.cc \
    grass.cpp \
    well.cpp \
    barrel.cpp \
    allien.cpp

QT += opengl designer
CONFIG -= app_bundle
CONFIG += console c++11 libstdc++
INCLUDEPATH += "../include"
INCLUDEPATH += $$PWD

RESOURCES += shaders.qrc \
    texture.qrc
