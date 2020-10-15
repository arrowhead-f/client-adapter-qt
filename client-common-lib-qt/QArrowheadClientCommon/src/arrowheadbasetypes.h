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

#ifndef ARROWHEADBASETYPES_H
#define ARROWHEADBASETYPES_H

#include "qarrowheadclientcommon_global.h"
#include "common.h"
#include <string>
#include <vector>
#include <map>
#include <QJsonObject>

namespace arrowhead {

struct QARROWHEADCLIENTCOMMON_EXPORT ArrowheadCloud{
    std::string operatorName;
    std::string cloudName;

    ArrowheadCloud(std::string operatorName, std::string cloudName) :
        operatorName(operatorName), cloudName(cloudName) {}

    QJsonObject toJsonObject() const;
    static ArrowheadCloud fromJsonObject(const QJsonObject& jsonObject);
};

struct QARROWHEADCLIENTCOMMON_EXPORT ArrowheadSystem{
    std::string systemName;
    std::string address;
    int port {8080};
    std::string authenticationInfo;

    ArrowheadSystem() = default;
    ArrowheadSystem(std::string systemName, std::string address,
                    int port, std::string authenticationInfo="") :
        systemName(systemName), address(address), port(port),
        authenticationInfo(authenticationInfo) {}

    QJsonObject toJsonObject() const;
    static ArrowheadSystem fromJsonObject(const QJsonObject& jsonObject);
};

using stringmap = std::map<std::string, std::string>;
using stringvector = std::vector<std::string>;

struct QARROWHEADCLIENTCOMMON_EXPORT ArrowheadService{
    std::string serviceDefinition;
    stringvector interfaces;
    SecurityType securityType;
    stringmap metadata;
    int version;
    std::string serviceUri;
    std::string endOfValidity;

    struct builder;

    QJsonObject toJsonObject() const;
    static ArrowheadService fromJsonObject(const QJsonObject& jsonObject);
    bool operator==(const ArrowheadService& other) const;

private:
    ArrowheadService(std::string serviceDefinition,
                     stringvector interfaces,
                     SecurityType securityType,
                     stringmap metadata,
                     int version,
                     std::string serviceUri,
                     std::string endOfValidity) :
        serviceDefinition(serviceDefinition), interfaces(interfaces),
        securityType(securityType), metadata(metadata), version(version),
        serviceUri(serviceUri), endOfValidity(endOfValidity) {}
};

struct QARROWHEADCLIENTCOMMON_EXPORT ArrowheadService::builder{
    std::string serviceDefinition;
    stringvector interfaces;
    SecurityType securityType = SecurityType::Invalid;
    stringmap metadata;
    int version = 0;
    std::string serviceUri;
    std::string endOfValidity;

    builder& setServiceDefinition(std::string value){
        serviceDefinition = value;
        return *this;
    }
    builder& setInterfaces(stringvector value){
        interfaces = value;
        return *this;
    }
    builder& setSecurityType(SecurityType value){
        securityType = value;
        return *this;
    }
    builder& setMetadata(stringmap value){
        metadata = value;
        return *this;
    }
    builder& setVersion(int value){
        version=value;
        return *this;
    }
    builder& setServiceUri(std::string value){
        serviceUri = value;
        return *this;
    }
    builder& setEndOfValidity(std::string value){
        endOfValidity = value;
        return *this;
    }

    ArrowheadService build() const{
        return ArrowheadService(serviceDefinition, interfaces,
                                securityType, metadata, version,
                                serviceUri, endOfValidity);
    }
};


}

#endif // ARROWHEADBASETYPES_H
