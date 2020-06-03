#include "testqarrowheadhttpclientsystemadapter.h"
#include <QtTest/QTest>
#include <QSettings>

static TestQArrowheadHttpClientSystemAdapter TEST_QARROWHEADHTTPCLIENTSYSTEMADAPTER;

using namespace arrowhead;

void TestQArrowheadHttpClientSystemAdapter::initTestCase()
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

    ahSettings.beginGroup("coreSystems");
    serviceRegistryBaseUrl = getStringParam("serviceRegistryBaseUrl");
    authorizationBaseUrl = getStringParam("authorizationBaseUrl");
    orchestratorBaseUrl = getStringParam("orchestratorBaseUrl");
    ahSettings.endGroup();

    ahSettings.beginGroup("mySystem");
    systemName = getStringParam("systemName");
    address = getStringParam("address");
    port = getIntParam("port");
    sslEnabled = getBoolParam("sslEnabled");
    if(sslEnabled){
        keyStore = (QString(CONF_DIR) + getStringParam("keyStore").data()).toStdString();
        keyStorePwd = getStringParam("keyStorePwd");
        disableHostnameVerification = getBoolParam("disableHostnameVerification");
    }
    ahSettings.endGroup();

    ahSettings.beginGroup("providedService");
    providedServiceDefinition = getStringParam("serviceDefinition");
    providedServiceInterface = getStringParam("interface");
    providedServiceUri = getStringParam("serviceURI");
    ahSettings.endGroup();

    ahSettings.beginGroup("requestedService");
    requestedServiceDefinition = getStringParam("serviceDefinition");
    requestedServiceInterface = getStringParam("interface");
    ahSettings.endGroup();

    auto clientSystem = std::make_unique<ArrowheadSystem>(
                systemName, address, port);
    ahAdapter = std::make_unique<QArrowheadHttpClientSystemAdapter>(
                serviceRegistryBaseUrl, authorizationBaseUrl, orchestratorBaseUrl,
                std::move(clientSystem));
    if(sslEnabled){
        ahAdapter->setSslConfig(keyStore, keyStorePwd, disableHostnameVerification);
    }
}

void TestQArrowheadHttpClientSystemAdapter::getServiceRegistryEcho()
{
    QCOMPARE(ReturnValue::Ok, ahAdapter->getServiceRegistryEcho());
}

void TestQArrowheadHttpClientSystemAdapter::queryService()
{
    ArrowheadServiceRequirement requestedService =
            ArrowheadServiceRequirement::builder()
            .setServiceDefinitionRequirement(requestedServiceDefinition)
            .build();
    std::unique_ptr<ServiceQueryList> uptr_serviceQueryList;
    ServiceQueryForm serviceQueryForm = ServiceQueryForm::builder()
            .setRequestedService(requestedService)
            .build();
    if(sslEnabled){
        QCOMPARE(ReturnValue::UnknownError, ahAdapter->queryService(serviceQueryForm, uptr_serviceQueryList));
    }
    else{
        QCOMPARE(ReturnValue::Ok, ahAdapter->queryService(serviceQueryForm, uptr_serviceQueryList));
        if(uptr_serviceQueryList != nullptr){
            if(!uptr_serviceQueryList->srEntryVector.empty()){
                QCOMPARE("service-register", uptr_serviceQueryList->srEntryVector[0].registeredService.service.serviceDefinition);
                QCOMPARE("service_registry", uptr_serviceQueryList->srEntryVector[0].registeredProvider.provider.systemName);
            }
        }
    }
}

void TestQArrowheadHttpClientSystemAdapter::registerService()
{
    ArrowheadService service = ArrowheadService::builder()
            .setServiceDefinition(providedServiceDefinition)
            .setInterfaces({providedServiceInterface})
            .setServiceUri(providedServiceUri)
            .build();
    ArrowheadSystem provider(systemName, address, port);
    ServiceRegistrationForm serviceRegistrationForm(service, provider);
    QCOMPARE(ReturnValue::Ok, ahAdapter->registerService(serviceRegistrationForm, uptr_serviceRegistryEntry));
    if(uptr_serviceRegistryEntry != nullptr){
        QCOMPARE("state-of-charge", uptr_serviceRegistryEntry->registeredService.service.serviceDefinition);
        QCOMPARE("canfilter", uptr_serviceRegistryEntry->registeredProvider.provider.systemName);
    }
}

void TestQArrowheadHttpClientSystemAdapter::unregisterService()
{
    QCOMPARE(ReturnValue::Ok, ahAdapter->unregisterService(*uptr_serviceRegistryEntry));
}

void TestQArrowheadHttpClientSystemAdapter::getAuthorizationEcho()
{
    QCOMPARE(ReturnValue::Ok, ahAdapter->getAuthorizationSystemEcho());
}

void TestQArrowheadHttpClientSystemAdapter::getPublicKey()
{
    if(sslEnabled){
        QCOMPARE(ReturnValue::Ok, ahAdapter->getPublicKey(authorizationPK));
        QVERIFY(!authorizationPK.empty());
    }
    else{
        QCOMPARE(ReturnValue::UnknownError, ahAdapter->getPublicKey(authorizationPK));
        QVERIFY(authorizationPK.empty());
    }
}

void TestQArrowheadHttpClientSystemAdapter::getOrchestratorEcho()
{
    QCOMPARE(ReturnValue::Ok, ahAdapter->getOrchestratorEcho());
}

void TestQArrowheadHttpClientSystemAdapter::requestOrchestration()
{
    std::unique_ptr<OrchestrationResponse> uptr_orchestrationResponse;
    ArrowheadServiceRequirement requestedService =
            ArrowheadServiceRequirement::builder()
            .setServiceDefinitionRequirement(requestedServiceDefinition)
            .setInterfaceRequirements({requestedServiceInterface})
            .build();
    ServiceRequestForm serviceRequestForm = ServiceRequestForm::builder()
            .setRequestedService(requestedService)
            .setRequesterSystem(ArrowheadSystem(systemName, address, port))
            .build();
    QCOMPARE(ReturnValue::Ok, ahAdapter->requestOrchestration(serviceRequestForm, uptr_orchestrationResponse));
    if(uptr_orchestrationResponse != nullptr){
        if(!uptr_orchestrationResponse->orchEntryVector.empty()){
            QCOMPARE("service-register", uptr_orchestrationResponse->orchEntryVector[0].orchestratedService.service.serviceDefinition);
            QCOMPARE("service_registry", uptr_orchestrationResponse->orchEntryVector[0].orchestratedProvider.provider.systemName);
        }
    }
}

void TestQArrowheadHttpClientSystemAdapter::requestOrchestrationById()
{
    std::unique_ptr<OrchestrationResponse> uptr_orchestrationResponse;
    int storeEntryId;
    if(uptr_serviceRegistryEntry != nullptr){
        storeEntryId = uptr_serviceRegistryEntry->registeredProvider.entryTag.id;
    }
    QCOMPARE(ReturnValue::Ok, ahAdapter->requestOrchestrationById(storeEntryId, uptr_orchestrationResponse));
}
