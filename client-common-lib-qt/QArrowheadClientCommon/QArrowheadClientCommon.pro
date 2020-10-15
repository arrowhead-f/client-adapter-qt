#-------------------------------------------------
#
# Project created by QtCreator 2020-05-07T22:35:55
#
#-------------------------------------------------

QT += network
QT -= gui

equals(QMAKE_TARGET.arch, "arm"){
    TARGET = QArrowheadClientCommonArm
}
else{
    TARGET = QArrowheadClientCommon
}

TEMPLATE = lib
DEFINES += QARROWHEADCLIENTCOMMON_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/common.cpp \
        src/arrowheadbasetypes.cpp \
        src/httpclient.cpp \
        src/logger.cpp

HEADERS += \
        src/arrowheadserverexception.h \
        src/common.h \
        src/arrowheadbasetypes.h \
        src/httpclient.h \
        src/logger.h \
    src/qarrowheadclientcommon_global.h

target.path = $$_PRO_FILE_PWD_/../../lib
INSTALLS += target

DESTDIR = $$_PRO_FILE_PWD_/../../lib
