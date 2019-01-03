TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = src/qtdbusextended/dbusextended-qt.pro \
                    src/qtmpris/mpris-qt.pro \
                    src/qtwebflix
qtwebflix.depends = qtmpris\
                     qtdbusextended

