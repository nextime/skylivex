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
 * File: jsbridge.h
 * 
 * Purpose:
 * Define a QObject used to export to javascript
 * some methods to make the webkit based GUI communicate
 * with the core.
 *
 */
#ifndef JSBRIDGE_H
#define JSBRIDGE_H

#include <QObject>
#include <QString>
#include "webwin.h"

class SkylivexWin; // forward declaration for typedef

/*
 * class JSBridge
 * This class connect the javascript object SkyliveX
 * to c++
 */
class JSBridge : public QObject
{
   Q_OBJECT

   public:
      SkylivexWin* wwin;
      void changePageContent(QString elementid, QString content);

   signals:
      void changeContent(QString elementid, QString content);
      void notify(QString content);
      void alertmsg(QString content);
      void public_received(QString user, QString content);
      void updateUserList(QString user, QString utype, QString direction);
      void deleteUserList();
      void changeTelescope(QString telescope);

   public slots:
      void pushLogin(QString username, QString password);
      void resizeWin(int width, int height);
      void toggleBorders(bool borders);
      void toggleTransparentBackground(bool transparentbg);
      void chat_message_send(QString dest, QString message);
      void chat_message_send(QString message);
      void change_telescope(QString tele);
      SkylivexWin* open_window(QString url, bool Modal);
};


#endif
