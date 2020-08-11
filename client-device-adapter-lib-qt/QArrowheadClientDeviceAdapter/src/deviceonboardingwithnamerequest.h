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

#ifndef DEVICEONBOARDINGWITHNAMEREQUEST_H
#define DEVICEONBOARDINGWITHNAMEREQUEST_H

#include "arrowheadbasetypes.h"
#include "arrowheaddevice.h"
#include "certificatecreationrequest.h"


namespace arrowhead {
    struct DeviceOnboardingWithNameRequest{
        CertificateCreationRequest ccr;
        ArrowheadDevice device;
        std::string endOfValidity;
        stringmap metadata;
        int version;
        
        DeviceOnboardingWithNameRequest() = default;
        DeviceOnboardingWithNameRequest(CertificateCreationRequest ccr, ArrowheadDevice device, std::string endOfValidity,
                                        std::map<std::string,std::string> metadata, int version):
                ccr(ccr), 
                device(device),
                endOfValidity(endOfValidity),
                metadata(metadata),
                version(version){}
        
        QJsonObject toJsonObject() const;
    };
}

#endif // DEVICEONBOARDINGWITHNAMEREQUEST_H
