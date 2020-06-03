#ifndef QARROWHEADCLIENTSYSTEMADAPTER_H
#define QARROWHEADCLIENTSYSTEMADAPTER_H

#include "common.h"
#include "arrowheadbasetypes.h"
#include "servicequeryform.h"
#include "servicequerylist.h"
#include "serviceregistrationform.h"
#include "servicerequestform.h"
#include "orchestrationresponse.h"
#include <memory>
#include <QNetworkAccessManager>

namespace arrowhead {

/**
 * @brief The QArrowheadClientSystemAdapter interface
 *
 * This class manages the communication with the Arrowhead core systems (Service Registry, Authorization and Orchestrator).
 * It provides functions to register and delete services in the Service Registry, and to request services
 * from the Orchestrator.
 */
class QArrowheadClientSystemAdapter
{
public:
    QArrowheadClientSystemAdapter(
            std::string serviceRegsitryBaseUrl,
            std::string authorizationBaseUrl,
            std::string orchestratorBaseUrl,
            std::unique_ptr<ArrowheadSystem> clientSystem) :
        serviceRegsitryBaseUrl(serviceRegsitryBaseUrl), authorizationBaseUrl(authorizationBaseUrl),
        orchestratorBaseUrl(orchestratorBaseUrl), clientSystem(std::move(clientSystem)) {}

    virtual ~QArrowheadClientSystemAdapter() = default;

    // Service Registry interface
    virtual ReturnValue getServiceRegistryEcho() = 0;
    virtual ReturnValue registerService(const ServiceRegistrationForm& serviceRegistrationForm, std::unique_ptr<ServiceRegistryEntry>& uptr_ServiceRegistryEntry) = 0;
    virtual ReturnValue unregisterService(const ServiceRegistryEntry& serviceRegistryEntry) = 0;
    virtual ReturnValue queryService(const ServiceQueryForm& serviceQueryForm, std::unique_ptr<ServiceQueryList>& uptr_ServiceQueryList) = 0;

    // Authorization System interface
    virtual ReturnValue getAuthorizationSystemEcho() = 0;
    virtual ReturnValue getPublicKey(std::string& publicKey) = 0;

    // Orchestrator interface
    virtual ReturnValue getOrchestratorEcho() = 0;
    virtual ReturnValue requestOrchestration(const ServiceRequestForm& serviceRequestForm, std::unique_ptr<OrchestrationResponse>& uptr_orchestrationResponse) = 0;
    virtual ReturnValue requestOrchestrationById(const int &storeEntryId, std::unique_ptr<OrchestrationResponse>& uptr_orchestrationResponse) = 0;

protected:
    std::string serviceRegsitryBaseUrl;
    std::string authorizationBaseUrl;
    std::string orchestratorBaseUrl;
    std::unique_ptr<ArrowheadSystem> clientSystem;
};


}

#endif // QARROWHEADCLIENTSYSTEMADAPTER_H
