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
typedef void (MainWin::*SKHandlerFunction)(SKMessage&);
//typedef int (MainWin::*SKHandlerFunction)(SKMessage&);

/*
 * class JSBridge
 * This class connect the javascript object SkyliveX
 * to c++
 */
class JSBridge : public QObject
{
   Q_OBJECT

   public:
      MainWin* mwin;
      void changePageContent(QString elementid, QString content);

   signals:
      void changeContent(QString elementid, QString content);
      void notify(QString content);
      void alert(QString content);

   public slots:
      void pushLogin(QString username, QString password);
      void resizeWin(int width, int height);
      void toggleBorders(bool borders);
      void toggleTransparentBackground(bool transparentbg);

};


/*
 * class MainWin
 * This is just a little webkit transparent window 
 * to show the splash screen
 */
class MainWin : public QWebView
{

   Q_OBJECT

   QUrl baseUrl;
   QString htmlfile;
   QString htmlFileCont;
   JSBridge jsbridge;

   private:
      QHash<QString, SKHandlerFunction> _handlers;
      void setHtmlFile(QString &fname);
      void setHtmlFile(QString &fname, bool borders, bool transparentbg);
      void setHtmlCont(QString cont, QUrl baseUrl, bool borders, bool transparentbg);

   public:
      MainWin(QString &htmlfile);
      ~MainWin();
      void sendMessage(SKMessage &msg);
      void registerHandler(QString type, SKHandlerFunction handler);
      void handle_corestarted(SKMessage &msg);
      void handle_connected(SKMessage &msg);
      void handle_asklogin(SKMessage &msg);
      void handle_alert(SKMessage &msg);
      void handle_notify(SKMessage &msg);
      void handle_loginres(SKMessage &msg);
      void toggleBorders(bool borders);
      void toggleTransparentBackground(bool transparentbg);

   public slots:
     void msgFromCore(SKMessage &msg);

   signals:
     void putMessage(SKMessage &msg);

   //  XXX Future usage
   //protected:
   //  void dragMoveEvent(QDragMoveEvent *ev);
};


#endif
