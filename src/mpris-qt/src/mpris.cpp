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


#include "mpris.h"

#include <QQmlEngine>
#include <QJSEngine>

static const char *playbackStatusStrings[] = { "Playing", "Paused", "Stopped" };
static const char *loopStatusStrings[] = { "None", "Track", "Playlist" };
static const char *metadataStrings[] = { "mpris:trackid", "mpris:length", "mpris:artUrl", "xesam:album", "xesam:albumArtist", "xesam:artist", "xesam:asText", "xesam:audioBPM", "xesam:autoRating", "xesam:comment", "xesam:composer", "xesam:contentCreated", "xesam:discNumber", "xesam:firstUsed", "xesam:genre", "xesam:lastUsed", "xesam:lyricist", "xesam:title", "xesam:trackNumber", "xesam:url", "xesam:useCount", "xesam:userRating" };


Mpris::Mpris(QObject *parent)
    : QObject(parent)
{
}

Mpris::~Mpris()
{
}

QObject *Mpris::api_factory(QQmlEngine *, QJSEngine *)
{
    return new Mpris;
}

QString Mpris::metadataToString(Mpris::Metadata metadata)
{
    return enumerationToString(metadata);
}


// Private

template <typename T> void Mpris::getEnumStringsAndSize(const char ***strings, int *size)
{
    if (is_same<T, Mpris::LoopStatus>::value) {
        *strings = loopStatusStrings;
        *size = arraySize(loopStatusStrings);
    } else if (is_same<T, Mpris::PlaybackStatus>::value) {
        *strings = playbackStatusStrings;
        *size = arraySize(playbackStatusStrings);
    } else if (is_same<T, Mpris::Metadata>::value) {
        *strings = metadataStrings;
        *size = arraySize(metadataStrings);
    } else {
        *strings = 0;
        *size = 0;
    }
}
template void Mpris::getEnumStringsAndSize<Mpris::LoopStatus>(const char ***strings, int *size);
template void Mpris::getEnumStringsAndSize<Mpris::PlaybackStatus>(const char ***strings, int *size);
template void Mpris::getEnumStringsAndSize<Mpris::Metadata>(const char ***strings, int *size);


template <typename T> QString Mpris::enumerationToString(T enumeration)
{
    const char **strings;
    int size;

    getEnumStringsAndSize<T>(&strings, &size);

    return enumeration >= 0 && enumeration < size
                                             ? QString(QLatin1String(strings[enumeration]))
                                             : QString();
}
template QString Mpris::enumerationToString<Mpris::LoopStatus>(Mpris::LoopStatus enumeration);
template QString Mpris::enumerationToString<Mpris::PlaybackStatus>(Mpris::PlaybackStatus enumeration);
template QString Mpris::enumerationToString<Mpris::Metadata>(Mpris::Metadata enumeration);


template <typename T> T Mpris::enumerationFromString(const QString &string)
{
    const char **strings;
    int size;

    getEnumStringsAndSize<T>(&strings, &size);

    for (int i = 0; i < size; ++i) {
        if (string == QLatin1String(strings[i])) {
            return T(i);
        }
    }

    return T(-1);
}
template Mpris::LoopStatus Mpris::enumerationFromString<Mpris::LoopStatus>(const QString &string);
template Mpris::PlaybackStatus Mpris::enumerationFromString<Mpris::PlaybackStatus>(const QString &string);
template Mpris::Metadata Mpris::enumerationFromString<Mpris::Metadata>(const QString &string);
