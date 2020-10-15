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
#ifndef LISTTYPE_H
#define LISTTYPE_H

#include <string>
#include <sstream>
#include <vector>

#include <QJsonArray>

#include "valuetype.h"

class ListType : public ValueType {
    std::vector<std::string> value;
public:
    ListType(const std::vector<std::string>& v) { value = v; }

    ListType(QJsonValue jsonValue) {
        QJsonArray jsonArray = jsonValue.toArray();
        for (auto v : jsonArray)
            value.push_back(v.toString().toStdString());
    }

    operator std::vector<std::string>() const { return value; }

    virtual std::string toString() const override {
        if(value.empty())
            return "-";

        std::stringstream ss;
        for (auto v : value)
            ss << v << " ";
        return ss.str();
    }

    virtual QJsonValue toJsonValue() const override {
        QJsonArray list;
        for (std::string v : value)
            list.push_back(v.c_str());
        return list;
    }
};

#endif // LISTTYPE_H
