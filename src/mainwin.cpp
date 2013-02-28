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
#include "webwin.h"
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

#define SENDER "maingui"


#if defined(Q_OS_MAC)
/*
 * For some reason youtube think that the default user agent 
 * on OSX doesn't support HTML5 videos, 
 * so, we change it faking the Linux one
 */
class QWebPageForMac : public QWebPage
{
   Q_OBJECT
   public:
     QWebPageForMac(WebWin* &win);
     QString userAgentForUrl(const QUrl &url) const;
};

QWebPageForMac::QWebPageForMac(WebWin* &win) : QWebPage(win)
{

}

QString QWebPageForMac::userAgentForUrl(const QUrl &url) const
{
   return "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.21 (KHTML, like Gecko) skylivex Safari/537.21";
}

#endif

MainWin::MainWin(QString &htmlfile)
      : SkylivexWin(htmlfile)
{

   registerHandler((QString)"coreStarted", (SKHandlerFunction)&MainWin::handle_corestarted);
   registerHandler((QString)"telescopeConnected", (SKHandlerFunction)&MainWin::handle_connected);
   registerHandler((QString)"asklogin", (SKHandlerFunction)&MainWin::handle_asklogin);
   registerHandler((QString)"loginok", (SKHandlerFunction)&MainWin::handle_loginres);
   registerHandler((QString)"loginfailed", (SKHandlerFunction)&MainWin::handle_loginres);

   registerHandler((QString)"openurl", (SKHandlerFunction)&MainWin::handle_openurl);
   registerHandler((QString)"youtubevideo", (SKHandlerFunction)&MainWin::handle_youtubevideo);
   registerHandler((QString)"closeyoutube", (SKHandlerFunction)&MainWin::handle_closeyoutube);

   msgsender = SENDER;
   yt_is_open=false;

}

MainWin::~MainWin()
{

}


void MainWin::handle_corestarted(SKMessage &msg)
{
   msg.handle = "connectTelescopes";
   sendMessage(msg);
   jsbridge->notify("Connecting...");

}

void MainWin::handle_connected(SKMessage &msg)
{
   std::cout << "Connected by " << msg.sender.toStdString() << std::endl;
   jsbridge->notify("Connected");
}

void MainWin::handle_asklogin(SKMessage &msg)
{
   std::cout << "asklogin by " << msg.sender.toStdString() << std::endl;
   jsbridge->notify("Logging in");
   QString html("gui/login.html");
   setHtmlFile(html, true, false);
   resize(250, 200);
}  


void MainWin::handle_loginres(SKMessage &msg)
{
   if(msg.handle=="loginok") 
   {
      QString html("gui/maingui.html");
      setHtmlFile(html, true, false);
      resize(800, 600);
   } 
   else 
   {
      std::cout << "LOGIN FAILED" << std::endl;
   }
}

void MainWin::handle_openurl(SKMessage &msg)
{
   if(msg.handle=="openurl")
   {
      if(msg.parameters.contains("url"))
      {
          std::cout << "OPEN URL " << msg.parameters["url"].toStdString() << std::endl;
         //if(msg.parameters.contains("width")
         //if(msg.parameters.contains("height);
         WebWin *wv = new WebWin;
         QWebPage *newWeb = new QWebPage(wv);

         wv->setPage(newWeb);
         wv->setAttribute(Qt::WA_DeleteOnClose, true);

         wv->setUrl(QUrl(msg.parameters["url"]));
         wv->show();

      }
   }
}

void MainWin::handle_youtubevideo(SKMessage &msg)
{
   if(msg.handle=="youtubevideo")
   {
      if(msg.parameters.contains("url"))
      {
         std::cout << "OPEN URL " << msg.parameters["url"].toStdString() << std::endl;
         //if(msg.parameters.contains("width")
         //if(msg.parameters.contains("height);
         if(!yt_is_open)
         {
            yt = new WebWin();
            yt_is_open=true;
            #if defined(Q_OS_MAC)
            QWebPageForMac *newWeb = new QWebPageForMac(yt);
            #else
            QWebPage *newWeb = new QWebPage(yt);
            #endif

            yt->setPage(newWeb);
            yt->setAttribute(Qt::WA_DeleteOnClose, true);
            yt->settings()->setAttribute(QWebSettings::PluginsEnabled, true);
            yt->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
            yt->setUrl(QUrl(msg.parameters["url"]));
            connect(yt, SIGNAL(closingWindow()), this, SLOT(ytclosesignal()));
            yt->show();
         } 
         else
         {
            yt->load(QUrl(msg.parameters["url"]));
         }

      }
   }
}

void MainWin::ytclosesignal()
{
   std::cout << "Youtube Window Closed " << std::endl; 
   yt_is_open=false;
}

void MainWin::handle_closeyoutube(SKMessage &msg)
{
   if(msg.handle=="closeyoutube")
   {
      try
      {
         if(yt_is_open)
            yt->close();
      }
      catch(int e)
      {
         std::cout << "ERROR Closing youtube window " << std::endl;
      }
   }
}

