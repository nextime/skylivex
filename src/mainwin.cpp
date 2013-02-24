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

MainWin::MainWin(QString &htmlfile)
      : WebWin(htmlfile)
{

   registerHandler((QString)"coreStarted", (SKHandlerFunction)&MainWin::handle_corestarted);
   registerHandler((QString)"telescopeConnected", (SKHandlerFunction)&MainWin::handle_connected);
   registerHandler((QString)"asklogin", (SKHandlerFunction)&MainWin::handle_asklogin);
   registerHandler((QString)"alert", (SKHandlerFunction)&MainWin::handle_alert);
   registerHandler((QString)"notify", (SKHandlerFunction)&MainWin::handle_notify);
   registerHandler((QString)"loginok", (SKHandlerFunction)&MainWin::handle_loginres);
   registerHandler((QString)"loginfailed", (SKHandlerFunction)&MainWin::handle_loginres);
   registerHandler((QString)"publicchatrcv", (SKHandlerFunction)&MainWin::handle_chatreceived);


   msgsender = SENDER;

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


void MainWin::handle_alert(SKMessage &msg)
{
   if(msg.parameters.contains("msg"))
      jsbridge->alertmsg(msg.parameters["msg"]);
}

void MainWin::handle_notify(SKMessage &msg)
{
   if(msg.parameters.contains("msg"))
      jsbridge->notify(msg.parameters["msg"]);
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

void MainWin::handle_chatreceived(SKMessage &msg)
{
   if(msg.handle=="publicchatrcv")
   {
      if(msg.parameters.contains("msg") && msg.parameters.contains("username"))
      {
         jsbridge->public_received(msg.parameters["username"], msg.parameters["msg"]);
      }
   }
}

