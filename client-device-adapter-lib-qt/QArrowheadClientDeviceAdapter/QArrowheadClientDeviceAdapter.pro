#-------------------------------------------------
#
# Project created by QtCreator 2020-04-16T12:32:45
#
#-------------------------------------------------

QT       += network
QT       -= gui

TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += $$_PRO_FILE_PWD_/src \
               $$_PRO_FILE_PWD_/../../client-common-lib-qt/QArrowheadClientCommon/src \

equals(QMAKE_TARGET.arch, "arm"){
    TARGET = QArrowheadClientDeviceAdapterArm
    LIBS += -L$$_PRO_FILE_PWD_/../../lib -lQArrowheadClientCommonArm
}
else{
    TARGET = QArrowheadClientDeviceAdapter
    LIBS += -L$$_PRO_FILE_PWD_/../../lib -lQArrowheadClientCommon
}


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
    src/arrowheaddevice.cpp \
    src/deviceonboardingresponse.cpp \
    src/deviceonboardingwithcsrrequest.cpp \
    src/deviceonboardingwithnamerequest.cpp \
    src/devicequeryform.cpp \
    src/devicequerylist.cpp \
    src/deviceregistrationform.cpp \
    src/deviceregistryentry.cpp \
    src/onboardingwithcsrrequest.cpp \
    src/qarrowheadhttpclientdeviceadapter.cpp \
    src/httpclient.cpp \
    src/onboardingwithnamerequest.cpp \
    src/keypair.cpp \
    src/certificatecreationrequest.cpp \
    src/certificate.cpp \
    src/onboardingresponse.cpp \
    src/systemonboardingresponse.cpp \
    src/systemonboardingwithcsrrequest.cpp \
    src/systemonboardingwithnamerequest.cpp \
    src/systemqueryform.cpp \
    src/systemquerylist.cpp \
    src/systemregistrationform.cpp \
    src/systemregistryentry.cpp

HEADERS += \
    src/arrowheaddevice.h \
    src/deviceonboardingresponse.h \
    src/deviceonboardingwithcsrrequest.h \
    src/deviceonboardingwithnamerequest.h \
    src/devicequeryform.h \
    src/devicequerylist.h \
    src/deviceregistrationform.h \
    src/deviceregistryentry.h \
    src/onboardingwithcsrrequest.h \
    src/qarrowheadclientdeviceadapter.h \
    src/qarrowheadhttpclientdeviceadapter.h \
    src/httpclient.h \
    src/onboardingwithnamerequest.h \
    src/keypair.h \
    src/certificatecreationrequest.h \
    src/certificate.h \
    src/onboardingresponse.h \
    src/systemonboardingresponse.h \
    src/systemonboardingwithcsrrequest.h \
    src/systemonboardingwithnamerequest.h \
    src/systemqueryform.h \
    src/systemquerylist.h \
    src/systemregistrationform.h \
    src/systemregistryentry.h

target.path = $$_PRO_FILE_PWD_/../../lib
INSTALLS += target

DESTDIR = $$_PRO_FILE_PWD_/../../lib
