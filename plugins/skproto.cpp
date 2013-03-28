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
#include <QByteArray>
#include <QtNetwork>
#include <QChar>

void SkyliveProtocol::startPlugin()
{
   SM_TCPCLIENT = HOME;
   cver=CLIENTVERSION;
   std::cout << "SkyliveProtocol initialized in thread " << thread() << std::endl;
   registerHandler((QString)"connectTelescopes", &SkyliveProtocol::handle_connect);
   registerHandler((QString)"putlogin", &SkyliveProtocol::handle_putlogin);
   registerHandler((QString)"publicChatSend", &SkyliveProtocol::handle_publicsend);
   registerHandler((QString)"changeTelescope", &SkyliveProtocol::handle_changetelescope);

   //pktTimer = new QTimer();
   //QObject::connect(pktTimer, SIGNAL(timeout()), this, SLOT(processPackets()));
   //pktTimer->start();


}

void SkyliveProtocol::pluginKicked()
{
   pktTimer = new QTimer();
   QObject::connect(pktTimer, SIGNAL(timeout()), this, SLOT(processPackets()));
   pktTimer->start();
}

void SkyliveProtocol::sendPacket(QString &cmd, QList<QString> &paramlist)
{
 
   QString params;
   for(int i=0;i<paramlist.size();i++)
   {
      if(i>0)
         params.append(PARAM_SEPARATOR);
      params.append(paramlist[i]);
   }
   sendPacket(cmd, params);

}

void SkyliveProtocol::sendPacket(const char* cmd, const char* params)
{
   QString pcmd(cmd);
   QString pparams(params);
   sendPacket(pcmd, pparams);
}

void SkyliveProtocol::sendPacket(QString &cmd, QString &params)
{
   SKProtoMsg pkt;
   pkt.cmd=cmd;
   pkt.params=params;
   sendPacket(pkt);
}

void SkyliveProtocol::sendPacket(SKProtoMsg &pkt)
{

   QByteArray skpkt;
   if(tcpSocket->isValid())
   {
      skpkt.append(PROTO_START);
      skpkt.append(pkt.cmd);
      skpkt.append(CMD_END);
      skpkt.append(pkt.params);
      pkt.computed_crc=0;
      for(int i=1;i<skpkt.size();i++)
      {
         pkt.computed_crc+=static_cast<int>(skpkt[i]);
      }
      skpkt.append(PARAM_END);
      skpkt.append(QString::number(pkt.computed_crc));
      skpkt.append(PROTO_END);
      tcpSocket->write(skpkt);
      std::cout << "Packet sent: " << pkt.cmd.toStdString() <<std::endl;
   } else {
      std::cout << "Cannote send packet: " << pkt.cmd.toStdString() <<std::endl;
   }
}


void SkyliveProtocol::processPackets()
{
   if(!protoQueue.isEmpty())
   {
      SKProtoMsg pkt;
      pkt = protoQueue.dequeue();
      std::cout << "Packet in Queue CRC " << pkt.crc.toInt() << " computed CRC " << pkt.computed_crc << std::endl;
      if(pkt.crc.toInt()==pkt.computed_crc)
      {
         std::cout << "Packet CRC OK command: " << pkt.cmd.toStdString() << " Params: " << pkt.params.toStdString()  <<std::endl;
         if(pkt.cmd=="LOGIN")
         {
            SKMessage msg("getlogin");
            sendMessage(msg);

         } 
         else if(pkt.cmd=="PING")
         {
            sendPacket("PONG", "NIL");
         }
         else if(pkt.cmd=="CPUBLIC")
         {
            QList<QString> paramlist = pkt.params.split(PARAM_SEPARATOR);
            if(paramlist.size() > 1) // For safety
            {
               SKMessage pmsg("publicchatrcv");
               pmsg.parameters.insert("username", QByteArray::fromPercentEncoding(paramlist[0].toLocal8Bit()));
               pmsg.parameters.insert("msg", QByteArray::fromPercentEncoding(paramlist[1].toLocal8Bit()));
               sendMessage(pmsg);
            }
         }
         else if(pkt.cmd=="CPRIVAT")
         {

         } 
         else if(pkt.cmd=="STATUS")
         {
               
         }
         else if(pkt.cmd=="CLIST")
         {
            SKMessage pmsg("userlist");
            QList<QString> paramlist = pkt.params.split(PARAM_SEPARATOR);
            QList<QString> admins;
            QList<QString> enableds;
            QList<QString> users;
            for(int i=0;i<paramlist.size();i++)
            {
               if(paramlist[i].length() > 2)
               {
                  if(paramlist[i].startsWith("!"))
                     admins.append(paramlist[i].remove(0, 1));
                  else if(paramlist[i].startsWith("@"))
                     enableds.append(paramlist[i].remove(0, 1));
                  else
                     users.append(paramlist[i].remove(0, 1));
               }
            }
            pmsg.listparams.insert("admins", admins);
            pmsg.listparams.insert("enableds", enableds);
            pmsg.listparams.insert("users", users);
            pmsg.parameters.insert("type", "all");
            sendMessage(pmsg);

         }
         else if(pkt.cmd=="ULIST")
         {
            SKMessage pmsg("userlist");
            QList<QString> paramlist = pkt.params.split(PARAM_SEPARATOR);
            if(paramlist[0].length() > 2)
            {
               if(paramlist[1]=="IN")
                  pmsg.parameters.insert("type", "in");
               else
                  pmsg.parameters.insert("type", "out");

               if(paramlist[0].startsWith("!"))
                  pmsg.parameters.insert("usertype", "admin");
               else if(paramlist[0].startsWith("@"))
                  pmsg.parameters.insert("usertype", "enabled");
               else
                  pmsg.parameters.insert("usertype", "user");
               pmsg.parameters.insert("user", paramlist[0].remove(0, 1));
               sendMessage(pmsg);

            }


         }
         else if(pkt.cmd=="ENABLE")
         {
            SKMessage loginmsg("loginfailed");
            if(pkt.params=="USERERRATO")
            {
               loginmsg.parameters.insert("why", "wronguser");  
            }
            else if(pkt.params=="ADMIN")
            {
               loginmsg.handle="loginok";
               loginmsg.parameters.insert("level", "admin");

            }
            else if(pkt.params=="ENABLED")
            {
               loginmsg.handle="loginok";
               loginmsg.parameters.insert("level", "enabled");
            }
            else if(pkt.params=="GUEST")
            {
               loginmsg.handle="loginok";
               loginmsg.parameters.insert("level", "guest");
            }
            else
            {
               loginmsg.parameters.insert("why", "unknown");
            }
            loginmsg.parameters.insert("response", pkt.params);
            sendMessage(loginmsg);
         }
         else if(pkt.cmd=="SERMES")
         {
            QList<QString> paramlist = pkt.params.split(PARAM_SEPARATOR);
            if(paramlist[0]=="ALERT")
            {
               if(paramlist.size() > 1) // For safety
               {
                  SKMessage alertmsg("alert");
                  alertmsg.parameters.insert("msg", QByteArray::fromPercentEncoding(paramlist[1].toLocal8Bit()));
                  sendMessage(alertmsg);
               }
            }
         }
         else if(pkt.cmd=="OPENYOUTUBE")
         {
            QList<QString> paramlist = pkt.params.split(PARAM_SEPARATOR);
            if(paramlist.size()==3)
            {

               SKMessage mmsg("youtubevideo");
               #if defined(Q_OS_WIN)
               QString  yt("http://www.youtube.com/embed/");
               yt.append(paramlist[0]);
               yt.append("?autoplay=1&start=");
               yt.append(paramlist[1]);
               #else
               //SKMessage mmsg("openurl");
               QString  yt("http://www.youtube.com/embed/");
               yt.append(paramlist[0]);
               yt.append("?html5=1&autoplay=1&start=");
               yt.append(paramlist[1]);
               #endif 
               mmsg.parameters.insert("url", yt);
               mmsg.parameters.insert("volume", paramlist[2]);
               sendMessage(mmsg);
            }
         }
         else if(pkt.cmd=="CLOSEYOUTUBE")
         {
            SKMessage mmsg("closeyoutube");
            QList<QString> paramlist = pkt.params.split(PARAM_SEPARATOR);
            mmsg.parameters.insert("when", paramlist[0]);
            sendMessage(mmsg);
         }
         else if(pkt.cmd=="IMAGE")
         {
            QList<QString> paramlist = pkt.params.split(PARAM_SEPARATOR);
            if(paramlist.size()==3)
            {
               SKMessage mmsg("openurl");
               mmsg.parameters.insert("url", QByteArray::fromPercentEncoding(paramlist[0].toLocal8Bit()));
               mmsg.parameters.insert("width", paramlist[1]);
               mmsg.parameters.insert("height", paramlist[2]);
               sendMessage(mmsg);
            }


         }
         else 
         {
            std::cout << "Unknown command from server" <<std::endl;
         }
         
      } else {
         std::cout << "Packet in Queue has invalid CRC. Discard it" <<std::endl;
      }
   } else {
      if(pktTimer->isActive())
         pktTimer->stop();
   }
}

void SkyliveProtocol::clearPkt()
{
   protoMsg.computed_crc=0;
   protoMsg.cmd.clear();
   protoMsg.params.clear();
   protoMsg.crc.clear();
}

void SkyliveProtocol::readFromNetwork()
{
   char c;
   int readidx=0;
   while(tcpSocket->bytesAvailable())
   {
      if(readidx > MAX_PACKETREAD)
      {
         std::cout << "MAX_PACKETREAD REACHED" << std::endl;
         return;
      }
      tcpSocket->read(&c, 1);
      //std::cout << "Read Byte: " << c <<  std::endl;
      switch(SM_TCPCLIENT)
      {
         case HOME:
            break;
         case CONNECTED:
            switch(c)
            {
               case PROTO_START:
                 clearPkt();
                 SM_TCPCLIENT=COMMAND;
                 break;
               default:
                 break;
            }
            break;
         case COMMAND:
            protoMsg.computed_crc+=static_cast<int>(c);
            switch(c)
            {
               case CMD_END:
                  SM_TCPCLIENT=PARAMS;
                  break;
               case PROTO_START:
                  clearPkt();
                  break;
               case PARAM_END:
               case PROTO_END:
                  SM_TCPCLIENT=CONNECTED;
                  break;
               default:
                  protoMsg.cmd.append(c);
            }
            break;
         case PARAMS:
            protoMsg.computed_crc+=static_cast<int>(c);
            switch(c)
            {
               case PARAM_END:
                  /*
                   * Bad. Bad. BAD! there is a fucking one message,
                   * the "IMAGE" one, that isn't standard and 
                   * doesn't uriencode the URL of the image.
                   * So, we need to manage this special case.
                   */
                  if(protoMsg.params=="http" && protoMsg.cmd=="IMAGE")
                  {
                     protoMsg.params.append(c);
                     break;
                  }

                  // Ok it isn't the "fucking special case"
                  SM_TCPCLIENT=CRC;
                  protoMsg.computed_crc-=static_cast<int>(c);
                  break;
               case PROTO_START:
                  clearPkt();
                  SM_TCPCLIENT=COMMAND;
                  break;
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
                 clearPkt();
                  SM_TCPCLIENT=COMMAND;
                  break;
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

void SkyliveProtocol::handle_connect(SKMessage msg)
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

void SkyliveProtocol::handle_putlogin(SKMessage msg)
{
   
   QString cmd("LOGIN");
   QList<QString> paramlist;
   paramlist.append(msg.parameters["username"]);
   paramlist.append(msg.parameters["password"]);
   paramlist.append(cver);
   sendPacket(cmd, paramlist);

}


void SkyliveProtocol::handle_publicsend(SKMessage msg)
{
   if(msg.parameters.size() > 0)
   {
      QString cmd("CPUBLIC");
      QList<QString> paramlist;
      QByteArray message(msg.parameters["msg"].toLocal8Bit());
      paramlist.append(message.toPercentEncoding());
      sendPacket(cmd, paramlist);
   }
}

void SkyliveProtocol::handle_changetelescope(SKMessage msg)
{
   if(msg.parameters.size() > 0)
   {
      QString cmd("CHTELE");
      QList<QString> paramlist;
      paramlist.append(msg.parameters["telescope"]);
      sendPacket(cmd, paramlist);
   }
}


void SkyliveProtocol::clientConnected()
{
   SM_TCPCLIENT = CONNECTED;
   SKMessage msg("telescopeConnected");
   sendMessage(msg);

}

void SkyliveProtocol::receiveMessage(SKMessage msg)
{
   std::cout << "SkyliveProtocol msg received: " << msg.handle.toStdString() << std::endl;
   if(_handlers.contains(msg.handle) && msg.sender!=SENDER)
   {
     SKHandlerFunction mf =_handlers[msg.handle];
     (this->*mf)(msg);
   }

}

void SkyliveProtocol::sendMessage(SKMessage msg)
{
   msg.sender=SENDER;
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

