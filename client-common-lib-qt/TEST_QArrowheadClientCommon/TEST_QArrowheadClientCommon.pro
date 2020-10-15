QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += $$_PRO_FILE_PWD_/src \
               $$_PRO_FILE_PWD_/../QArrowheadClientCommon/src \

equals(QMAKE_TARGET.arch, "arm"){
    LIBS += -L$$_PRO_FILE_PWD_/../../lib -lQArrowheadClientCommonArm
}
else{
    LIBS += -L$$_PRO_FILE_PWD_/../../lib -lQArrowheadClientCommon
}

HEADERS += \
    src/testsuite.h \
    src/test_common.h \
    src/test_arrowheadbasetypes.h

SOURCES +=  \
    src/main.cpp \
    src/test_common.cpp \
    src/test_arrowheadbasetypes.cpp

# Set RPATH for this executable in order to find shared libraries in custom target folder
QMAKE_LFLAGS += -Wl,-rpath,"$$_PRO_FILE_PWD_/../../lib"
