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
 * File: mainwin.h
 * 
 * Purpose: 
 * This file define the special WebView derived object for
 * the main window
 *
 */
#ifndef MAINWIN_H
#define MAINWIN_H

#include <QUrl>
#include <QFile>
#include <QHash>
#include <QString>
#include <QObject>
#include <ipcmsg.h>
#include "webwin.h"
#include "jsbridge.h"

/*
 * class MainWin
 * This is needed cause the MainWindow is something
 * different from the others, all the window
 * with access to the c++ core will be subwindow of this one,
 * but this will also serve the login window and the splashscreen
 */
class MainWin : public SkylivexWin
{

   Q_OBJECT

   public:
      MainWin(QString &htmlfile);
      ~MainWin();
      QString msgsender;      // The name of the mainwindow for the IPC messages
      WebWin* yt;             // a pointer to the special (singleton) Youtube window (if exists)
      bool yt_is_open;        // a boolean to indicate if the Youtube window is open
      void handle_corestarted(SKMessage &msg);
      void handle_connected(SKMessage &msg);
      void handle_asklogin(SKMessage &msg);
      void handle_loginres(SKMessage &msg);
      void handle_openurl(SKMessage &msg);
      void handle_youtubevideo(SKMessage &msg);
      void handle_closeyoutube(SKMessage &msg);
   public slots:
     void ytclosesignal();   // This slot is needed to close the special youtube window

};


#endif
