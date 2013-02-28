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
 * File: mainwin.cpp 
 * 
 * Purpose:
 * This define special methods for
 * the mainwindow
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

/* Give a name for the IPC messages to this window */
#define SENDER "maingui"


/* 
 * Method: MainWin
 *
 * Arguments:
 *   - QString &htmlfile
 *
 * This method initialize the mainwindow object
 * pointing it to a local html file, and register
 * the handlers for the IPC messages that need to be
 * handled from the main window.
 *
 * This class is derived from the SkylivexWin class,
 * so, this window object will have a JSBridge
 * and can communicate bidirectionally with the 
 * HTML/javascript side
 *
 * This will be the primary GUI window, and it will also
 * be the father of all others subwindow.
 */
MainWin::MainWin(QString &htmlfile)
      : SkylivexWin(htmlfile)
{

   // Handle the client initialization stage
   registerHandler((QString)"coreStarted", (SKHandlerFunction)&MainWin::handle_corestarted);

   // Called when the skylive server is connected ( in TCP meaning of connected )
   registerHandler((QString)"telescopeConnected", (SKHandlerFunction)&MainWin::handle_connected);

   // called when we have a login request from the server or from a plugin
   registerHandler((QString)"asklogin", (SKHandlerFunction)&MainWin::handle_asklogin);

   // Called when a login succeed
   registerHandler((QString)"loginok", (SKHandlerFunction)&MainWin::handle_loginres);

   // Called when a login fail
   registerHandler((QString)"loginfailed", (SKHandlerFunction)&MainWin::handle_loginres);

   // Called when a plugin ask us to open an URL, for example for skylive Slides
   registerHandler((QString)"openurl", (SKHandlerFunction)&MainWin::handle_openurl);

   // Called when a plugin ask us to open the special youtube page window
   registerHandler((QString)"youtubevideo", (SKHandlerFunction)&MainWin::handle_youtubevideo);

   // Called when a plugin ask us to close the special youtube page window
   registerHandler((QString)"closeyoutube", (SKHandlerFunction)&MainWin::handle_closeyoutube);

   // Set the IPC messages sender name
   msgsender = SENDER;

   // At initialization, the special youtube page window is closed
   yt_is_open=false;

}

/* Destructor */
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
            //#if defined(Q_OS_MAC)
            //QWebPageForMac *newWeb = new QWebPageForMac(yt);
            //#else
            QWebPage *newWeb = new QWebPage(yt);
            //#endif

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

