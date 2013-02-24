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
#include "webwin.h"
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
#include "jsbridge.h"

#define SENDER "webwin"

WebWin::WebWin(QString &htmlfile)
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

   jsbridge = new JSBridge();
   jsbridge->wwin = qobject_cast<WebWin *>(this);
   page()->mainFrame()->addToJavaScriptWindowObject("SkyliveX", jsbridge);


   connect(page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(refreshJsObject()));


}

WebWin::~WebWin()
{

}

// XXX This can be used in future to permit
//     to drag a window borderless on the desktop.
/*
void WebWin::dragMoveEvent(QDragMoveEvent *ev)
{
 std::cout << "Drag Move Event " << ev->pos().x() << std::endl;
}
*/

void WebWin::refreshJsObject()
{
   //page()->mainFrame()->addToJavaScriptWindowObject("SkyliveX", &jsbridge);
   jsbridge = new JSBridge();
   jsbridge->wwin = qobject_cast<WebWin *>(this);
   page()->mainFrame()->addToJavaScriptWindowObject("SkyliveX", jsbridge);

}

void WebWin::setHtmlFile(QString &fname)
{

   QFile filename(fname);
   filename.open(QIODevice::ReadOnly);
   htmlFileCont = QString::fromUtf8(filename.readAll().constData());
   setHtml(htmlFileCont, baseUrl);
   //page()->mainFrame()->addToJavaScriptWindowObject("SkyliveX", &jsbridge);
   //jsbridge.mwin=qobject_cast<WebWin *>(this);
}

void WebWin::setHtmlFile(QString &fname, bool borders, bool transparentbg)
{
   toggleBorders(borders);
   toggleTransparentBackground(transparentbg);
   setHtmlFile(fname);

}

void WebWin::setHtmlCont(QString cont, QUrl baseUrl, bool borders, bool transparentbg)
{
   toggleBorders(borders);
   toggleTransparentBackground(transparentbg);
   setHtml(cont, baseUrl);
}


void WebWin::msgFromCore(SKMessage &msg)
{
   std::cout << "WebWindow msg reveived: " << msg.handle.toStdString() << std::endl;
   if(_handlers.contains(msg.handle) && msg.sender != SENDER)
   {
      SKHandlerFunction mf =_handlers[msg.handle];
      (this->*mf)(msg);
   }

}

void WebWin::toggleBorders(bool borders)
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

void WebWin::toggleTransparentBackground(bool transparentbg)
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

void WebWin::sendMessage(SKMessage &msg)
{
   msg.sender=SENDER;
   emit putMessage(msg);
}

void WebWin::registerHandler(QString type, SKHandlerFunction handler)
{
  _handlers[type] = handler;

}

