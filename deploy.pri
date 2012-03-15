!contains(APP_CONFIG, deploy_dist) {
    win32 {
        CONFIG(debug, debug|release) {
            DEPLOY_DIST_DIR = $${OUT_PWD}/debug
        } else {
            DEPLOY_DIST_DIR = $${OUT_PWD}/release
        }
    }
}

contains(APP_CONFIG, deploy_dist) {
    win32 {
        QTDLL_RUNDEPENDS += mingwm10.dll libgcc_s_dw2-1.dll

        contains(QT, core) {
            QTDLL_RUNDEPENDS_DEBUG += QtCored4.dll
            QTDLL_RUNDEPENDS_RELEASE += QtCore4.dll
        }
        contains(QT, gui) {
            QTDLL_RUNDEPENDS_DEBUG += QtGuid4.dll
            QTDLL_RUNDEPENDS_RELEASE += QtGui4.dll
        }
        contains(QT, webkit) {
            QTDLL_RUNDEPENDS_DEBUG += QtWebKitd4.dll
            QTDLL_RUNDEPENDS_RELEASE += QtWebKit4.dll
            QTIMAGEFORMATS_RUNDEPENDS_DEBUG += qtiffd4.dll qmngd4.dll qsvgd4.dll qgifd4.dll qicod4.dll qjpegd4.dll
            QTIMAGEFORMATS_RUNDEPENDS_RELEASE += qtiffd4.dll qmngd4.dll qsvgd4.dll qgifd4.dll qicod4.dll qjpegd4.dll
            OPENSSL_RUNDEPENDS += libeay32.dll libssl32.dll ssleay32.dll
        }
        contains(QT, script) | contains(QT, webkit) {
            QTDLL_RUNDEPENDS_DEBUG += QtScriptd4.dll
            QTDLL_RUNDEPENDS_RELEASE += QtScript4.dll
        }
        contains(QT, xml) | contains(QT, webkit) {
            QTDLL_RUNDEPENDS_DEBUG += QtXmld4.dll
            QTDLL_RUNDEPENDS_RELEASE += QtXml4.dll
        }
        contains(QT, network) | contains(QT, webkit) {
            QTDLL_RUNDEPENDS_DEBUG += QtNetworkd4.dll
            QTDLL_RUNDEPENDS_RELEASE += QtNetwork4.dll
        }
        contains(QT, phonon) | contains(QT, webkit) {
            QTDLL_RUNDEPENDS_DEBUG += phonond4.dll
            QTDLL_RUNDEPENDS_RELEASE += phonon4.dll
        }
        contains(QT, svg) {
            QTDLL_RUNDEPENDS_DEBUG += QtSvgd4.dll
            QTDLL_RUNDEPENDS_RELEASE += QtSvg.dll
            QTICONENGINES_RUNDEPENDS_DEBUG += qsvgicond4.dll
            QTICONENGINES_RUNDEPENDS_RELEASE += qsvgicond4.dll
        }

        CONFIG(debug, debug|release) {
            DEPLOY_DIST_DIR = $${DEPLOY_DIST_DIR}/debug
            QTDLL_RUNDEPENDS += $$QTDLL_RUNDEPENDS_DEBUG
            QTI18N_RUNDEPENDS += $$QTI18N_RUNDEPENDS_DEBUG
            QTIMAGEFORMATS_RUNDEPENDS += $$QTIMAGEFORMATS_RUNDEPENDS_DEBUG
            QTICONENGINES_RUNDEPENDS += $$QTICONENGINES_RUNDEPENDS_DEBUG
            OPENSSL_RUNDEPENDS += $$OPENSSL_RUNDEPENDS_DEBUG
        } else {
            DEFINES += RELEASE
            DEPLOY_DIST_DIR = $${DEPLOY_DIST_DIR}/release
            QTDLL_RUNDEPENDS += $$QTDLL_RUNDEPENDS_RELEASE
            QTI18N_RUNDEPENDS += $$QTI18N_RUNDEPENDS_RELEASE
            QTIMAGEFORMATS_RUNDEPENDS += $$QTIMAGEFORMATS_RUNDEPENDS_RELEASE
            QTICONENGINES_RUNDEPENDS += $$QTICONENGINES_RUNDEPENDS_RELEASE
            OPENSSL_RUNDEPENDS += $$OPENSSL_RUNDEPENDS_RELEASE
        }

        QTDLL_DIR =             "C:/QtSDK/Desktop/Qt/4.8.0/mingw/bin"
        QTI18N_DIR =            "C:/QtSDK/Desktop/Qt/4.8.0/mingw/translations"
        QTIMAGEFORMATS_DIR =    "C:/QtSDK/Desktop/Qt/4.8.0/mingw/plugins/imageformats"
        QTICONENGINES_DIR =     "C:/QtSDK/Desktop/Qt/4.8.0/mingw/plugins/iconengines"
        OPENSSL_DIR =           "C:/Program Files (x86)/OpenSSL-Win32"

        qt_run_depends.path = $$DEPLOY_DIST_DIR

        for(file, QTDLL_RUNDEPENDS) {
            qt_run_depends.files += $$QTDLL_DIR/$$file
        }
        for(file, QTI18N_RUNDEPENDS) {
            qt_run_depends.files += $$QTI18N_DIR/$$file
        }
        for(file, QTIMAGEFORMATS_RUNDEPENDS) {
            qt_run_depends.files += $$QTIMAGEFORMATS_DIR/$$file
        }
        for(file, QTICONENGINES_RUNDEPENDS) {
            qt_run_depends.files += $$QTICONENGINES_DIR/$$file
        }
        for(file, OPENSSL_RUNDEPENDS) {
            qt_run_depends.files += $$OPENSSL_DIR/$$file
        }

        INSTALLS += qt_run_depends target
    }
}
