#-------------------------------------------------
#
# Project created by QtCreator 2018-10-04T20:44:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ToneMix
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    harmonicplaylistgenerator.cpp \
    track.cpp \
    tagreader.cpp \
    maincontroller.cpp \
    tonenotationtranslator.cpp \
    thememanager.cpp \
    mytableview.cpp

HEADERS += \
        mainwindow.h \
    harmonicplaylistgenerator.h \
    track.h \
    tagreader.h \
    maincontroller.h \
    emptyplaylistexception.h \
    nofocusproxystyle.h \
    tonenotationtranslator.h \
    thememanager.h \
    mytableview.h

FORMS += \
        mainwindow.ui

RC_FILE = 1.rc

#DEFINES+=MAKE_TAGLIB_LIB

#DEPENDPATH += "C:/Libraries/taglib/bin"
#LIBS += C:/Libraries/taglib/lib/libtag.a
#INCLUDEPATH += C:/Libraries/taglib/include/taglib


#win32: LIBS += -L$$PWD/../../../../Libraries/taglib/lib/ -ltag

#INCLUDEPATH += $$PWD/../../../../Libraries/taglib/include/taglib
#DEPENDPATH += $$PWD/../../../../Libraries/taglib/include/taglib

#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../../../Libraries/taglib/lib/tag.lib
#else:win32-g++: PRE_TARGETDEPS += $$PWD/../../../../Libraries/taglib/lib/libtag.a





win32: LIBS += $$PWD/../../../../Libraries/taglib/lib/libtag.dll.a

INCLUDEPATH += $$PWD/../../../../Libraries/taglib/include/taglib
DEPENDPATH += $$PWD/../../../../Libraries/taglib/include/taglib

DISTFILES += \
    .gitignore
