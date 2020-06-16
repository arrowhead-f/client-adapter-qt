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

#include "servicequeryform.h"
#include <QJsonArray>

namespace arrowhead {

QJsonObject ServiceQueryForm::toJsonObject() const
{
    // Mandatory fields: serviceDefinitionRequirement
    QJsonObject jsonObject = requestedService.toJsonObject();
    jsonObject.insert("pingProviders", pingProviders);

    return jsonObject;
}

ServiceQueryForm::builder::builder() :
    requestedService(ArrowheadServiceRequirement::builder().build()),
    pingProviders(false) {}

ServiceQueryForm::builder& ServiceQueryForm::builder::setRequestedService(ArrowheadServiceRequirement value){
    requestedService = value;
    return *this;
}

ServiceQueryForm::builder& ServiceQueryForm::builder::setPingProviders(bool value){
    pingProviders = value;
    return *this;
}

ServiceQueryForm ServiceQueryForm::builder::build() const{
    return ServiceQueryForm(requestedService,
                            pingProviders);
}

}


