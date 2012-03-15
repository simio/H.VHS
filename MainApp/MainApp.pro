#-------------------------------------------------
#
# Project created by QtCreator 2012-03-12T10:53:27
#
#-------------------------------------------------

QT       += core gui webkit network xml svg

TARGET = HuggpunktVHS
TEMPLATE = app

# När VERSION uppdateras måste även vhs4.rc uppdateras
VERSION = $HVHS_VERSION

TRANSLATIONS = hvhs_sv.ts qt_sv.ts

include( ../shared.pri )

win32 {
    copyfiles.files = conf

    !contains(HVHS_CONFIG, finalise) {
        CONFIG(debug, debug|release) {
            DEPLOY_DIR = $$OUT_PWD/debug
        } else {
            DEPLOY_DIR = $$OUT_PWD/release
        }
        copyfiles.path = $$DEPLOY_DIR
        INSTALLS += copyfiles
    }

    contains(HVHS_CONFIG, finalise) {
        QTDLL_DIR = "C:/QtSDK/Desktop/Qt/4.8.0/mingw/bin"
        QTI18N_DIR = "C:/QtSDK/Desktop/Qt/4.8.0/mingw/translations"
        QTIMAGEFORMATS_DIR = "C:/QtSDK/Desktop/Qt/4.8.0/mingw/plugins/imageformats"
        QTICONENGINES_DIR = "C:/QtSDK/Desktop/Qt/4.8.0/mingw/plugins/iconengines"

        CONFIG(debug, debug|release) {
            DEPLOY_TYPE = debug
            QTDLL_DIR_DEPENDS += QtGuid4.dll QtCored4.dll phonond4.dll QtNetworkd4.dll \
                                QtScriptd4.dll QtSvgd4.dll QtWebKitd4.dll QtXmld4.dll
            QTIMAGEFORMATS_DIR_DEPENDS += qtiffd4.dll qmngd4.dll qsvgd4.dll qgifd4.dll \
                qicod4.dll qjpegd4.dll
            QTICONENGINES_DIR_DEPENDS += qsvgicond4.dll
        } else {
            DEFINES += RELEASE
            DEPLOY_TYPE = release
            QTDLL_DIR_DEPENDS += QtGui4.dll QtCore4.dll phonon4.dll QtNetwork4.dll \
                QtScript4.dll QtSvg4.dll QtWebKit4.dll QtXml4.dll
            QTIMAGEFORMATS_DIR_DEPENDS += qtiff4.dll qmng4.dll qsvg4.dll qgif4.dll \
                qico4.dll qjpeg4.dll
            QTICONENGINES_DIR_DEPENDS += qsvgicon4.dll
        }
        DEPLOY_DIR = $$DEPLOY_DIR/$$DEPLOY_TYPE

        QTI18N_DIR_DEPENDS += qt_sv.qm
        QTDLL_DIR_DEPENDS += mingwm10.dll libgcc_s_dw2-1.dll

        target.path = $$DEPLOY_DIR

        copyfiles.path = $$DEPLOY_DIR

        depends.path = $$DEPLOY_DIR
        for(file, QTDLL_DIR_DEPENDS) {
            depends.files += $$QTDLL_DIR/$$file
        }
        for(file, QTI18N_DIR_DEPENDS) {
            depends.files += $$QTDLL_DIR/$$file
        }

        imageformats.path = $$DEPLOY_DIR/imageformats
        for(file, QTIMAGEFORMATS_DIR_DEPENDS) {
            imageformats.files += $$QTIMAGEFORMATS_DIR/$$file
        }

        iconengines.path = $$DEPLOY_DIR/iconengines
        for(file, QTICONENGINES_DIR_DEPENDS) {
            iconengines.files += $$QTICONENGINES_DIR/$$file
        }

        INSTALLS += target copyfiles depends imageformats iconengines
    }
}

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
    conf/media.xml \
    conf/format.xml

RESOURCES += \
    vhs4.qrc
