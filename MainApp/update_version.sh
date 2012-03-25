#!/bin/sh

CYGWIN_BIN="$1"
PLATFORM="$2"
APP_VERSION="$3"
RC_FILENAME="$4"
PUBLIC_APP_TAG="$5"

if [ X$PLATFORM = Xwin32 ]; then
    SED="$CYGWIN_BIN/sed.exe"
    DIRNAME="$CYGWIN_BIN/dirname.exe"
    GIT="$CYGWIN_BIN/git.exe"
    CAT="$CYGWIN_BIN/cat.exe"
    WC="$CYGWIN_BIN/wc.exe"
    DATE="$CYGWIN_BIN/date.exe"
fi

PRO_PWD=$($DIRNAME "$0")
VERSION_H="$PRO_PWD/version.h"
GIT_HASH_FILE="$PRO_PWD/.lastgithash"
DATETIME_FILE="$PRO_PWD/.lastdatetime"
RC_FILE="$PRO_PWD/$RC_FILENAME"

# Get git hash
GIT_HASH=$(cd "$PRO_PWD" && $GIT log "--pretty=format:%h" -n 1)

# Get git branch
GIT_BRANCH=$(cd "$PRO_PWD" && $GIT rev-parse --abbrev-ref HEAD)

# Get git (annotated) tag, if an exact match exists
GIT_TAG=$(cd "$PRO_PWD" && $GIT describe --exact-match HEAD 2> /dev/null)

# Check if hash has changed
LAST_GIT_HASH=$($CAT "$GIT_HASH_FILE" | $SED 's/^[ \t]*//;s/[ \t]*$//')
LAST_DATETIME=$($CAT "$DATETIME_FILE" | $SED 's/^[ \t]*//;s/[ \t]*$//')

if [ X$GIT_HASH != X$LAST_GIT_HASH ]; then
    # Append current date and time to version number
    DATETIME=$($DATE -u +%Y%m%d%H%M)
    echo -n $DATETIME > "$DATETIME_FILE"

    # Update hash file
    echo -n $GIT_HASH > "$GIT_HASH_FILE"
else
    # No new commits. Use last datetime.
    DATETIME=$($CAT "$DATETIME_FILE")
    if [ -e "$VERSION_H" ]; then
        exit 0
    fi
fi

APP_VERSION=$APP_VERSION.$DATETIME

if [ X$GIT_TAG = X ]; then
    DISP_TAG="(none)"
else
    DISP_TAG=$GIT_TAG
fi

echo "------------------------------------------------------------------------"
echo -n "    Updated "

# Write to Windows resource file
if [ X$PLATFORM = Xwin32 ]; then
    RC_VERSION=$(echo $APP_VERSION | $SED s/\\./,/g)
    $SED s/_FILEVERSION_/$RC_VERSION/g < "$PRO_PWD/rc.template" > "$RC_FILE"
    echo -n "$RC_FILENAME and "
fi

# Write version.h
$CAT << EOF > "$VERSION_H"
#define APP_VER         "$APP_VERSION"
#define GIT_HASH        "$GIT_HASH"
#define GIT_TAG         "$GIT_TAG"
#define GIT_BRANCH      "$GIT_BRANCH"
#define PUBLIC_APP_TAG  "$PUBLIC_APP_TAG"
EOF

echo "version.h:"
echo "    $APP_VERSION $PUBLIC_APP_TAG:$GIT_TAG ($GIT_BRANCH/$GIT_HASH)"
echo "------------------------------------------------------------------------"
