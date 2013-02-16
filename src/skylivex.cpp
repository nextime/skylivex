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

#include <QPluginLoader>
#include <QApplication>
#include <QDir>
#include <QObject>
#include <QString>
#include "skylivex.h"
#include "pluginsinterfaces.h"
#include <iostream>



// Load and initialize plugins and shared memory communication
void SkyliveX::initialize()
{
   std::cout << "antani" << std::endl;
   loadPlugins();
}


// read messages from plugins and dispatch to others
void SkyliveX::process()
{
   //std::cout << "process" << std::endl;
   std::string sarca("ANTANI!");
   sendMsgToMainWin(sarca);
}


void SkyliveX::loadPlugins() 
{
   QDir pluginsDir = QDir(qApp->applicationDirPath());
   pluginsDir.cd("plugins");

   std::cout << "Try to load plugins in folder " << pluginsDir.path().toStdString() << std::endl;

   foreach(QString fileName, pluginsDir.entryList(QDir::Files)) 
   {
      std::cout << "Testing " << pluginsDir.absoluteFilePath(fileName).toStdString() << std::endl;
      QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
      QObject *plugin = loader.instance();
      if (plugin) 
      {
         std::cout << "Loading " << fileName.toStdString() << std::endl;
         initializePlugin(plugin, fileName);
         //pluginFileNames += fileName;SkylivexPluginInterface
      }
      else 
      {
         std::cout << loader.errorString().toStdString() << std::endl;
         std::cout << plugin << std::endl;
      }
  }
}

void SkyliveX::initializePlugin(QObject *plugin, QString filename) 
{
   skylivexPluginInterface = qobject_cast<SkylivexPluginInterface *>(plugin);
   if (skylivexPluginInterface)
   {
      std::cout << "Plugin file " << filename.toStdString() << " is valid." << std::endl;
      // now the plugin can be initialized and used
   }
}

void SkyliveX::sendMsgToMainWin(std::string &msg)
{
   //std::cout <<  "Send To MainWin: " << msg << std::endl;
   emit msgForMainWin(msg);
}
