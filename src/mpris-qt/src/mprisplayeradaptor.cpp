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
 * Implementation of adaptor class MprisPlayerAdaptor
 */

static const QString mprisPlayerInterface = QStringLiteral("org.mpris.MediaPlayer2.Player");


MprisPlayerAdaptor::MprisPlayerAdaptor(MprisPlayer *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(false);

    connect(parent, SIGNAL(canControlChanged()), this, SLOT(onCanControlChanged()));
    connect(parent, SIGNAL(canGoNextChanged()), this, SLOT(onCanGoNextChanged()));
    connect(parent, SIGNAL(canGoPreviousChanged()), this, SLOT(onCanGoPreviousChanged()));
    connect(parent, SIGNAL(canPauseChanged()), this, SLOT(onCanPauseChanged()));
    connect(parent, SIGNAL(canPlayChanged()), this, SLOT(onCanPlayChanged()));
    connect(parent, SIGNAL(canSeekChanged()), this, SLOT(onCanSeekChanged()));
    connect(parent, SIGNAL(loopStatusChanged()), this, SLOT(onLoopStatusChanged()));
    connect(parent, SIGNAL(maximumRateChanged()), this, SLOT(onMaximumRateChanged()));
    connect(parent, SIGNAL(metadataChanged()), this, SLOT(onMetadataChanged()));
    connect(parent, SIGNAL(minimumRateChanged()), this, SLOT(onMinimumRateChanged()));
    connect(parent, SIGNAL(playbackStatusChanged()), this, SLOT(onPlaybackStatusChanged()));
    // PositionChanged signal is not forwarded through DBus ...
    connect(parent, SIGNAL(rateChanged()), this, SLOT(onRateChanged()));
    connect(parent, SIGNAL(shuffleChanged()), this, SLOT(onShuffleChanged()));
    connect(parent, SIGNAL(volumeChanged()), this, SLOT(onVolumeChanged()));
    connect(parent, SIGNAL(seeked(qlonglong)), this, SIGNAL(Seeked(qlonglong)));
}

MprisPlayerAdaptor::~MprisPlayerAdaptor()
{
    // destructor
}

bool MprisPlayerAdaptor::canControl() const
{
    return static_cast<MprisPlayer *>(parent())->canControl();
}

bool MprisPlayerAdaptor::canGoNext() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());
    return player->canControl() && player->canGoNext();
}

bool MprisPlayerAdaptor::canGoPrevious() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());
    return player->canControl() && player->canGoPrevious();
}

bool MprisPlayerAdaptor::canPause() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());
    return player->canControl() && player->canPause();
}

bool MprisPlayerAdaptor::canPlay() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());
    return player->canControl() && player->canPlay();
}

bool MprisPlayerAdaptor::canSeek() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());
    return player->canControl() && player->canSeek();
}

QString MprisPlayerAdaptor::loopStatus() const
{
    return Mpris::enumerationToString(static_cast<MprisPlayer *>(parent())->loopStatus());
}

void MprisPlayerAdaptor::setLoopStatus(const QString &value)
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());
    if (player->canControl()) {
        emit player->loopStatusRequested(Mpris::enumerationFromString<Mpris::LoopStatus>(value));
        return;
    }

    // We cannot send an error reply in a property setter so we just
    // complain here
    qDebug() << Q_FUNC_INFO << "Requested to modify the loop status but it is not supported.";
}

double MprisPlayerAdaptor::maximumRate() const
{
    return static_cast<MprisPlayer *>(parent())->maximumRate();
}

QVariantMap MprisPlayerAdaptor::metadata() const
{
    return static_cast<MprisPlayer *>(parent())->metadata();
}

double MprisPlayerAdaptor::minimumRate() const
{
    return static_cast<MprisPlayer *>(parent())->minimumRate();
}

QString MprisPlayerAdaptor::playbackStatus() const
{
    return Mpris::enumerationToString(static_cast<MprisPlayer *>(parent())->playbackStatus());
}

qlonglong MprisPlayerAdaptor::position() const
{
    return static_cast<MprisPlayer *>(parent())->position();
}

double MprisPlayerAdaptor::rate() const
{
    return static_cast<MprisPlayer *>(parent())->rate();
}

void MprisPlayerAdaptor::setRate(double value)
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());
    QString errorMessage;

    if (!player->canControl()) {
        errorMessage = QStringLiteral("Requested to modify the rate but it is not supported.");
    } else if (player->minimumRate() > value) {
        errorMessage = QStringLiteral("Requested to modify the rate under its minimum allowed value.");
    } else if (player->maximumRate() < value) {
        errorMessage = QStringLiteral("Requested to modify the rate over its maximum allowed value.");
    }

    if (errorMessage.isEmpty()) {
        if (value == 0) {
            emit player->pauseRequested();
        } else {
            emit player->rateRequested(value);
        }
        return;
    }

    // We cannot send an error reply in a property setter so we just
    // complain here
    qDebug() << Q_FUNC_INFO << errorMessage;
}

bool MprisPlayerAdaptor::shuffle() const
{
    return static_cast<MprisPlayer *>(parent())->shuffle();
}

void MprisPlayerAdaptor::setShuffle(bool value)
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());
    if (player->canControl()) {
        emit player->shuffleRequested(value);
        return;
    }

    // We cannot send an error reply in a property setter so we just
    // complain here
    qDebug() << Q_FUNC_INFO << "Requested to modify the shuffle but it is not supported.";
}

double MprisPlayerAdaptor::volume() const
{
    return static_cast<MprisPlayer *>(parent())->volume();
}

void MprisPlayerAdaptor::setVolume(double value)
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());
    if (player->canControl()) {
        emit player->volumeRequested(value < 0 ? 0 : value);
        return;
    }

    // We cannot send an error reply in a property setter so we just
    // complain here
    qDebug() << Q_FUNC_INFO << "Requested to modify the volume but it is not supported.";
}

void MprisPlayerAdaptor::Next()
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());
    if (!player->canControl()) {
        player->sendErrorReply(QDBusError::NotSupported, QStringLiteral("Wanted to move to next track but it is not supported."));
        return;
    }

    if (!player->canGoNext()) {
        return;
    }

    emit player->nextRequested();
}

void MprisPlayerAdaptor::OpenUri(const QString &Uri)
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());
    if (!player->canControl()) {
        player->sendErrorReply(QDBusError::NotSupported, QStringLiteral("Wanted to open an url but it is not supported."));
        return;
    }

    QUrl url(Uri, QUrl::StrictMode);
    if (!url.isValid()) {
        player->sendErrorReply(QDBusError::InvalidArgs, QStringLiteral("Wanted to open an url but the url is invalid."));
    }

    if (!player->supportedUriSchemes().contains(url.scheme())) {
        player->sendErrorReply(QDBusError::NotSupported, QStringLiteral("Wanted to open an url but the scheme is not supported."));
    }

    QMimeDatabase db;
    QMimeType mime;
    if (url.isLocalFile()) {
        mime = db.mimeTypeForFile(url.toLocalFile());
    } else {
        mime = db.mimeTypeForFile(url.fileName(), QMimeDatabase::MatchExtension);
    }
    QStringList mimeNames = mime.aliases();
    mimeNames.prepend(mime.name());
    for (int i = 0; i < mimeNames.size(); i++) {
        if (player->supportedMimeTypes().contains(mimeNames[i])) {
            emit player->openUriRequested(url);
            return;
        }
    }

    player->sendErrorReply(QDBusError::NotSupported, QStringLiteral("Wanted to open an url but the mime type is not supported."));
}

void MprisPlayerAdaptor::Pause()
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());
    if (!player->canControl()) {
        player->sendErrorReply(QDBusError::NotSupported, QStringLiteral("Wanted to pause but it is not supported."));
        return;
    }

    if (!player->canPause()) {
        return;
    }

    switch (player->playbackStatus()) {
    case Mpris::Playing:
    case Mpris::Stopped:
        emit player->pauseRequested();
        break;
    case Mpris::Paused:
    default:
        // Nothing to do
        break;
    }
}

void MprisPlayerAdaptor::Play()
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());
    if (!player->canControl()) {
        player->sendErrorReply(QDBusError::NotSupported, QStringLiteral("Wanted to play but it is not supported."));
        return;
    }

    if (!player->canPlay()) {
        return;
    }

    switch (player->playbackStatus()) {
    case Mpris::Stopped:
    case Mpris::Paused:
        emit player->playRequested();
        break;
    case Mpris::Playing:
    default:
        // Nothing to do
        break;
    }
}

void MprisPlayerAdaptor::PlayPause()
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());
    if (!player->canControl()) {
        player->sendErrorReply(QDBusError::NotSupported, QStringLiteral("Wanted to play or pause but it is not supported."));
        return;
    }

    switch (player->playbackStatus()) {
    case Mpris::Playing:
        if (!player->canPause()) {
            player->sendErrorReply(QDBusError::NotSupported, QStringLiteral("Wanted to pause but it is not supported."));
            return;
        }

        emit player->pauseRequested();
        break;
    case Mpris::Stopped:
    case Mpris::Paused:
        if (!player->canPlay()) {
            player->sendErrorReply(QDBusError::NotSupported, QStringLiteral("Wanted to play but it is not supported."));
            return;
        }

        emit player->playRequested();
        break;
    default:
        // Nothing to do
        break;
    }
}

void MprisPlayerAdaptor::Previous()
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());
    if (!player->canControl()) {
        player->sendErrorReply(QDBusError::NotSupported, QStringLiteral("Wanted to move to previous track but it is not supported."));
        return;
    }

    if (!player->canGoPrevious()) {
        return;
    }

    emit player->previousRequested();
}

void MprisPlayerAdaptor::Seek(qlonglong Offset)
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());
    if (!player->canControl()) {
        player->sendErrorReply(QDBusError::NotSupported, QStringLiteral("Wanted to seek but it is not supported."));
        return;
    }

    if (!player->canSeek() || Offset == 0) {
        return;
    }

    if (Offset < 0) {
        emit player->seekRequested(Offset);
        return;
    }

    QVariantMap metadata = player->metadata();
    QVariant length = metadata[Mpris::metadataToString(Mpris::Length)];
    if (length.isValid() && (player->position() + Offset) > length.toLongLong()) {
        emit player->nextRequested();
        return;
    }

    emit player->seekRequested(Offset);
}

void MprisPlayerAdaptor::SetPosition(const QDBusObjectPath &TrackId, qlonglong Position)
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());
    if (!player->canControl()) {
        player->sendErrorReply(QDBusError::NotSupported, QStringLiteral("Wanted to move to position but it is not supported."));
        return;
    }

    if (!player->canSeek()) {
        return;
    }

    QVariantMap metadata = player->metadata();
    QVariant trackId = metadata[Mpris::metadataToString(Mpris::TrackId)];
    QVariant length = metadata[Mpris::metadataToString(Mpris::Length)];
    if (!trackId.isValid() || !length.isValid()) {
        return;
    }

    if (trackId.value<QDBusObjectPath>() != TrackId) {
        player->sendErrorReply(QDBusError::InvalidArgs, QStringLiteral("Wanted to move to position but the TrackId is not the current one."));
        return;
    }

    if (Position > length.toLongLong()) {
        player->sendErrorReply(QDBusError::InvalidArgs, QStringLiteral("Wanted to move to position but the position is off range."));
        return;
    }

    emit player->setPositionRequested(TrackId, Position);
}

void MprisPlayerAdaptor::Stop()
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());
    if (!player->canControl()) {
        player->sendErrorReply(QDBusError::NotSupported, QStringLiteral("Wanted to stop but it is not supported."));
        return;
    }

    switch (player->playbackStatus()) {
    case Mpris::Playing:
    case Mpris::Paused:
        emit player->stopRequested();
        break;
    case Mpris::Stopped:
    default:
        // Nothing to do
        break;
    }
}


// Private
void MprisPlayerAdaptor::onCanControlChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    // canControlChanged signal is not forwarded through DBus, but
    // we can notify about the rest of "Can*" properties

    QVariantMap changedProperties;
    if (player->canGoNext()) {
        changedProperties[QStringLiteral("CanGoNext")] = QVariant(player->canControl());
    }
    if (player->canGoPrevious()) {
        changedProperties[QStringLiteral("CanGoPrevious")] = QVariant(player->canControl());
    }
    if (player->canPause()) {
        changedProperties[QStringLiteral("CanPause")] = QVariant(player->canControl());
    }
    if (player->canPlay()) {
        changedProperties[QStringLiteral("CanPlay")] = QVariant(player->canControl());
    }
    if (player->canSeek()) {
        changedProperties[QStringLiteral("CanSeek")] = QVariant(player->canControl());
    }

    if (changedProperties.isEmpty()) {
        return;
    }

    player->notifyPropertiesChanged(mprisPlayerInterface, changedProperties, QStringList());
}

void MprisPlayerAdaptor::onCanGoNextChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    if (!player->canControl()) {
        return;
    }

    QVariantMap changedProperties;
    changedProperties[QStringLiteral("CanGoNext")] = QVariant(player->canGoNext());

    player->notifyPropertiesChanged(mprisPlayerInterface, changedProperties, QStringList());
}

void MprisPlayerAdaptor::onCanGoPreviousChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    if (!player->canControl()) {
        return;
    }

    QVariantMap changedProperties;
    changedProperties[QStringLiteral("CanGoPrevious")] = QVariant(player->canGoPrevious());

    player->notifyPropertiesChanged(mprisPlayerInterface, changedProperties, QStringList());
}

void MprisPlayerAdaptor::onCanPauseChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    if (!player->canControl()) {
        return;
    }

    QVariantMap changedProperties;
    changedProperties[QStringLiteral("CanPause")] = QVariant(player->canPause());

    player->notifyPropertiesChanged(mprisPlayerInterface, changedProperties, QStringList());
}

void MprisPlayerAdaptor::onCanPlayChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    if (!player->canControl()) {
        return;
    }

    QVariantMap changedProperties;
    changedProperties[QStringLiteral("CanPlay")] = QVariant(player->canPlay());

    player->notifyPropertiesChanged(mprisPlayerInterface, changedProperties, QStringList());
}

void MprisPlayerAdaptor::onCanSeekChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    if (!player->canControl()) {
        return;
    }

    QVariantMap changedProperties;
    changedProperties[QStringLiteral("CanSeek")] = QVariant(player->canSeek());

    player->notifyPropertiesChanged(mprisPlayerInterface, changedProperties, QStringList());
}

void MprisPlayerAdaptor::onLoopStatusChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    QVariantMap changedProperties;
    changedProperties[QStringLiteral("LoopStatus")] = QVariant(Mpris::enumerationToString(player->loopStatus()));

    player->notifyPropertiesChanged(mprisPlayerInterface, changedProperties, QStringList());
}

void MprisPlayerAdaptor::onMaximumRateChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    if (player->maximumRate() < 1) {
        qmlInfo(this) << "Maximum rate should be equal or above 1";
        return;
    }

    QVariantMap changedProperties;
    changedProperties[QStringLiteral("MaximumRate")] = QVariant(player->maximumRate());

    player->notifyPropertiesChanged(mprisPlayerInterface, changedProperties, QStringList());
}

void MprisPlayerAdaptor::onMetadataChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    QVariantMap changedProperties;
    changedProperties[QStringLiteral("Metadata")] = QVariant(player->metadata());

    player->notifyPropertiesChanged(mprisPlayerInterface, changedProperties, QStringList());
}

void MprisPlayerAdaptor::onMinimumRateChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    if (player->minimumRate() > 1) {
        qmlInfo(this) << "Minimum rate should be equal or less than 1";
        return;
    }

    QVariantMap changedProperties;
    changedProperties[QStringLiteral("MinimumRate")] = QVariant(player->minimumRate() < 0 ? 0 : player->minimumRate());

    player->notifyPropertiesChanged(mprisPlayerInterface, changedProperties, QStringList());
}

void MprisPlayerAdaptor::onPlaybackStatusChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    QVariantMap changedProperties;
    changedProperties[QStringLiteral("PlaybackStatus")] = QVariant(Mpris::enumerationToString(player->playbackStatus()));

    player->notifyPropertiesChanged(mprisPlayerInterface, changedProperties, QStringList());
}

void MprisPlayerAdaptor::onRateChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    if (player->rate() <= 0 || player->rate() < player->minimumRate() || player->rate() > player->maximumRate()) {
        qmlInfo(this) << "Rate should never be negative or out of the minimum and maximum limits";
        return;
    }

    QVariantMap changedProperties;
    changedProperties[QStringLiteral("Rate")] = QVariant(player->rate());

    player->notifyPropertiesChanged(mprisPlayerInterface, changedProperties, QStringList());
}

void MprisPlayerAdaptor::onShuffleChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    QVariantMap changedProperties;
    changedProperties[QStringLiteral("Shuffle")] = QVariant(player->shuffle());

    player->notifyPropertiesChanged(mprisPlayerInterface, changedProperties, QStringList());
}

void MprisPlayerAdaptor::onVolumeChanged() const
{
    MprisPlayer * const player = static_cast<MprisPlayer *>(parent());

    QVariantMap changedProperties;
    changedProperties[QStringLiteral("Volume")] = QVariant(player->volume() < 0 ? 0 : player->volume());

    player->notifyPropertiesChanged(mprisPlayerInterface, changedProperties, QStringList());
}
