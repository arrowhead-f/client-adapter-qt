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

#ifndef SYSTEMREGISTRYENTRY_H
#define SYSTEMREGISTRYENTRY_H

#include "arrowheadbasetypes.h"
#include "arrowheaddevice.h"

namespace arrowhead {

struct SystemRegistryEntry
{
    RegistryTag entryTag;
    ArrowheadDevice provider;
    RegistryTag deviceTag;
    ArrowheadSystem system;
    RegistryTag systemTag;
    std::string endOfValidity;
    stringmap metadata;
    int version;

    SystemRegistryEntry() = default;
    SystemRegistryEntry(RegistryTag entryTag, ArrowheadDevice provider, RegistryTag deviceTag, ArrowheadSystem system, RegistryTag systemTag, std::string endOfValidity, stringmap metadata, int version) : entryTag(entryTag), provider(provider), deviceTag(deviceTag), system(system), systemTag(systemTag), endOfValidity(endOfValidity), metadata(metadata), version(version) {}

    static SystemRegistryEntry fromJsonObject(const QJsonObject &srfJson);
};

}

#endif // SYSTEMREGISTRYENTRY_H
