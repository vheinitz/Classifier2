#-------------------------------------------------
#
# Project created by QtCreator 2017-02-16T23:29:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Classifier2
DESTDIR = ../../build
TEMPLATE = vcapp
INCLUDEPATH += . ../.. \

SOURCES += main.cpp\
        gui.cpp \
    projectdata.cpp \
	objectselector.cpp \
    dlgaddclass.cpp \
    dlgedittag.cpp

HEADERS  += gui.h \
    projectdata.h \
	objectselector.h \
    dlgaddclass.h \
    dlgedittag.h

FORMS    += gui.ui \
    dlgaddclass.ui \
    dlgedittag.ui

win32 {
	CONFIG(debug, debug|release) {
		LIBS += \
		$$DESTDIR/cl2.lib
	} else {
		LIBS += \
		$$DESTDIR/cl2.lib
	}
}
