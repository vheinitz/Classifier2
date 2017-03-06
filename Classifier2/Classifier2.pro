#-------------------------------------------------
#
# Project created by QtCreator 2017-02-16T23:29:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Classifier2
DESTDIR = ../output
TEMPLATE = vcapp
INCLUDEPATH += . ../Libs \

SOURCES += main.cpp\
        gui.cpp \
    projectdata.cpp \
	objectselector.cpp \
    dlgaddclass.cpp

HEADERS  += gui.h \
    projectdata.h \
	objectselector.h \
    dlgaddclass.h

FORMS    += gui.ui \
    dlgaddclass.ui

win32 {
	CONFIG(debug, debug|release) {
		LIBS += \
		../output/cl2.lib
	} else {
		LIBS += \
		../output/cl2.lib
	}
}