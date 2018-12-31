 TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = src/mpris-qt \
                    src/dbusextended-qt \
                    src/qtwebflix
qtwebflix.depends = mpris=qt
