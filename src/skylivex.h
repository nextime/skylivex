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
 * File: skylivex.h
 * 
 * Purpose:
 *
 */
#ifndef SKYLIVEX_H
#define SKYLIVEX_H

#include <QObject>


/*
 * class SkyliveX
 * This is the core of the SkyliveX client.
 * Here the inter-thread and inter-process communication
 * with plugins is done, so, the magic happen here
 */
class SkyliveX : public QObject
{
   Q_OBJECT

   public:
     SkyliveX(QObject *parent=0) : QObject(parent) {}
     ~SkyliveX() {}

   public slots:
     void initialize();
     void process();

   signals:
      void finished();
};

#endif
