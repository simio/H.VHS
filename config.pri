#
#   APP_CONFIG
#
#   deploy_dist         Creates a folder containing all files necessary for distribution.
#

APP_VERSION  =              0.4.0.0                 # MainApp/generate_vhs_rc.sh requires the x.x.x.x format
APP_TAG =                   devel
APP_CONFIG +=               #deploy_dist

HVHS_ENABLED_EXTENSIONS =    dummy


## WIN32

win32 {
    CYGWIN_BIN = c:/Cygwin/bin
}
