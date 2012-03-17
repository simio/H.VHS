TEMPLATE = subdirs

SUBDIRS += \
    MainApp \
    extensions

OTHER_FILES += \
    config.pri \
    global.pri \
    deploy.pri \
    xmlschema/vhsxml.rnc \
    xmlschema/vhsxml.rng \
    xmlschema/make.sh
