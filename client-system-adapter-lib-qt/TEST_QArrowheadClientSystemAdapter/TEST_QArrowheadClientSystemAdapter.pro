QT += testlib network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += $$_PRO_FILE_PWD_/src \
               $$_PRO_FILE_PWD_/../QArrowheadClientSystemAdapter/src \
               $$_PRO_FILE_PWD_/../../client-common-lib-qt/QArrowheadClientCommon/src

equals(QMAKE_TARGET.arch, "arm"){
    LIBS += -L$$_PRO_FILE_PWD_/../../lib -lQArrowheadClientSystemAdapterArm
    LIBS += -L$$_PRO_FILE_PWD_/../../lib -lQArrowheadClientCommonArm
}
else{
    LIBS += -L$$_PRO_FILE_PWD_/../../lib -lQArrowheadClientSystemAdapter
    LIBS += -L$$_PRO_FILE_PWD_/../../lib -lQArrowheadClientCommon
}

HEADERS += \
    src/testsuite.h \
    src/testqarrowheadhttpclientsystemadapter.h

SOURCES += \
    src/main.cpp \
    src/testqarrowheadhttpclientsystemadapter.cpp

OTHER_FILES += \
    etc/arrowhead.ini

# Add either SSL_ENABLED or SSL_DISABLED define here
DEFINES += SSL_ENABLED
#DEFINES += SSL_DISABLED

CONFIG_DIR=$$_PRO_FILE_PWD_/etc/
INITIALIZATION_FILE=$$_PRO_FILE_PWD_/etc/arrowhead.ini
DEFINES += CONF_DIR=\"\\\"$${CONFIG_DIR}\\\"\"
DEFINES += INI_FILE=\"\\\"$${INITIALIZATION_FILE}\\\"\"
