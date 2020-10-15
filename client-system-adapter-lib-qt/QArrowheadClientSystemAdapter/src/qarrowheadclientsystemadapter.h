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

#ifndef QARROWHEADCLIENTSYSTEMADAPTER_H
#define QARROWHEADCLIENTSYSTEMADAPTER_H

#include "qarrowheadclientsystemadapter_global.h"
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
class QAHSYSADAPTER_EXPORT QArrowheadClientSystemAdapter
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
