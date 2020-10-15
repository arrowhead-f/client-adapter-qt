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

#ifndef COMMON_H
#define COMMON_H

#include "qarrowheadclientcommon_global.h"
#include <string>
#include <QObject>
#include <QJsonObject>

namespace arrowhead {

enum class QARROWHEADCLIENTCOMMON_EXPORT ReturnValue{
    Ok = 0,
    InvalidValue = 1,
    InvalidArgNum = 2,
    NetworkError = 3,
    PeerError = 4,
    UnknownError = 5,
    Unimplemented = 6
};

enum class QARROWHEADCLIENTCOMMON_EXPORT SecurityType{
    Invalid,
    NotSecure,
    Certificate,
    Token
};

QARROWHEADCLIENTCOMMON_EXPORT std::string typeToString(const SecurityType& type);
QARROWHEADCLIENTCOMMON_EXPORT SecurityType securityTypeFromString(const std::string& typeString);

struct QARROWHEADCLIENTCOMMON_EXPORT RegistryTag{
    int id {-1};
    std::string createdAt;
    std::string updatedAt;

    RegistryTag() = default;
    RegistryTag(int id, std::string createdAt, std::string updatedAt) :
        id(id), createdAt(createdAt), updatedAt(updatedAt) {}

    QJsonObject toJsonObject() const;
    static RegistryTag fromJsonObject(const QJsonObject& jsonObject);
};

}

Q_DECLARE_METATYPE(arrowhead::SecurityType)

#endif // COMMON_H
