include(qtdbusextended/common.pri)

TEMPLATE = lib
CONFIG += qt staticlib

QT = core dbus

TARGET = $${QTDBUSEXTENDEDLIB}

DEFINES += QT_DBUS_EXTENDED_LIBRARY

SOURCES += \
    qtdbusextended/src/dbusextendedabstractinterface.cpp \
    qtdbusextended/src/dbusextendedpendingcallwatcher.cpp

HEADERS += \
    qtdbusextended/src/dbusextended.h \
    qtdbusextended/src/dbusextendedabstractinterface.h \
    qtdbusextended/src/dbusextendedpendingcallwatcher_p.h

INCLUDEPATH += qtdbusextended/src

