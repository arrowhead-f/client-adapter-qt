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

#include "servicerequestform.h"
#include <QJsonArray>

namespace arrowhead {

QJsonObject ServiceRequestForm::PreferredProvider::toJsonObject() const{
    QJsonObject jsonObject;
    jsonObject.insert("providerCloud", providerCloud.toJsonObject());
    jsonObject.insert("providerSystem", providerSystem.toJsonObject());
    return jsonObject;
}

std::string typeToString(const ServiceRequestForm::OrchestrationFlagKey& type){
    std::string typeString;
    switch (type) {
    case ServiceRequestForm::OrchestrationFlagKey::MatchMaking:
        typeString = "matchmaking";
        break;
    case ServiceRequestForm::OrchestrationFlagKey::OnlyPreferred:
        typeString = "onlyPreferred";
        break;
    case ServiceRequestForm::OrchestrationFlagKey::OverrideStore:
        typeString = "overrideStore";
        break;
    case ServiceRequestForm::OrchestrationFlagKey::PingProviders:
        typeString = "pingProviders";
        break;
    case ServiceRequestForm::OrchestrationFlagKey::MetadataSearch:
        typeString = "metadataSearch";
        break;
    case ServiceRequestForm::OrchestrationFlagKey::EnableInterCloud:
        typeString = "enableInterCloud";
        break;
    case ServiceRequestForm::OrchestrationFlagKey::TriggerInterCloud:
        typeString = "triggerInterCloud";
        break;
    case ServiceRequestForm::OrchestrationFlagKey::Invalid:
        typeString = "";
        break;
    }
    return typeString;
}

ServiceRequestForm::OrchestrationFlagKey orchestrationFlagKeyFromString(const std::string& typeString){
    if(typeString == "matchmaking"){
        return ServiceRequestForm::OrchestrationFlagKey::MatchMaking;
    }
    else if(typeString == "onlyPreferred"){
        return ServiceRequestForm::OrchestrationFlagKey::OnlyPreferred;
    }
    else if(typeString == "overrideStore"){
        return ServiceRequestForm::OrchestrationFlagKey::OverrideStore;
    }
    else if(typeString == "pingProviders"){
        return ServiceRequestForm::OrchestrationFlagKey::PingProviders;
    }
    else if(typeString == "metadataSearch"){
        return ServiceRequestForm::OrchestrationFlagKey::MetadataSearch;
    }
    else if(typeString == "enableInterCloud"){
        return ServiceRequestForm::OrchestrationFlagKey::EnableInterCloud;
    }
    else if(typeString == "triggerInterCloud"){
        return ServiceRequestForm::OrchestrationFlagKey::TriggerInterCloud;
    }
    else{
        return ServiceRequestForm::OrchestrationFlagKey::Invalid;
    }
}

QJsonObject ServiceRequestForm::toJsonObject() const
{
    // Mandatory fields: requesterSystem, requestedService
    QJsonObject jsonObject;
    jsonObject.insert("requesterSystem", requesterSystem.toJsonObject());
    jsonObject.insert("requestedService", requestedService.toJsonObject());

    if(!preferredProviders.empty()){
        QJsonArray pProviderJsonArray;
        for(auto pProvider : preferredProviders){
            pProviderJsonArray.append(pProvider.toJsonObject());
        }
        jsonObject.insert("preferredProviders", pProviderJsonArray);
    }

    if(!orchestrationFlags.empty()){
        QJsonObject oFlagsJsonObject;
        for(auto kvpair : orchestrationFlags){
            oFlagsJsonObject.insert(typeToString(kvpair.first).data(), kvpair.second);
        }
        jsonObject.insert("orchestrationFlags", oFlagsJsonObject);
    }

    return jsonObject;
}


ServiceRequestForm::builder::builder() :
    requestedService(ArrowheadServiceRequirement::builder().build()) {}

ServiceRequestForm::builder& ServiceRequestForm::builder::setRequestedService(ArrowheadServiceRequirement value){
    requestedService = value;
    return *this;
}

ServiceRequestForm::builder& ServiceRequestForm::builder::setRequesterSystem(ArrowheadSystem value){
    requesterSystem = value;
    return *this;
}

ServiceRequestForm::builder& ServiceRequestForm::builder::setPreferredProviders(std::vector<PreferredProvider> value){
    preferredProviders = value;
    return *this;
}

ServiceRequestForm::builder& ServiceRequestForm::builder::setOrchestrationFlags(std::map<OrchestrationFlagKey, bool> value){
    orchestrationFlags = value;
    return *this;
}

ServiceRequestForm ServiceRequestForm::builder::build() const{
    return ServiceRequestForm(requestedService, requesterSystem,
                              preferredProviders, orchestrationFlags);
}

}
