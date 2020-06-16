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

#ifndef ARROWHEADSERVICEREQUIREMENT_H
#define ARROWHEADSERVICEREQUIREMENT_H

#include "common.h"
#include "arrowheadbasetypes.h"
#include <string>
#include <vector>
#include <QJsonObject>

namespace arrowhead {

struct ArrowheadServiceRequirement
{
    std::string serviceDefinitionRequirement;
    stringvector interfaceRequirements;
    std::vector<SecurityType> securityRequirements;
    stringmap metadataRequirements;
    int versionRequirement;
    int minVersionRequirement;
    int maxVersionRequirement;

    struct builder;

    QJsonObject toJsonObject() const;

private:
    ArrowheadServiceRequirement(std::string serviceDefinitionRequirement,
                                stringvector interfaceRequirements,
                                std::vector<SecurityType> securityRequirements,
                                stringmap metadataRequirements,
                                int versionRequirement,
                                int minVersionRequirement,
                                int maxVersionRequirement) :
                    serviceDefinitionRequirement(serviceDefinitionRequirement),
                    interfaceRequirements(interfaceRequirements),
                    securityRequirements(securityRequirements),
                    metadataRequirements(metadataRequirements),
                    versionRequirement(versionRequirement),
                    minVersionRequirement(minVersionRequirement),
                    maxVersionRequirement(maxVersionRequirement) {}
};

struct ArrowheadServiceRequirement::builder{
private:
    std::string serviceDefinitionRequirement;
    stringvector interfaceRequirements;
    std::vector<SecurityType> securityRequirements;
    stringmap metadataRequirements;
    int versionRequirement = 0;
    int minVersionRequirement = 0;
    int maxVersionRequirement = 0;

public:
    builder& setServiceDefinitionRequirement(std::string value);
    builder& setInterfaceRequirements(stringvector value);
    builder& setSecurityRequirements(std::vector<SecurityType> value);
    builder& setMetadataRequirements(stringmap value);
    builder& setVersionRequirement(int value);
    builder& setMinVersionRequirement(int value);
    builder& setMaxVersionRequirement(int value);
    ArrowheadServiceRequirement build() const;
};

}

#endif // ARROWHEADSERVICEREQUIREMENT_H
