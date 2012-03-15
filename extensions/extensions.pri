include( ../global.pri )

contains(HVHS_ENABLED_EXTENSIONS, $$HVHS_EXTENSION_NAME)
{
    TEMPLATE = $$HVHS_EXTENSION_TEMPLATE

    QT = $$HVHS_EXTENSION_QT

    VERSION = $$HVHS_EXTENSION_VERSION

    win32 {
        contains(HVHS_CONFIG, deploy_dist) {

            DEPLOYDIR = ../$$DEPLOY_DIR
            CONFIG(debug, debug|release) {
                DEPLOYTYPE = debug
                RUN_QTDLL_DEPENDS += RUN_QTDLL_DEPENDS_DEBUG
            } else {
                DEFINES += RELEASE
                DEPLOYTYPE = release
                RUN_QTDLL_DEPENDS += RUN_QTDLL_DEPENDS_RELEASE
            }
            DEPLOYDIR = $$DEPLOYDIR/$$DEPLOYTYPE
            DEPLOYDIR_EXT = $$DEPLOYDIR/extensions/$$HVHS_EXTENSION_NAME
            extension_definition.path = $$DEPLOYDIR_EXT
            extension_definition.files = $${HVHS_EXTENSION_NAME}.xml

            target.path = $$DEPLOYDIR_EXT

            run_depends.path = $$DEPLOYDIR
            for(file, RUN_QTDLL_DEPENDS) {
                run_depends.files += $$QTDLL_DIR/$$file
            }
            for(file, RUN_QTI18N_DEPENDS) {
                run_depends.files += $$QTI18N_DIR/$$file
            }
            for(file, RUN_IMAGEFORMATS_DEPENDS) {
                run_depends.files += $$QTIMAGEFORMATS_DIR/$$file
            }
            for(file, RUN_QTICONENGINES_DEPENDS) {
                run_depends.files += $$QTICONENGINES_DIR/$$file
            }

            INSTALLS += target extension_definition run_depends
        }
    }
}
