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


#include "mprisplayer.h"

#include "mprisplayer_p.h"

#include <qqmlinfo.h>

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusPendingCall>
#include <QDBusPendingCallWatcher>
#include <QDBusReply>

static const QString serviceNamePrefix = QStringLiteral("org.mpris.MediaPlayer2.");
static const QString mprisObjectPath = QStringLiteral("/org/mpris/MediaPlayer2");
static const QString dBusPropertiesInterface = QStringLiteral("org.freedesktop.DBus.Properties");
static const QString dBusPropertiesChangedSignal = QStringLiteral("PropertiesChanged");


MprisPlayer::MprisPlayer(QObject *parent)
    : QObject(parent)
    , QDBusContext()
    , m_mprisRootAdaptor(new MprisRootAdaptor(this))
    , m_mprisPlayerAdaptor(new MprisPlayerAdaptor(this))
    , m_canQuit(false)
    , m_canRaise(false)
    , m_canSetFullscreen(false)
    , m_fullscreen(false)
    , m_hasTrackList(false)
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
    QDBusConnection connection = QDBusConnection::sessionBus();

    if (!connection.isConnected()) {
        qmlInfo(this) << "Failed attempting to connect to DBus";
    } else if (!connection.registerObject(mprisObjectPath, this)) {
        qmlInfo(this) << "Failed attempting to register object path. Already registered?";
    }
}

MprisPlayer::~MprisPlayer()
{
    unregisterService();
}

QString MprisPlayer::serviceName() const
{
    return m_serviceName;
}

void MprisPlayer::setServiceName(const QString &serviceName)
{
    if (m_serviceName == serviceName) {
        return;
    }

    unregisterService();
    m_serviceName = serviceName;
    registerService();

    emit serviceNameChanged();
}


// Mpris2 Root Interface
bool MprisPlayer::canQuit() const
{
    return m_canQuit;
}

void MprisPlayer::setCanQuit(bool canQuit)
{
    if (m_canQuit == canQuit) {
        return;
    }

    m_canQuit = canQuit;
    emit canQuitChanged();
}

bool MprisPlayer::canRaise() const
{
    return m_canRaise;
}

void MprisPlayer::setCanRaise(bool canRaise)
{
    if (m_canRaise == canRaise) {
        return;
    }

    m_canRaise = canRaise;
    emit canRaiseChanged();
}

bool MprisPlayer::canSetFullscreen() const
{
    return m_canSetFullscreen;
}

void MprisPlayer::setCanSetFullscreen(bool canSetFullscreen)
{
    if (m_canSetFullscreen == canSetFullscreen) {
        return;
    }

    m_canSetFullscreen = canSetFullscreen;
    emit canSetFullscreenChanged();
}

QString MprisPlayer::desktopEntry() const
{
    return m_desktopEntry;
}

void MprisPlayer::setDesktopEntry(const QString &desktopEntry)
{
    if (m_desktopEntry == desktopEntry) {
        return;
    }

    m_desktopEntry = desktopEntry;
    emit desktopEntryChanged();
}

bool MprisPlayer::fullscreen() const
{
    return m_fullscreen;
}

void MprisPlayer::setFullscreen(bool fullscreen)
{
    if (m_fullscreen == fullscreen) {
        return;
    }

    m_fullscreen = fullscreen;
    emit fullscreenChanged();
}

bool MprisPlayer::hasTrackList() const
{
    return m_hasTrackList;
}

void MprisPlayer::setHasTrackList(bool hasTrackList)
{
    if (m_hasTrackList == hasTrackList) {
        return;
    }

    m_hasTrackList = hasTrackList;
    emit hasTrackListChanged();
}

QString MprisPlayer::identity() const
{
    return m_identity;
}

void MprisPlayer::setIdentity(const QString &identity)
{
    if (m_identity == identity) {
        return;
    }

    m_identity = identity;
    emit identityChanged();
}

QStringList MprisPlayer::supportedUriSchemes() const
{
    return m_supportedUriSchemes;
}

void MprisPlayer::setSupportedUriSchemes(const QStringList &supportedUriSchemes)
{
    if (m_supportedUriSchemes == supportedUriSchemes) {
        return;
    }

    m_supportedUriSchemes = supportedUriSchemes;
    emit supportedUriSchemesChanged();
}

QStringList MprisPlayer::supportedMimeTypes() const
{
    return m_supportedMimeTypes;
}

void MprisPlayer::setSupportedMimeTypes(const QStringList &supportedMimeTypes)
{
    if (m_supportedMimeTypes == supportedMimeTypes) {
        return;
    }

    m_supportedMimeTypes = supportedMimeTypes;
    emit supportedMimeTypesChanged();
}

// Mpris2 Player Interface
bool MprisPlayer::canControl() const
{
    return m_canControl;
}

void MprisPlayer::setCanControl(bool canControl)
{
    if (m_canControl == canControl) {
        return;
    }

    m_canControl = canControl;
    emit canControlChanged();
}

bool MprisPlayer::canGoNext() const
{
    return m_canGoNext;
}

void MprisPlayer::setCanGoNext(bool canGoNext)
{
    if (m_canGoNext == canGoNext) {
        return;
    }

    m_canGoNext = canGoNext;
    emit canGoNextChanged();
}

bool MprisPlayer::canGoPrevious() const
{
    return m_canGoPrevious;
}

void MprisPlayer::setCanGoPrevious(bool canGoPrevious)
{
    if (m_canGoPrevious == canGoPrevious) {
        return;
    }

    m_canGoPrevious = canGoPrevious;
    emit canGoPreviousChanged();
}

bool MprisPlayer::canPause() const
{
    return m_canPause;
}

void MprisPlayer::setCanPause(bool canPause)
{
    if (m_canPause == canPause) {
        return;
    }

    m_canPause = canPause;
    emit canPauseChanged();
}

bool MprisPlayer::canPlay() const
{
    return m_canPlay;
}

void MprisPlayer::setCanPlay(bool canPlay)
{
    if (m_canPlay == canPlay) {
        return;
    }

    m_canPlay = canPlay;
    emit canPlayChanged();
}

bool MprisPlayer::canSeek() const
{
    return m_canSeek;
}

void MprisPlayer::setCanSeek(bool canSeek)
{
    if (m_canSeek == canSeek) {
        return;
    }

    m_canSeek = canSeek;
    emit canSeekChanged();
}

Mpris::LoopStatus MprisPlayer::loopStatus() const
{
    return m_loopStatus;
}

void MprisPlayer::setLoopStatus(Mpris::LoopStatus loopStatus)
{
    if (m_loopStatus == loopStatus) {
        return;
    }

    m_loopStatus = loopStatus;
    emit loopStatusChanged();
}

double MprisPlayer::maximumRate() const
{
    return m_maximumRate;
}

void MprisPlayer::setMaximumRate(double maximumRate)
{
    if (m_maximumRate == maximumRate) {
        return;
    }

    m_maximumRate = maximumRate;
    emit maximumRateChanged();
}

QVariantMap MprisPlayer::metadata() const
{
    return m_typedMetadata;
}

void MprisPlayer::setMetadata(const QVariantMap &metadata)
{
    if (m_metadata == metadata) {
        return;
    }

    m_metadata = metadata;
    m_typedMetadata = typeMetadata(metadata);
    emit metadataChanged();
}

double MprisPlayer::minimumRate() const
{
    return m_minimumRate;
}

void MprisPlayer::setMinimumRate(double minimumRate)
{
    if (m_minimumRate == minimumRate) {
        return;
    }

    m_minimumRate = minimumRate;
    emit minimumRateChanged();
}

Mpris::PlaybackStatus MprisPlayer::playbackStatus() const
{
    return m_playbackStatus;
}

void MprisPlayer::setPlaybackStatus(Mpris::PlaybackStatus playbackStatus)
{
    if (m_playbackStatus == playbackStatus) {
        return;
    }

    m_playbackStatus = playbackStatus;
    emit playbackStatusChanged();
}

qlonglong MprisPlayer::position() const
{
    return m_position;
}

void MprisPlayer::setPosition(qlonglong position)
{
    if (m_position == position) {
        return;
    }

    m_position = position;
    emit positionChanged();
}

double MprisPlayer::rate() const
{
    return m_rate;
}

void MprisPlayer::setRate(double rate)
{
    if (m_rate == rate) {
        return;
    }

    m_rate = rate;
    emit rateChanged();
}

bool MprisPlayer::shuffle() const
{
    return m_shuffle;
}

void MprisPlayer::setShuffle(bool shuffle)
{
    if (m_shuffle == shuffle) {
        return;
    }

    m_shuffle= shuffle;
    emit shuffleChanged();
}

double MprisPlayer::volume() const
{
    return m_volume;
}

void MprisPlayer::setVolume(double volume)
{
    if (m_volume == volume) {
        return;
    }

    m_volume = volume;
    emit volumeChanged();
}


// Private

QVariantMap MprisPlayer::typeMetadata(const QVariantMap &aMetadata)
{
    QVariantMap metadata;
    QVariantMap::const_iterator i = aMetadata.constBegin();
    while (i != aMetadata.constEnd()) {
        switch (Mpris::enumerationFromString<Mpris::Metadata>(i.key())) {
        case Mpris::TrackId:
            metadata.insert(i.key(), QVariant::fromValue(QDBusObjectPath(i.value().toString())));
            break;
        case Mpris::Length:
            metadata.insert(i.key(), QVariant::fromValue(i.value().toLongLong()));
            break;
        case Mpris::ArtUrl:
        case Mpris::Url:
            metadata.insert(i.key(), QVariant::fromValue(i.value().toUrl().toString()));
            break;
        case Mpris::Album:
        case Mpris::AsText:
        case Mpris::Title:
            metadata.insert(i.key(), QVariant::fromValue(i.value().toString()));
            break;
        case Mpris::AlbumArtist:
        case Mpris::Artist:
        case Mpris::Comment:
        case Mpris::Composer:
        case Mpris::Genre:
        case Mpris::Lyricist:
            metadata.insert(i.key(), QVariant::fromValue(i.value().toStringList()));
            break;
        case Mpris::AudioBPM:
        case Mpris::DiscNumber:
        case Mpris::TrackNumber:
        case Mpris::UseCount:
            metadata.insert(i.key(), QVariant::fromValue(i.value().toInt()));
            break;
        case Mpris::AutoRating:
        case Mpris::UserRating:
            metadata.insert(i.key(), QVariant::fromValue(i.value().toFloat()));
            break;
        case Mpris::ContentCreated:
        case Mpris::FirstUsed:
        case Mpris::LastUsed:
            metadata.insert(i.key(), QVariant::fromValue(i.value().toDate().toString(Qt::ISODate)));
            break;
        case Mpris::InvalidMetadata:
            // Passing with the original type and hoping the user used
            // a type supported by DBus
            metadata.insert(i.key(), i.value());
            break;
        default:
            // Nothing to do
            break;
        }

        ++i;
    }

    return metadata;
}

void MprisPlayer::registerService()
{
    if (m_serviceName.isEmpty()) {
        qmlInfo(this) << "Failed to register service: empty service name";
        return;
    }

    QDBusConnection connection = QDBusConnection::sessionBus();

    if (!connection.isConnected()) {
        qmlInfo(this) << "Failed attempting to connect to DBus";
        return;
    }

    if (!connection.registerService(QString(serviceNamePrefix).append(m_serviceName))) {
        qmlInfo(this) << "Failed attempting to register service: " << m_serviceName << " Already taken?";
    }

    return;
}

void MprisPlayer::unregisterService()
{
    if (!m_serviceName.isEmpty()) {
        QDBusConnection connection = QDBusConnection::sessionBus();
        connection.unregisterService(QString(serviceNamePrefix).append(m_serviceName));
    }
}

void MprisPlayer::notifyPropertiesChanged(const QString& interfaceName, const QVariantMap &changedProperties, const QStringList &invalidatedProperties) const
{
    if (m_serviceName.isEmpty()) {
        return;
    }

    QDBusConnection connection = QDBusConnection::sessionBus();

    if (!connection.isConnected()) {
        qmlInfo(this) << "Failed attempting to connect to DBus";
        return;
    }

    QDBusMessage message = QDBusMessage::createSignal(mprisObjectPath,
                                                      dBusPropertiesInterface,
                                                      dBusPropertiesChangedSignal);

    QList<QVariant> arguments;
    arguments << QVariant(interfaceName) << QVariant(changedProperties) << QVariant(invalidatedProperties);
    message.setArguments(arguments);

    if (!connection.send(message)) {
        qmlInfo(this) << "Failed to send DBus property notification signal";
    }
}
