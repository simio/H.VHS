HVHS_EXTENSION_NAME = localfile
HVHS_EXTENSION_VERSION = 1

HVHS_EXTENSION_TEMPLATE = lib
HVHS_EXTENSION_QT = core

RUN_QTDLL_DEPENDS += mingwm10.dll libgcc_s_dw2-1.dll
RUN_QTDLL_DEPENDS_DEBUG += QtGuid4.dll QtCored4.dll
RUN_QTDLL_DEPENDS_RELEASE += QtGui4.dll QtCore4.dll

include( ../extensions.pri )

OTHER_FILES += \
    localfile.xml
