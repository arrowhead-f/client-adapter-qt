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

#ifndef ONBOARDINGWITHNAMEREQUEST_H
#define ONBOARDINGWITHNAMEREQUEST_H

#include "qarrowheadclientdeviceadapter_global.h"
#include "certificatecreationrequest.h"

namespace arrowhead {

struct QAHDEVADAPTER_EXPORT OnboardingWithNameRequest
{
    CertificateCreationRequest ccr;

    OnboardingWithNameRequest(CertificateCreationRequest ccr) : ccr(ccr) {}
    QJsonObject toJsonObject() const;
};

}

#endif // ONBOARDINGWITHNAMEREQUEST_H
