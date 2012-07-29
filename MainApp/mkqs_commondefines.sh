#!/bin/sh

CYGWIN_BIN="$1"
PLATFORM="$2"

if [ X$PLATFORM = Xwin32 ]; then
    DIRNAME="$CYGWIN_BIN/dirname.exe"
    SED="$CYGWIN_BIN/sed.exe"
    GREP="$CYGWIN_BIN/grep.exe"
else if [ X$PLATFORM = Xposix ]; then
        DIRNAME="$(which dirname)"
        SED="$(which sed)"
        GREP="$(which grep)"
    fi
fi

PRO_PWD=$($DIRNAME "$0")
INPUTFILE="$PRO_PWD/extensionmanager/interfaces/if_commondefines.h"
OUTPUTFILE="$PRO_PWD/extensionmanager/interfaces/if_commondefines.qs"

echo "/* This file is automatically regenerated each build. Resistance is futile." > "$OUTPUTFILE"
echo " */" >> "$OUTPUTFILE"
echo >> "$OUTPUTFILE"

$GREP -vE '(#ifdef|#ifndef|#endif)' "$INPUTFILE" | \
    $SED 's/#define[ \t]\+\([^ \t]\+\)[ \t]\+\(.\+\)$/var \1 = \2/' | \
    $GREP -vE '#define' \
    >> "$OUTPUTFILE"
