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
#include "pluginsinterfaces.h"
#include "ipcmsg.h"
#include <iostream>
#include "skproto.h"



void SkyliveProtocol::startPlugin()
{
   std::cout << "SkyliveProtocol initialized in thread " << thread() << std::endl;
   registerHandler((QString)"connectTelescopes", &SkyliveProtocol::handle_connect);
}


void SkyliveProtocol::handle_connect(SKMessage::SKMessage msg)
{
   std::cout << "SkyliveProtocol connect : " << msg.handle.toStdString() << std::endl;
}

void SkyliveProtocol::receiveMessage(SKMessage::SKMessage msg)
{
   std::cout << "SkyliveProtocol msg received: " << msg.handle.toStdString() << std::endl;
}

void SkyliveProtocol::sendMessage(SKMessage::SKMessage msg)
{
   emit putMessage(msg);
}


void SkyliveProtocol::execute(SKMessage::SKMessage &msg)
{ 
  if(_handlers.contains(msg.handle))
  {
    SKHandlerFunction mf =_handlers[msg.handle];
    (this->*mf)(msg);
  }
}

void SkyliveProtocol::registerHandler(QString type, SKHandlerFunction handler)
{
  _handlers[type] = handler;
  
}

