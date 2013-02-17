TEMPLATE = lib
SOURCES = skproto.cpp ../src/ipcmsg.cpp
CONFIG += plugin
HEADERS = skproto.h ../src/ipcmsg.h
INCLUDEPATH = ../src
QT += core network widgets
DESTDIR = ../build/plugins
