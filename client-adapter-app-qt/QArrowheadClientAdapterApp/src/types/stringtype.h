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
#ifndef STRINGTYPE_H
#define STRINGTYPE_H

#include "valuetype.h"

class StringType : public ValueType {
    std::string value;
public:
    StringType(const std::string& v) { value = v; }

    StringType(QJsonValue jsonValue) {
        value = jsonValue.toString().toStdString();
    }

    operator std::string() const { return value; }

    virtual std::string toString() const override {
        if(value.empty())
            return "-";

        return value;
    }

    virtual QJsonValue toJsonValue() const override {
        return value.c_str();
    }
};

#endif // STRINGTYPE_H
