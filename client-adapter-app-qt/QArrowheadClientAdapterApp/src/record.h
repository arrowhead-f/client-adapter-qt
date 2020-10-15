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
#ifndef RECORD_H
#define RECORD_H

#include <map>
#include <vector>
#include <memory>
#include <algorithm>

#include <QJsonObject>
#include <QJsonDocument>

#include "types/valuetype.h"
#include "types/stringtype.h"
#include "types/integertype.h"
#include "types/maptype.h"
#include "types/listtype.h"

#include "enums/deviceparam.h"
#include "enums/systemparam.h"
#include "enums/serviceparam.h"
#include "enums/orchestrationparam.h"
#include "enums/orchestrationrequestparam.h"
#include "enums/settingsparam.h"

template<typename T>
class Record
{
    std::map<T, std::shared_ptr<ValueType>> values;
    std::vector<T> parameters;
public:
    void addField(T parameter, const std::string& value){
        if (values.count(parameter) == 0) {
            ParamInfo paraminfo = parameter.getParamInfo();
            if (paraminfo.parametertype != ParamInfo::string)
                throw  std::runtime_error("Bad data type for " + parameter.toString());

            values[parameter] = std::make_shared<StringType>(value);
            parameters.push_back(parameter);
        }
    }

    void addField(T parameter, int value){
        if (values.count(parameter) == 0) {
            ParamInfo paraminfo = parameter.getParamInfo();
            if (paraminfo.parametertype != ParamInfo::integer)
                throw  std::runtime_error("Bad data type for " + parameter.toString());

            values[parameter] = std::make_shared<IntegerType>(value);
            parameters.push_back(parameter);
        }
    }

    void addField(T parameter, const std::map<std::string, std::string>& value){
        if (values.count(parameter) == 0) {
            ParamInfo paraminfo = parameter.getParamInfo();
            if (paraminfo.parametertype != ParamInfo::map)
                throw  std::runtime_error("Bad data type for " + parameter.toString());

            values[parameter] = std::make_shared<MapType>(value);
            parameters.push_back(parameter);
        }
    }

    void addField(T parameter, const std::vector<std::string>& value){
        if (values.count(parameter) == 0) {
            ParamInfo paraminfo = parameter.getParamInfo();
            if (paraminfo.parametertype != ParamInfo::list)
                throw  std::runtime_error("Bad data type for " + parameter.toString());

            values[parameter] = std::make_shared<ListType>(value);
            parameters.push_back(parameter);
        }
    }

    void removeField(T parameter) {
        values.erase(parameter);
        parameters.erase(std::remove(parameters.begin(), parameters.end(), parameter), parameters.end());
    }

    void projectFields(std::vector<T> projectParameters) {
        std::map<T, std::shared_ptr<ValueType>> projectedValues;
        for (auto param : projectParameters) {
            if (values.count(param) != 0)
              //  throw std::runtime_error("Parameter \'" + param.toString() + "\' does not have a value.");
                projectedValues[param] = values[param];
        }
        values.swap(projectedValues);
        parameters.clear();
        for (auto i : values)
            parameters.push_back(i.first);
    }


    std::string getString(T parameter) const {
        if(!values.count(parameter))
            return "";
        return dynamic_cast<const StringType&>(*values.at(parameter));
    }

    int getInteger(T parameter) const {
        if(!values.count(parameter))
            return 0;
        return dynamic_cast<IntegerType&>(*values.at(parameter));
    }

    std::map<std::string, std::string> getMap(T parameter) const {
        if(!values.count(parameter))
            return {};
        return dynamic_cast<MapType&>(*values.at(parameter));
    }

    std::vector<std::string> getList(T parameter) const {
        if(!values.count(parameter))
            return {};
        return dynamic_cast<ListType&>(*values.at(parameter));
    }

    std::string toString(T parameter) const {
        return values.at(parameter)->toString();
    }

    std::vector<T> getParameters() {
        return parameters;
    }

    bool containsAllMandatoryFields() {
        std::vector<T> allParameters = T::getParameters();
        for (T p : allParameters) {
            if (p.getParamInfo().ismandatory == ParamInfo::mandatory) {
                auto foundi = std::find(parameters.begin(), parameters.end(), p);
                if (foundi == parameters.end())
                    return false;
            }
        }
        return true;
    }

    QByteArray toByteArray() const {
        QJsonObject obj;

        for (T param : parameters)
            obj.insert(param.toString().c_str(), values.at(param)->toJsonValue());

        return QJsonDocument(obj).toJson();
    }

    void updateFromByteArray(const QByteArray& byteArray) {
        QJsonObject obj = QJsonDocument::fromJson(byteArray).object();
        for (auto i = obj.begin(); i != obj.end(); i++) {
            T param = T::fromString(i.key().toStdString());
            switch (i.value().type()) {
                case QJsonValue::Double:
                    addField(param, *std::make_shared<IntegerType>(i.value()));
                    break;
                case QJsonValue::Object:
                    addField(param, *std::make_shared<MapType>(i.value()));
                    break;
                case QJsonValue::Array:
                    addField(param, *std::make_shared<ListType>(i.value()));
                    break;
                default:
                    addField(param, *std::make_shared<StringType>(i.value()));
            }
        }

        parameters.clear();

        for (auto i : values)
            parameters.push_back(i.first);
    }
};

#endif // RECORD_H
