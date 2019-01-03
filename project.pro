TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = src/qtdbusextended \
                    src/qtmpris \
                    src/qtwebflix
qtwebflix.depends = qtmpris\
                     qtdbusextended

