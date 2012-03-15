include( ../global.pri )

TEMPLATE = $$HVHS_EXTENSION_TEMPLATE
QT = $$HVHS_EXTENSION_QT
VERSION = $$HVHS_EXTENSION_VERSION

contains(HVHS_ENABLED_EXTENSIONS, $${HVHS_EXTENSION_NAME}) {
    # Extension .pro files are in a subdir of a subdir
    DEPLOY_DIST_DIR = ../$${DEPLOY_DIST_DIR}

    include( ../deploy.pri )

    # Put the extension definition in deploy_dist-style subdir only if desploy_dist is set.
    contains(APP_CONFIG, deploy_dist) {
        EXTENSION_DEPLOY_DIST_DIR = $$DEPLOY_DIST_DIR/extensions/$$HVHS_EXTENSION_NAME
    } else {
        EXTENSION_DEPLOY_DIST_DIR = $$DEPLOY_DIST_DIR
    }

    target.path = $$EXTENSION_DEPLOY_DIST_DIR

    extension_definition.path = $$EXTENSION_DEPLOY_DIST_DIR
    extension_definition.files = $${HVHS_EXTENSION_NAME}.xml

    INSTALLS += extension_definition
}
