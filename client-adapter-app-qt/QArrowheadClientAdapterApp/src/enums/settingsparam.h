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
#ifndef SETTINGSPARAM_H
#define SETTINGSPARAM_H

#include <algorithm>

#include <QObject>
#include <QMetaEnum>

#include "paraminfo.h"

class SettingsParam
{
    Q_GADGET
public:
    enum type {
        onboarding_system_address,
        onboarding_system_port,

        authentication_method,
        shared_secret,

        private_key,
        //private_key_password,
        public_key,

        key_store,
        key_store_password,

        log_file,
        log_level
    };
    Q_ENUM(type)

    SettingsParam(type other) {
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

    static SettingsParam fromString(std::string value) {
        std::replace(value.begin(), value.end(), '-', '_');
        bool success;
        type param = type(QMetaEnum::fromType<type>().keyToValue(value.c_str(), &success));
        if (!success)
            throw std::runtime_error("Settings does not have a parameter named \'" + value + '\'');
        return param;
    }

    static std::vector<SettingsParam> getParameters(){
        auto en = QMetaEnum::fromType<type>();
        std::vector<SettingsParam> params;
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
    {{onboarding_system_address, {ParamInfo::string,  ParamInfo::not_mandatory}},
     {onboarding_system_port,    {ParamInfo::integer, ParamInfo::not_mandatory}},
     {authentication_method,     {ParamInfo::string,  ParamInfo::not_mandatory}},
     {shared_secret,             {ParamInfo::string,  ParamInfo::not_mandatory}},
     {private_key,               {ParamInfo::string,  ParamInfo::not_mandatory}},
     {public_key,                {ParamInfo::string,  ParamInfo::not_mandatory}},
     {key_store,                 {ParamInfo::string,  ParamInfo::not_mandatory}},
     {key_store_password,        {ParamInfo::string,  ParamInfo::not_mandatory}},
     {log_file,                  {ParamInfo::string,  ParamInfo::not_mandatory}},
     {log_level,                 {ParamInfo::string,  ParamInfo::not_mandatory}}};
};

#endif // SETTINGSPARAM_H
