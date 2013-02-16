SOURCES = main.cpp \
          mainwin.cpp  \
          skylivex.cpp 

HEADERS = skylivex.h \
          mainwin.h \ 
          pluginsinterfaces.h

QT += core network webkitwidgets widgets

OTHER_FILES += \
    ../gui/splash.html \
    ../gui/img/logo.png
DESTDIR = ./
