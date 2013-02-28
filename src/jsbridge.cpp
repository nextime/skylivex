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
 * File: jsbridge.cpp 
 * 
 * Purpose:
 * Define an interface between javascript in the 
 * Webkit based gui and the c++ based core of the application
 *
 */
#include <QString>
#include <QWebPage>
#include <iostream>
#include "ipcmsg.h"
#include "jsbridge.h"


/*
 * Method: changePageContent
 *
 * Arguments:
 *   - Qstring elementid: a string with an element id in the html
 *   - QString content: this will be the .innerHTML content of the
 *                      element we want to change
 *
 * This method give our core a way to change directly the content
 * of an HTML node by a javascript call like
 * document.getElementById(elementid).innerHTML=content;
 * in javascript
 */
void JSBridge::changePageContent(QString elementid, QString content)
{
   emit changeContent(elementid, content);
}


/*
 * Method: pushLogin
 *
 * Arguments: 
 *   - Qstring username: the username passed to the login form
 *   - QString password: the password passed to the login form
 *
 * This method is to be called on the javascript side
 * to communicate to the core the login info compiled
 * by the user on the login form
 *
 * It will push an IPC message to the core and other plugins
 */
void JSBridge::pushLogin(QString username, QString password)
{
   std::cout << "pushLogin called from JS"  << std::endl;
   SKMessage loginmsg("putlogin");
   loginmsg.parameters.insert("username", username);
   loginmsg.parameters.insert("password", password);
   wwin->sendMessage(loginmsg);
}

/*
 * Method: resizeWin
 *
 * Arguments:
 *   - int width: an integer for the window width
 *   - int height: an integer for the window height
 *
 * This method is to be called from the javascript
 * side to make the GUI window resize to the desidered size
 */
void JSBridge::resizeWin(int width, int height)
{
   std::cout << "resizeWin called from JS"  << std::endl;
   wwin->resize(width, height);
}

/*
 * Method: toggleBorders
 *
 * Arguments: 
 *   - bool borders: a boolean value defining if we want decorators on 
 *                   the gui window
 *
 * With this method we give javascript a way to enable or disable window
 * decorators
 */
void JSBridge::toggleBorders(bool borders)
{
   wwin->toggleBorders(borders);
}

/*
 * Method: toggleTransparentBackground
 *
 * Arguments: 
 *   - bool transparentbg: a boolean value defining if we want
 *                         the GUI window have a transparent
 *                         background
 *
 * This method give the javascript side a way to toggle the
 * background of the window transparent or not
 */
void JSBridge::toggleTransparentBackground(bool transparentbg)
{
   wwin->toggleTransparentBackground(transparentbg);
}

/*
 * Method: chat_message_send
 *
 * Arguments:
 *   - QString dest: the username destination of the chat message
 *   - Qstring message: the private chat message
 *
 * This method is to be called from javascript to send a private
 * message in chat to a specified user
 */
void JSBridge::chat_message_send(QString dest, QString message)
{

}

/*
 * Method: chat_message_send
 *
 * Arguments:
 *   - QString message: the chat message to be sent
 *
 * This method is to be called from the javascript
 * to send a public message on chat, on the active channel/telescope
 */
void JSBridge::chat_message_send(QString message)
{
   std::cout << "public message send called from JS"  << std::endl;
   SKMessage chatmsg("publicChatSend");
   chatmsg.parameters.insert("msg", message);
   wwin->sendMessage(chatmsg);
}

/*
 * Method: change_telescope
 *
 * Arguments:
 *   - QString tele: the name of the telescope we ask to change to
 *
 * This method, called from the javascript side, permit to change
 * the active telescope
 */
void JSBridge::change_telescope(QString tele)
{
   std::cout << "Telescope change requested from JS" << std::endl;
   SKMessage msg("changeTelescope");
   msg.parameters.insert("telescope", tele);
   wwin->sendMessage(msg);

}

/*
 * Method: open_window
 *
 * Arguments:
 *   - Qstring url: this contain the URL to be opened in new window
 *   - bool Modal: if true, the new window is modal
 *
 * This method, called from the javascript side, permit
 * to open a new window to an URL (local or remote) that
 * will have the JSBridge accessible.
 *
 * This needs to be used carefully and only on strictly trusted
 * URL ( better if only on local one! ), cause make
 * an untrusted site the permission to access to our core
 * by using the JSBridge object can be (IS!) a security issue.
 */
SkylivexWin* JSBridge::open_window(QString url, bool Modal)
{
   if(Modal)
      return wwin->createSkyliveWindow(url, QWebPage::WebModalDialog);
   else
      return wwin->createSkyliveWindow(url, QWebPage::WebBrowserWindow);
}

