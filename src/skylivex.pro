SOURCES = main.cpp \
          webwin.cpp \
          mainwin.cpp  \
          skylivex.cpp \
          ipcmsg.cpp \
          jsbridge.cpp

HEADERS = skylivex.h \
          webwin.h \
          mainwin.h \ 
          pluginsinterfaces.h \
          ipcmsg.h \
          sksettings.h \
          jsbridge.h 

QT += core network webkitwidgets widgets

OTHER_FILES += ../gui/ \
    ../gui/*.js \
    ../gui/*.html \
    ../gui/img/*
DESTDIR = ../build/
