TEMPLATE = subdirs
CONFIG += ordered


SUBDIRS = qtdbusextended \
          qtmpris \
          src

qtdbusextended.file = lib/qtdbusextended.pro


qtmpris.file = lib/qtmpris.pro
qtmpris.depends = qtdbusextended


src.depends = qtmpris\
              qtdbusextended