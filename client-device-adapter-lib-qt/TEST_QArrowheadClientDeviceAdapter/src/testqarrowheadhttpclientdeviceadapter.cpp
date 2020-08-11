/****************************************************************************
**
** Copyright 2020 evopro Innovation Kft.
**
** This file is part of arrowhead-f/client-adapter-qt.
**
** arrowhead-f/client-adapter-qt is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**	
** arrowhead-f/client-adapter-qt is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU Lesser General Public License for more details.
**	
** You should have received a copy of the GNU Lesser General Public License
** along with arrowhead-f/client-adapter-qt. If not, see <https://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "testqarrowheadhttpclientdeviceadapter.h"
#include "onboardingwithnamerequest.h"
#include "onboardingresponse.h"
#include <QtTest/QTest>
#include <QSettings>
#include <QRegularExpression>

static TestQArrowheadHttpClientDeviceAdapter TEST_QARROWHEADHTTPCLIENTDEVICEADAPTER;

using namespace arrowhead;

void TestQArrowheadHttpClientDeviceAdapter::initTestCase()
{
    QSettings ahSettings(iniFileName.data(), QSettings::IniFormat);

    auto getStringParam = [&](const auto & variableName) {
        return ahSettings.value(variableName).toString().toStdString();
    };
    auto getIntParam = [&](const auto & variableName) {
        return ahSettings.value(variableName).toInt();
    };
    auto getBoolParam = [&](const auto & variableName) {
        return ahSettings.value(variableName).toBool();
    };

    auto getConfigFileAbsPath = [&](const auto & configFileName){
        return (QString(CONF_DIR) + configFileName.data()).toStdString();
    };
    auto readSslKeyFromFile = [&](QString fileName, QSsl::KeyType keyType, const QByteArray password=""){
        QFile keyFile(fileName);
        if(keyFile.open(QFile::ReadOnly)){
            qDebug() << "keyFile=" << keyFile.fileName() << " opened successfully.";
            QSslKey key(&keyFile, QSsl::Rsa, QSsl::Pem, keyType, password);
            keyFile.close();
            return key;
        }
        return QSslKey();
    };

    auto readPemFromFile = [&](std::string fileName){
        QFile keyFile(fileName.data());
        QString content;
        if(keyFile.open(QFile::ReadOnly)){
            qDebug() << "keyFile=" << keyFile.fileName() << " opened successfully.";
            content = keyFile.readAll();
            keyFile.close();
            content
                    .remove(QRegularExpression(".*-{5}\n*"))
                    .remove(QRegularExpression("[\\n\\t\\r]"));
        }
        return content.toStdString();
    };

    ahSettings.beginGroup("coreSystems");
    onboardingControllerBaseUrl = getStringParam("onboardingControllerBaseUrl");
    deviceRegistryBaseUrl = getStringParam("deviceRegistryBaseUrl");
    systemRegistryBaseUrl = getStringParam("systemRegistryBaseUrl");

    ahSettings.endGroup();

    for (int i = 1; i <= 3; i++) {
        ahSettings.beginGroup(QString("myDevice") + QString::number(i));
        std::string deviceName = getStringParam("deviceName");
        std::string macAddress = getStringParam("macAddress");
        std::string address = getStringParam("address");
        testDevices.emplace_back(deviceName, macAddress, address);
        ahSettings.endGroup();
    }

    ahSettings.beginGroup("authentication");
    std::string method = getStringParam("method");
    authMethod = HttpClient::authMethodFromString(method);
    if(authMethod == HttpClient::AuthenticationMethod::BasicAuthentication){
        sharedSecret = getStringParam("sharedSecret");
        privateKeyFile = getConfigFileAbsPath(getStringParam("privateKey"));
        privateKeyPwd = getStringParam("privateKeyPwd");
        privateKey = readSslKeyFromFile(privateKeyFile.data(), QSsl::PrivateKey, privateKeyPwd.data());
        publicKey = readSslKeyFromFile(getConfigFileAbsPath(getStringParam("publicKey")).data(), QSsl::PublicKey);
    }
    else if(authMethod == HttpClient::AuthenticationMethod::ClientCertificate){
        privateKeyFile = getConfigFileAbsPath(getStringParam("privateKey"));
        privateKeyPwd = getStringParam("privateKeyPwd");
        certificateFile = getConfigFileAbsPath(getStringParam("certificate"));
    }
    keyStore = getConfigFileAbsPath(getStringParam("keyStore"));
    keyStorePwd = getStringParam("keyStorePwd");
    onboardingCSR = readPemFromFile(getConfigFileAbsPath(getStringParam("onboardingCSR")));
    onboardingPrivateKey = readPemFromFile(getConfigFileAbsPath(getStringParam("onboardingPrivateKey")));
    onboardingPublicKey = readPemFromFile(getConfigFileAbsPath(getStringParam("onboardingPublicKey")));
    ahSettings.endGroup();

    ahSettings.beginGroup("ssl");
    caCertificateFile = getConfigFileAbsPath(getStringParam("caCertificate"));
    disableHostnameVerification = getBoolParam("disableHostnameVerification");
    ahSettings.endGroup();

    for (int i = 1; i <= 3; i++) {
        ahSettings.beginGroup(QString("mySystem") + QString::number(i));
        std::string systemName = getStringParam("systemName");
        std::string address = getStringParam("address");
        int port = getIntParam("port");
        testSystems.emplace_back(systemName, address, port);
        ahSettings.endGroup();
    }

    ahAdapter = std::make_unique<QArrowheadHttpClientDeviceAdapter>(
                onboardingControllerBaseUrl,
                deviceRegistryBaseUrl,
                systemRegistryBaseUrl);
    if(authMethod == HttpClient::AuthenticationMethod::BasicAuthentication){
        ahAdapter->getHttpClient().setSslConfig(caCertificateFile, disableHostnameVerification);
        ahAdapter->getHttpClient().setSharedSecret(sharedSecret);
    }
    else {
        ahAdapter->getHttpClient().setSslConfig(
                        keyStore, keyStorePwd, disableHostnameVerification);
    }


}

void TestQArrowheadHttpClientDeviceAdapter::getOnboardingControllerEcho()
{
    QCOMPARE(ReturnValue::Ok, ahAdapter->getOnboardingControllerEcho());
}

void TestQArrowheadHttpClientDeviceAdapter::getSystemRegistryEcho(){
    QCOMPARE(ReturnValue::Ok, ahAdapter->getSystemRegistryEcho());
}

void TestQArrowheadHttpClientDeviceAdapter::getDeviceRegistryEcho(){
    QCOMPARE(ReturnValue::Ok, ahAdapter->getDeviceRegistryEcho());
}


void TestQArrowheadHttpClientDeviceAdapter::requestOnboardingWithName()
{
    std::unique_ptr<OnboardingResponse> uptr_onboardingResponse;

    KeyPair keyPair("", "PKCS#8", onboardingPrivateKey,onboardingPublicKey);

    std::string commonName = testDevices[1].deviceName;
    OnboardingWithNameRequest onboardingRequest(CertificateCreationRequest(commonName, keyPair));


    QCOMPARE(ReturnValue::Ok, ahAdapter->requestOnboardingWithName(onboardingRequest, uptr_onboardingResponse));
}

void TestQArrowheadHttpClientDeviceAdapter::requestOnboardingWithCsr(){
    std::unique_ptr<OnboardingResponse> uptr_onboardingResponse;

    OnboardingWithCsrRequest onboardingRequest(onboardingCSR);
    QCOMPARE(ReturnValue::Ok, ahAdapter->requestOnboardingWithCsr(onboardingRequest, uptr_onboardingResponse));

    if (uptr_onboardingResponse != nullptr) {
        onboardingCertificate = uptr_onboardingResponse->onboardingCertificate.certificate;
    }
}


void TestQArrowheadHttpClientDeviceAdapter::registerDevice(){
    std::unique_ptr<arrowhead::DeviceRegistryEntry> uptr_deviceRegistryEntry;

    DeviceRegistrationForm deviceRegistrationForm;
    deviceRegistrationForm.device = testDevices[0];
    deviceRegistrationForm.version = 0;

    ahAdapter->getHttpClient().setOnboardingSslConfig(onboardingCertificate, onboardingPrivateKey);
    QCOMPARE(ReturnValue::Ok, ahAdapter->registerDevice(deviceRegistrationForm, uptr_deviceRegistryEntry));

    if (uptr_deviceRegistryEntry != nullptr) {
        QCOMPARE(testDevices[0].deviceName, uptr_deviceRegistryEntry->device.deviceName);
        registeredDevices.push_back(*uptr_deviceRegistryEntry);
    }
}

void TestQArrowheadHttpClientDeviceAdapter::registerOnboardingDeviceWithName(){

    std::unique_ptr<DeviceOnboardingResponse> uptr_onboardingResponse;

    KeyPair keyPair("", "PKCS#8", onboardingPrivateKey, onboardingPublicKey);
    std::string commonName = testDevices[1].deviceName;

    DeviceOnboardingWithNameRequest onboardingRequest(
                CertificateCreationRequest{commonName, keyPair},
                testDevices[1],
                "", std::map<std::string,std::string>{},0);                                               //version

    QCOMPARE(ReturnValue::Ok, ahAdapter->registerOnboardingDeviceWithName(onboardingRequest, uptr_onboardingResponse));

    if (uptr_onboardingResponse != nullptr) {
        QCOMPARE(testDevices[1].deviceName, uptr_onboardingResponse->deviceRegistryEntry.device.deviceName);
        registeredDevices.push_back(uptr_onboardingResponse->deviceRegistryEntry);
    }
}

void TestQArrowheadHttpClientDeviceAdapter::registerOnboardingDeviceWithCsr(){
    std::unique_ptr<DeviceOnboardingResponse> uptr_onboardingResponse;


    DeviceOnboardingWithCsrRequest onboardingRequest(
                onboardingCSR,
                testDevices[2],
                "", std::map<std::string,std::string>{}, 0);

    QCOMPARE(ReturnValue::Ok, ahAdapter->registerOnboardingDeviceWithCsr(onboardingRequest, uptr_onboardingResponse));

    if (uptr_onboardingResponse != nullptr) {
        QCOMPARE(testDevices[2].deviceName, uptr_onboardingResponse->deviceRegistryEntry.device.deviceName);
        registeredDevices.push_back(uptr_onboardingResponse->deviceRegistryEntry);
        deviceCertificate = uptr_onboardingResponse->certificateResponse.certificate;
    }
}

void TestQArrowheadHttpClientDeviceAdapter::queryDevice(){
    DeviceQueryForm deviceQueryForm;
    deviceQueryForm.deviceNameRequirements = testDevices[0].deviceName;
    deviceQueryForm.versionRequirement = 0;
    deviceQueryForm.minVersionRequirement = 0;
    deviceQueryForm.maxVersionRequirement = 0;
    std::unique_ptr<DeviceQueryList> uptr_deviceQueryList;

    QCOMPARE(ReturnValue::Ok, ahAdapter->queryDevice(deviceQueryForm, uptr_deviceQueryList));

    if (uptr_deviceQueryList != nullptr && !uptr_deviceQueryList->devices.empty()) {
        QCOMPARE(testDevices[0].deviceName, uptr_deviceQueryList->devices[0].device.deviceName);
    }
}


void TestQArrowheadHttpClientDeviceAdapter::registerSystem(){
    std::unique_ptr<arrowhead::SystemRegistryEntry> uptr_systemRegistryEntry;

    SystemRegistrationForm systemRegistrationForm;
    systemRegistrationForm.system = testSystems[0];
    systemRegistrationForm.provider = testDevices[0];
    systemRegistrationForm.version = 0;

    ahAdapter->getHttpClient().setOnboardingSslConfig(deviceCertificate, onboardingPrivateKey);
    QCOMPARE(ReturnValue::Ok, ahAdapter->registerSystem(systemRegistrationForm, uptr_systemRegistryEntry));

    if (uptr_systemRegistryEntry != nullptr) {
        QCOMPARE(testSystems[0].systemName, uptr_systemRegistryEntry->system.systemName);
        QCOMPARE(testDevices[0].deviceName, uptr_systemRegistryEntry->provider.deviceName);
        registeredSystems.push_back(*uptr_systemRegistryEntry);
    }

}

void TestQArrowheadHttpClientDeviceAdapter::registerOnboardingSystemWithName(){
    std::unique_ptr<SystemOnboardingResponse> uptr_onboardingResponse;

    KeyPair keyPair("", "PKCS#8", onboardingPrivateKey, onboardingPublicKey);
    std::string commonName = testSystems[1].systemName;

    SystemOnboardingWithNameRequest onboardingRequest(
                CertificateCreationRequest(commonName, keyPair),  //csr
                testDevices[1],      //device
                testSystems[1],      //system
                "",                                               //endOfValidity
                stringmap{},                                      //metadata
                0);                                               //version

    QCOMPARE(ReturnValue::Ok, ahAdapter->registerOnboardingSystemWithName(onboardingRequest, uptr_onboardingResponse));

    if (uptr_onboardingResponse != nullptr) {
        QCOMPARE(testSystems[1].systemName, uptr_onboardingResponse->systemRegistryEntry.system.systemName);
        QCOMPARE(testDevices[1].deviceName, uptr_onboardingResponse->systemRegistryEntry.provider.deviceName);
        registeredSystems.push_back(uptr_onboardingResponse->systemRegistryEntry);
    }

}

void TestQArrowheadHttpClientDeviceAdapter::registerOnboardingSystemWithCsr(){
    std::unique_ptr<SystemOnboardingResponse> uptr_onboardingResponse;

    SystemOnboardingWithCsrRequest onboardingRequest(
                onboardingCSR,          //csr
                testDevices[2],      //device
                testSystems[2],     //system
                "",                                 //endOfValidity
                stringmap{},                        //metadata
                0);                                 //version

    QCOMPARE(ReturnValue::Ok, ahAdapter->registerOnboardingSystemWithCsr(onboardingRequest, uptr_onboardingResponse));

    if (uptr_onboardingResponse != nullptr) {
        QCOMPARE(testSystems[2].systemName, uptr_onboardingResponse->systemRegistryEntry.system.systemName);
        QCOMPARE(testDevices[2].deviceName, uptr_onboardingResponse->systemRegistryEntry.provider.deviceName);
        registeredSystems.push_back(uptr_onboardingResponse->systemRegistryEntry);
    }
}

void TestQArrowheadHttpClientDeviceAdapter::querySystem(){
    SystemQueryForm systemQueryForm;
    systemQueryForm.systemNameRequirements = testSystems[0].systemName;
    systemQueryForm.pingProviders = false;
    systemQueryForm.versionRequirement = 0;
    systemQueryForm.minVersionRequirement = 0;
    systemQueryForm.maxVersionRequirement = 0;
    std::unique_ptr<SystemQueryList> uptr_systemQueryList;

    QCOMPARE(ReturnValue::Ok, ahAdapter->querySystem(systemQueryForm, uptr_systemQueryList));

    if (uptr_systemQueryList != nullptr && !uptr_systemQueryList->systems.empty()) {
        QCOMPARE(testSystems[0].systemName, uptr_systemQueryList->systems[0].system.systemName);
    }
}


void TestQArrowheadHttpClientDeviceAdapter::unregisterSystem(){
    for (auto& systemEntry : registeredSystems)
        QCOMPARE(ReturnValue::Ok, ahAdapter->unregisterSystem(systemEntry));
}

void TestQArrowheadHttpClientDeviceAdapter::unregisterDevice(){
    ahAdapter->getHttpClient().setOnboardingSslConfig(onboardingCertificate, onboardingPrivateKey);
    for (auto& deviceEntry : registeredDevices)
        QCOMPARE(ReturnValue::Ok, ahAdapter->unregisterDevice(deviceEntry));
}
