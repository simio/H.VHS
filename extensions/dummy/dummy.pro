HVHS_EXTENSION_NAME = dummy
HVHS_EXTENSION_VERSION = 1

HVHS_EXTENSION_TEMPLATE = lib
HVHS_EXTENSION_QT = core

DEPENDS += mingwm10.dll libgcc_s_dw2-1.dll
DEBUG_DEPENDS += QtGuid4.dll QtCored4.dll
RELEASE_DEPENDS += QtGui4.dll QtCore4.dll

include( ../extensions.pri )

OTHER_FILES += \
    dummy.xml
