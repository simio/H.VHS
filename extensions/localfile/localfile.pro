HVHS_EXTENSION_NAME = localfile
HVHS_EXTENSION_VERSION = 1
HVHS_EXTENSION_TEMPLATE = lib
HVHS_EXTENSION_QT = core

INCLUDEPATH    += ../../MainApp/extensionmanager/interfaces
CONFIG         += plugin
TARGET          = $$qtLibraryTarget(localfile)

include( ../hvhs_extension.pri )

OTHER_FILES += \
    localfile.xml

HEADERS += \
    localfile.h

SOURCES += \
    localfile.cpp
