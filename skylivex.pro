TEMPLATE = subdirs
SUBDIRS = src/skylivex.pro \
          plugins/skproto.pro \
          plugins/skauth.pro

DATAFILES.path = build
DATAFILES.files = doc gui LICENSE.txt COPYING skylivex.ico

INSTALLS += DATAFILES

#unix:!macx {
#}
macx {
ICON=skylivex.icns
}
win32 {
RC_FILE = windows.rc
}
