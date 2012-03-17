#-------------------------------------------------
#
# Project created by QtCreator 2012-03-12T10:53:27
#
#-------------------------------------------------

include( ../global.pri )

## BUILD
QT                 += core gui webkit network xml svg
TARGET              = HuggpunktVHS
TEMPLATE            = app
TRACK_BUILD_NUMBER  = true

VERSION             = $$APP_VERSION
TRANSLATIONS        = hvhs_sv.ts qt_sv.ts

QMAKE_CXXFLAGS     += -DAPP_VER=\\\"$$VERSION\\\"

## DEPLOYMENT
include( ../deploy.pri )

target.path = $$DEPLOY_DIST_DIR

copyfiles.files = $$files(prebuilt_xml/*.xml)
copyfiles.path = $$DEPLOY_DIST_DIR/conf

INSTALLS += copyfiles

## PROJECT
SOURCES += main.cpp\
    consolewindow.cpp \
    vhsxml/reader.cpp \
    vhsxml/transportreader.cpp \
    vhsxml/formatreader.cpp \
    vhsxml/cassettereader.cpp \
    vhsxml/extensionreader.cpp \
    extension/extension.cpp \
    extension/extensionmanager.cpp \
    extension/formatdefinition.cpp \
    extension/transportdefinition.cpp \
    browser/webbrowsercombobox.cpp \
    browser/webview.cpp \
    browser/webwindowguisetup.cpp \
    webwindow.cpp \
    config/configuration.cpp \
    config/messagehandler.cpp \
    config/portability.cpp

HEADERS  += webwindow.h \
    consolewindow.h \
    main.h \
    vhsxml/reader.h \
    vhsxml/transportreader.h \
    vhsxml/formatreader.h \
    vhsxml/cassettereader.h \
    vhsxml/extensionreader.h \
    extension/extension.h \
    extension/extensionmanager.h \
    extension/formatdefinition.h \
    extension/transportdefinition.h \
    browser/webbrowsercombobox.h \
    browser/webview.h \
    config/configuration.h \
    config/messagehandler.h \
    config/portability.h

FORMS    += webwindow.ui \
    consolewindow.ui

win32:RC_FILE = VHS4.rc

OTHER_FILES += \
    vhs4.rc \
    README \
    prebuilt_xml/format.xml \
    prebuilt_xml/transport.xml

RESOURCES += \
    vhs4.qrc
