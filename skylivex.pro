TEMPLATE = subdirs
SUBDIRS = src/skylivex.pro \
          plugins/skproto.pro

DATAFILES.path = build
DATAFILES.files = doc gui LICENSE.txt COPYING

INSTALLS += DATAFILES

