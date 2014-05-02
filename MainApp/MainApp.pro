#-------------------------------------------------
#
# Project created by QtCreator 2012-03-12T10:53:27
#
#-------------------------------------------------

APP_PRO_DIR = ".."
include( $$APP_PRO_DIR/global.pri )

## BUILD
QT                 += core gui widgets network script svg webkit webkitwidgets xml
TARGET              = HuggpunktVHS
TEMPLATE            = app

VERSION             = $$APP_VERSION
TRANSLATIONS        = hvhs_sv.ts qt_sv.ts

## DEPLOYMENT
include( ../deploy.pri )

target.path = $$DEPLOY_DIST_DIR

win32 {
    exists( "version.h" ) {
        system ( "del version.h" )
    }

    RC_FILE = vhs.rc
    CYGWIN_PRO_FILE_PWD = $$replace(_PRO_FILE_PWD_, "C:", "/cygdrive/c")
    winrc.target = $$RC_FILE
    winrc.depends =
    winrc.commands = \
	$${CYGWIN_BIN}/bash.exe "'$${CYGWIN_PRO_FILE_PWD}/update_version.sh'" \
        	$$CYGWIN_BIN win32 $$VERSION $$RC_FILE $$PUBLIC_APP_TAG

    version_h.target = version.h
    version_h.depends = winrc

    IF_COMMONDEFINES_QS = extensionmanager/interfaces/if_commondefines.qs
    if_commondefines_qs.target = $$IF_COMMONDEFINES_QS
    if_commondefines_qs.depends = FORCE
    if_commondefines_qs.commands = \
	$${CYGWIN_BIN}/bash.exe \
		"'$${CYGWIN_PRO_FILE_PWD}/mkqs_commondefines.sh'" \
		$$CYGWIN_BIN win32

    PRE_TARGETDEPS += $$RC_FILE
    QMAKE_EXTRA_TARGETS += winrc
}
unix {
    exists( "version.h" ) {
        system ( "rm version.h" )
    }

    version_h.target = version.h
    version_h.depends =
    version_h.commands = \
	sh $${_PRO_FILE_PWD_}/update_version.sh unused unix $$VERSION \
		unused $$PUBLIC_APP_TAG

    IF_COMMONDEFINES_QS = extensionmanager/interfaces/if_commondefines.qs
    if_commondefines_qs.target = $$IF_COMMONDEFINES_QS
    if_commondefines_qs.depends = FORCE
    if_commondefines_qs.commands = \
	sh $${_PRO_FILE_PWD_}/mkqs_commondefines.sh nonsense unix
}

PRE_TARGETDEPS += version.h $$IF_COMMONDEFINES_QS
QMAKE_EXTRA_TARGETS += version_h if_commondefines_qs

presets.files = presets
presets.path = $$DEPLOY_DIST_DIR

INSTALLS += presets

contains(APP_CONFIG, deploy_dist):DEFINES += HIDE_DEVEL_INFO


## PROJECT
SOURCES += \
    main.cpp \
    consolewindow.cpp \
    vhsxml/transportreader.cpp \
    vhsxml/formatreader.cpp \
    vhsxml/cassettereader.cpp \
    vhsxml/extensionreader.cpp \
    browser/webbrowsercombobox.cpp \
    browser/webview.cpp \
    browser/webwindowguisetup.cpp \
    webwindow.cpp \
    config/configuration.cpp \
    config/messagehandler.cpp \
    extensionmanager/definitiontable.cpp \
    vhsxml/documentreader.cpp \
    vhsxml/elementparser.cpp \
    config/platformdependent.cpp \
    extensionmanager/extensionmanager.cpp \
    extensionmanager/definitions/definition.cpp \
    extensionmanager/definitions/extensiondefinition.cpp \
    extensionmanager/definitions/formatdefinition.cpp \
    extensionmanager/definitions/transportdefinition.cpp \
    extensionmanager/extension/javascriptextension.cpp \
    extensionmanager/extension/qtpluginextension.cpp \
    extensionmanager/jobmanager/job.cpp \
    extensionmanager/jobmanager/jobmanager.cpp \
    extensionmanager/jobmanager/jobthread.cpp \
    appwideclasses/person.cpp \
    appwideclasses/versionnumber.cpp \
    appwideclasses/hurl.cpp \
    appwideclasses/cassette.cpp \
    appwideclasses/cassettecontent.cpp

HEADERS  += \
    webwindow.h \
    consolewindow.h \
    main.h \
    vhsxml/transportreader.h \
    vhsxml/formatreader.h \
    vhsxml/cassettereader.h \
    vhsxml/extensionreader.h \
    browser/webbrowsercombobox.h \
    browser/webview.h \
    config/configuration.h \
    config/messagehandler.h \
    extensionmanager/definitiontable.h \
    vhsxml/documentreader.h \
    vhsxml/elementparser.h \
    config/platformdependent.h \
    extensionmanager/extensionmanager.h \
    extensionmanager/definitions/definition.h \
    extensionmanager/definitions/extensiondefinition.h \
    extensionmanager/definitions/formatdefinition.h \
    extensionmanager/definitions/transportdefinition.h \
    extensionmanager/extension/javascriptextension.h \
    extensionmanager/extension/extension.h \
    extensionmanager/extension/qtpluginextension.h \
    extensionmanager/jobmanager/job.h \
    extensionmanager/jobmanager/jobmanager.h \
    extensionmanager/jobmanager/jobthread.h \
    extensionmanager/interfaces/common_if.h \
    extensionmanager/interfaces/hooks_if.h \
    extensionmanager/interfaces/streams_if.h \
    extensionmanager/interfaces/if_commondefines.h \
    hvhsexception.h \
    appwideclasses/appwideclasses.h \
    appwideclasses/person.h \
    appwideclasses/versionnumber.h \
    appwideclasses/hurl.h \
    appwideclasses/cassette.h \
    appwideclasses/cassettecontent.h \
    config/configurationdefaults.h \
    extensionmanager/definitions/definitiondata.h \
    extensionmanager/interfaces/filters_if.h

FORMS    += \
    webwindow.ui \
    consolewindow.ui

OTHER_FILES += \
    README \
    presets/format.xml \
    presets/transport.xml \
    update_version.sh \
    rc.template \
    mkqs_commondefines.sh

RESOURCES += \
    vhs.qrc
