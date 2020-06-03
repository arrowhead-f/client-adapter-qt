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
