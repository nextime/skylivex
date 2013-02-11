/*  ____  _          _ _          __  __
 * / ___|| | ___   _| (_)_   _____\ \/ /
 * \___ \| |/ / | | | | \ \ / / _ \\  / 
 *  ___) |   <| |_| | | |\ V /  __//  \ Remote Telescopes
 * |____/|_|\_\\__, |_|_| \_/ \___/_/\_\ For the masses
 *             |___/               
 *
 * Copyright (C) 2013 Franco (nextime) Lanza <nextime@nexlab.it>
 * Copyright (C) 2013 Ivan Bellia <skylive@skylive.it>
 *
 * All rights reserved.
 *
 * This file is part of SkyliveX.
 *
 * SkyliveX is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 ********************************************************************
 *
 * File: main.cpp
 * 
 * Purpose: Core file for the SkyliveX client. It provide core
 * functionality as the enter point of the client and
 * to load plugins and make them communicate.
 *
 */
#include <QWebView>
#include <QWebFrame>
#include <QDesktopWidget>
#include <QApplication>
#include <QFile>
#include <QUrl>
#include <QDir>


int main(int argc, char *argv[])
{
    QApplication skylivex(argc, argv);

    QWebView splashwin;

    QFile loadinghtml("gui/splash.html");
    loadinghtml.open(QIODevice::ReadOnly);
    QString splashFile = QString::fromUtf8(loadinghtml.readAll().constData());
    QUrl baseUrl = QUrl::fromLocalFile(QDir::current().absoluteFilePath("gui/dummy.html"));

    splashwin.setWindowFlags(Qt::FramelessWindowHint);
    QPalette palette = splashwin.palette();
    palette.setBrush(QPalette::Base, Qt::transparent);
    splashwin.page()->setPalette(palette);
    splashwin.setAttribute(Qt::WA_TranslucentBackground, true);
    splashwin.setAttribute(Qt::WA_OpaquePaintEvent, false);
    //splashwin.setGeometry(0, 0, QApplication::desktop()->size().width(), QApplication::desktop()->size().height());

    splashwin.setHtml(splashFile, baseUrl);
    splashwin.resize(300,200);
    splashwin.show();


    return skylivex.exec();

}

