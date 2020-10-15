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

#ifndef TESTQARROWHEADHTTPCLIENTDEVICEADAPTER_H
#define TESTQARROWHEADHTTPCLIENTDEVICEADAPTER_H

#include "testsuite.h"
#include "qarrowheadhttpclientdeviceadapter.h"
#include <QObject>
#include "certificate.h"

class TestQArrowheadHttpClientDeviceAdapter : public TestSuite
{
    Q_OBJECT
public:
    using TestSuite::TestSuite;

private slots:
    void initTestCase();

    void getOnboardingControllerEcho();
    void getDeviceRegistryEcho();
    void getSystemRegistryEcho();

    void requestOnboardingWithName();
    void requestOnboardingWithCsr();

    void registerDevice();
    void registerOnboardingDeviceWithName();
    void registerOnboardingDeviceWithCsr();
    void queryDevice();

    void registerSystem();
    void registerOnboardingSystemWithName();
    void registerOnboardingSystemWithCsr();
    void querySystem();

    void unregisterSystem();
    void unregisterDevice();



private:
    std::string iniFileName = INI_FILE;
    
    std::string onboardingControllerBaseUrl;
    std::string deviceRegistryBaseUrl;
    std::string systemRegistryBaseUrl;

    arrowhead::HttpClient::AuthenticationMethod authMethod;
    std::string sharedSecret;
    QSslKey privateKey;
    QSslKey publicKey;
    std::string certificateFile;
    std::string privateKeyFile;
    std::string privateKeyPwd;
    std::string keyStore;
    std::string keyStorePwd;

    std::string caCertificateFile;
    bool disableHostnameVerification;

    std::unique_ptr<arrowhead::QArrowheadHttpClientDeviceAdapter> ahAdapter;

    std::vector<arrowhead::ArrowheadDevice> testDevices;
    std::vector<arrowhead::ArrowheadSystem> testSystems;

    std::vector<arrowhead::DeviceRegistryEntry> registeredDevices;
    std::vector<arrowhead::SystemRegistryEntry> registeredSystems;


    std::string onboardingCSR;
    std::string onboardingPrivateKey;
    std::string onboardingPublicKey;

    std::string onboardingCertificate;
    std::string deviceCertificate;

};

#endif // TESTQARROWHEADHTTPCLIENTDEVICEADAPTER_H
