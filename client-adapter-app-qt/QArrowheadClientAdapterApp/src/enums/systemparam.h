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
#ifndef SYSTEMPARAM_H
#define SYSTEMPARAM_H

#include <algorithm>

#include <QObject>
#include <QMetaEnum>

#include "paraminfo.h"

class SystemParam
{
    Q_GADGET
public:
    enum type {
        id,
        name,
        address,
        port,
        created_at,
        updated_at,
        version,
        end_of_validity,
        authentication_info,
        metadata
    };
    Q_ENUM(type)

    SystemParam(type other) {
        p = other;
    }

    operator type() const {
        return p;
    }

    std::string toString() const {
        std::string str = QMetaEnum::fromType<type>().valueToKey(p);
        std::replace(str.begin(), str.end(), '_', '-');
        return str;
    }

    static SystemParam fromString(std::string value) {
        std::replace(value.begin(), value.end(), '-', '_');
        bool success;
        type param = type(QMetaEnum::fromType<type>().keyToValue(value.c_str(), &success));
        if (!success)
            throw std::runtime_error("Systems do not have a parameter named \'" + value + '\'');
        return param;
    }

    static std::vector<SystemParam> getParameters(){
        auto en = QMetaEnum::fromType<type>();
        std::vector<SystemParam> params;
        for(int index = 0; index < en.keyCount(); index++)
            params.push_back((type)en.value(index));

        return params;
    }

    ParamInfo getParamInfo() {
        return paraminfo.at(p);
    }

private:
    type p;

    static const inline std::map<type, ParamInfo> paraminfo =
    {{id,                  {ParamInfo::integer,   ParamInfo::not_mandatory}},
     {name,                {ParamInfo::string,  ParamInfo::mandatory}},
     {address,             {ParamInfo::string,  ParamInfo::not_mandatory}},
     {port,                {ParamInfo::integer, ParamInfo::mandatory}},
     {metadata,            {ParamInfo::map,     ParamInfo::not_mandatory}},
     {created_at,          {ParamInfo::string,  ParamInfo::not_mandatory}},
     {updated_at,          {ParamInfo::string,  ParamInfo::not_mandatory}},
     {authentication_info, {ParamInfo::string,  ParamInfo::not_mandatory}},
     {end_of_validity,     {ParamInfo::string,  ParamInfo::not_mandatory}},
     {version,             {ParamInfo::integer, ParamInfo::not_mandatory}}};
};

#endif // SYSTEMPARAM_H
