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

#ifndef CERTIFICATE_H
#define CERTIFICATE_H

#include "qarrowheadclientdeviceadapter_global.h"
#include "keypair.h"
#include <string>
#include <QJsonObject>

namespace arrowhead {

struct QAHDEVADAPTER_EXPORT Certificate
{
    enum class CertificateType{
        Ah_Onboarding,
        Ah_Device,
        Ah_System,
        Invalid
    };

    std::string certificate;
    std::string certificateFormat;
    CertificateType certificateType;
    KeyPair keyPair;

    Certificate() = default;
    Certificate(std::string certificate, std::string certificateFormat,
                CertificateType certificateType, KeyPair keyPair) :
        certificate(certificate), certificateFormat(certificateFormat),
        certificateType(certificateType), keyPair(keyPair) {}
    QJsonObject toJsonObject() const;
    static Certificate fromJsonObject(const QJsonObject &certJson);
    static std::string typeToString(CertificateType type);
    static CertificateType certificateTypeFromString(std::string typeString);
};

}

#endif // CERTIFICATE_H
