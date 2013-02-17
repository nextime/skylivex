TEMPLATE = subdirs
SUBDIRS = src/skylivex.pro \
          plugins/skproto.pro \
          plugins/skauth.pro

DATAFILES.path = build
DATAFILES.files = doc gui LICENSE.txt COPYING

INSTALLS += DATAFILES

