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

#include "certificate.h"

namespace arrowhead {

std::string Certificate::typeToString(CertificateType type)
{
    switch(type){
    case CertificateType::Ah_Onboarding:
        return "AH_ONBOARDING";
    case CertificateType::Ah_Device:
        return "AH_DEVICE";
    case CertificateType::Ah_System:
        return "AH_SYSTEM";
    default:
        return "INVALID";
    }
}

Certificate::CertificateType Certificate::certificateTypeFromString(std::string typeString){
    if(typeString == "AH_ONBOARDING")
        return CertificateType::Ah_Onboarding;
    else if(typeString == "AH_DEVICE")
        return CertificateType::Ah_Device;
    else if(typeString == "AH_SYSTEM")
        return CertificateType::Ah_System;
    else return CertificateType::Invalid;
}

QJsonObject Certificate::toJsonObject() const{
    QJsonObject certJson;
    certJson.insert("certificate", certificate.data());
    certJson.insert("certificateFormat", certificateFormat.data());
    certJson.insert("certificateType", typeToString(certificateType).data());
    certJson.insert("keyPairDTO", keyPair.toJsonObject());
    return certJson;
}

Certificate Certificate::fromJsonObject(const QJsonObject &certJson){
    auto certificate = certJson["certificate"].toString().toStdString();
    auto certificateFormat = certJson["certificateFormat"].toString().toStdString();
    auto certificateType = certificateTypeFromString(certJson["certificateType"].toString().toStdString());
    auto keyPair = KeyPair::fromJsonObject(certJson["keyPairDTO"].toObject());
    return Certificate(certificate, certificateFormat, certificateType, keyPair);
}

}

