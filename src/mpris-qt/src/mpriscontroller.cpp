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


#include "mpriscontroller.h"

#include "mpriscontroller_p.h"

#include <QDBusConnection>
#include <QDBusPendingReply>
#include <QDBusPendingCallWatcher>

static const QString mprisObjectPath = QStringLiteral("/org/mpris/MediaPlayer2");

MprisController::MprisController(const QString &service, const QDBusConnection &connection, QObject *parent)
    : QObject(parent)
    , m_mprisRootInterface(new MprisRootInterface(service, mprisObjectPath, connection, this))
    , m_mprisPlayerInterface(new MprisPlayerInterface(service, mprisObjectPath, connection, this))
    , m_initedRootInterface(false)
    , m_initedPlayerInterface(false)
    , m_requestedPosition(false)
    , m_canControlReceived(false)
{
    // Mpris Root Interface
    connect(m_mprisRootInterface, &MprisRootInterface::asyncGetAllPropertiesFinished, this, &MprisController::onAsyncGetAllRootPropertiesFinished);
    connect(m_mprisRootInterface, &MprisRootInterface::canQuitChanged, this, &MprisController::canQuitChanged);
    connect(m_mprisRootInterface, &MprisRootInterface::canRaiseChanged, this, &MprisController::canRaiseChanged);
    connect(m_mprisRootInterface, &MprisRootInterface::canSetFullscreenChanged, this, &MprisController::canSetFullscreenChanged);
    connect(m_mprisRootInterface, &MprisRootInterface::desktopEntryChanged, this, &MprisController::desktopEntryChanged);
    connect(m_mprisRootInterface, &MprisRootInterface::fullscreenChanged, this, &MprisController::fullscreenChanged);
    connect(m_mprisRootInterface, &MprisRootInterface::hasTrackListChanged, this, &MprisController::hasTrackListChanged);
    connect(m_mprisRootInterface, &MprisRootInterface::identityChanged, this, &MprisController::identityChanged);
    connect(m_mprisRootInterface, &MprisRootInterface::supportedMimeTypesChanged, this, &MprisController::supportedMimeTypesChanged);
    connect(m_mprisRootInterface, &MprisRootInterface::supportedUriSchemesChanged, this, &MprisController::supportedUriSchemesChanged);
    m_mprisRootInterface->setUseCache(true);

    // Mpris Player Interface
    connect(m_mprisPlayerInterface, &MprisPlayerInterface::asyncGetAllPropertiesFinished, this, &MprisController::onAsyncGetAllPlayerPropertiesFinished);
    connect(m_mprisPlayerInterface, &MprisPlayerInterface::canControlChanged, this, &MprisController::onCanControlChanged);
    connect(m_mprisPlayerInterface, &MprisPlayerInterface::canGoNextChanged, this, &MprisController::canGoNextChanged);
    connect(m_mprisPlayerInterface, &MprisPlayerInterface::canGoPreviousChanged, this, &MprisController::canGoPreviousChanged);
    connect(m_mprisPlayerInterface, &MprisPlayerInterface::canPauseChanged, this, &MprisController::canPauseChanged);
    connect(m_mprisPlayerInterface, &MprisPlayerInterface::canPlayChanged, this, &MprisController::canPlayChanged);
    connect(m_mprisPlayerInterface, &MprisPlayerInterface::canSeekChanged, this, &MprisController::canSeekChanged);
    connect(m_mprisPlayerInterface, &MprisPlayerInterface::canSeekChanged, this, &MprisController::canSeekChanged);
    connect(m_mprisPlayerInterface, &MprisPlayerInterface::loopStatusChanged, this, &MprisController::loopStatusChanged);
    connect(m_mprisPlayerInterface, &MprisPlayerInterface::maximumRateChanged, this, &MprisController::maximumRateChanged);
    connect(m_mprisPlayerInterface, &MprisPlayerInterface::metadataChanged, this, &MprisController::metadataChanged);
    connect(m_mprisPlayerInterface, &MprisPlayerInterface::minimumRateChanged, this, &MprisController::minimumRateChanged);
    connect(m_mprisPlayerInterface, &MprisPlayerInterface::playbackStatusChanged, this, &MprisController::playbackStatusChanged);
    connect(m_mprisPlayerInterface, &MprisPlayerInterface::positionChanged, this, &MprisController::onPositionChanged);
    connect(m_mprisPlayerInterface, &MprisPlayerInterface::rateChanged, this, &MprisController::rateChanged);
    connect(m_mprisPlayerInterface, &MprisPlayerInterface::shuffleChanged, this, &MprisController::shuffleChanged);
    connect(m_mprisPlayerInterface, &MprisPlayerInterface::volumeChanged, this, &MprisController::volumeChanged);
    connect(m_mprisPlayerInterface, &MprisPlayerInterface::seeked, this, &MprisController::seeked);
    m_mprisPlayerInterface->setUseCache(true);

    // This will initialize the properties, if needed
    isValid();
}

MprisController::~MprisController()
{
}

bool MprisController::isValid() const
{
    if (!m_mprisRootInterface->isValid() || !m_mprisPlayerInterface->isValid()) {
        m_initedRootInterface = false;
        m_initedPlayerInterface = false;
        return false;
    }

    if (!m_initedRootInterface) {
        m_mprisRootInterface->getAllProperties();
        if (m_mprisRootInterface->lastExtendedError().isValid()) {
            qWarning() << Q_FUNC_INFO
                       << "Error" << m_mprisRootInterface->lastExtendedError().name()
                       << "happened:" << m_mprisRootInterface->lastExtendedError().message();
        }
    }

    if (!m_initedPlayerInterface) {
        m_mprisPlayerInterface->getAllProperties();
        if (m_mprisPlayerInterface->lastExtendedError().isValid()) {
            qWarning() << Q_FUNC_INFO
                       << "Error" << m_mprisPlayerInterface->lastExtendedError().name()
                       << "happened:" << m_mprisPlayerInterface->lastExtendedError().message();
        }
    }

    return m_initedRootInterface && m_initedPlayerInterface;
}

// Mpris2 Root Interface
bool MprisController::quit()
{
    if (!canQuit()) {
        qDebug() << Q_FUNC_INFO << "The method is not allowed";
        return false;
    }

    QDBusPendingReply<> reply = m_mprisRootInterface->Quit();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                     this, SLOT(onFinishedPendingCall(QDBusPendingCallWatcher*)));

    return true;
}

bool MprisController::raise()
{
    if (!canRaise()) {
        qDebug() << Q_FUNC_INFO << "The method is not allowed";
        return false;
    }

    QDBusPendingReply<> reply = m_mprisRootInterface->Raise();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                     this, SLOT(onFinishedPendingCall(QDBusPendingCallWatcher*)));

    return true;
}

// Mpris2 Player Interface
bool MprisController::next()
{
    if (!canGoNext()) {
        qDebug() << Q_FUNC_INFO << "The method is not allowed";
        return false;
    }

    QDBusPendingReply<> reply = m_mprisPlayerInterface->Next();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                     this, SLOT(onFinishedPendingCall(QDBusPendingCallWatcher*)));

    return true;
}

bool MprisController::openUri(const QUrl &uri)
{
    if (!canControl()) {
        qDebug() << Q_FUNC_INFO << "The method is not allowed";
        return false;
    }

    if (!uri.isValid()) {
        qDebug() << Q_FUNC_INFO << "The uri is invalid";
        return false;
    }

    if (!m_mprisRootInterface->supportedUriSchemes().contains(uri.scheme())) {
        qDebug() << Q_FUNC_INFO << "The scheme is not supported";
        return false;
    }

    QMimeDatabase db;
    QMimeType mime;
    if (uri.isLocalFile()) {
        mime = db.mimeTypeForFile(uri.toLocalFile());
    } else {
        mime = db.mimeTypeForFile(uri.fileName(), QMimeDatabase::MatchExtension);
    }
    QStringList mimeNames = mime.aliases();
    mimeNames.prepend(mime.name());
    for (int i = 0; i < mimeNames.size(); i++) {
        if (m_mprisRootInterface->supportedMimeTypes().contains(mimeNames[i])) {
            QDBusPendingReply<> reply = m_mprisPlayerInterface->OpenUri(uri.toString());
            QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
            QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                             this, SLOT(onFinishedPendingCall(QDBusPendingCallWatcher*)));
            return true;
        }
    }

    qDebug() << Q_FUNC_INFO << "The mime type is not supported";
    return false;
}

bool MprisController::pause()
{
    if (!canPause()) {
        qDebug() << Q_FUNC_INFO << "The method is not allowed";
        return false;
    }

    QDBusPendingReply<> reply = m_mprisPlayerInterface->Pause();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                     this, SLOT(onFinishedPendingCall(QDBusPendingCallWatcher*)));

    return true;
}

bool MprisController::play()
{
    if (!canPlay()) {
        qDebug() << Q_FUNC_INFO << "The method is not allowed";
        return false;
    }

    QDBusPendingReply<> reply = m_mprisPlayerInterface->Play();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                     this, SLOT(onFinishedPendingCall(QDBusPendingCallWatcher*)));

    return true;
}

bool MprisController::playPause()
{
    if (!canPause()) {
        qDebug() << Q_FUNC_INFO << "The method is not allowed";
        return false;
    }

    QDBusPendingReply<> reply = m_mprisPlayerInterface->PlayPause();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                     this, SLOT(onFinishedPendingCall(QDBusPendingCallWatcher*)));

    return true;
}

bool MprisController::previous()
{
    if (!canGoPrevious()) {
        qDebug() << Q_FUNC_INFO << "The method is not allowed";
        return false;
    }

    QDBusPendingReply<> reply = m_mprisPlayerInterface->Previous();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                     this, SLOT(onFinishedPendingCall(QDBusPendingCallWatcher*)));

    return true;
}

bool MprisController::seek(qlonglong offset)
{
    if (!canSeek()) {
        qDebug() << Q_FUNC_INFO << "The method is not allowed";
        return false;
    }

    QDBusPendingReply<> reply = m_mprisPlayerInterface->Seek(offset);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                     this, SLOT(onFinishedPendingCall(QDBusPendingCallWatcher*)));

    return true;
}

bool MprisController::setPosition(qlonglong position)
{
    if (!canSeek()) {
        qDebug() << Q_FUNC_INFO << "The method is not allowed";
        return false;
    }

    QVariant trackId = m_mprisPlayerInterface->metadata()[Mpris::metadataToString(Mpris::TrackId)];
    if (!trackId.isValid()) {
        qDebug() << Q_FUNC_INFO << "Unknown trackId in which to set the position";
        return false;
    }

    return setPosition(trackId.value<QDBusObjectPath>().path(), position);
}

bool MprisController::setPosition(const QString &aTrackId, qlonglong position)
{
    if (!canSeek()) {
        qDebug() << Q_FUNC_INFO << "The method is not allowed";
        return false;
    }

    QDBusObjectPath trackId(aTrackId);
    if (trackId.path().isEmpty()) {
        qDebug() << Q_FUNC_INFO << "trackId doesn't map to a valid DBus object path";
        return false;
    }

    QVariant length = m_mprisPlayerInterface->metadata()[Mpris::metadataToString(Mpris::Length)];
    if (!length.isValid()) {
        qlonglong reportedLength = length.toLongLong();

        if (position < 0 || position > reportedLength) {
            qDebug() << Q_FUNC_INFO << "Unknown trackId in which to set the position";
            return false;
        }
    }

    QDBusPendingReply<> reply = m_mprisPlayerInterface->SetPosition(trackId, position);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                     this, SLOT(onFinishedPendingCall(QDBusPendingCallWatcher*)));

    return true;
}

bool MprisController::stop()
{
    if (!canControl()) {
        qDebug() << Q_FUNC_INFO << "The method is not allowed";
        return false;
    }

    QDBusPendingReply<> reply = m_mprisPlayerInterface->Stop();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                     this, SLOT(onFinishedPendingCall(QDBusPendingCallWatcher*)));

    return true;
}


// Slots

QString MprisController::service() const
{
    return m_mprisRootInterface->service();
}

// Mpris2 Root Interface
bool MprisController::canQuit() const
{
    if (isValid()) {
        return m_mprisRootInterface->canQuit();
    }

    return false;
}

bool MprisController::canRaise() const
{
    if (isValid()) {
        return m_mprisRootInterface->canRaise();
    }

    return false;
}

bool MprisController::canSetFullscreen() const
{
    if (isValid()) {
        return m_mprisRootInterface->canSetFullscreen();
    }

    return false;
}

QString MprisController::desktopEntry() const
{
    if (isValid()) {
        return m_mprisRootInterface->desktopEntry();
    }

    return QString();
}

bool MprisController::fullscreen() const
{
    if (isValid()) {
        return m_mprisRootInterface->fullscreen();
    }

    return false;
}

void MprisController::setFullscreen(bool fullscreen)
{
    if (isValid()) {
        m_mprisRootInterface->setFullscreen(fullscreen);
    }
}

bool MprisController::hasTrackList() const
{
    if (isValid()) {
        return m_mprisRootInterface->hasTrackList();
    }

    return false;
}

QString MprisController::identity() const
{
    if (isValid()) {
        return m_mprisRootInterface->identity();
    }

    return QString();
}

QStringList MprisController::supportedUriSchemes() const
{
    if (isValid()) {
        return m_mprisRootInterface->supportedUriSchemes();
    }

    return QStringList();
}

QStringList MprisController::supportedMimeTypes() const
{
    if (isValid()) {
        return m_mprisRootInterface->supportedMimeTypes();
    }

    return QStringList();
}

// Mpris2 Player Interface
bool MprisController::canControl() const
{
    if (isValid()) {
        return m_mprisPlayerInterface->canControl();
    }

    return false;
}

bool MprisController::canGoNext() const
{
    if (isValid() && canControl()) {
        return m_mprisPlayerInterface->canGoNext();
    }

    return false;
}

bool MprisController::canGoPrevious() const
{
    if (isValid() && canControl()) {
        return m_mprisPlayerInterface->canGoPrevious();
    }

    return false;
}

bool MprisController::canPause() const
{
    if (isValid() && canControl()) {
        return m_mprisPlayerInterface->canPause();
    }

    return false;
}

bool MprisController::canPlay() const
{
    if (isValid() && canControl()) {
        return m_mprisPlayerInterface->canPlay();
    }

    return false;
}

bool MprisController::canSeek() const
{
    if (isValid() && canControl()) {
        return m_mprisPlayerInterface->canSeek();
    }

    return false;
}

Mpris::LoopStatus MprisController::loopStatus() const
{
    if (isValid()) {
        return Mpris::enumerationFromString<Mpris::LoopStatus>(m_mprisPlayerInterface->loopStatus());
    }

    return Mpris::None;
}

void MprisController::setLoopStatus(Mpris::LoopStatus loopStatus)
{
    if (isValid()) {
        m_mprisPlayerInterface->setLoopStatus(Mpris::enumerationToString(loopStatus));
    }
}

double MprisController::maximumRate() const
{
    if (isValid()) {
        return m_mprisPlayerInterface->maximumRate();
    }

    return 1;
}

QVariantMap MprisController::metadata() const
{
    if (isValid()) {
        return m_mprisPlayerInterface->metadata();
    }

    return QVariantMap();
}

double MprisController::minimumRate() const
{
    if (isValid()) {
        return m_mprisPlayerInterface->minimumRate();
    }

    return 1;
}

Mpris::PlaybackStatus MprisController::playbackStatus() const
{
    if (isValid()) {
        return Mpris::enumerationFromString<Mpris::PlaybackStatus>(m_mprisPlayerInterface->playbackStatus());
    }

    return Mpris::Stopped;
}

qlonglong MprisController::position() const
{
    if (isValid()) {
        m_mprisPlayerInterface->setSync(true);
        m_mprisPlayerInterface->setUseCache(false);
        qlonglong result = m_mprisPlayerInterface->position();
        m_mprisPlayerInterface->setUseCache(true);
        m_mprisPlayerInterface->setSync(false);
        return result;
    }

    return -1;
}

void MprisController::requestPosition() const
{
    if (m_requestedPosition) {
        return;
    }

    if (isValid()) {
        m_mprisPlayerInterface->setUseCache(false);
        m_mprisPlayerInterface->position();
        m_mprisPlayerInterface->setUseCache(true);
        if (m_mprisPlayerInterface->lastExtendedError().isValid()) {
            qWarning() << Q_FUNC_INFO
                       << "Failed requesting the current position in the MPRIS2 Player Interface!!!";
            return;
        }
        m_requestedPosition = true;
    }
}

double MprisController::rate() const
{
    if (isValid()) {
        return m_mprisPlayerInterface->rate();
    }

    return 1;
}

void MprisController::setRate(double rate)
{
    if (isValid()) {
        m_mprisPlayerInterface->setRate(rate);
    }
}

bool MprisController::shuffle() const
{
    if (isValid()) {
        return m_mprisPlayerInterface->shuffle();
    }

    return false;
}

void MprisController::setShuffle(bool shuffle)
{
    if (isValid()) {
        m_mprisPlayerInterface->setShuffle(shuffle);
    }
}

double MprisController::volume() const
{
    if (isValid()) {
        return m_mprisPlayerInterface->volume();
    }

    return 0;
}

void MprisController::setVolume(double volume)
{
    if (isValid()) {
        m_mprisPlayerInterface->setVolume(volume);
    }
}


// Protected

void MprisController::onAsyncGetAllRootPropertiesFinished()
{
    if (m_mprisRootInterface->lastExtendedError().isValid()) {
        qWarning() << Q_FUNC_INFO
                   << "Error" << m_mprisRootInterface->lastExtendedError().name()
                   << "happened:" << m_mprisRootInterface->lastExtendedError().message();
        return;
    }

    m_initedRootInterface = true;
}

void MprisController::onAsyncGetAllPlayerPropertiesFinished()
{
    if (m_mprisPlayerInterface->lastExtendedError().isValid()) {
        qWarning() << Q_FUNC_INFO
                   << "Error" << m_mprisPlayerInterface->lastExtendedError().name()
                   << "happened:" << m_mprisPlayerInterface->lastExtendedError().message();
        return;
    }

    m_initedPlayerInterface = true;
}

void MprisController::onCanControlChanged()
{
    // On first reception, we are using a "GetAll" so we can skip this
    if (m_canControlReceived) {
        // I could disconnect and re-connect the signals so I avoid
        // double arriving signals but this really shouldn't happen
        // ever.
        emit canGoNextChanged();
        emit canGoPreviousChanged();
        emit canPauseChanged();
        emit canPlayChanged();
        emit canSeekChanged();
        qWarning() << Q_FUNC_INFO
                   << "CanControl is not supposed to change its value!";
        return;
    }

    m_canControlReceived = true;
}

void MprisController::onPositionChanged(qlonglong aPosition)
{
    m_requestedPosition = false;
    emit positionChanged(aPosition);
}


// Private

void MprisController::onFinishedPendingCall(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<> reply = *call;
    if (reply.isError()) {
        qWarning() << Q_FUNC_INFO
                   << "Error" << reply.error().name()
                   << "happened:" << reply.error().message();
    }

    call->deleteLater();
}
