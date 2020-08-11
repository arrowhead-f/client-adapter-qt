QT += testlib network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += $$_PRO_FILE_PWD_/src \
               $$_PRO_FILE_PWD_/../QArrowheadClientDeviceAdapter/src \
               $$_PRO_FILE_PWD_/../../client-common-lib-qt/QArrowheadClientCommon/src

equals(QMAKE_TARGET.arch, "arm"){
    LIBS += -L$$_PRO_FILE_PWD_/../../lib -lQArrowheadClientDeviceAdapterArm
    LIBS += -L$$_PRO_FILE_PWD_/../../lib -lQArrowheadClientCommonArm
}
else{
    LIBS += -L$$_PRO_FILE_PWD_/../../lib -lQArrowheadClientDeviceAdapter
    LIBS += -L$$_PRO_FILE_PWD_/../../lib -lQArrowheadClientCommon
}

HEADERS += \
    src/testsuite.h \
    src/testqarrowheadhttpclientdeviceadapter.h

SOURCES += \
    src/main.cpp \
    src/testqarrowheadhttpclientdeviceadapter.cpp

OTHER_FILES += \
    etc/arrowhead.ini

CONFIG_DIR=$$_PRO_FILE_PWD_/etc/
INITIALIZATION_FILE=$$_PRO_FILE_PWD_/etc/arrowhead.ini
DEFINES += CONF_DIR=\"\\\"$${CONFIG_DIR}\\\"\"
DEFINES += INI_FILE=\"\\\"$${INITIALIZATION_FILE}\\\"\"
