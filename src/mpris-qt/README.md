Qt and QML MPRIS interface and adaptor
======================================

[MPRIS v.2](http://specifications.freedesktop.org/mpris-spec/latest/index.html) specification implementation for Qt and QML plugin.


Installation:
-------------

MPRIS for Qt depends on [Extended DBus for Qt](https://github.com/nemomobile/qtdbusextended) so make sure to install it before building this project.


```
$ qmake && make && make install
```


TO-DO:
------

* Implement the optional [Tracklist](http://specifications.freedesktop.org/mpris-spec/latest/Track_List_Interface.html) and [Playlists](http://specifications.freedesktop.org/mpris-spec/latest/Playlists_Interface.html) interfaces.
