#include "arrowheadbasetypes.h"
#include <QJsonArray>
#include <QStringList>

namespace arrowhead {

QJsonObject ArrowheadCloud::toJsonObject() const {
    QJsonObject jsonObject;
    jsonObject.insert("operator", operatorName.data());
    jsonObject.insert("name", cloudName.data());
    return jsonObject;
}

ArrowheadCloud ArrowheadCloud::fromJsonObject(const QJsonObject& jsonObject){
    std::string operatorName = jsonObject["operator"].toString().toStdString();
    std::string cloudName = jsonObject["name"].toString().toStdString();
    return ArrowheadCloud(operatorName, cloudName);
}

QJsonObject ArrowheadSystem::toJsonObject() const {
    QJsonObject jsonObject;
    jsonObject.insert("systemName", systemName.data());
    jsonObject.insert("address", address.data());
    jsonObject.insert("port", port);
    jsonObject.insert("authenticationInfo", authenticationInfo.data());
    return jsonObject;
}

ArrowheadSystem ArrowheadSystem::fromJsonObject(const QJsonObject& jsonObject){
    std::string systemName = jsonObject["systemName"].toString().toStdString();
    std::string address = jsonObject["address"].toString().toStdString();
    int port = jsonObject["port"].toInt();
    std::string authenticationInfo = jsonObject["authenticationInfo"].toString().toStdString();
    return ArrowheadSystem(systemName, address, port, authenticationInfo);
}

QJsonObject ArrowheadService::toJsonObject() const {
    // Mandatory fields: serviceDefinition, interfaces, serviceUri
    QJsonObject jsonObject;
    jsonObject.insert("serviceDefinition", serviceDefinition.data());
    QJsonArray ifaceJsonArray;
    for(auto iface : interfaces){
        ifaceJsonArray.append(iface.data());
    }
    jsonObject.insert("interfaces", ifaceJsonArray);
    jsonObject.insert("serviceUri", serviceUri.data());
    if(securityType != SecurityType::Invalid){
        jsonObject.insert("secure", typeToString(securityType).data());
    }

    if(!metadata.empty()){
        QJsonObject metadataJsonObject;
        for(auto kvpair : metadata){
            metadataJsonObject.insert(kvpair.first.data(), kvpair.second.data());
        }
        jsonObject.insert("metadata", metadataJsonObject);
    }

    if(version != 0){
        jsonObject.insert("version", version);
    }
    if(!endOfValidity.empty()){
        jsonObject.insert("endOfValidity", endOfValidity.data());
    }

    return jsonObject;
}

ArrowheadService ArrowheadService::fromJsonObject(const QJsonObject& jsonObject){
    QJsonObject servDefJsonObject = jsonObject["serviceDefinition"].toObject();
    if(servDefJsonObject.empty()){
        servDefJsonObject = jsonObject["service"].toObject();
    }
    std::string serviceDefinition = servDefJsonObject["serviceDefinition"].toString().toStdString();
    QJsonArray ifaceJsonArray = jsonObject["interfaces"].toArray();
    stringvector interfaces;
    for(auto iface : ifaceJsonArray){
        interfaces.push_back(iface.toString().toStdString());
    }
    SecurityType securityType = securityTypeFromString(jsonObject["secure"].toString().toStdString());
    QJsonObject metadataJsonObject = jsonObject["metadata"].toObject();
    stringmap metadata;
    for(auto key : metadataJsonObject.keys()){
        metadata.emplace(std::make_pair(
                             key.toStdString(),
                             metadataJsonObject[key].toString().toStdString()));
    }
    int version = jsonObject["version"].toInt();
    std::string serviceUri = jsonObject["serviceUri"].toString().toStdString();
    std::string endOfValidity = jsonObject["endOfValidity"].toString().toStdString();
    return ArrowheadService(serviceDefinition,
                            interfaces,
                            securityType,
                            metadata,
                            version,
                            serviceUri,
                            endOfValidity);
}

bool ArrowheadService::operator==(const ArrowheadService &other) const{
    if(other.serviceDefinition != serviceDefinition) return false;
    if(other.interfaces != interfaces) return false;
    if(other.securityType != securityType) return false;
    if(other.metadata != metadata) return false;
    if(other.version != version) return false;
    if(other.serviceUri != serviceUri) return false;
    if(other.endOfValidity != endOfValidity) return false;
    return true;
}

}
