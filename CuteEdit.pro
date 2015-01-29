#-------------------------------------------------
#
# Project created by QtCreator 2015-01-15T13:09:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += qscintilla2

TARGET = CuteEdit
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qscieditor.cpp \
    preferences.cpp

HEADERS  += mainwindow.h \
    qscieditor.h \
    preferences.h

FORMS    += mainwindow.ui \
    qscieditor.ui \
    preferences.ui

RESOURCES += \
    icons.qrc

LIBS += -lqscintilla2

TRANSLATIONS += en.ts \
				fi.ts
