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
