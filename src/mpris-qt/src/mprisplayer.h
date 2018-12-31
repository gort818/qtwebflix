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


#ifndef MPRISPLAYER_H
#define MPRISPLAYER_H

#include <MprisQt>
#include <Mpris>

#include <QDBusContext>
#include <QDBusPendingCallWatcher>
#include <QDBusObjectPath>


class MprisRootAdaptor;
class MprisPlayerAdaptor;
class MPRIS_QT_EXPORT MprisPlayer : public QObject,
                                    protected QDBusContext
{
    Q_OBJECT
    Q_PROPERTY(QString serviceName READ serviceName WRITE setServiceName NOTIFY serviceNameChanged)

    // Mpris2 Root Interface
    Q_PROPERTY(bool canQuit READ canQuit WRITE setCanQuit NOTIFY canQuitChanged)
    Q_PROPERTY(bool canRaise READ canRaise WRITE setCanRaise NOTIFY canRaiseChanged)
    Q_PROPERTY(bool canSetFullscreen READ canSetFullscreen WRITE setCanSetFullscreen NOTIFY canSetFullscreenChanged)
    Q_PROPERTY(QString desktopEntry READ desktopEntry WRITE setDesktopEntry NOTIFY desktopEntryChanged)
    Q_PROPERTY(bool fullscreen READ fullscreen WRITE setFullscreen NOTIFY fullscreenChanged)
    Q_PROPERTY(bool hasTrackList READ hasTrackList WRITE setHasTrackList NOTIFY hasTrackListChanged)
    Q_PROPERTY(QString identity READ identity WRITE setIdentity NOTIFY identityChanged)
    Q_PROPERTY(QStringList supportedUriSchemes READ supportedUriSchemes WRITE setSupportedUriSchemes NOTIFY supportedUriSchemesChanged)
    Q_PROPERTY(QStringList supportedMimeTypes READ supportedMimeTypes WRITE setSupportedMimeTypes NOTIFY supportedMimeTypesChanged)

    // Mpris2 Player Interface
    Q_PROPERTY(bool canControl READ canControl WRITE setCanControl NOTIFY canControlChanged)
    Q_PROPERTY(bool canGoNext READ canGoNext WRITE setCanGoNext NOTIFY canGoNextChanged)
    Q_PROPERTY(bool canGoPrevious READ canGoPrevious WRITE setCanGoPrevious NOTIFY canGoPreviousChanged)
    Q_PROPERTY(bool canPause READ canPause WRITE setCanPause NOTIFY canPauseChanged)
    Q_PROPERTY(bool canPlay READ canPlay WRITE setCanPlay NOTIFY canPlayChanged)
    Q_PROPERTY(bool canSeek READ canSeek WRITE setCanSeek NOTIFY canSeekChanged)
    Q_PROPERTY(Mpris::LoopStatus loopStatus READ loopStatus WRITE setLoopStatus NOTIFY loopStatusChanged)
    Q_PROPERTY(double maximumRate READ maximumRate WRITE setMaximumRate NOTIFY maximumRateChanged)
    Q_PROPERTY(QVariantMap metadata READ metadata WRITE setMetadata NOTIFY metadataChanged)
    Q_PROPERTY(double minimumRate READ minimumRate WRITE setMinimumRate NOTIFY minimumRateChanged)
    Q_PROPERTY(Mpris::PlaybackStatus playbackStatus READ playbackStatus WRITE setPlaybackStatus NOTIFY playbackStatusChanged)
    Q_PROPERTY(qlonglong position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(double rate READ rate WRITE setRate NOTIFY rateChanged)
    Q_PROPERTY(bool shuffle READ shuffle WRITE setShuffle NOTIFY shuffleChanged)
    Q_PROPERTY(double volume READ volume WRITE setVolume NOTIFY volumeChanged)

public:

    MprisPlayer(QObject *parent = 0);
    ~MprisPlayer();

public slots:

    QString serviceName() const;
    void setServiceName(const QString &serviceName);

    // Mpris2 Root Interface
    bool canQuit() const;
    void setCanQuit(bool canQuit);

    bool canRaise() const;
    void setCanRaise(bool canRaise);

    bool canSetFullscreen() const;
    void setCanSetFullscreen(bool canSetFullscreen);

    QString desktopEntry() const;
    void setDesktopEntry(const QString &desktopEntry);

    bool fullscreen() const;
    void setFullscreen(bool fullscreen);

    bool hasTrackList() const;
    void setHasTrackList(bool hasTrackList);

    QString identity() const;
    void setIdentity(const QString &identity);

    QStringList supportedUriSchemes() const;
    void setSupportedUriSchemes(const QStringList &supportedUriSchemes);

    QStringList supportedMimeTypes() const;
    void setSupportedMimeTypes(const QStringList &supportedMimeTypes);

    // Mpris2 Player Interface
    bool canControl() const;
    void setCanControl(bool canControl);

    bool canGoNext() const;
    void setCanGoNext(bool canGoNext);

    bool canGoPrevious() const;
    void setCanGoPrevious(bool canGoPrevious);

    bool canPause() const;
    void setCanPause(bool canPause);

    bool canPlay() const;
    void setCanPlay(bool canPlay);

    bool canSeek() const;
    void setCanSeek(bool canSeek);

    Mpris::LoopStatus loopStatus() const;
    void setLoopStatus(Mpris::LoopStatus loopStatus);

    double maximumRate() const;
    void setMaximumRate(double maximumRate);

    QVariantMap metadata() const;
    void setMetadata(const QVariantMap &metadata);

    double minimumRate() const;
    void setMinimumRate(double minimumRate);

    Mpris::PlaybackStatus playbackStatus() const;
    void setPlaybackStatus(Mpris::PlaybackStatus playbackStatus);

    qlonglong position() const;
    void setPosition(qlonglong position);

    double rate() const;
    void setRate(double rate);

    bool shuffle() const;
    void setShuffle(bool shuffle);

    double volume() const;
    void setVolume(double volume);

signals:
    void serviceNameChanged();

    // Mpris2 Root Interface
    void canQuitChanged();
    void canRaiseChanged();
    void canSetFullscreenChanged();
    void desktopEntryChanged();
    void fullscreenChanged();
    void hasTrackListChanged();
    void identityChanged();
    void supportedUriSchemesChanged();
    void supportedMimeTypesChanged();
    void fullscreenRequested(bool fullscreen);
    void quitRequested();
    void raiseRequested();

    // Mpris2 Player Interface
    void canControlChanged();
    void canGoNextChanged();
    void canGoPreviousChanged();
    void canPauseChanged();
    void canPlayChanged();
    void canSeekChanged();
    void loopStatusChanged();
    void maximumRateChanged();
    void metadataChanged();
    void minimumRateChanged();
    void playbackStatusChanged();
    void positionChanged();
    void rateChanged();
    void shuffleChanged();
    void volumeChanged();
    void loopStatusRequested(Mpris::LoopStatus loopStatus);
    void rateRequested(double rate);
    void shuffleRequested(bool shuffle);
    void volumeRequested(double volume);
    void nextRequested();
    void openUriRequested(const QUrl &url);
    void pauseRequested();
    void playRequested();
    void playPauseRequested();
    void previousRequested();
    void seekRequested(qlonglong offset);
    void seeked(qlonglong position);
    void setPositionRequested(const QDBusObjectPath &trackId, qlonglong position);
    void stopRequested();

private:
    static QVariantMap typeMetadata(const QVariantMap &aMetadata);

    void registerService();
    void unregisterService();
    void notifyPropertiesChanged(const QString& interfaceName, const QVariantMap &changedProperties, const QStringList &invalidatedProperties) const;

    MprisRootAdaptor *m_mprisRootAdaptor;
    MprisPlayerAdaptor *m_mprisPlayerAdaptor;

    QString m_serviceName;

    // Mpris2 Root Interface
    bool m_canQuit;
    bool m_canRaise;
    bool m_canSetFullscreen;
    QString m_desktopEntry;
    bool m_fullscreen;
    bool m_hasTrackList;
    QString m_identity;
    QStringList m_supportedUriSchemes;
    QStringList m_supportedMimeTypes;

    // Mpris2 Player Interface
    bool m_canControl;
    bool m_canGoNext;
    bool m_canGoPrevious;
    bool m_canPause;
    bool m_canPlay;
    bool m_canSeek;
    Mpris::LoopStatus m_loopStatus;
    double m_maximumRate;
    QVariantMap m_metadata;
    QVariantMap m_typedMetadata;
    double m_minimumRate;
    Mpris::PlaybackStatus m_playbackStatus;
    qlonglong m_position;
    double m_rate;
    bool m_shuffle;
    double m_volume;

    friend class MprisRootAdaptor;
    friend class MprisPlayerAdaptor;
};

#endif /* MPRISPLAYER_H */
