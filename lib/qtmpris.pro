include(qtmpris/common.pri)

TEMPLATE = lib
CONFIG += qt staticlib

QT = core dbus qml

TARGET = $${MPRISQTLIB}

DEFINES += MPRIS_QT_LIBRARY

SOURCES += \
    qtmpris/src/mpris.cpp \
    qtmpris/src/mprisrootadaptor.cpp \
    qtmpris/src/mprisplayeradaptor.cpp \
    qtmpris/src/mprisplayer.cpp \
    qtmpris/src/mpriscontroller.cpp \
    qtmpris/src/mprismanager.cpp \
    qtmpris/src/mprisplayerinterface.cpp \
    qtmpris/src/mprisrootinterface.cpp

HEADERS += \
    qtmpris/src/mprisqt.h \
    qtmpris/src/mpris.h \
    qtmpris/src/mprisplayer.h \
    qtmpris/src/mprisplayer_p.h \
    qtmpris/src/mpriscontroller.h \
    qtmpris/src/mpriscontroller_p.h \
    qtmpris/src/mprismanager.h

OTHER_FILES += qtmpris/src/org.mpris.MediaPlayer2.xml \
    qtmpris/src/org.mpris.MediaPlayer2.Player.xml

INCLUDEPATH += qtmpris/src qtdbusextended/src

LIBS += -L. -ldbusextended-qt5

