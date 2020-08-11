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

#include "deviceregistryentry.h"

namespace arrowhead {

    DeviceRegistryEntry DeviceRegistryEntry::fromJsonObject(const QJsonObject &jsonObject){

        RegistryTag entryTag = RegistryTag::fromJsonObject(jsonObject);
        ArrowheadDevice device = ArrowheadDevice::fromJsonObject(jsonObject["device"].toObject());
        RegistryTag deviceTag = RegistryTag::fromJsonObject(jsonObject["device"].toObject());
        std::string endOfValidity = jsonObject["endOfValidity"].toString().toStdString();

        QJsonObject metadataJsonObject = jsonObject["metadata"].toObject();
        std::map<std::string, std::string> metadata;
        for(auto key : metadataJsonObject.keys()){
            metadata.emplace(std::make_pair(
                                 key.toStdString(),
                                 metadataJsonObject[key].toString().toStdString()));
        }

        int version = jsonObject["version"].toInt();



        return DeviceRegistryEntry(entryTag, device, deviceTag, endOfValidity, metadata, version);
    }

}
