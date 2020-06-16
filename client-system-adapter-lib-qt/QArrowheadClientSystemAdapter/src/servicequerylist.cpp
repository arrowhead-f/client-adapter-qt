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

#include "servicequerylist.h"
#include <QJsonArray>

namespace arrowhead {

ServiceQueryList ServiceQueryList::fromJsonObject(const QJsonObject &jsonObj)
{
    std::vector<ServiceRegistryEntry> srEntryVector;
    int unfilteredHits = jsonObj["unfilteredHits"].toInt();

    QJsonArray serviceQueryData = jsonObj["serviceQueryData"].toArray();
    for(auto srEntry : serviceQueryData){
        srEntryVector.push_back(ServiceRegistryEntry::fromJsonObject(
                                    srEntry.toObject()));
    }

    return ServiceQueryList(srEntryVector, unfilteredHits);
}

}
