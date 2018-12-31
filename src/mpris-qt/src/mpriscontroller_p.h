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


#ifndef MPRISCONTROLLER_P_H
#define MPRISCONTROLLER_P_H

#include <DBusExtendedAbstractInterface>

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface org.mpris.MediaPlayer2
 */
class MprisRootInterface: public DBusExtendedAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.mpris.MediaPlayer2"; }

public:
    MprisRootInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~MprisRootInterface();

    Q_PROPERTY(bool CanQuit READ canQuit NOTIFY canQuitChanged)
    inline bool canQuit()
    { return qvariant_cast< bool >(internalPropGet("CanQuit", &m_canQuit)); }

    Q_PROPERTY(bool CanRaise READ canRaise NOTIFY canRaiseChanged)
    inline bool canRaise()
    { return qvariant_cast< bool >(internalPropGet("CanRaise", &m_canRaise)); }

    Q_PROPERTY(bool CanSetFullscreen READ canSetFullscreen NOTIFY canSetFullscreenChanged)
    inline bool canSetFullscreen()
    { return qvariant_cast< bool >(internalPropGet("CanSetFullscreen", &m_canSetFullscreen)); }

    Q_PROPERTY(QString DesktopEntry READ desktopEntry NOTIFY desktopEntryChanged)
    inline QString desktopEntry()
    { return qvariant_cast< QString >(internalPropGet("DesktopEntry", &m_desktopEntry)); }

    Q_PROPERTY(bool Fullscreen READ fullscreen WRITE setFullscreen NOTIFY fullscreenChanged)
    inline bool fullscreen()
    { return qvariant_cast< bool >(internalPropGet("Fullscreen", &m_fullscreen)); }
    inline void setFullscreen(bool value)
    { m_fullscreen = value; internalPropSet("Fullscreen", QVariant::fromValue(value), &m_fullscreen); }

    Q_PROPERTY(bool HasTrackList READ hasTrackList NOTIFY hasTrackListChanged)
    inline bool hasTrackList()
    { return qvariant_cast< bool >(internalPropGet("HasTrackList", &m_hasTrackList)); }

    Q_PROPERTY(QString Identity READ identity NOTIFY identityChanged)
    inline QString identity()
    { return qvariant_cast< QString >(internalPropGet("Identity", &m_identity)); }

    Q_PROPERTY(QStringList SupportedMimeTypes READ supportedMimeTypes NOTIFY supportedMimeTypesChanged)
    inline QStringList supportedMimeTypes()
    { return qvariant_cast< QStringList >(internalPropGet("SupportedMimeTypes", &m_supportedMimeTypes)); }

    Q_PROPERTY(QStringList SupportedUriSchemes READ supportedUriSchemes NOTIFY supportedUriSchemesChanged)
    inline QStringList supportedUriSchemes()
    { return qvariant_cast< QStringList >(internalPropGet("SupportedUriSchemes", &m_supportedUriSchemes)); }

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> Quit()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Quit"), argumentList);
    }

    inline QDBusPendingReply<> Raise()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Raise"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void canQuitChanged(bool canQuit);
    void canRaiseChanged(bool canRaise);
    void canSetFullscreenChanged(bool canSetFullscreen);
    void desktopEntryChanged(const QString &desktopEntry);
    void fullscreenChanged(bool fullscreen);
    void hasTrackListChanged(bool hasTrackList);
    void identityChanged(const QString &identity);
    void supportedMimeTypesChanged(const QStringList &supportedMimeTypes);
    void supportedUriSchemesChanged(const QStringList &supportedUriSchemes);

private Q_SLOTS:
    void onPropertyChanged(const QString &propertyName, const QVariant &value);

private:
    bool m_canQuit;
    bool m_canRaise;
    bool m_canSetFullscreen;
    QString m_desktopEntry;
    bool m_fullscreen;
    bool m_hasTrackList;
    QString m_identity;
    QStringList m_supportedUriSchemes;
    QStringList m_supportedMimeTypes;
};

namespace org {
    namespace mpris {
        typedef ::MprisRootInterface MediaPlayer2;
    }
}


/*
 * Proxy class for interface org.mpris.MediaPlayer2
 */
class MprisPlayerInterface: public DBusExtendedAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.mpris.MediaPlayer2.Player"; }

public:
    MprisPlayerInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~MprisPlayerInterface();

    Q_PROPERTY(bool CanControl READ canControl NOTIFY canControlChanged)
    inline bool canControl()
    { return qvariant_cast< bool >(internalPropGet("CanControl", &m_canControl)); }

    Q_PROPERTY(bool CanGoNext READ canGoNext NOTIFY canGoNextChanged)
    inline bool canGoNext()
    { return qvariant_cast< bool >(internalPropGet("CanGoNext", &m_canGoNext)); }

    Q_PROPERTY(bool CanGoPrevious READ canGoPrevious NOTIFY canGoPreviousChanged)
    inline bool canGoPrevious()
    { return qvariant_cast< bool >(internalPropGet("CanGoPrevious", &m_canGoPrevious)); }

    Q_PROPERTY(bool CanPause READ canPause NOTIFY canPauseChanged)
    inline bool canPause()
    { return qvariant_cast< bool >(internalPropGet("CanPause", &m_canPause)); }

    Q_PROPERTY(bool CanPlay READ canPlay NOTIFY canPlayChanged)
    inline bool canPlay()
    { return qvariant_cast< bool >(internalPropGet("CanPlay", &m_canPlay)); }

    Q_PROPERTY(bool CanSeek READ canSeek NOTIFY canSeekChanged)
    inline bool canSeek()
    { return qvariant_cast< bool >(internalPropGet("CanSeek", &m_canSeek)); }

    Q_PROPERTY(QString LoopStatus READ loopStatus WRITE setLoopStatus NOTIFY loopStatusChanged)
    inline QString loopStatus()
    { return qvariant_cast< QString >(internalPropGet("LoopStatus", &m_loopStatus)); }
    inline void setLoopStatus(const QString &value)
    { m_loopStatus = value; internalPropSet("LoopStatus", QVariant::fromValue(value), &m_loopStatus); }

    Q_PROPERTY(double MaximumRate READ maximumRate NOTIFY maximumRateChanged)
    inline double maximumRate()
    { return qvariant_cast< double >(internalPropGet("MaximumRate", &m_maximumRate)); }

    Q_PROPERTY(QVariantMap Metadata READ metadata NOTIFY metadataChanged)
    inline QVariantMap metadata()
    { return qvariant_cast< QVariantMap >(internalPropGet("Metadata", &m_metadata)); }

    Q_PROPERTY(double MinimumRate READ minimumRate NOTIFY minimumRateChanged)
    inline double minimumRate()
    { return qvariant_cast< double >(internalPropGet("MinimumRate", &m_minimumRate)); }

    Q_PROPERTY(QString PlaybackStatus READ playbackStatus NOTIFY playbackStatusChanged)
    inline QString playbackStatus()
    { return qvariant_cast< QString >(internalPropGet("PlaybackStatus", &m_playbackStatus)); }

    Q_PROPERTY(qlonglong Position READ position NOTIFY positionChanged)
    inline qlonglong position()
    { return qvariant_cast< qlonglong >(internalPropGet("Position", &m_position)); }

    Q_PROPERTY(double Rate READ rate WRITE setRate NOTIFY rateChanged)
    inline double rate()
    { return qvariant_cast< double >(internalPropGet("Rate", &m_rate)); }
    inline void setRate(double value)
    { m_rate = value; internalPropSet("Rate", QVariant::fromValue(value), &m_rate); }

    Q_PROPERTY(bool Shuffle READ shuffle WRITE setShuffle NOTIFY shuffleChanged)
    inline bool shuffle()
    { return qvariant_cast< bool >(internalPropGet("Shuffle", &m_shuffle)); }
    inline void setShuffle(bool value)
    { m_shuffle = value; internalPropSet("Shuffle", QVariant::fromValue(value), &m_shuffle); }

    Q_PROPERTY(double Volume READ volume WRITE setVolume NOTIFY volumeChanged)
    inline double volume()
    { return qvariant_cast< double >(internalPropGet("Volume", &m_volume)); }
    inline void setVolume(double value)
    { m_volume = value; internalPropSet("Volume", QVariant::fromValue(value), &m_volume); }

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> Next()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Next"), argumentList);
    }

    inline QDBusPendingReply<> OpenUri(const QString &Uri)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(Uri);
        return asyncCallWithArgumentList(QLatin1String("OpenUri"), argumentList);
    }

    inline QDBusPendingReply<> Pause()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Pause"), argumentList);
    }

    inline QDBusPendingReply<> Play()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Play"), argumentList);
    }

    inline QDBusPendingReply<> PlayPause()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("PlayPause"), argumentList);
    }

    inline QDBusPendingReply<> Previous()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Previous"), argumentList);
    }

    inline QDBusPendingReply<> Seek(qlonglong Offset)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(Offset);
        return asyncCallWithArgumentList(QLatin1String("Seek"), argumentList);
    }

    inline QDBusPendingReply<> SetPosition(const QDBusObjectPath &TrackId, qlonglong Position)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(TrackId) << QVariant::fromValue(Position);
        return asyncCallWithArgumentList(QLatin1String("SetPosition"), argumentList);
    }

    inline QDBusPendingReply<> Stop()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Stop"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void canControlChanged(bool canControl);
    void canGoNextChanged(bool canGoNext);
    void canGoPreviousChanged(bool canGoPrevious);
    void canPauseChanged(bool canPause);
    void canPlayChanged(bool canPlay);
    void canSeekChanged(bool canSeek);
    void loopStatusChanged(const QString &loopStatus);
    void maximumRateChanged(double maximumRate);
    void metadataChanged(const QVariantMap &metadata);
    void minimumRateChanged(double minimumRate);
    void playbackStatusChanged(const QString &playbackStatus);
    void positionChanged(qlonglong rate);
    void rateChanged(double rate);
    void shuffleChanged(bool shuffle);
    void volumeChanged(double volume);
    void seeked(qlonglong Position);

private Q_SLOTS:
    void onPropertyChanged(const QString &propertyName, const QVariant &value);

private:
    bool m_canControl;
    bool m_canGoNext;
    bool m_canGoPrevious;
    bool m_canPause;
    bool m_canPlay;
    bool m_canSeek;
    QString m_loopStatus;
    double m_maximumRate;
    QVariantMap m_metadata;
    double m_minimumRate;
    QString m_playbackStatus;
    qlonglong m_position;
    double m_rate;
    bool m_shuffle;
    double m_volume;
};

namespace MediaPlayer2 {
    typedef ::MprisPlayerInterface Player;
}

#endif /* MPRISROOTINTERFACE_P_H */
