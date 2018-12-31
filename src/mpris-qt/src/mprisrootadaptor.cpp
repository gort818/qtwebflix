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


#include "mprisplayer_p.h"

#include "mprisplayer.h"

#include <qqmlinfo.h>

#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class MprisRootAdaptor
 */

static const QString mprisRootInterface = QStringLiteral("org.mpris.MediaPlayer2");


MprisRootAdaptor::MprisRootAdaptor(MprisPlayer *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(false);

    connect(parent, SIGNAL(canQuitChanged()), this, SLOT(onCanQuitChanged()));
    connect(parent, SIGNAL(canRaiseChanged()), this, SLOT(onCanRaiseChanged()));
    connect(parent, SIGNAL(canSetFullscreenChanged()), this, SLOT(onCanSetFullscreenChanged()));
    connect(parent, SIGNAL(desktopEntryChanged()), this, SLOT(onDesktopEntryChanged()));
    connect(parent, SIGNAL(fullscreenChanged()), this, SLOT(onFullscreenChanged()));
    connect(parent, SIGNAL(hasTrackListChanged()), this, SLOT(onHasTrackListChanged()));
    connect(parent, SIGNAL(identityChanged()), this, SLOT(onIdentityChanged()));
    connect(parent, SIGNAL(supportedUriSchemesChanged()), this, SLOT(onSupportedUriSchemesChanged()));
    connect(parent, SIGNAL(supportedMimeTypesChanged()), this, SLOT(onSupportedMimeTypesChanged()));
}

MprisRootAdaptor::~MprisRootAdaptor()
{
    // destructor
}

bool MprisRootAdaptor::canQuit() const
{
    return static_cast<MprisPlayer *>(parent())->canQuit();
}

bool MprisRootAdaptor::canRaise() const
{
    return static_cast<MprisPlayer *>(parent())->canRaise();
}

bool MprisRootAdaptor::canSetFullscreen() const
{
    return static_cast<MprisPlayer *>(parent())->canSetFullscreen();
}

QString MprisRootAdaptor::desktopEntry() const
{
    return static_cast<MprisPlayer *>(parent())->desktopEntry();
}

bool MprisRootAdaptor::fullscreen() const
{
    return static_cast<MprisPlayer *>(parent())->fullscreen();
}

void MprisRootAdaptor::setFullscreen(bool value)
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());
    if (player->canSetFullscreen()) {
        emit player->fullscreenRequested(value);
        return;
    }

    // We cannot send an error reply in a property setter so we just
    // complain here
    if (value) {
        qDebug() << Q_FUNC_INFO << "Requested to fullscreen, but not supported";
    } else {
        qDebug() << Q_FUNC_INFO << "Requested to unfullscreen, but not supported";
    }
}

bool MprisRootAdaptor::hasTrackList() const
{
    return static_cast<MprisPlayer *>(parent())->hasTrackList();
}

QString MprisRootAdaptor::identity() const
{
    return static_cast<MprisPlayer *>(parent())->identity();
}

QStringList MprisRootAdaptor::supportedMimeTypes() const
{
    return static_cast<MprisPlayer *>(parent())->supportedMimeTypes();
}

QStringList MprisRootAdaptor::supportedUriSchemes() const
{
    return static_cast<MprisPlayer *>(parent())->supportedUriSchemes();
}

void MprisRootAdaptor::Quit()
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());
    if (player->canQuit()) {
        emit player->quitRequested();
        return;
    }

    player->sendErrorReply(QDBusError::NotSupported, QStringLiteral("Quit requested but not supported."));
}

void MprisRootAdaptor::Raise()
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());
    if (player->canRaise()) {
        emit player->raiseRequested();
        return;
    }

    player->sendErrorReply(QDBusError::NotSupported, QStringLiteral("Raise requested but not supported."));
}


// Private
void MprisRootAdaptor::onCanQuitChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    QVariantMap changedProperties;
    changedProperties[QStringLiteral("CanQuit")] = QVariant(player->canQuit());

    player->notifyPropertiesChanged(mprisRootInterface, changedProperties, QStringList());
}

void MprisRootAdaptor::onCanRaiseChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    QVariantMap changedProperties;
    changedProperties[QStringLiteral("CanRaise")] = QVariant(player->canRaise());

    player->notifyPropertiesChanged(mprisRootInterface, changedProperties, QStringList());
}

void MprisRootAdaptor::onCanSetFullscreenChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    QVariantMap changedProperties;
    changedProperties[QStringLiteral("CanSetFullscreen")] = QVariant(player->canSetFullscreen());

    player->notifyPropertiesChanged(mprisRootInterface, changedProperties, QStringList());
}

void MprisRootAdaptor::onDesktopEntryChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    QVariantMap changedProperties;
    changedProperties[QStringLiteral("DesktopEntry")] = QVariant(player->desktopEntry());

    player->notifyPropertiesChanged(mprisRootInterface, changedProperties, QStringList());
}

void MprisRootAdaptor::onFullscreenChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    QVariantMap changedProperties;
    changedProperties[QStringLiteral("Fullscreen")] = QVariant(player->fullscreen());

    player->notifyPropertiesChanged(mprisRootInterface, changedProperties, QStringList());
}

void MprisRootAdaptor::onHasTrackListChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    QVariantMap changedProperties;
    changedProperties[QStringLiteral("HasTrackList")] = QVariant(player->hasTrackList());

    player->notifyPropertiesChanged(mprisRootInterface, changedProperties, QStringList());
}

void MprisRootAdaptor::onIdentityChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    QVariantMap changedProperties;
    changedProperties[QStringLiteral("Identity")] = QVariant(player->identity());

    player->notifyPropertiesChanged(mprisRootInterface, changedProperties, QStringList());
}

void MprisRootAdaptor::onSupportedUriSchemesChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    QVariantMap changedProperties;
    changedProperties[QStringLiteral("SupportedUriSchemes")] = QVariant(player->supportedUriSchemes());

    player->notifyPropertiesChanged(mprisRootInterface, changedProperties, QStringList());
}

void MprisRootAdaptor::onSupportedMimeTypesChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    QVariantMap changedProperties;
    changedProperties[QStringLiteral("SupportedMimeTypes")] = QVariant(player->supportedMimeTypes());

    player->notifyPropertiesChanged(mprisRootInterface, changedProperties, QStringList());
}
