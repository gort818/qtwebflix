Extended DBus for Qt
====================

Qt provides several classes for DBus communication.

Extended DBus for Qt provides the following main additional features to the QDBusAbstractInterface class:

* DBus provides a [Properties Interface](http://dbus.freedesktop.org/doc/dbus-specification.html#standard-interfaces-properties) which specifies a *PropertiesChanged* signal whose reception is not implemented in the properties bridge by QtDBus. Extended DBus for Qt provides it.
* The DBus Properties Interface also specifies a *GetAll* method not implemented in the standard QtDBus. Extended DBus for Qt provides it.
* QtDBus properties mechanism is completely synchronous. Extended DBus for Qt provides an additional asynchronous alternative.
* An alternative and very simple cache mechanism is also implemented in Extended DBus for Qt so the DBus traffic is reduced to the minimum.


Installation:
-------------

```
$ qmake && make && make install
```


Examples:
---------

For a working example of Extended DBus for Qt check the [MPRIS for Qt](https://github.com/nemomobile/qtmpris) project.


TO-DO:
------

* QtDBus provides the *qdbuscpp2xml* and *qdbusxml2cpp* tools for automagically generate a XML DBus specification file from the C++ code or the other way around, to generate C++ code from a XML DBus specification file. An adaption of such tools would be needed so we can also generate those files from Extended DBus for Qt.
