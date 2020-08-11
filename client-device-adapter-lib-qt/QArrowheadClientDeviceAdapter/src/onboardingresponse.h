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

#ifndef ONBOARDINGRESPONSE_H
#define ONBOARDINGRESPONSE_H

#include "arrowheadbasetypes.h"
#include "certificate.h"
#include <string>
#include <QJsonObject>

namespace arrowhead {

struct OnboardingResponse
{
    std::string rootCertificate;
    std::string intermediateCertificate;
    Certificate onboardingCertificate;
    ArrowheadService deviceRegistry;
    ArrowheadService systemRegistry;
    ArrowheadService serviceRegistry;
    ArrowheadService orchestrationService;

    OnboardingResponse(std::string rootCertificate,
                       std::string intermediateCertificate,
                       Certificate onboardingCertificate,
                       ArrowheadService deviceRegistry,
                       ArrowheadService systemRegistry,
                       ArrowheadService serviceRegistry,
                       ArrowheadService orchestrationService) :
                rootCertificate(rootCertificate),
                intermediateCertificate(intermediateCertificate),
                onboardingCertificate(onboardingCertificate),
                deviceRegistry(deviceRegistry),
                systemRegistry(systemRegistry),
                serviceRegistry(serviceRegistry),
                orchestrationService(orchestrationService) {}
    static OnboardingResponse fromJsonObject(const QJsonObject &orJson);
};

}



#endif // ONBOARDINGRESPONSE_H
