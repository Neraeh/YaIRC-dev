#-------------------------------------------------
#
# Project created by QtCreator 2015-07-12T16:12:11
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = YaIRC
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    irc.cpp \
    chan.cpp \
    user.cpp

HEADERS  += mainwindow.h \
    irc.h \
    chan.h \
    user.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

QMAKE_CFLAGS_RELEASE += -O2 -pipe
QMAKE_CXXFLAGS_RELEASE += $$QMAKE_CFLAGS_RELEASE
QMAKE_LFLAGS_RELEASE += -s

win32 {
    RC_FILE = resources.rc
}
