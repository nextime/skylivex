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
#include <QWebPage>
#include <QWebFrame>
#include <QFile>
#include <QUrl>
#include <QList>
#include <QDir>
#include <QString>
#include <QPalette>
#include <QDragMoveEvent>
#include <iostream>
#include "ipcmsg.h"
#include "jsbridge.h"
#include "QWebSettings"

#define SENDER "webwin"

WebWin::WebWin(QString &htmlfile)
      : QWebView(0)
{
   baseUrl = QUrl::fromLocalFile(QDir::current().absoluteFilePath("gui/dummy.html"));
   
   settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
   settings()->setAttribute(QWebSettings::JavascriptCanCloseWindows, true);
   //settings->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);

   QPalette pal = palette();
   pal.setBrush(QPalette::Base, Qt::transparent);

   setWindowFlags(Qt::FramelessWindowHint);
   page()->setPalette(pal);
   setAttribute(Qt::WA_TranslucentBackground, true);
   setAttribute(Qt::WA_OpaquePaintEvent, false);
   setHtmlFile(htmlfile);
   resize(250,200);


   msgsender = SENDER;


}

WebWin::WebWin()
      : QWebView()
{
   baseUrl = QUrl::fromLocalFile(QDir::current().absoluteFilePath("gui/dummy.html"));
   settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
   settings()->setAttribute(QWebSettings::JavascriptCanCloseWindows, true);
   msgsender = SENDER;
}

WebWin::~WebWin()
{

}

QWebView* WebWin::createWindow(QWebPage::WebWindowType type)
{
   WebWin *wv = new WebWin;
   QWebPage *newWeb = new QWebPage(wv);
   wv->setPage(newWeb);
   wv->setAttribute(Qt::WA_DeleteOnClose, true);
   if (type == QWebPage::WebModalDialog)
      wv->setWindowModality(Qt::ApplicationModal);
   wv->show();
   return wv;
}


// XXX This can be used in future to permit
//     to drag a window borderless on the desktop.
/*
void WebWin::dragMoveEvent(QDragMoveEvent *ev)
{
 std::cout << "Drag Move Event " << ev->pos().x() << std::endl;
}
*/

void WebWin::setHtmlFile(QString &fname)
{

   QFile filename(fname);
   filename.open(QIODevice::ReadOnly);
   htmlFileCont = QString::fromUtf8(filename.readAll().constData());
   setHtml(htmlFileCont, baseUrl);
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
   if(_handlers.contains(msg.handle) && msg.sender != msgsender)
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
   msg.sender = msgsender;
   emit putMessage(msg);
}

void WebWin::registerHandler(QString type, SKHandlerFunction handler)
{
  _handlers[type] = handler;

}


SkylivexWin::SkylivexWin(QString &htmlfile)
      : WebWin(htmlfile)
{

   msgsender = "SkylivexWin";
   
   jsbridge = new JSBridge();
   jsbridge->wwin = qobject_cast<SkylivexWin *>(this);
   page()->mainFrame()->addToJavaScriptWindowObject("SkyliveX", jsbridge);

   connect(page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(refreshJsObject()));

   registerHandler((QString)"alert", (SKHandlerFunction)&SkylivexWin::handle_alert);
   registerHandler((QString)"notify", (SKHandlerFunction)&SkylivexWin::handle_notify);
   registerHandler((QString)"publicchatrcv", (SKHandlerFunction)&SkylivexWin::handle_chatreceived);


}

SkylivexWin::SkylivexWin()
      : WebWin()
{
   jsbridge = new JSBridge();
   jsbridge->wwin = qobject_cast<SkylivexWin *>(this);
   page()->mainFrame()->addToJavaScriptWindowObject("SkyliveX", jsbridge);

   connect(page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(refreshJsObject()));
   msgsender = SENDER;

   registerHandler((QString)"alert", (SKHandlerFunction)&SkylivexWin::handle_alert);
   registerHandler((QString)"notify", (SKHandlerFunction)&SkylivexWin::handle_notify);
   registerHandler((QString)"publicchatrcv", (SKHandlerFunction)&SkylivexWin::handle_chatreceived);


}

SkylivexWin* SkylivexWin::createSkyliveWindow(QString url, QWebPage::WebWindowType type)
{
   SkylivexWin *wv = new SkylivexWin;
   QWebPage *newWeb = new QWebPage(wv);
   wv->jsbridge = new JSBridge();
   wv->jsbridge->wwin = qobject_cast<SkylivexWin *>(wv);
   newWeb->mainFrame()->addToJavaScriptWindowObject("SkyliveX", wv->jsbridge);

   connect(newWeb->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), wv, SLOT(refreshJsObject()));

   wv->setPage(newWeb);
   wv->setAttribute(Qt::WA_DeleteOnClose, true);
   if (type == QWebPage::WebModalDialog)
      wv->setWindowModality(Qt::ApplicationModal);
   QList<QString> urilist = url.split("://");
   if(urilist.size() <= 1)
   {
      QString uristring = baseUrl.toString().replace("dummy.html", "");
      url.prepend(uristring);
      std::cout << "transform uri in local file " << url.toStdString() << std::endl;
   }
   wv->setUrl(QUrl(url));
   wv->show();
   return wv;
}




SkylivexWin::~SkylivexWin()
{

}

void SkylivexWin::refreshJsObject()
{
   jsbridge = new JSBridge();
   jsbridge->wwin = qobject_cast<SkylivexWin *>(this);
   page()->mainFrame()->addToJavaScriptWindowObject("SkyliveX", jsbridge);

}


void SkylivexWin::handle_alert(SKMessage &msg)
{
   if(msg.parameters.contains("msg"))
      jsbridge->alertmsg(msg.parameters["msg"]);
}

void SkylivexWin::handle_notify(SKMessage &msg)
{
   if(msg.parameters.contains("msg"))
      jsbridge->notify(msg.parameters["msg"]);
}

void SkylivexWin::handle_chatreceived(SKMessage &msg)
{
   if(msg.handle=="publicchatrcv")
   {
      if(msg.parameters.contains("msg") && msg.parameters.contains("username"))
      {
         jsbridge->public_received(msg.parameters["username"], msg.parameters["msg"]);
      }
   }
}

