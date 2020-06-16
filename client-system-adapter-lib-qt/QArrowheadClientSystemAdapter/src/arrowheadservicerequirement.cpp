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

#include "arrowheadservicerequirement.h"
#include <QJsonArray>

namespace arrowhead {

QJsonObject ArrowheadServiceRequirement::toJsonObject() const
{
    // Mandatory fields: serviceDefinitionRequirement
    QJsonObject jsonObject;
    jsonObject.insert("serviceDefinitionRequirement", serviceDefinitionRequirement.data());

    if(!interfaceRequirements.empty()){
        QJsonArray ifaceJsonArray;
        for(auto &iface : interfaceRequirements){
            ifaceJsonArray.append(iface.data());
        }
        jsonObject.insert("interfaceRequirements", ifaceJsonArray);
    }

    if(!securityRequirements.empty()){
        QJsonArray securityJsonArray;
        for(auto sec : securityRequirements){
            securityJsonArray.append(typeToString(sec).data());
        }
        jsonObject.insert("securityRequirements", securityJsonArray);
    }

    if(!metadataRequirements.empty()){
        QJsonObject metadataJsonObject;
        for(auto kvpair : metadataRequirements){
            metadataJsonObject.insert(kvpair.first.data(), kvpair.second.data());
        }
        jsonObject.insert("metadataRequirements", metadataJsonObject);
    }

    if(versionRequirement != 0)
        jsonObject.insert("versionRequirement", versionRequirement);

    if(minVersionRequirement != 0)
        jsonObject.insert("minVersionRequirement", minVersionRequirement);

    if(maxVersionRequirement != 0)
        jsonObject.insert("maxVersionRequirement", maxVersionRequirement);

    return jsonObject;
}

ArrowheadServiceRequirement::builder& ArrowheadServiceRequirement::builder::setServiceDefinitionRequirement(std::string value){
    serviceDefinitionRequirement = value;
    return *this;
}

ArrowheadServiceRequirement::builder& ArrowheadServiceRequirement::builder::setInterfaceRequirements(stringvector value){
    interfaceRequirements = value;
    return *this;
}

ArrowheadServiceRequirement::builder& ArrowheadServiceRequirement::builder::setSecurityRequirements(std::vector<SecurityType> value){
    securityRequirements = value;
    return *this;
}

ArrowheadServiceRequirement::builder& ArrowheadServiceRequirement::builder::setMetadataRequirements(stringmap value){
    metadataRequirements = value;
    return *this;
}

ArrowheadServiceRequirement::builder& ArrowheadServiceRequirement::builder::setVersionRequirement(int value){
    versionRequirement = value;
    return *this;
}

ArrowheadServiceRequirement::builder& ArrowheadServiceRequirement::builder::setMinVersionRequirement(int value){
    minVersionRequirement = value;
    return *this;
}

ArrowheadServiceRequirement::builder& ArrowheadServiceRequirement::builder::setMaxVersionRequirement(int value){
    maxVersionRequirement = value;
    return *this;
}

ArrowheadServiceRequirement ArrowheadServiceRequirement::builder::build() const{
    return ArrowheadServiceRequirement(serviceDefinitionRequirement,
                            interfaceRequirements,
                            securityRequirements,
                            metadataRequirements,
                            versionRequirement,
                            minVersionRequirement,
                            maxVersionRequirement);
}

}
