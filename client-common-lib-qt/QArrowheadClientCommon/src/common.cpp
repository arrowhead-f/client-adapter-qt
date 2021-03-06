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
