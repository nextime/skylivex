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
#include <QDir>
#include <QString>
#include <QPalette>
#include <iostream>
#include "ipcmsg.h"

MainWin::MainWin(QFile &htmlfile)
      : QWebView(0)
{
   htmlfile.open(QIODevice::ReadOnly);
   htmlFileName = QString::fromUtf8(htmlfile.readAll().constData());
   QUrl baseUrl = QUrl::fromLocalFile(QDir::current().absoluteFilePath("gui/dummy.html"));

   QPalette pal = palette();
   pal.setBrush(QPalette::Base, Qt::transparent);

   setWindowFlags(Qt::FramelessWindowHint);
   page()->setPalette(pal);
   setAttribute(Qt::WA_TranslucentBackground, true);
   setAttribute(Qt::WA_OpaquePaintEvent, false);

   setHtml(htmlFileName, baseUrl);
   resize(250,200);

   page()->mainFrame()->addToJavaScriptWindowObject("SkyliveX", &jsbridge);

   registerHandler((QString)"coreStarted", &MainWin::handle_corestarted);
}

MainWin::~MainWin()
{

}

void MainWin::msgFromCore(SKMessage::SKMessage &msg)
{
   std::cout << "MainWindow msg reveived: " << msg.handle.toStdString() << std::endl;
   if(_handlers.contains(msg.handle))
   {
      SKHandlerFunction mf =_handlers[msg.handle];
      (this->*mf)(msg);
   }

}


void MainWin::sendMessage(SKMessage::SKMessage &msg)
{
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

void JSBridge::changePageContent(QString elementid, QString content)
{
   emit changeContent(elementid, content);
}

