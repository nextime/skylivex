SOURCES = src/main.cpp \
          src/mainwin.cpp  \
          src/skylivex.cpp 

HEADERS = src/skylivex.h \
          src/mainwin.h \ 
          src/pluginsinterfaces.h

QT += core network webkitwidgets widgets

OTHER_FILES += \
    gui/splash.html \
    gui/img/logo.png
