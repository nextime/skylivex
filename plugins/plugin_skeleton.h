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
#ifndef SKAUTH_H
#define SKAUTH_H

#define SENDER "plugin_skeleton"

#include <QObject>
#include <QHash>
#include <QString>
#include <QtPlugin>
#include "pluginsinterfaces.h"
#include "ipcmsg.h"

class SkylivePluginSkeleton;
typedef void (SkylivePluginSkeleton::*SKHandlerFunction)(SKMessage);

class SkylivePluginSkeleton : public QObject, SkylivexPluginInterface
{
   Q_OBJECT
   Q_PLUGIN_METADATA(IID "com.skylivex.SkylivexPlugin/1.0" FILE "plugin_skeleton.json")
   Q_INTERFACES(SkylivexPluginInterface)

   private:
      QHash<QString, SKHandlerFunction> _handlers;

   public:
      void startPlugin();
      void sendMessage(SKMessage msg);
      void registerHandler(QString type, SKHandlerFunction handler);
      void handle_something(SKMessage msg);



   public slots:
      void receiveMessage(SKMessage msg);
      void pluginKicked();
   signals:
      void putMessage(SKMessage  msg);

};

#endif
