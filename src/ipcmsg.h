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

#ifndef IPCMSG_H
#define IPCMSG_H

#include <QTime>
#include <QHash>
#include <QString>


// Forward declarations
class SkylivexWin;

/*
 * SKMessage
 * An object representing an IPC message
 * between threads
 */
class SKMessage
{

   public:
      SKMessage();
      SKMessage(const SKMessage &other);
      ~SKMessage();

      QTime time;
      QString sender;
      QString handle;
      SkylivexWin* webwin;
      QHash<QString, QString > parameters;

      SKMessage(QString s, QString h, QHash<QString, QString > p);
      SKMessage(QString h, QHash<QString, QString > p);
      SKMessage(QString s, QString h);
      SKMessage(QString h);
      SKMessage(QString h, SkylivexWin* win);
};


#endif
