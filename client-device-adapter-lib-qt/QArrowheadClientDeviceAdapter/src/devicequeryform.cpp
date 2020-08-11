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

#include "devicequeryform.h"


namespace arrowhead {
        
    QJsonObject DeviceQueryForm::toJsonObject() const{
        QJsonObject jsonObject;

        if(!addressRequirement.empty())
            jsonObject.insert("addressRequirement", addressRequirement.data());

        //Mandatory
        jsonObject.insert("deviceNameRequirements", deviceNameRequirements.data());

        if(!macAddressRequirement.empty())
            jsonObject.insert("macAddressRequirement", macAddressRequirement.data());

        if(maxVersionRequirement!=0)
            jsonObject.insert("maxVersionRequirement", maxVersionRequirement);

        if(!metadataRequirements.empty()){
            QJsonObject metadataJsonObject;
            for(auto kvpair : metadataRequirements){
                metadataJsonObject.insert(kvpair.first.data(), kvpair.second.data());
            }
            jsonObject.insert("metadataRequirements", metadataJsonObject);
        }

        if(minVersionRequirement!=0)
            jsonObject.insert("minVersionRequirement", minVersionRequirement);

        if(versionRequirement!=0)
            jsonObject.insert("versionRequirement", versionRequirement);


        return jsonObject;
    }

}
