#-------------------------------------------------
#
# Project created by QtCreator 2012-03-12T10:53:27
#
#-------------------------------------------------

## BUILD
QT                 += core gui webkit network xml svg
TARGET              = HuggpunktVHS
TEMPLATE            = app
TRACK_BUILD_NUMBER  = true

include( ../global.pri )

## DEPLOYMENT
include( ../deploy.pri )

target.path = $$DEPLOY_DIST_DIR

copyfiles.files = conf
copyfiles.path = $$DEPLOY_DIST_DIR

INSTALLS += copyfiles

## PROJECT
SOURCES += main.cpp\
    webwindow.cpp \
    messagehandler.cpp \
    portability.cpp \
    consolewindow.cpp \
    webbrowsercombobox.cpp \
    webview.cpp \
    configuration.cpp \
    webwindowguisetup.cpp \
    extensionmanager.cpp \
    extension.cpp \
    mediadefinition.cpp \
    formatdefinition.cpp \
    vhsxml.cpp

HEADERS  += webwindow.h \
    messagehandler.h \
    portability.h \
    consolewindow.h \
    main.h \
    webbrowsercombobox.h \
    webview.h \
    configuration.h \
    extensionmanager.h \
    extension.h \
    mediadefinition.h \
    formatdefinition.h \
    vhsxml.h

FORMS    += webwindow.ui \
    consolewindow.ui

win32:RC_FILE = VHS4.rc

OTHER_FILES += \
    vhs4.rc \
    README \
    conf/media.xml \
    conf/format.xml

RESOURCES += \
    vhs4.qrc
