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

#ifndef QARROWHEADHTTPCLIENTSYSTEMADAPTER_H
#define QARROWHEADHTTPCLIENTSYSTEMADAPTER_H

#include "qarrowheadclientsystemadapter_global.h"
#include "httpclient.h"
#include "qarrowheadclientsystemadapter.h"
#include <QSslConfiguration>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QSslKey>

namespace arrowhead {

class QAHSYSADAPTER_EXPORT QArrowheadHttpClientSystemAdapter : public QArrowheadClientSystemAdapter
{
public:
    using QArrowheadClientSystemAdapter::QArrowheadClientSystemAdapter;

    // Service Registry interface
    ReturnValue getServiceRegistryEcho();
    ReturnValue registerService(const ServiceRegistrationForm& serviceRegistrationForm, std::unique_ptr<ServiceRegistryEntry>& uptr_ServiceRegistryEntry);
    ReturnValue unregisterService(const ServiceRegistryEntry& serviceRegistryEntry);
    ReturnValue queryService(const ServiceQueryForm& serviceQueryForm, std::unique_ptr<ServiceQueryList>& uptr_ServiceQueryList);

    // Authorization System interface
    ReturnValue getAuthorizationSystemEcho();
    ReturnValue getPublicKey(std::string& publicKey);

    // Orchestrator interface
    ReturnValue getOrchestratorEcho();
    ReturnValue requestOrchestration(const ServiceRequestForm& serviceRequestForm, std::unique_ptr<OrchestrationResponse>& uptr_orchestrationResponse);
    ReturnValue requestOrchestrationById(const int &storeEntryId, std::unique_ptr<OrchestrationResponse>& uptr_orchestrationResponse);

    HttpClient& getHttpClient(){
        return httpClient;
    }

private:
    HttpClient httpClient;
};

}

#endif // QARROWHEADHTTPCLIENTSYSTEMADAPTER_H
