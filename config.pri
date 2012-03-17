#
#   APP_CONFIG
#
#   deploy_dist         Creates a folder containing all files necessary for distribution.
#

APP_VERSION  =              0.4.0                   # MainApp/update_version.sh requires x.y.z format
APP_TAG =                   devel
APP_CONFIG +=               #deploy_dist

HVHS_ENABLED_EXTENSIONS =    dummy


## WIN32

win32 {
    CYGWIN_BIN = c:/Cygwin/bin
}
