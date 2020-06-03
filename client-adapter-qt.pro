TEMPLATE = subdirs

SUBDIRS += \
    client-common-lib-qt \
    client-system-adapter-lib-qt \
#    client-device-adapter-lib-qt \
#    client-adapter-app-qt

#client-adapter-app-qt.depends = client-system-adapter-lib-qt
#client-adapter-app-qt.depends = client-device-adapter-lib-qt
client-system-adapter-lib-qt.depends = client-common-lib-qt
#client-device-adapter-lib-qt.depends = client-common-lib-qt
