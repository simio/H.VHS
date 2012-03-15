#
#   HVHS_CONFIG
#
#   deploy_dist     Creates a folder containing all files necessary for distribution.
#

HVHS_CONFIG    += deploy_dist

#
#   HVHS_ENABLED_EXTENSIONS
#
#   Extensions connected to build.
#

HVHS_ENABLED_EXTENSIONS += dummy

HVHS_VERSION    = 0.4.0.0
HVHS_TAG        = 0.4-devel

DEPLOY_DIR = ../deploy

win32 {
    QTDLL_DIR = "C:/QtSDK/Desktop/Qt/4.8.0/mingw/bin"
    QTI18N_DIR = "C:/QtSDK/Desktop/Qt/4.8.0/mingw/translations"
    QTIMAGEFORMATS_DIR = "C:/QtSDK/Desktop/Qt/4.8.0/mingw/plugins/imageformats"
    QTICONENGINES_DIR = "C:/QtSDK/Desktop/Qt/4.8.0/mingw/plugins/iconengines"
}
