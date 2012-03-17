#!/bin/sh

APP_VERSION="$1"
CYGWIN_BIN="$2"

SED="$CYGWIN_BIN/sed.exe"
DIRNAME="$CYGWIN_BIN/dirname.exe"
PRO_PWD=$($DIRNAME "$0")

RC_VERSION=$(echo $APP_VERSION | $SED s/\\./,/g)

$SED s/_FILEVERSION_/$RC_VERSION/g < "$PRO_PWD/vhs_rc.template" > "$PRO_PWD/vhs.rc"

echo "vhs.rc updated"
