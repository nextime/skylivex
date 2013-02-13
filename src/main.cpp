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
 * File: main.cpp
 * 
 * Purpose: Core file for the SkyliveX client. It provide core
 * functionality as the enter point of the client and
 * to load plugins and make them communicate.
 *
 */
#include <QApplication>
#include <QFile>
#include <QObject>

#include "skylivex.h"

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
     void run()
     {
         emit finished();
     }

   signals:
      void finished();
};

/*
 * Not less important than the core is the main window.
 * It is a Wekbit object that load our HTML/js gui
 * for the main window!
 */
//class MainWindow


/*
 * main loop.
 * Here lives some objects: 
 *  - the initial splash screen
 *  - the plugin messages emitter/receiver
 *  - the main window
 *  - the plugin manager
 *  - the config parser
 */
int main(int argc, char *argv[])
{

   // Start our application object
   QApplication skylivex(argc, argv);

   // Start the splash screen. also
   // the splash screen is a (transparent) webkit object
   QFile splashfile("gui/splash.html");
   SplashPage splashwin(splashfile);
   splashwin.show();
   
   // Instance of the core ITC/IPC messasing
   //SkyliveX *skx = new SkyliveX(&skylivex);

   // connect the "finished" signal coming from the ITC/IPC to the qui call
   //QObject::connect(skx, SIGNAL(finished()), &skx, SLOT(quit()));

   // and give a slot to the ITC/IPC in the main loop
   //QTimer::signalShot(0, skx, SLOT(run()));

   // and then.. go!
   return skylivex.exec();

}

