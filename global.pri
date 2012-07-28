include( config.pri )

DEPLOY_DIST_DIR = $$APP_PRO_DIR/package-$${APP_VERSION}$${PUBLIC_APP_TAG}

unix    { NOOP = true }
win32   { NOOP = rem }
