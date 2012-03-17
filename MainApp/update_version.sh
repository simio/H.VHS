#!/bin/sh

CYGWIN_BIN="$1"
PLATFORM="$2"
APP_VERSION="$3"
RC_FILENAME="$4"

if [ X$PLATFORM == Xwin32 ]; then
    SED="$CYGWIN_BIN/sed.exe"
    DIRNAME="$CYGWIN_BIN/dirname.exe"
    GIT="$CYGWIN_BIN/git.exe"
    CAT="$CYGWIN_BIN/cat.exe"
    WC="$CYGWIN_BIN/wc.exe"
fi

PRO_PWD=$($DIRNAME "$0")
VERSION_H="$PRO_PWD/version.h"
COMMIT_COUNT_FILE="$PRO_PWD/.commitcounter"
RC_FILE="$PRO_PWD/$RC_FILENAME"

# Append git commit count
COMMIT_COUNT=$(cd "$PRO_PWD" && $GIT rev-list --all --abbrev-commit --no-merges | $WC -l)
APP_VERSION=$APP_VERSION.$COMMIT_COUNT

# Exit if git commit count hasn't changed since last run
LAST_COMMIT_COUNT=$($CAT "$COMMIT_COUNT_FILE")
if [ X$LAST_COMMIT_COUNT == X$COMMIT_COUNT ]; then
    exit 0
fi

# Write git commit count
echo -n $COMMIT_COUNT > "$COMMIT_COUNT_FILE"

# Write to Windows resource file
if [ X$PLATFORM == Xwin32 ]; then
    RC_VERSION=$(echo $APP_VERSION | $SED s/\\./,/g)
    $SED s/_FILEVERSION_/$RC_VERSION/g < "$PRO_PWD/rc.template" > "$RC_FILE"
    echo "$RC_FILENAME updated to $RC_VERSION"
fi

# Write version.h
echo "#define APP_VER \"$APP_VERSION\"" > "$PRO_PWD/version.h"
echo "version.h updated to $APP_VERSION"
