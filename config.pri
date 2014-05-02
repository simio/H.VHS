#
#   APP_CONFIG
#
#   deploy_dist         Creates a folder containing all files necessary for distribution.
#

APP_VERSION  =              0.4.0                   # MainApp/update_version.sh requires x.y.z format
PUBLIC_APP_TAG =            devel                   # For human consumption. Must be alphanumerical (no spaces) and not empty.
APP_CONFIG +=               #deploy_dist

#HVHS_ENABLED_EXTENSIONS =    dummy dummyjs localfile
HVHS_ENABLED_EXTENSIONS =    ""


## WIN32

win32 {
    CYGWIN_BIN = d:/cygwin64/bin
}
