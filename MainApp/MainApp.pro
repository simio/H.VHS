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

QMAKE_CXXFLAGS     += -DAPP_TAG=\\\"$$APP_TAG\\\"

## DEPLOYMENT
include( ../deploy.pri )

target.path = $$DEPLOY_DIST_DIR

win32 {
    RC_FILE = vhs.rc
    CYGWIN_PRO_FILE_PWD = $$replace(_PRO_FILE_PWD_, "C:", "/cygdrive/c")
    winrc.target = $$RC_FILE
    winrc.commands = $${CYGWIN_BIN}/bash.exe "'$${CYGWIN_PRO_FILE_PWD}/update_version.sh'" $$CYGWIN_BIN win32 $$VERSION $$RC_FILE
    winrc.depends = FORCE

    version_h.target = version.h
    version_h.depends = winrc

    PRE_TARGETDEPS += $$RC_FILE
    QMAKE_EXTRA_TARGETS += winrc
}

PRE_TARGETDEPS += version.h
QMAKE_EXTRA_TARGETS += version_h

presets.files = presets
presets.path = $$DEPLOY_DIST_DIR

INSTALLS += presets

contains(APP_CONFIG, deploy_dist):DEFINES += HIDE_COMMIT_COUNT


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
    extension/definition.cpp \
    config/systemdependent.cpp

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
    extension/definition.h \
    config/systemdependent.h

FORMS    += \
    webwindow.ui \
    consolewindow.ui

OTHER_FILES += \
    README \
    presets/format.xml \
    presets/transport.xml \
    update_version.sh \
    rc.template

RESOURCES += \
    vhs.qrc
