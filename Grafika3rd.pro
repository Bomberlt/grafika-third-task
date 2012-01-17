#-------------------------------------------------
#
# Project created by QtCreator 2012-01-17T14:55:51
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Grafika3rd
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -lopengl32 -lglu32 #-lglut32

INCLUDEPATH = C:/glut-3.5/include
LIBS += C:/glut-3.7.6-bin/glut32.lib

SOURCES += main.cpp
