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

#ifndef SYSTEMQUERYFORM_H
#define SYSTEMQUERYFORM_H

#include <string>
#include <map>
#include <QJsonObject>

#include "arrowheadbasetypes.h"

namespace arrowhead {

struct SystemQueryForm
{
    std::string systemNameRequirements;
    std::string deviceNameRequirements;
    stringmap metadataRequirements;
    bool pingProviders;
    int versionRequirement;
    int minVersionRequirement;
    int maxVersionRequirement;

    SystemQueryForm() = default;
    SystemQueryForm(std::string systemNameRequirements, std::string deviceNameRequirements, stringmap metadataRequirements, bool pingProviders, int versionRequirement, int minVersionRequirement, int maxVersionRequirement) : systemNameRequirements(systemNameRequirements), deviceNameRequirements(deviceNameRequirements), metadataRequirements(metadataRequirements), pingProviders(pingProviders), versionRequirement(versionRequirement), minVersionRequirement(minVersionRequirement), maxVersionRequirement(maxVersionRequirement) {}

    QJsonObject toJsonObject() const;
};

}

#endif // SYSTEMQUERYFORM_H
