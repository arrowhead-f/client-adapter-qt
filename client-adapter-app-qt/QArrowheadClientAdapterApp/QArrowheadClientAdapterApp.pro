QT += core network
QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

APP_SRC_DIR = $$_PRO_FILE_PWD_/src

INCLUDEPATH += $$APP_SRC_DIR \
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

include(src.pri)

SOURCES +=  src/main.cpp

CONFIG_DIR=$$_PRO_FILE_PWD_/etc/
INITIALIZATION_FILE=$$_PRO_FILE_PWD_/etc/settings.json
DEFINES += CONF_DIR=\"\\\"$${CONFIG_DIR}\\\"\"
DEFINES += INI_FILE=\"\\\"$${INITIALIZATION_FILE}\\\"\"

# Set RPATH for this executable in order to find shared libraries in custom target folder
QMAKE_LFLAGS += -Wl,-rpath,"$$_PRO_FILE_PWD_/../../lib"

DOCUMENTATION_PATH=$$_PRO_FILE_PWD_/doc/
DEFINES += DOC_PATH=\"\\\"$${DOCUMENTATION_PATH}\\\"\"

#TO GENERATE USER DOCUMENTATION:
#DEFINES += CREATE_DOCUMENTATION
