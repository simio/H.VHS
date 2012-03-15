include( ../global.pri )

TEMPLATE = $$HVHS_EXTENSION_TEMPLATE
QT = $$HVHS_EXTENSION_QT
VERSION = $$HVHS_EXTENSION_VERSION

contains(HVHS_ENABLED_EXTENSIONS, $${HVHS_EXTENSION_NAME}) {
    DEPLOY_DIST_DIR = ../$${DEPLOY_DIST_DIR}
    include( ../qt_depends.pri )

    EXTENSION_DEPLOY_DIST_DIR = $$DEPLOY_DIST_DIR/extensions/$$HVHS_EXTENSION_NAME

    target.path = $$EXTENSION_DEPLOY_DIST_DIR

    extension_definition.path = $$EXTENSION_DEPLOY_DIST_DIR
    extension_definition.files = $${HVHS_EXTENSION_NAME}.xml

    INSTALLS += target extension_definition
}
