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
#ifndef MAPTYPE_H
#define MAPTYPE_H

#include <string>
#include <sstream>
#include <map>

#include <QJsonObject>

#include "valuetype.h"

class MapType : public ValueType {
    std::map<std::string, std::string> value;
public:
    MapType(const std::map<std::string, std::string>& v) { value = v; }

    operator std::map<std::string, std::string>() const { return value; }

    virtual std::string toString() const override {
        if(value.empty())
            return "-";

        std::stringstream ss;
        for (auto v : value)
            ss << v.first << ':' << v.second << " ";
        return ss.str();
    }

    virtual QJsonValue toJsonValue() const override {
        QJsonObject map;
        for(auto v : value){
            map.insert(v.first.c_str(), v.second.c_str());
        }
        return map;
    }

    MapType(QJsonValue jsonValue) {
        QJsonObject jsonObject = jsonValue.toObject();
        for (auto i = jsonObject.begin(); i != jsonObject.end(); i++)
            value.emplace(i.key().toStdString(), i.value().toString().toStdString());
    }
};

#endif // MAPTYPE_H
