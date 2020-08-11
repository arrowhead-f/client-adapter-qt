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

#include "arrowheaddevice.h"

namespace arrowhead {
QJsonObject ArrowheadDevice::toJsonObject() const {
    QJsonObject jsonObject;
    jsonObject.insert("deviceName", deviceName.data());
    jsonObject.insert("macAddress", macAddress.data());
    jsonObject.insert("address", address.data());
    jsonObject.insert("authenticationInfo", authenticationInfo.data());
    return jsonObject;
}

ArrowheadDevice ArrowheadDevice::fromJsonObject(const QJsonObject &objJson){
    std::string deviceName = objJson["deviceName"].toString().toStdString();
    std::string macAddress = objJson["macAddress"].toString().toStdString();
    std::string address = objJson["address"].toString().toStdString();
    std::string authenticationInfo = objJson["authenticationInfo"].toString().toStdString();
    return ArrowheadDevice(deviceName, macAddress, address, authenticationInfo);
}
}


