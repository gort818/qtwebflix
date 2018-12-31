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


#ifndef MPRIS_H
#define MPRIS_H

#include <MprisQt>

#include <QtCore/QObject>
#include <QtCore/QString>

class QQmlEngine;
class QJSEngine;

class MPRIS_QT_EXPORT Mpris : public QObject
{
    Q_OBJECT

    Q_ENUMS(PlaybackStatus LoopStatus Metadata)

public:

    enum PlaybackStatus {
        InvalidPlaybackStatus = -1,
        Playing,
        Paused,
        Stopped
    };

    enum LoopStatus {
        InvalidLoopStatus = -1,
        None,
        Track,
        Playlist
    };

    enum Metadata {
        InvalidMetadata = -1,
        TrackId,
        Length,
        ArtUrl,
        Album,
        AlbumArtist,
        Artist,
        AsText,
        AudioBPM,
        AutoRating,
        Comment,
        Composer,
        ContentCreated,
        DiscNumber,
        FirstUsed,
        Genre,
        LastUsed,
        Lyricist,
        Title,
        TrackNumber,
        Url,
        UseCount,
        UserRating
    };

    Mpris(QObject *parent = 0);
    ~Mpris();

    static QObject *api_factory(QQmlEngine *, QJSEngine *);

    Q_INVOKABLE static QString metadataToString(Metadata metadata);

private:
    template<class T, int N> static int arraySize(T (&)[N]) { return N; };
    template<typename T, typename U> struct is_same { static const bool value = false; };
    template<typename T> struct is_same<T,T> { static const bool value = true; };
    template<typename T> static void getEnumStringsAndSize(const char ***strings, int *size);
    template<typename T> static QString enumerationToString(T enumeration);
    template<typename T> static T enumerationFromString(const QString &string);

    friend class MprisRootAdaptor;
    friend class MprisPlayerAdaptor;
    friend class MprisPlayer;
    friend class MprisController;
};

#endif /* MPRIS_H */
