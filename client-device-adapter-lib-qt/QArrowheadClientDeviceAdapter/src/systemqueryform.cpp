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

#include "systemqueryform.h"

namespace arrowhead {

QJsonObject SystemQueryForm::toJsonObject() const {
    QJsonObject srfJson;

    if(!deviceNameRequirements.empty())
        srfJson.insert("deviceNameRequirements", deviceNameRequirements.data());

    if(maxVersionRequirement!=0)
        srfJson.insert("maxVersionRequirement", maxVersionRequirement);


    if(!metadataRequirements.empty()){
        QJsonObject metadataJsonObject;
        for(auto kvpair : metadataRequirements){
            metadataJsonObject.insert(kvpair.first.data(), kvpair.second.data());
        }
        srfJson.insert("metadataRequirements", metadataJsonObject);
    }
    if(minVersionRequirement!=0)
        srfJson.insert("minVersionRequirement", minVersionRequirement);

    //Mandatory
    srfJson.insert("pingProviders", pingProviders);

    //Mandatory
    srfJson.insert("systemNameRequirements", systemNameRequirements.data());

    if(versionRequirement!=0)
        srfJson.insert("versionRequirement", versionRequirement);

    return srfJson;

}
}
