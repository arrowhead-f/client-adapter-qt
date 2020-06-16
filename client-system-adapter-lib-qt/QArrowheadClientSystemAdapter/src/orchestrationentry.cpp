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

#include "orchestrationentry.h"
#include <QJsonArray>

namespace arrowhead {

std::string typeToString(const OrchestrationEntry::Warning &type){
    std::string typeString;
    switch (type) {
    case OrchestrationEntry::Warning::TtlExpired:
        typeString = "TTL_EXPIRED";
        break;
    case OrchestrationEntry::Warning::TtlUnknown:
        typeString = "TTL_UNKNWON";
        break;
    case OrchestrationEntry::Warning::TtlExpiring:
        typeString = "TTL_EXPIRING";
        break;
    case OrchestrationEntry::Warning::FromOtherCloud:
        typeString = "FROM_OTHER_CLOUD";
        break;
    case OrchestrationEntry::Warning::Invalid:
        typeString = "";
        break;
    }
    return typeString;
}

OrchestrationEntry::Warning orchestrationWarningFromString(const std::string &typeString){
    if(typeString == "TTL_EXPIRED"){
        return OrchestrationEntry::Warning::TtlExpired;
    }
    else if(typeString == "TTL_UNKNWON"){
        return OrchestrationEntry::Warning::TtlUnknown;
    }
    else if(typeString == "TTL_EXPIRING"){
        return OrchestrationEntry::Warning::TtlExpiring;
    }
    else if(typeString == "FROM_OTHER_CLOUD"){
        return OrchestrationEntry::Warning::FromOtherCloud;
    }
    else{
        return OrchestrationEntry::Warning::Invalid;
    }
}

OrchestrationEntry OrchestrationEntry::fromJsonObject(const QJsonObject &jsonObj){

    ArrowheadService service = ArrowheadService::fromJsonObject(jsonObj);
    RegistryTag serviceTag = RegistryTag::fromJsonObject(jsonObj["service"].toObject());

    ArrowheadSystem provider = ArrowheadSystem::fromJsonObject(jsonObj["provider"].toObject());
    RegistryTag providerTag = RegistryTag::fromJsonObject(jsonObj["provider"].toObject());

    QJsonObject aTokensJsonObj = jsonObj["authorizationTokens"].toObject();
    stringmap authorizationTokens;
    for(const auto &key : aTokensJsonObj.keys()){
        authorizationTokens.emplace(std::make_pair(
                                        key.toStdString(),
                                        aTokensJsonObj[key].toString().toStdString()));
    }

    QJsonArray warnJsonArray = jsonObj["warnings"].toArray();
    std::vector<Warning> warnings;
    for(const auto warn : warnJsonArray){
        warnings.push_back(orchestrationWarningFromString(warn.toString().toStdString()));
    }

    return OrchestrationEntry(ServiceEntry(service, serviceTag),
                              ProviderEntry(provider, providerTag),
                              authorizationTokens, warnings);
}

}
