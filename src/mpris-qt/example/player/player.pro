include(../../common.pri)

TEMPLATE = app
TARGET = player
CONFIG += qt link_pkgconfig

QT += qml quick

DEPENDPATH += ../../src
INCLUDEPATH += ../../src
LIBS += -L../../src -l$${MPRISQTLIB}
PKGCONFIG = dbusextended-qt5

SOURCES += \
    player.cpp

target.path = /usr/bin

INSTALLS += target
