#-------------------------------------------------
#
# Project created by QtCreator 2012-03-12T10:53:27
#
#-------------------------------------------------

APP_PRO_DIR = ".."
include( $$APP_PRO_DIR/global.pri )

## BUILD
QT                 += core gui webkit network xml svg
TARGET              = HuggpunktVHS
TEMPLATE            = app

VERSION             = $$APP_VERSION
TRANSLATIONS        = hvhs_sv.ts qt_sv.ts

QMAKE_CXXFLAGS     += -DAPP_VER=\\\"$$VERSION\\\"


## DEPLOYMENT
include( ../deploy.pri )

target.path = $$DEPLOY_DIST_DIR

win32 {
    CYGWIN_PRO_FILE_PWD = $$replace(_PRO_FILE_PWD_, "C:", "/cygdrive/c")
    winrc.target = vhs.rc
    winrc.commands = $${CYGWIN_BIN}/bash.exe "'$${CYGWIN_PRO_FILE_PWD}/update_vhs_rc.sh'" $$VERSION $$CYGWIN_BIN
    winrc.depends = FORCE

    PRE_TARGETDEPS += vhs.rc
    QMAKE_EXTRA_TARGETS += winrc
    RC_FILE = vhs.rc
}

presets.files = presets
presets.path = $$DEPLOY_DIST_DIR

INSTALLS += presets


## PROJECT
SOURCES += \
    main.cpp\
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
    config/portability.cpp \
    extension/definition.cpp

HEADERS  += \
    webwindow.h \
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
    config/portability.h \
    extension/definition.h

FORMS    += \
    webwindow.ui \
    consolewindow.ui

OTHER_FILES += \
    README \
    presets/format.xml \
    presets/transport.xml \
    vhs_rc.template \
    update_vhs_rc.sh

RESOURCES += \
    vhs.qrc
