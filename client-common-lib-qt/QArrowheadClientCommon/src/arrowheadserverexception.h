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
#ifndef ARROWHEADSERVEREXCEPTION_H
#define ARROWHEADSERVEREXCEPTION_H

#include "qarrowheadclientcommon_global.h"
#include <exception>
#include <qjsonobject.h>
#include <string>

class QARROWHEADCLIENTCOMMON_EXPORT ArrowheadServerException : public std::exception
{
    std::string msg;
    int code;
    std::string type;
    std::string origin;

public:
    ArrowheadServerException(std::string msg, int code, std::string type, std::string origin)
        :msg{msg}, code{code}, type{type}, origin{origin}
    {}

    const char * what() const noexcept override {
        return msg.c_str();
    }


    std::string getErrorMessage(){
        return msg;
    }

    int getErrorCode(){
        return code;
    }

    std::string getExceptionType(){
        return type;
    }

    std::string getOrigin(){
        return origin;
    }

    static ArrowheadServerException fromJsonObject(const QJsonObject &jsonObject){
        std::string msg = jsonObject["errorMessage"].toString().toStdString();
        int code = jsonObject["errorCode"].toInt();
        std::string type = jsonObject["exceptionType"].toString().toStdString();
        std::string origin = jsonObject["origin"].toString().toStdString();

        return ArrowheadServerException(msg, code, type, origin);
    }
};

#endif // ARROWHEADSERVEREXCEPTION_H
