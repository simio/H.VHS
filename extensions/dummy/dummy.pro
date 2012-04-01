HVHS_EXTENSION_NAME = dummy
HVHS_EXTENSION_VERSION = 1
HVHS_EXTENSION_TEMPLATE = lib
HVHS_EXTENSION_QT = core

INCLUDEPATH    += ../../MainApp/extensionmanager/interfaces
CONFIG         += plugin
TARGET          = $$qtLibraryTarget(dummy)

include( ../hvhs_extension.pri )

OTHER_FILES += \
    dummy.xml

HEADERS += \
    dummy.h

SOURCES += \
    dummy.cpp
