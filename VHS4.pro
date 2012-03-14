#-------------------------------------------------
#
# Project created by QtCreator 2012-03-12T10:53:27
#
#-------------------------------------------------

QT       += core gui webkit network xml svg

TARGET = HuggpunktVHS
TEMPLATE = app

# När VERSION uppdateras måste även vhs4.rc uppdateras
VERSION = 0.4.0.0

TRANSLATIONS = hvhs_sv.ts

#DEFINES += RELEASE

QMAKE_CXXFLAGS += -DAPP_VER=\\\"$$VERSION\\\" \
                  -DPRO_PATH=\"\\\"$$_PRO_FILE_PWD_\\\"\"

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
    extensions/media.xml \
    extensions/extensions.xml \
    conf/media.xml \
    conf/format.xml

RESOURCES += \
    vhs4.qrc
