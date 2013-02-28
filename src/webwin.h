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
#ifndef WEBWIN_H
#define WEBWIN_H

#include <QWebView>
#include <QWebPage>
#include <QUrl>
#include <QFile>
#include <QHash>
#include <QString>
#include <QObject>
#include <ipcmsg.h>
#include "jsbridge.h"

class WebWin; // forward declaration for typedef
class JSBridge; // forward declaration
// This is for member pointers to map messages
typedef void (WebWin::*SKHandlerFunction)(SKMessage&);
//typedef int (WebWin::*SKHandlerFunction)(SKMessage&);


/*
 * class WebWin
 * This is just a little webkit transparent window 
 * to show the splash screen
 */
class WebWin : public QWebView
{

   Q_OBJECT

   QString htmlfile;
   QString htmlFileCont;

   private:
      QHash<QString, SKHandlerFunction> _handlers;

   public:
      WebWin();
      WebWin(QString &htmlfile);
      ~WebWin();
      void closeEvent(QCloseEvent *event);
      void setHtmlFile(QString &fname);
      void setHtmlFile(QString &fname, bool borders, bool transparentbg);
      void setHtmlCont(QString cont, QUrl baseUrl, bool borders, bool transparentbg);
      void sendMessage(SKMessage &msg);
      void registerHandler(QString type, SKHandlerFunction handler);
      void toggleBorders(bool borders);
      void toggleTransparentBackground(bool transparentbg);
      QWebView* createWindow(QWebPage::WebWindowType type);
      QString msgsender;
      QUrl baseUrl;

   public slots:
     void msgFromCore(SKMessage &msg);

   signals:
     void putMessage(SKMessage &msg);
     void closingWindow();

   //  XXX Future usage
   //protected:
   //  void dragMoveEvent(QDragMoveEvent *ev);
};


class SkylivexWin : public WebWin
{

   Q_OBJECT

   public:
      SkylivexWin();
      SkylivexWin(QString &htmlfile);
      ~SkylivexWin();
      SkylivexWin* createSkyliveWindow(QString url, QWebPage::WebWindowType type);
      JSBridge* jsbridge;
      QString msgsender;

      void handle_alert(SKMessage &msg);
      void handle_notify(SKMessage &msg);
      void handle_chatreceived(SKMessage &msg);

   private slots:
      void refreshJsObject();

};


#endif
