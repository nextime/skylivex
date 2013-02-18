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
 * File: skylivex.h
 * 
 * Purpose:
 *
 */
#ifndef MAINWIN_H
#define MAINWIN_H

#include <QWebView>
#include <QUrl>
#include <QFile>
#include <QHash>
#include <QString>
#include <QObject>
#include <ipcmsg.h>


class MainWin; // forward declaration for typedef
// This is for member pointers to map messages
typedef void (MainWin::*SKHandlerFunction)(SKMessage::SKMessage&);

/*
 * class MainWin
 * This is just a little webkit transparent window 
 * to show the splash screen
 */
class MainWin : public QWebView
{

   Q_OBJECT

   QUrl baseurl;
   QFile htmlfile;
   QString htmlFileName;

   private:
      QHash<QString, SKHandlerFunction> _handlers;

   public:
      MainWin(QFile &htmlfile);
      ~MainWin();
      void sendMessage(SKMessage::SKMessage &msg);
      void registerHandler(QString type, SKHandlerFunction handler);
      void handle_corestarted(SKMessage::SKMessage &msg);

   public slots:
     void msgFromCore(SKMessage::SKMessage &msg);

   signals:
     void putMessage(SKMessage::SKMessage &msg);

};


#endif
