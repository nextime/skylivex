TEMPLATE = lib
SOURCES = plugin_skeleton.cpp ../src/ipcmsg.cpp
CONFIG += plugin
HEADERS = plugin_skeleton.h ../src/ipcmsg.h
INCLUDEPATH = ../src
QT += core network widgets
DESTDIR = ../build/plugins
