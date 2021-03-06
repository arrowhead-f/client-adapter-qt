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

#ifndef ORCHESTRATIONRESPONSE_H
#define ORCHESTRATIONRESPONSE_H

#include "qarrowheadclientsystemadapter_global.h"
#include "orchestrationentry.h"
#include <vector>
#include <QJsonObject>

namespace arrowhead {

struct QAHSYSADAPTER_EXPORT OrchestrationResponse
{
    std::vector<OrchestrationEntry> orchEntryVector;

    OrchestrationResponse(std::vector<OrchestrationEntry> orchEntryVector) :
        orchEntryVector(orchEntryVector) {}

    static OrchestrationResponse fromJsonObject(const QJsonObject &jsonObj);
};

}


#endif // ORCHESTRATIONRESPONSE_H
