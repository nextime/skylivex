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
#include <QTcpSocket>
#include <QtNetwork>


void SkyliveProtocol::startPlugin()
{
   SM_TCPCLIENT = HOME;
   std::cout << "SkyliveProtocol initialized in thread " << thread() << std::endl;
   registerHandler((QString)"connectTelescopes", &SkyliveProtocol::handle_connect);

   pktTimer = new QTimer();
   QObject::connect(pktTimer, SIGNAL(timeout()), this, SLOT(processPackets()));
   pktTimer->start();

}


void SkyliveProtocol::processPackets()
{
   if(!protoQueue.isEmpty())
   {
      SKProtoMsg pkt;
      pkt = protoQueue.dequeue();
      QString cmd(pkt.cmd);
      std::cout << "Packages in Queue: " << cmd.toStdString() <<std::endl;

   } else {
      if(pktTimer->isActive())
         pktTimer->stop();
   }
}

void SkyliveProtocol::readFromNetwork()
{
   char c;
   int readidx=0;
   while(tcpSocket->bytesAvailable())
   {
      if(readidx > MAX_PACKETREAD)
         return;
      tcpSocket->read(&c, 1);
      switch(SM_TCPCLIENT)
      {
         case HOME:
            break;
         case CONNECTED:
            switch(c)
            {
               case PROTO_START:
               {
                 SKProtoMsg protoMsg;
                 SM_TCPCLIENT=COMMAND;
                 break;
               }
               default:
                 break;
            }
            break;
         case COMMAND:
            protoMsg.computed_crc+=c;
            switch(c)
            {
               case CMD_END:
                  SM_TCPCLIENT=PARAMS;
                  break;
               case PROTO_START:
               {
                  SKProtoMsg protoMsg;
                  break;
               }
               case PARAM_END:
               case PROTO_END:
                  SM_TCPCLIENT=CONNECTED;
                  break;
               default:
                  protoMsg.cmd.append(c);
            }
            break;
         case PARAMS:
            protoMsg.computed_crc+=c;
            switch(c)
            {
               case PARAM_END:
                  SM_TCPCLIENT=CRC;
                  break;
               case PROTO_START:
               {
                  SKProtoMsg protoMsg;
                  SM_TCPCLIENT=COMMAND;
                  break;
               }
               case CMD_END:
               case PROTO_END:
                  SM_TCPCLIENT=CONNECTED;
                  break;
               default:
                  protoMsg.params.append(c);
            }
            break;
         case CRC:
            switch(c)
            {
               case PROTO_START:
               {
                  SKProtoMsg protoMsg;
                  SM_TCPCLIENT=COMMAND;
                  break;
               }
               case PROTO_END:
                  protoQueue.enqueue(protoMsg);
                  //processPackets();
                  if(!pktTimer->isActive())
                     pktTimer->start();
               case CMD_END:
               case PARAM_END:
                  SM_TCPCLIENT=CONNECTED;
                  break;
               default:
                  protoMsg.crc.append(c);
            }
            break;
      }
      readidx++;
   }
}

void SkyliveProtocol::handle_connect(SKMessage::SKMessage msg)
{
   authenticated=false;
   std::cout << "SkyliveProtocol connect: " << msg.handle.toStdString() << std::endl;
   tcpSocket = new QTcpSocket(this);
   connect(tcpSocket, SIGNAL(connected()), this, SLOT(clientConnected()));
   connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readFromNetwork()));
   connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayError(QAbstractSocket::SocketError)));
   tcpSocket->abort();
   tcpSocket->connectToHost(SERVERHOST, SERVERPORT);
}

void SkyliveProtocol::clientConnected()
{
   SM_TCPCLIENT = CONNECTED;
}

void SkyliveProtocol::receiveMessage(SKMessage::SKMessage msg)
{
   std::cout << "SkyliveProtocol msg received: " << msg.handle.toStdString() << std::endl;
   if(_handlers.contains(msg.handle))
   {
     SKHandlerFunction mf =_handlers[msg.handle];
     (this->*mf)(msg);
   }

}

void SkyliveProtocol::sendMessage(SKMessage::SKMessage msg)
{
   emit putMessage(msg);
}


void SkyliveProtocol::registerHandler(QString type, SKHandlerFunction handler)
{
  _handlers[type] = handler;
  
}

void SkyliveProtocol::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        std::cout << "Host not found"  << std::endl;
        break;
    case QAbstractSocket::ConnectionRefusedError:
        std::cout << "connection refused"  << std::endl;
        break;
    default:
        std::cout << "networ error: " << std::endl;
    }
    SM_TCPCLIENT = HOME;

}
