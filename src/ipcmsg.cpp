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
 * File: 
 * 
 * Purpose:
 *
 */

#include <QTime>
#include <QHash>
#include <QString>
#include <iostream>
#include "ipcmsg.h"
#include "pluginsinterfaces.h"

SKMessage::SKMessage(QString s, QString h, QHash<QString, QString > p)
{
   handle=h;
   sender=s;
   parameters=p;
   time = QTime::currentTime();  
   std::cout << "SKMessage initialized "  << handle.toStdString() << std::endl;
}

SKMessage::SKMessage(QString h, QHash<QString, QString > p)
{
   sender= QString("unknown");
   time = QTime::currentTime();
   parameters = p;
   handle = h;
}

SKMessage::SKMessage(QString h) 
{
   sender= QString("unknown");
   handle = h;
   time = QTime::currentTime();
}

SKMessage::SKMessage()
{
   sender= QString("unknown");
   handle = QString("none");
   time = QTime::currentTime();

}

SKMessage::~SKMessage()
{
}

SKMessage::SKMessage(const SKMessage &other)
{
   handle=other.handle;
   sender=other.sender;
   parameters=other.parameters;
   time=other.time;
}

SKHandlers::SKHandlers()
{
}

void SKHandlers::execute(SKMessage::SKMessage &msg)
{
  //if(_handlers.contains(msg.handle))
  //   _handlers[msg.handle](msg);

}

void SKHandlers::registerHandler(QString type, SKHandlerFunction handler)
{
  _handlers[type] = handler;

}

SKHandlers::~SKHandlers()
{
}
