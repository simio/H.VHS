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

QMAKE_CXXFLAGS += -DAPP_VER=\\\"$$VERSION\\\"

SOURCES += main.cpp\
        webwindow.cpp \
    messagehandler.cpp \
    configuration.cpp \
    portability.cpp \
    consolewindow.cpp \
    webbrowsercombobox.cpp \
    webview.cpp

HEADERS  += webwindow.h \
    messagehandler.h \
    configuration.h \
    portability.h \
    consolewindow.h \
    main.h \
    webbrowsercombobox.h \
    webview.h

FORMS    += webwindow.ui \
    consolewindow.ui

win32:RC_FILE = VHS4.rc

OTHER_FILES += \
    vhs4.rc

RESOURCES += \
    vhs4.qrc
