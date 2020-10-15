QT       += core network

QT       -= gui

CONFIG += console
CONFIG += console depend_includepath

TEMPLATE = lib
DEFINES += QARROWHEADCLIENTSYSTEMADAPTER_LIBRARY

INCLUDEPATH += $$_PRO_FILE_PWD_/src \
               $$_PRO_FILE_PWD_/../../client-common-lib-qt/QArrowheadClientCommon/src \

equals(QMAKE_TARGET.arch, "arm"){
    TARGET = QArrowheadClientSystemAdapterArm
    LIBS += -L$$_PRO_FILE_PWD_/../../lib -lQArrowheadClientCommonArm
}
else{
    TARGET = QArrowheadClientSystemAdapter
    LIBS += -L$$_PRO_FILE_PWD_/../../lib -lQArrowheadClientCommon
}

SOURCES += \
    src/servicequeryform.cpp \
    src/serviceregistryentry.cpp \
    src/servicequerylist.cpp \
    src/serviceregistrationform.cpp \
    src/servicerequestform.cpp \
    src/arrowheadservicerequirement.cpp \
    src/orchestrationresponse.cpp \
    src/orchestrationentry.cpp \
    src/qarrowheadhttpclientsystemadapter.cpp

HEADERS += \
    src/qarrowheadclientsystemadapter.h \
    src/servicequeryform.h \
    src/serviceregistryentry.h \
    src/servicequerylist.h \
    src/serviceregistrationform.h \
    src/servicerequestform.h \
    src/arrowheadservicerequirement.h \
    src/orchestrationresponse.h \
    src/orchestrationentry.h \
    src/qarrowheadhttpclientsystemadapter.h \
    src/qarrowheadclientsystemadapter_global.h

target.path = $$_PRO_FILE_PWD_/../../lib
INSTALLS += target

DESTDIR = $$_PRO_FILE_PWD_/../../lib

# Set RPATH for this executable in order to find shared libraries in custom target folder
QMAKE_LFLAGS += -Wl,-rpath,"$$_PRO_FILE_PWD_/../../lib"
