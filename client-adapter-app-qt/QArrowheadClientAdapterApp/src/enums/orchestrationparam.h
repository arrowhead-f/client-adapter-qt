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
#ifndef ORCHESTRATIONPARAM_H
#define ORCHESTRATIONPARAM_H

#include <algorithm>

#include <QObject>
#include <QMetaEnum>

#include "paraminfo.h"

class OrchestrationParam
{
    Q_GADGET
public:
    enum type {
        name,                   //string

        provider_id,           //int
        provider_system_name,
        provider_address,
        provider_port,
        provider_authentication_info,
        provider_created_at,
        provider_updated_at,

        service_id,           //int
        service_name,           //string
        service_created_at,           //string
        service_updated_at,           //string
        service_uri,            //string

        secure,                 //string
        metadata,               //map
        interfaces,             //list
        version,                //integer
        authorization_tokens,   //map
        warnings,               //list
    };
    Q_ENUM(type)

    OrchestrationParam(type other) {
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

    static OrchestrationParam fromString(std::string value) {
        std::replace(value.begin(), value.end(), '-', '_');
        bool success;
        type param = type(QMetaEnum::fromType<type>().keyToValue(value.c_str(), &success));
        if (!success)
            throw std::runtime_error("Orchestrations do not have a parameter named \'" + value + '\'');
        return param;
    }

    static std::vector<OrchestrationParam> getParameters(){
        auto en = QMetaEnum::fromType<type>();
        std::vector<OrchestrationParam> params;
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
    {
        {name,                   {ParamInfo::string,  ParamInfo::mandatory}},
        {provider_id,                  {ParamInfo::integer, ParamInfo::not_mandatory}},     //int
        {provider_system_name,         {ParamInfo::string, ParamInfo::not_mandatory}},
        {provider_address,             {ParamInfo::string, ParamInfo::not_mandatory}},
        {provider_port,                {ParamInfo::integer, ParamInfo::not_mandatory}},
        {provider_authentication_info, {ParamInfo::string, ParamInfo::not_mandatory}},
        {provider_created_at,          {ParamInfo::string, ParamInfo::not_mandatory}},
        {provider_updated_at,          {ParamInfo::string, ParamInfo::not_mandatory}},
        {service_id,             {ParamInfo::integer,  ParamInfo::not_mandatory}},
        {service_name,           {ParamInfo::string,  ParamInfo::not_mandatory}},
        {service_created_at,     {ParamInfo::string,  ParamInfo::not_mandatory}},
        {service_updated_at,     {ParamInfo::string,  ParamInfo::not_mandatory}},
        {service_uri,            {ParamInfo::string,  ParamInfo::not_mandatory}},
        {secure,                 {ParamInfo::string,  ParamInfo::not_mandatory}},
        {metadata,               {ParamInfo::map,     ParamInfo::not_mandatory}},
        {interfaces,             {ParamInfo::list,    ParamInfo::not_mandatory}},
        {version,                {ParamInfo::integer, ParamInfo::not_mandatory}},
        {authorization_tokens,   {ParamInfo::map,     ParamInfo::not_mandatory}},
        {warnings,               {ParamInfo::list,    ParamInfo::not_mandatory}}
    };
};

#endif // ORCHESTRATIONPARAM_H
