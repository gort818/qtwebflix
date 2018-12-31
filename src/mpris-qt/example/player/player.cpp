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


#include <Mpris>
#include <MprisPlayer>

#include <QtQuick>

#include <QGuiApplication>


int main(int argc, char *argv[])
{
    QGuiApplication *app = new QGuiApplication(argc, argv);
    QQuickView *view = new QQuickView;

    qmlRegisterSingletonType<Mpris>("org.nemomobile.qtmpris", 1, 0, "Mpris", Mpris::api_factory);
    qmlRegisterType<MprisPlayer>("org.nemomobile.qtmpris", 1, 0, "MprisPlayer");

    view->setSource(app->applicationDirPath().append("/../qml/player.qml"));
    view->show();

    int retVal = app->exec();

    return retVal;
}
