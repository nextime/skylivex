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
#ifndef PLUGINSINTERFACES_H
#define PLUGINSINTERFACES_H
#include <QtPlugin>

QT_BEGIN_NAMESPACE
class QStringList;
class QObject;
QT_END_NAMESPACE

class SkylivexPluginInterface
{
   public: 
      virtual ~SkylivexPluginInterface() {}
      virtual QStringList importer() const = 0;
      virtual void startPlugin() = 0;
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(SkylivexPluginInterface,
                    "com.skylivex.SkylivexPlugin/1.0")
QT_END_NAMESPACE
#endif
