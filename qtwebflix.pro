QT       += webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtwebflix
TEMPLATE = app
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#Check  version of qt to enable hide scrollbars available in qt 5.10+
equals(QT_MAJOR_VERSION, 5):!lessThan(QT_MINOR_VERSION, 10) {
   DEFINES += HAS_SCROLLBAR
} else {
  message(Qt $$QT_VERSION ScrollBars not supported in this version.)
}

#Get current git tag and use for version number
BASE_GIT_COMMAND = git --git-dir $$PWD/.git --work-tree $$PWD
GIT_VERSION = $$system($$BASE_GIT_COMMAND describe --always --tags)
DEFINES += GIT_VERSION=\\\"$$GIT_VERSION\\\"

CONFIG += link_pkgconfig
PKGCONFIG = mpris-qt5


SOURCES += src/main.cpp\
           src/mainwindow.cpp \
           src/urlrequestinterceptor.cpp \
           src/commandlineparser.cpp\
           src/mprisinterface.cpp

HEADERS  += src/mainwindow.h \
            src/urlrequestinterceptor.h \
            src/commandlineparser.h\
            src/mprisinterface.h

FORMS    += ui/mainwindow.ui

RESOURCES = resources/jquery.qrc \

DISTFILES +=



