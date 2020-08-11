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

#ifndef DEVICEONBOARDINGRESPONSE_H
#define DEVICEONBOARDINGRESPONSE_H

#include "certificate.h"
#include "deviceregistryentry.h"

namespace arrowhead {
    struct DeviceOnboardingResponse{
        Certificate certificateResponse;
        DeviceRegistryEntry deviceRegistryEntry;

        DeviceOnboardingResponse() = default;
        DeviceOnboardingResponse(Certificate certificateResponse, DeviceRegistryEntry deviceRegistryEntry):
                certificateResponse(certificateResponse), deviceRegistryEntry(deviceRegistryEntry){}

        static DeviceOnboardingResponse fromJsonObject(const QJsonObject &deviceOnboardingResponseJson);
    };

}
#endif // DEVICEONBOARDINGRESPONSE_H
