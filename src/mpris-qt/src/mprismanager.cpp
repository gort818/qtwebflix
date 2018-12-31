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


#include "mprismanager.h"

#include "mpriscontroller.h"

#include <qqmlinfo.h>

#include <QDBusConnection>
#include <QDBusConnectionInterface>

#include <QtCore/QSignalMapper>

static const QString mprisNameSpace = QStringLiteral("org.mpris.MediaPlayer2.*");
static const QString dBusService = QStringLiteral("org.freedesktop.DBus");
static const QString dBusObjectPath = QStringLiteral("/org/freedesktop/DBus");
static const QString dBusInterface = QStringLiteral("org.freedesktop.DBus");
static const QString dBusNameOwnerChangedSignal = QStringLiteral("NameOwnerChanged");


MprisManager::MprisManager(QObject *parent)
    : QObject(parent)
    , m_singleService(false)
    , m_playbackStatusMapper(new QSignalMapper(this))
{
    QDBusConnection connection = QDBusConnection::sessionBus();

    if (!connection.isConnected()) {
        qmlInfo(this) << "Failed attempting to connect to DBus";
        return;
    }

    connection.connect(dBusService, dBusObjectPath, dBusInterface, dBusNameOwnerChangedSignal,
                       QStringList(), QString(),
                       this, SLOT(onNameOwnerChanged(QString, QString, QString)));

    QStringList serviceNames = connection.interface()->registeredServiceNames();
    QStringList::const_iterator i = serviceNames.constBegin();
    while (i != serviceNames.constEnd()) {
        QRegExp rx(mprisNameSpace);
        rx.setPatternSyntax(QRegExp::Wildcard);
        if (rx.exactMatch(*i)) {
            onServiceAppeared(*i);
        }

        ++i;
    }
}

MprisManager::~MprisManager()
{
}

// Mpris2 Root Interface
bool MprisManager::quit() const
{
    return checkController(Q_FUNC_INFO) && m_currentController->quit();
}

bool MprisManager::raise() const
{
    return checkController(Q_FUNC_INFO) && m_currentController->raise();
}

// Mpris2 Player Interface
bool MprisManager::next() const
{
    return checkController(Q_FUNC_INFO) && m_currentController->next();
}

bool MprisManager::openUri(const QUrl &uri) const
{
    return checkController(Q_FUNC_INFO) && m_currentController->openUri(uri);
}

bool MprisManager::pause() const
{
    return checkController(Q_FUNC_INFO) && m_currentController->pause();
}

bool MprisManager::play() const
{
    return checkController(Q_FUNC_INFO) && m_currentController->play();
}

bool MprisManager::playPause() const
{
    return checkController(Q_FUNC_INFO) && m_currentController->playPause();
}

bool MprisManager::previous() const
{
    return checkController(Q_FUNC_INFO) && m_currentController->previous();
}

bool MprisManager::seek(qlonglong offset) const
{
    return checkController(Q_FUNC_INFO) && m_currentController->seek(offset);
}

bool MprisManager::setPosition(qlonglong position) const
{
    return checkController(Q_FUNC_INFO) && m_currentController->setPosition(position);
}

bool MprisManager::setPosition(const QString &trackId, qlonglong position) const
{
    return checkController(Q_FUNC_INFO) && m_currentController->setPosition(trackId, position);
}

bool MprisManager::stop() const
{
    return checkController(Q_FUNC_INFO) && m_currentController->stop();
}


// Slots

bool MprisManager::singleService() const
{
    return m_singleService;
}

void MprisManager::setSingleService(bool single)
{
    if (m_singleService == single) {
        return;
    }

    m_singleService = single;
    emit singleServiceChanged();
}

QString MprisManager::currentService() const
{
    return m_currentController.isNull() ? QString() : m_currentController->service();
}

void MprisManager::setCurrentService(const QString &service)
{
    if (!m_currentController.isNull() && m_currentController->service() == service) {
        return;
    }

    QRegExp rx(mprisNameSpace);
    rx.setPatternSyntax(QRegExp::Wildcard);
    if (!rx.exactMatch(service)) {
        qmlInfo(this) << service << "is not a proper Mpris2 service";
        return;
    }

    QSharedPointer<MprisController> controller = availableController(service);
    if (controller.isNull()) {
        controller = QSharedPointer<MprisController>(new MprisController(service, QDBusConnection::sessionBus(), this));
    } else {
        m_availableControllers.move(m_availableControllers.indexOf(controller), 0);
    }

    setCurrentController(controller);
}

QStringList MprisManager::availableServices() const
{
    QStringList result;

    QList< QSharedPointer<MprisController> >::const_iterator i = m_availableControllers.constBegin();
    while (i != m_availableControllers.constEnd()) {
        const QSharedPointer<MprisController> controller = *i;
        result.append(controller->service());

        ++i;
    }

    return result;
}

// Mpris2 Root Interface
bool MprisManager::canQuit() const
{
    return checkController(Q_FUNC_INFO) && m_currentController->canQuit();
}

bool MprisManager::canRaise() const
{
    return checkController(Q_FUNC_INFO) && m_currentController->canRaise();
}

bool MprisManager::canSetFullscreen() const
{
    return checkController(Q_FUNC_INFO) && m_currentController->canSetFullscreen();
}

QString MprisManager::desktopEntry() const
{
    return checkController(Q_FUNC_INFO) ? m_currentController->desktopEntry() : QString();
}

bool MprisManager::fullscreen() const
{
    return checkController(Q_FUNC_INFO) && m_currentController->fullscreen();
}

void MprisManager::setFullscreen(bool fullscreen)
{
    if (checkController(Q_FUNC_INFO)) {
        m_currentController->setFullscreen(fullscreen);
    }
}

bool MprisManager::hasTrackList() const
{
    return checkController(Q_FUNC_INFO) && m_currentController->hasTrackList();
}

QString MprisManager::identity() const
{
    return checkController(Q_FUNC_INFO) ? m_currentController->identity() : QString();
}

QStringList MprisManager::supportedUriSchemes() const
{
    return checkController(Q_FUNC_INFO) ? m_currentController->supportedUriSchemes() : QStringList();
}

QStringList MprisManager::supportedMimeTypes() const
{
    return checkController(Q_FUNC_INFO) ? m_currentController->supportedMimeTypes() : QStringList();
}

// Mpris2 Player Interface
bool MprisManager::canControl() const
{
    return checkController(Q_FUNC_INFO) && m_currentController->canControl();
}

bool MprisManager::canGoNext() const
{
    return checkController(Q_FUNC_INFO) && m_currentController->canGoNext();
}

bool MprisManager::canGoPrevious() const
{
    return checkController(Q_FUNC_INFO) && m_currentController->canGoPrevious();
}

bool MprisManager::canPause() const
{
    return checkController(Q_FUNC_INFO) && m_currentController->canPause();
}

bool MprisManager::canPlay() const
{
    return checkController(Q_FUNC_INFO) && m_currentController->canPlay();
}

bool MprisManager::canSeek() const
{
    return checkController(Q_FUNC_INFO) && m_currentController->canSeek();
}

Mpris::LoopStatus MprisManager::loopStatus() const
{
    return checkController(Q_FUNC_INFO) ? m_currentController->loopStatus() : Mpris::None;
}

void MprisManager::setLoopStatus(Mpris::LoopStatus loopStatus)
{
    if (checkController(Q_FUNC_INFO)) {
        m_currentController->setLoopStatus(loopStatus);
    }
}

double MprisManager::maximumRate() const
{
    return checkController(Q_FUNC_INFO) ? m_currentController->maximumRate() : 1;
}

QVariantMap MprisManager::metadata() const
{
    return checkController(Q_FUNC_INFO) ? m_currentController->metadata() : QVariantMap();
}

double MprisManager::minimumRate() const
{
    return checkController(Q_FUNC_INFO) ? m_currentController->minimumRate() : 1;
}

Mpris::PlaybackStatus MprisManager::playbackStatus() const
{
    return checkController(Q_FUNC_INFO) ? m_currentController->playbackStatus() : Mpris::Stopped;
}

qlonglong MprisManager::position() const
{
    return checkController(Q_FUNC_INFO) ? m_currentController->position() : 0;
}

void MprisManager::requestPosition() const
{
    if (checkController(Q_FUNC_INFO)) {
        m_currentController->requestPosition();
    }
}

double MprisManager::rate() const
{
    return checkController(Q_FUNC_INFO) ? m_currentController->rate() : 1;
}

void MprisManager::setRate(double rate)
{
    if (checkController(Q_FUNC_INFO)) {
        m_currentController->setRate(rate);
    }
}

bool MprisManager::shuffle() const
{
    return checkController(Q_FUNC_INFO) && m_currentController->shuffle();
}

void MprisManager::setShuffle(bool shuffle)
{
    if (checkController(Q_FUNC_INFO)) {
        m_currentController->setShuffle(shuffle);
    }
}

double MprisManager::volume() const
{
    return checkController(Q_FUNC_INFO) ? m_currentController->volume() : 0;
}

void MprisManager::setVolume(double volume)
{
    if (checkController(Q_FUNC_INFO)) {
        m_currentController->setVolume(volume);
    }
}


// Private

void MprisManager::onNameOwnerChanged(const QString &service, const QString &oldOwner, const QString &newOwner)
{
    // Unfortunately, QDBus doesn't allow flexible signal watchers.
    // Would it allow them, we could pass the filter "arg0namespace='org.mpris.MediaPlayer2'"
    // Therefore, we will receive here all the name changes in the
    // bus, not just the ones for our name space of interest, and we
    // will have to filter on our own :(

    QRegExp rx(mprisNameSpace);
    rx.setPatternSyntax(QRegExp::Wildcard);
    if (!rx.exactMatch(service)) {
        return;
    }

    if (oldOwner.isEmpty()) {
        onServiceAppeared(service);
        return;
    }

    if (newOwner.isEmpty()) {
        onServiceVanished(service);
        return;
    }

    // Service changed owner. Nothing to do ...
}

void MprisManager::onServiceAppeared(const QString &service)
{
    QSharedPointer<MprisController> controller = availableController(service);
    if (!controller.isNull()) {
        Q_ASSERT(m_availableControllers.contains(controller));
        m_availableControllers.removeOne(controller);
        m_otherPlayingControllers.removeOne(controller);
    } else {
        if (!m_currentController.isNull() && service == m_currentController->service()) {
            controller = m_currentController;
        } else {
            controller = QSharedPointer<MprisController>(new MprisController(service, QDBusConnection::sessionBus(), this));
        }

        connect(controller.data(), SIGNAL(playbackStatusChanged()), m_playbackStatusMapper, SLOT(map()));
        m_playbackStatusMapper->setMapping(controller.data(), controller->service());
        connect(m_playbackStatusMapper, SIGNAL(mapped(QString)), this, SLOT(onAvailableControllerPlaybackStatusChanged(QString)));
    }

    if (m_currentController.isNull()) {
        setCurrentController(controller);
    }

    if (controller == m_currentController) {
        m_availableControllers.prepend(controller);
        emit availableServicesChanged();
        return;
    }

    if (!m_singleService
        && m_currentController->playbackStatus() != Mpris::Playing) {
        m_availableControllers.prepend(controller);
        setCurrentController(controller);
    } else {
        m_availableControllers.insert(1, controller);
        if (controller->playbackStatus() == Mpris::Playing) {
            m_otherPlayingControllers.prepend(controller);
        }
    }

    emit availableServicesChanged();
}

void MprisManager::onServiceVanished(const QString &service)
{
    QSharedPointer<MprisController> controller = availableController(service);
    if (!controller.isNull()) {
        Q_ASSERT(m_availableControllers.contains(controller));
        m_availableControllers.removeOne(controller);
        m_otherPlayingControllers.removeOne(controller);
    }

    if (!m_currentController.isNull() && service == m_currentController->service()) {
        if (m_singleService) {
            emit availableServicesChanged();
            return;
        }

        if (!m_availableControllers.isEmpty()) {
            setCurrentController(m_availableControllers[0]);
        } else {
            setCurrentController(QSharedPointer<MprisController>());
        }
    }

    emit availableServicesChanged();
}

void MprisManager::onAvailableControllerPlaybackStatusChanged(const QString &service)
{
    QSharedPointer<MprisController> controller = availableController(service);
    Q_ASSERT(!controller.isNull());

    if (m_currentController == controller) {
        if (m_currentController->playbackStatus() == Mpris::Playing) {
            return;
        }

        if (!m_otherPlayingControllers.isEmpty()) {
            QSharedPointer<MprisController> currentController = m_otherPlayingControllers.takeFirst();
            m_availableControllers.move(m_availableControllers.indexOf(currentController), 0);
            setCurrentController(currentController);
        }
    } else {
        if (controller->playbackStatus() != Mpris::Playing) {
            m_otherPlayingControllers.removeOne(controller);
            return;
        }

        if (!m_singleService
            && m_currentController->playbackStatus() != Mpris::Playing) {
            setCurrentController(controller);
        } else {
            m_availableControllers.move(m_availableControllers.indexOf(controller), 1);
            m_otherPlayingControllers.removeOne(controller); // Just in case, shouldn't be needed
            m_otherPlayingControllers.prepend(controller);
        }
    }
}

QSharedPointer<MprisController> MprisManager::availableController(const QString &service)
{
    QList< QSharedPointer<MprisController> >::iterator i = m_availableControllers.begin();
    while (i != m_availableControllers.end()) {
        QSharedPointer<MprisController> controller = *i;
        if (!controller.isNull() && controller->service() == service) {
            return controller;
        }

        ++i;
    }

    return QSharedPointer<MprisController>();
}

void MprisManager::setCurrentController(QSharedPointer<MprisController> controller)
{
    if (controller == m_currentController) {
        return;
    }

    if (!m_currentController.isNull()) {
        // Mpris Root Interface
        disconnect(m_currentController.data(), &MprisController::canQuitChanged, this, &MprisManager::canQuitChanged);
        disconnect(m_currentController.data(), &MprisController::canRaiseChanged, this, &MprisManager::canRaiseChanged);
        disconnect(m_currentController.data(), &MprisController::canSetFullscreenChanged, this, &MprisManager::canSetFullscreenChanged);
        disconnect(m_currentController.data(), &MprisController::desktopEntryChanged, this, &MprisManager::desktopEntryChanged);
        disconnect(m_currentController.data(), &MprisController::fullscreenChanged, this, &MprisManager::fullscreenChanged);
        disconnect(m_currentController.data(), &MprisController::hasTrackListChanged, this, &MprisManager::hasTrackListChanged);
        disconnect(m_currentController.data(), &MprisController::identityChanged, this, &MprisManager::identityChanged);
        disconnect(m_currentController.data(), &MprisController::supportedUriSchemesChanged, this, &MprisManager::supportedUriSchemesChanged);
        disconnect(m_currentController.data(), &MprisController::supportedMimeTypesChanged, this, &MprisManager::supportedMimeTypesChanged);

        // Mpris Player Interface
        disconnect(m_currentController.data(), &MprisController::canControlChanged, this, &MprisManager::canControlChanged);
        disconnect(m_currentController.data(), &MprisController::canGoNextChanged, this, &MprisManager::canGoNextChanged);
        disconnect(m_currentController.data(), &MprisController::canGoPreviousChanged, this, &MprisManager::canGoPreviousChanged);
        disconnect(m_currentController.data(), &MprisController::canPauseChanged, this, &MprisManager::canPauseChanged);
        disconnect(m_currentController.data(), &MprisController::canPlayChanged, this, &MprisManager::canPlayChanged);
        disconnect(m_currentController.data(), &MprisController::canSeekChanged, this, &MprisManager::canSeekChanged);
        disconnect(m_currentController.data(), &MprisController::loopStatusChanged, this, &MprisManager::loopStatusChanged);
        disconnect(m_currentController.data(), &MprisController::maximumRateChanged, this, &MprisManager::maximumRateChanged);
        disconnect(m_currentController.data(), &MprisController::metadataChanged, this, &MprisManager::metadataChanged);
        disconnect(m_currentController.data(), &MprisController::minimumRateChanged, this, &MprisManager::minimumRateChanged);
        disconnect(m_currentController.data(), &MprisController::playbackStatusChanged, this, &MprisManager::playbackStatusChanged);
        disconnect(m_currentController.data(), &MprisController::positionChanged, this, &MprisManager::positionChanged);
        disconnect(m_currentController.data(), &MprisController::rateChanged, this, &MprisManager::rateChanged);
        disconnect(m_currentController.data(), &MprisController::shuffleChanged, this, &MprisManager::shuffleChanged);
        disconnect(m_currentController.data(), &MprisController::volumeChanged, this, &MprisManager::volumeChanged);
        disconnect(m_currentController.data(), &MprisController::seeked, this, &MprisManager::seeked);

        if (m_currentController->playbackStatus() == Mpris::Playing) {
            m_otherPlayingControllers.prepend(m_currentController);
        }
    }

    m_currentController = controller;

    if (!m_currentController.isNull()) {
        // Mpris Root Interface
        connect(m_currentController.data(), &MprisController::canQuitChanged, this, &MprisManager::canQuitChanged);
        connect(m_currentController.data(), &MprisController::canRaiseChanged, this, &MprisManager::canRaiseChanged);
        connect(m_currentController.data(), &MprisController::canSetFullscreenChanged, this, &MprisManager::canSetFullscreenChanged);
        connect(m_currentController.data(), &MprisController::desktopEntryChanged, this, &MprisManager::desktopEntryChanged);
        connect(m_currentController.data(), &MprisController::fullscreenChanged, this, &MprisManager::fullscreenChanged);
        connect(m_currentController.data(), &MprisController::hasTrackListChanged, this, &MprisManager::hasTrackListChanged);
        connect(m_currentController.data(), &MprisController::identityChanged, this, &MprisManager::identityChanged);
        connect(m_currentController.data(), &MprisController::supportedUriSchemesChanged, this, &MprisManager::supportedUriSchemesChanged);
        connect(m_currentController.data(), &MprisController::supportedMimeTypesChanged, this, &MprisManager::supportedMimeTypesChanged);
        connect(m_currentController.data(), &MprisController::canControlChanged, this, &MprisManager::canControlChanged);

        // Mpris Player Interface
        connect(m_currentController.data(), &MprisController::canGoNextChanged, this, &MprisManager::canGoNextChanged);
        connect(m_currentController.data(), &MprisController::canGoPreviousChanged, this, &MprisManager::canGoPreviousChanged);
        connect(m_currentController.data(), &MprisController::canPauseChanged, this, &MprisManager::canPauseChanged);
        connect(m_currentController.data(), &MprisController::canPlayChanged, this, &MprisManager::canPlayChanged);
        connect(m_currentController.data(), &MprisController::canSeekChanged, this, &MprisManager::canSeekChanged);
        connect(m_currentController.data(), &MprisController::loopStatusChanged, this, &MprisManager::loopStatusChanged);
        connect(m_currentController.data(), &MprisController::maximumRateChanged, this, &MprisManager::maximumRateChanged);
        connect(m_currentController.data(), &MprisController::metadataChanged, this, &MprisManager::metadataChanged);
        connect(m_currentController.data(), &MprisController::minimumRateChanged, this, &MprisManager::minimumRateChanged);
        connect(m_currentController.data(), &MprisController::playbackStatusChanged, this, &MprisManager::playbackStatusChanged);
        connect(m_currentController.data(), &MprisController::rateChanged, this, &MprisManager::rateChanged);
        connect(m_currentController.data(), &MprisController::shuffleChanged, this, &MprisManager::shuffleChanged);
        connect(m_currentController.data(), &MprisController::volumeChanged, this, &MprisManager::volumeChanged);
        connect(m_currentController.data(), &MprisController::seeked, this, &MprisManager::seeked);

        if (m_currentController->playbackStatus() == Mpris::Playing) {
            m_otherPlayingControllers.removeOne(m_currentController);
        }
    }

    emit currentServiceChanged();
}

bool MprisManager::checkController(const char *callerName) const
{
    if (m_currentController.isNull()) {
        qWarning() << callerName << "None service available/selected";
        return false;
    }

    return true;
}
