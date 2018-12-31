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

#include "mpris.h"

#include <QtCore/QtDebug>


/*
 * Implementation of interface class MprisPlayerInterface
 */

MprisPlayerInterface::MprisPlayerInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : DBusExtendedAbstractInterface(service, path, staticInterfaceName(), connection, parent)
    , m_canControl(false)
    , m_canGoNext(false)
    , m_canGoPrevious(false)
    , m_canPause(false)
    , m_canPlay(false)
    , m_canSeek(false)
    , m_loopStatus(Mpris::None)
    , m_maximumRate(1)
    , m_minimumRate(1)
    , m_playbackStatus(Mpris::Stopped)
    , m_position(0)
    , m_rate(1)
    , m_shuffle(false)
    , m_volume(0)
{
    connect(this, SIGNAL(propertyChanged(QString, QVariant)), this, SLOT(onPropertyChanged(QString, QVariant)));
}

MprisPlayerInterface::~MprisPlayerInterface()
{
}

void MprisPlayerInterface::onPropertyChanged(const QString &propertyName, const QVariant &value)
{
    if (propertyName == QStringLiteral("CanControl")) {
        bool canControl = value.toBool();
        if (m_canControl != canControl) {
            m_canControl = canControl;
            emit canControlChanged(m_canControl);
        }
    } else if (propertyName == QStringLiteral("CanGoNext")) {
        bool canGoNext = value.toBool();
        if (m_canGoNext != canGoNext) {
            m_canGoNext = canGoNext;
            emit canGoNextChanged(m_canGoNext);
        }
    } else if (propertyName == QStringLiteral("CanGoPrevious")) {
        bool canGoPrevious = value.toBool();
        if (m_canGoPrevious != canGoPrevious) {
            m_canGoPrevious = canGoPrevious;
            emit canGoPreviousChanged(m_canGoPrevious);
        }
    } else if (propertyName == QStringLiteral("CanPause")) {
        bool canPause = value.toBool();
        if (m_canPause != canPause) {
            m_canPause = canPause;
            emit canPauseChanged(m_canPause);
        }
    } else if (propertyName == QStringLiteral("CanPlay")) {
        bool canPlay = value.toBool();
        if (m_canPlay != canPlay) {
            m_canPlay = canPlay;
            emit canPlayChanged(m_canPlay);
        }
    } else if (propertyName == QStringLiteral("CanSeek")) {
        bool canSeek = value.toBool();
        if (m_canSeek != canSeek) {
            m_canSeek = canSeek;
            emit canSeekChanged(m_canSeek);
        }
    } else if (propertyName == QStringLiteral("LoopStatus")) {
        QString loopStatus = value.toString();
        if (m_loopStatus != loopStatus) {
            m_loopStatus = loopStatus;
            emit loopStatusChanged(m_loopStatus);
        }
    } else if (propertyName == QStringLiteral("MaximumRate")) {
        bool maximumRate = value.toDouble();
        if (m_maximumRate != maximumRate) {
            m_maximumRate = maximumRate;
            emit maximumRateChanged(m_maximumRate);
        }
    } else if (propertyName == QStringLiteral("Metadata")) {
        QVariantMap metadata = value.toMap();
        if (m_metadata != metadata) {
            m_metadata = metadata;
            emit metadataChanged(m_metadata);
        }
    } else if (propertyName == QStringLiteral("MinimumRate")) {
        double minimumRate = value.toDouble();
        if (m_minimumRate != minimumRate) {
            m_minimumRate = minimumRate;
            emit minimumRateChanged(m_minimumRate);
        }
    } else if (propertyName == QStringLiteral("PlaybackStatus")) {
        QString playbackStatus = value.toString();
        if (m_playbackStatus != playbackStatus) {
            m_playbackStatus = playbackStatus;
            emit playbackStatusChanged(m_playbackStatus);
        }
    } else if (propertyName == QStringLiteral("Position")) {
        qlonglong position = value.toLongLong();
        if (m_position != position) {
            m_position = position;
            emit positionChanged(m_position);
        }
    } else if (propertyName == QStringLiteral("Rate")) {
        double rate = value.toDouble();
        if (m_rate != rate) {
            m_rate = rate;
            emit rateChanged(m_rate);
        }
    } else if (propertyName == QStringLiteral("Shuffle")) {
        bool shuffle = value.toBool();
        if (m_shuffle != shuffle) {
            m_shuffle = shuffle;
            emit shuffleChanged(m_shuffle);
        }
    } else if (propertyName == QStringLiteral("Volume")) {
        double volume = value.toDouble();
        if (m_volume != volume) {
            m_volume = volume;
            emit volumeChanged(m_volume);
        }
    } else {
        qWarning() << Q_FUNC_INFO
                   << "Received PropertyChanged signal from unknown property: "
                   << propertyName;
    }
}

