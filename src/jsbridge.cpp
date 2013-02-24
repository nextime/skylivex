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
#include <QString>
#include <iostream>
#include "ipcmsg.h"
#include "jsbridge.h"


void JSBridge::changePageContent(QString elementid, QString content)
{
   emit changeContent(elementid, content);
}

void JSBridge::pushLogin(QString username, QString password)
{
   std::cout << "pushLogin called from JS"  << std::endl;
   SKMessage loginmsg("putlogin");
   loginmsg.parameters.insert("username", username);
   loginmsg.parameters.insert("password", password);
   wwin->sendMessage(loginmsg);
}

void JSBridge::resizeWin(int width, int height)
{
   wwin->resize(width, height);
}

void JSBridge::toggleBorders(bool borders)
{
   wwin->toggleBorders(borders);
}

void JSBridge::toggleTransparentBackground(bool transparentbg)
{
   wwin->toggleTransparentBackground(transparentbg);
}

/* a private message */
void JSBridge::chat_message_send(QString dest, QString message)
{

}

/* a public message */
void JSBridge::chat_message_send(QString message)
{
   std::cout << "public message send called from JS"  << std::endl;
   SKMessage chatmsg("publicChatSend");
   chatmsg.parameters.insert("msg", message);
   wwin->sendMessage(chatmsg);
}

void JSBridge::change_telescope(QString tele)
{
   std::cout << "Telescope change requested from JS" << std::endl;
   SKMessage msg("changeTelescope");
   msg.parameters.insert("telescope", tele);
   wwin->sendMessage(msg);

}
