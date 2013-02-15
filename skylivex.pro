SOURCES = src/main.cpp \
          src/mainwin.cpp  \
          src/skylivex.cpp #  src/plugins.cpp

HEADERS = src/skylivex.h\
          src/mainwin.h # src/plugins.h src/pluginsinterfaces.h

QT += network webkitwidgets widgets

OTHER_FILES += \
    gui/splash.html \
    gui/img/logo.png
