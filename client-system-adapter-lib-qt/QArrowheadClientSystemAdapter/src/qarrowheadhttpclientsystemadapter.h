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

#include "qarrowheadclientsystemadapter.h"
#include <QSslConfiguration>
#include <QNetworkReply>
#include <QNetworkAccessManager>

namespace arrowhead {

class QArrowheadHttpClientSystemAdapter : public QArrowheadClientSystemAdapter
{
public:
    QArrowheadHttpClientSystemAdapter(
            std::string serviceRegsitryBaseUrl,
            std::string authorizationBaseUrl,
            std::string orchestratorBaseUrl,
            std::unique_ptr<ArrowheadSystem> clientSystem) :
        QArrowheadClientSystemAdapter (serviceRegsitryBaseUrl,
                                       authorizationBaseUrl,
                                       orchestratorBaseUrl,
                                       std::move(clientSystem)) {}

    ReturnValue setSslConfig(std::string p12FileName, std::string password, bool disableHostnameVerification=false);

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

private:
    ReturnValue performHttpOperation(QNetworkAccessManager::Operation operationType,
                                     std::unique_ptr<QNetworkRequest> &request, std::unique_ptr<QNetworkReply> &reply,
                                     std::shared_ptr<const QByteArray> payload = nullptr);
    const QString typeToString(const QNetworkAccessManager::Operation &operationType) const;
    QNetworkAccessManager qnam;
    QSslConfiguration sslConfig;
    bool disableHostnameVerification=false;
};

}

#endif // QARROWHEADHTTPCLIENTSYSTEMADAPTER_H
