TEMPLATE = lib
SOURCES = skauth.cpp ../src/ipcmsg.cpp
CONFIG += plugin
HEADERS = skauth.h ../src/ipcmsg.h
INCLUDEPATH = ../src
QT += core network widgets
DESTDIR = ../build/plugins
