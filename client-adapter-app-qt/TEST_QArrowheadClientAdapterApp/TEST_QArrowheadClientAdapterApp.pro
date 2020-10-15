QT += core network testlib
QT -= gui

CONFIG += qt c++17 console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

APP_SRC_DIR = $$_PRO_FILE_PWD_/../QArrowheadClientAdapterApp/src

INCLUDEPATH += \
                $$APP_SRC_DIR \
                $$_PRO_FILE_PWD_/../../client-system-adapter-lib-qt/QArrowheadClientSystemAdapter/src \
                $$_PRO_FILE_PWD_/../../client-device-adapter-lib-qt/QArrowheadClientDeviceAdapter/src \
                $$_PRO_FILE_PWD_/../../client-common-lib-qt/QArrowheadClientCommon/src

equals(QMAKE_TARGET.arch, "arm"){
    #message($$QMAKE_TARGET.arch)
    LIBS += -L$$_PRO_FILE_PWD_/../../lib -lQArrowheadClientSystemAdapterArm
    LIBS += -L$$_PRO_FILE_PWD_/../../lib -lQArrowheadClientDeviceAdapterArm
    LIBS += -L$$_PRO_FILE_PWD_/../../lib -lQArrowheadClientCommonArm
}
else{
    #message(x86)
    LIBS += -L$$_PRO_FILE_PWD_/../../lib -lQArrowheadClientSystemAdapter
    LIBS += -L$$_PRO_FILE_PWD_/../../lib -lQArrowheadClientDeviceAdapter
    LIBS += -L$$_PRO_FILE_PWD_/../../lib -lQArrowheadClientCommon
}

include(../QArrowheadClientAdapterApp/src.pri)

SOURCES +=  tst_test_qarrowheadclientadapterapp.cpp


CONFIG_DIR=$$_PRO_FILE_PWD_/etc/
INITIALIZATION_FILE=$$_PRO_FILE_PWD_/etc/settings.json
DEFINES += CONF_DIR=\"\\\"$${CONFIG_DIR}\\\"\"
DEFINES += INI_FILE=\"\\\"$${INITIALIZATION_FILE}\\\"\"

# Set RPATH for this executable in order to find shared libraries in custom target folder
QMAKE_LFLAGS += -Wl,-rpath,"$$_PRO_FILE_PWD_/../../lib"
