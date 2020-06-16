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

#ifndef TESTQARROWHEADHTTPCLIENTSYSTEMADAPTER_H
#define TESTQARROWHEADHTTPCLIENTSYSTEMADAPTER_H

#include "testsuite.h"
#include "qarrowheadhttpclientsystemadapter.h"
#include <QObject>

class TestQArrowheadHttpClientSystemAdapter : public TestSuite
{
    Q_OBJECT
public:
    using TestSuite::TestSuite;

private slots:
    void initTestCase();
    void getServiceRegistryEcho();
    void queryService();
    void registerService();
    void unregisterService();
    void getAuthorizationEcho();
    void getPublicKey();
    void getOrchestratorEcho();
    void requestOrchestration();
    void requestOrchestrationById();

private:
    std::string iniFileName = INI_FILE;
    std::string serviceRegistryBaseUrl;
    std::string authorizationBaseUrl;
    std::string orchestratorBaseUrl;
    std::string systemName;
    std::string address;
    int port;
    bool sslEnabled;
    std::string keyStore;
    std::string keyStorePwd;
    bool disableHostnameVerification;
    std::string providedServiceDefinition;
    std::string providedServiceInterface;
    std::string providedServiceUri;
    std::string requestedServiceDefinition;
    std::string requestedServiceInterface;

    std::unique_ptr<arrowhead::QArrowheadHttpClientSystemAdapter> ahAdapter;
    std::unique_ptr<arrowhead::ServiceRegistryEntry> uptr_serviceRegistryEntry;
    std::string authorizationPK;
};

#endif // TESTQARROWHEADHTTPCLIENTSYSTEMADAPTER_H
