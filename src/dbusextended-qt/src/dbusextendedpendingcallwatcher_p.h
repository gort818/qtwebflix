// -*- c++ -*-

/*!
 *
 * Copyright (C) 2015 Jolla Ltd.
 *
 * Contact: Valerio Valerio <valerio.valerio@jolla.com>
 * Author: Andres Gomez <andres.gomez@jolla.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


//
//  W A R N I N G
//  -------------
//
// This file is not part of the public API.  This header file may
// change from version to version without notice, or even be
// removed.
//
// We mean it.
//
//


#ifndef DBUSEXTENDEDPENDINGCALLWATCHER_P_H
#define DBUSEXTENDEDPENDINGCALLWATCHER_P_H

#include <QDBusPendingCallWatcher>
#include <QDBusError>

class DBusExtendedPendingCallWatcher: public QDBusPendingCallWatcher
{
    Q_OBJECT

public:
    explicit DBusExtendedPendingCallWatcher(const QDBusPendingCall &call,
                                            const QString &asyncProperty,
                                            const QVariant &previousValue,
                                            QObject *parent = 0);
    ~DBusExtendedPendingCallWatcher();

    Q_PROPERTY(QString AsyncProperty READ asyncProperty)
    inline QString asyncProperty() const { return m_asyncProperty; }

    Q_PROPERTY(QVariant PreviousValue READ previousValue)
    inline QVariant previousValue() const { return m_previousValue; }

private:
    QString m_asyncProperty;
    QVariant m_previousValue;
};

#endif /* DBUSEXTENDEDPENDINGCALLWATCHER_P_H */
