SOURCES = main.cpp \
          mainwin.cpp  \
          skylivex.cpp \
          ipcmsg.cpp

HEADERS = skylivex.h \
          mainwin.h \ 
          pluginsinterfaces.h \
          ipcmsg.h

QT += core network webkitwidgets widgets

OTHER_FILES += ../gui/ \
    ../gui/*.js \
    ../gui/*.html \
    ../gui/img/*
DESTDIR = ../build/
