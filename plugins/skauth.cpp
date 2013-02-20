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
#include "skauth.h"

void SkyliveAuth::startPlugin()
{
   std::cout << "SkyliveAuth initialized in thread " << thread() << std::endl;

   registerHandler((QString)"getlogin", &SkyliveAuth::handle_getlogin);
}


void SkyliveAuth::receiveMessage(SKMessage::SKMessage msg)
{
   std::cout << "SkyliveAuth msg received: " << msg.handle.toStdString() << std::endl;
   if(_handlers.contains(msg.handle) && msg.sender != SENDER)
   {
     SKHandlerFunction mf =_handlers[msg.handle];
     (this->*mf)(msg);
   }    

}


void SkyliveAuth::sendMessage(SKMessage::SKMessage msg)
{
   msg.sender=SENDER;
   emit putMessage(msg);
}

void SkyliveAuth::registerHandler(QString type, SKHandlerFunction handler)
{
  _handlers[type] = handler;
   
}  

void SkyliveAuth::handle_getlogin(SKMessage::SKMessage msg)
{
   std::cout << "Auth module handle Login by " << msg.sender.toStdString() << std::endl;
   /*
    * XXX: This is, for the moment, a dummy plugin.
    * Here we should check if we have a saved user and pass,
    * and ask the main process only if we donesn't yet have one
    * or if the user has logged out.
    */
    SKMessage::SKMessage smsg("asklogin");
    sendMessage(smsg);

    
}

