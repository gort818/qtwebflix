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


#include "mpriscontroller_p.h"

#include <QtCore/QtDebug>


/*
 * Implementation of interface class MprisRootInterface
 */

MprisRootInterface::MprisRootInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : DBusExtendedAbstractInterface(service, path, staticInterfaceName(), connection, parent)
    , m_canQuit(false)
    , m_canRaise(false)
    , m_canSetFullscreen(false)
    , m_fullscreen(false)
    , m_hasTrackList(false)
{
    connect(this, SIGNAL(propertyChanged(QString, QVariant)), this, SLOT(onPropertyChanged(QString, QVariant)));
}

MprisRootInterface::~MprisRootInterface()
{
}

void MprisRootInterface::onPropertyChanged(const QString &propertyName, const QVariant &value)
{
    if (propertyName == QStringLiteral("CanQuit")) {
        bool canQuit = value.toBool();
        if (m_canQuit != canQuit) {
            m_canQuit = canQuit;
            emit canQuitChanged(m_canQuit);
        }
    } else if (propertyName == QStringLiteral("CanRaise")) {
        bool canRaise = value.toBool();
        if (m_canRaise != canRaise) {
            m_canRaise = canRaise;
            emit canRaiseChanged(m_canRaise);
        }
    } else if (propertyName == QStringLiteral("CanSetFullscreen")) {
        bool canSetFullscreen = value.toBool();
        if (m_canSetFullscreen != canSetFullscreen) {
            m_canSetFullscreen = canSetFullscreen;
            emit canSetFullscreenChanged(m_canSetFullscreen);
        }
    } else if (propertyName == QStringLiteral("DesktopEntry")) {
        QString desktopEntry = value.toString();
        if (m_desktopEntry != desktopEntry) {
            m_desktopEntry = desktopEntry;
            emit desktopEntryChanged(m_desktopEntry);
        }
    } else if (propertyName == QStringLiteral("Fullscreen")) {
        bool fullscreen = value.toBool();
        if (m_fullscreen != fullscreen) {
            m_fullscreen = fullscreen;
            emit fullscreenChanged(m_fullscreen);
        }
    } else if (propertyName == QStringLiteral("HasTrackList")) {
        bool hasTrackList = value.toBool();
        if (m_hasTrackList != hasTrackList) {
            m_hasTrackList = hasTrackList;
            emit hasTrackListChanged(m_hasTrackList);
        }
    } else if (propertyName == QStringLiteral("Identity")) {
        QString identity= value.toString();
        if (m_identity != identity) {
            m_identity = identity;
            emit identityChanged(m_identity);
        }
    } else if (propertyName == QStringLiteral("SupportedMimeTypes")) {
        QStringList supportedUriSchemes = value.toStringList();
        if (m_supportedUriSchemes != supportedUriSchemes) {
            m_supportedUriSchemes = supportedUriSchemes;
            emit supportedMimeTypesChanged(m_supportedUriSchemes);
        }
    } else if (propertyName == QStringLiteral("SupportedUriSchemes")) {
        QStringList supportedMimeTypes = value.toStringList();
        if (m_supportedMimeTypes != supportedMimeTypes) {
            m_supportedMimeTypes = supportedMimeTypes;
            emit supportedUriSchemesChanged(m_supportedMimeTypes);
        }
    } else {
        qWarning() << Q_FUNC_INFO
                   << "Received PropertyChanged signal from unknown property: "
                   << propertyName;
    }
}

