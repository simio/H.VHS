TEMPLATE = lib

QT = core

VERSION = 1

HVHS_EXTENSION_NAME = localfile

include( ../../shared.pri )

win32 {
    contains(HVHS_CONFIG, finalise) {
        DEPENDS += mingwm10.dll libgcc_s_dw2-1.dll

        DEPLOYDIR = ../$$DEPLOY_DIR
        CONFIG(debug, debug|release) {
            DEPLOYTYPE = debug
            DEPENDS += QtGuid4.dll QtCored4.dll
        } else {
            DEFINES += RELEASE
            DEPLOYTYPE = release
            DEPENDS += QtGui4.dll QtCore4.dll
        }
        DEPLOYDIR = $$DEPLOYDIR/$$DEPLOYTYPE
        DEPLOYDIR_EXT = $$DEPLOYDIR/extensions/$$HVHS_EXTENSION_NAME
        extension.path = $$DEPLOYDIR_EXT
        extension.files = localfile.xml

        target.path = $$DEPLOYDIR_EXT

        depends.path = $$DEPLOYDIR
        QTDLLDIR = "C:/QtSDK/Desktop/Qt/4.8.0/mingw/bin"
        for(file, DEPENDS) {
            depends.files += $$QTDLLDIR/$$file
        }

        INSTALLS += extension target depends
    }
}

OTHER_FILES += \
    localfile.xml
