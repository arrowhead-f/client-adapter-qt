#include "common.h"

namespace arrowhead {
std::string typeToString(const SecurityType& type){
    std::string typeString;
    switch(type) {
    case SecurityType::NotSecure:
        typeString = "NOT_SECURE";
        break;
    case SecurityType::Certificate:
        typeString = "CERTIFICATE";
        break;
    case SecurityType::Token:
        typeString = "TOKEN";
        break;
    case SecurityType::Invalid:
        typeString = "";
        break;
    }
    return typeString;
}

SecurityType securityTypeFromString(const std::string& typeString){
    if(typeString == "NOT_SECURE"){
        return SecurityType::NotSecure;
    }
    else if(typeString == "CERTIFICATE"){
        return SecurityType::Certificate;
    }
    else if(typeString == "TOKEN"){
        return SecurityType::Token;
    }
    else{
        return SecurityType::Invalid;
    }
}

QJsonObject RegistryTag::toJsonObject() const{
    QJsonObject jsonObject;
    jsonObject.insert("id", id);
    jsonObject.insert("createdAt", createdAt.data());
    jsonObject.insert("updatedAt", updatedAt.data());
    return jsonObject;
}

RegistryTag RegistryTag::fromJsonObject(const QJsonObject &jsonObject){
    int id = jsonObject["id"].toInt();
    std::string createdAt = jsonObject["createdAt"].toString().toStdString();
    std::string updatedAt = jsonObject["updatedAt"].toString().toStdString();
    return RegistryTag(id, createdAt, updatedAt);
}

}
