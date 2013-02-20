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
 * File: splashpage.cpp 
 * 
 * Purpose:
 *
 */
#include "mainwin.h"
#include <QWebView>
#include <QWebFrame>
#include <QFile>
#include <QUrl>
#include <QDir>
#include <QString>
#include <QPalette>
#include <QDragMoveEvent>
#include <iostream>
#include "ipcmsg.h"

#define SENDER "maingui"

MainWin::MainWin(QString &htmlfile)
      : QWebView(0)
{
   baseUrl = QUrl::fromLocalFile(QDir::current().absoluteFilePath("gui/dummy.html"));
   
   QPalette pal = palette();
   pal.setBrush(QPalette::Base, Qt::transparent);

   setWindowFlags(Qt::FramelessWindowHint);
   page()->setPalette(pal);
   setAttribute(Qt::WA_TranslucentBackground, true);
   setAttribute(Qt::WA_OpaquePaintEvent, false);
   setHtmlFile(htmlfile);
   resize(250,200);

   jsbridge.mwin=qobject_cast<MainWin *>(this);

   registerHandler((QString)"coreStarted", &MainWin::handle_corestarted);
   registerHandler((QString)"telescopeConnected", &MainWin::handle_connected);
   registerHandler((QString)"asklogin", &MainWin::handle_asklogin);
}

MainWin::~MainWin()
{

}

// XXX This can be used in future to permit
//     to drag a window borderless on the desktop.
/*
void MainWin::dragMoveEvent(QDragMoveEvent *ev)
{
 std::cout << "Drag Move Event " << ev->pos().x() << std::endl;
}
*/

void MainWin::setHtmlFile(QString &fname)
{

   QFile filename(fname);
   filename.open(QIODevice::ReadOnly);
   htmlFileCont = QString::fromUtf8(filename.readAll().constData());
   setHtml(htmlFileCont, baseUrl);
   page()->mainFrame()->addToJavaScriptWindowObject("SkyliveX", &jsbridge);

}

void MainWin::setHtmlFile(QString &fname, bool borders, bool transparentbg)
{
   toggleBorders(borders);
   toggleTransparentBackground(transparentbg);
   setHtmlFile(fname);

}

void MainWin::setHtmlCont(QString cont, QUrl baseUrl, bool borders, bool transparentbg)
{
   toggleBorders(borders);
   toggleTransparentBackground(transparentbg);
   setHtml(cont, baseUrl);
}


void MainWin::msgFromCore(SKMessage::SKMessage &msg)
{
   std::cout << "MainWindow msg reveived: " << msg.handle.toStdString() << std::endl;
   if(_handlers.contains(msg.handle) && msg.sender != SENDER)
   {
      SKHandlerFunction mf =_handlers[msg.handle];
      (this->*mf)(msg);
   }

}

void MainWin::toggleBorders(bool borders)
{

   Qt::WindowFlags flags = windowFlags();
   if(borders)
   {
     if(flags & Qt::FramelessWindowHint)
     {
        flags &= ~Qt::FramelessWindowHint;
        setWindowFlags(flags);
        show();
     }
   }
   else
   {
     if(!(flags & Qt::FramelessWindowHint))
     {
        flags &= Qt::FramelessWindowHint;
        setWindowFlags(flags);
        show();
     }
   }
}

void MainWin::toggleTransparentBackground(bool transparentbg)
{
   QPalette pal = palette();
   if(transparentbg)
   {
      pal.setBrush(QPalette::Base, Qt::transparent);
      setAttribute(Qt::WA_TranslucentBackground, true);
      setAttribute(Qt::WA_OpaquePaintEvent, false);
   }
   else
   {
      pal.setBrush(QPalette::Base, Qt::white);
      setAttribute(Qt::WA_TranslucentBackground, false);
      setAttribute(Qt::WA_OpaquePaintEvent, true);
   }
   page()->setPalette(pal);

}

void MainWin::sendMessage(SKMessage::SKMessage &msg)
{
   msg.sender=SENDER;
   emit putMessage(msg);
}

void MainWin::registerHandler(QString type, SKHandlerFunction handler)
{
  _handlers[type] = handler;

}


void MainWin::handle_corestarted(SKMessage::SKMessage &msg)
{
   msg.handle = "connectTelescopes";
   sendMessage(msg);
   jsbridge.notify("Connecting...");

}

void MainWin::handle_connected(SKMessage::SKMessage &msg)
{
   std::cout << "Connected by " << msg.sender.toStdString() << std::endl;
   jsbridge.notify("Connected");
}

void MainWin::handle_asklogin(SKMessage::SKMessage &msg)
{
   std::cout << "asklogin by " << msg.sender.toStdString() << std::endl;
   jsbridge.notify("Logging in");
   QString html("gui/login.html");
   setHtmlFile(html, true, false);
   resize(250, 200);
}  


void JSBridge::changePageContent(QString elementid, QString content)
{
   emit changeContent(elementid, content);
}

void JSBridge::pushLogin(QString username, QString password)
{
   std::cout << "pushLogin called from JS"  << std::endl;
   SKMessage::SKMessage loginmsg("putlogin");
   loginmsg.parameters.insert("username", username);
   loginmsg.parameters.insert("password", password);
   mwin->sendMessage(loginmsg);
}

void JSBridge::resizeWin(int width, int height)
{
   mwin->resize(width, height);
}

void JSBridge::toggleBorders(bool borders)
{
   mwin->toggleBorders(borders);
}

void JSBridge::toggleTransparentBackground(bool transparentbg)
{
   mwin->toggleTransparentBackground(transparentbg);
}

