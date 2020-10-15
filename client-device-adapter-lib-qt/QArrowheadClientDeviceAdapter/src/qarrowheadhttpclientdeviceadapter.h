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

#ifndef QARROWHEADHTTPCLIENTDEVICEADAPTER_H
#define QARROWHEADHTTPCLIENTDEVICEADAPTER_H

#include "qarrowheadclientdeviceadapter_global.h"
#include "arrowheadbasetypes.h"
#include "arrowheaddevice.h"
#include "onboardingwithnamerequest.h"
#include "onboardingresponse.h"
#include "onboardingwithcsrrequest.h"
#include "devicequeryform.h"
#include "devicequerylist.h"
#include "deviceregistrationform.h"
#include "deviceonboardingwithnamerequest.h"
#include "deviceonboardingwithcsrrequest.h"
#include "systemqueryform.h"
#include "systemquerylist.h"
#include "systemregistrationform.h"
#include "systemonboardingwithnamerequest.h"
#include "deviceonboardingresponse.h"
#include "systemonboardingresponse.h"
#include "systemonboardingwithcsrrequest.h"
#include "qarrowheadclientdeviceadapter.h"
#include "httpclient.h"
#include "onboardingwithnamerequest.h"
#include "onboardingresponse.h"
#include <QNetworkAccessManager>

namespace arrowhead {

class QAHDEVADAPTER_EXPORT QArrowheadHttpClientDeviceAdapter : public QArrowheadClientDeviceAdapter
{
public:

    using QArrowheadClientDeviceAdapter::QArrowheadClientDeviceAdapter;

    // Onboarding Controller Interface
    ReturnValue getOnboardingControllerEcho() override;
    ReturnValue requestOnboardingWithName(
            const OnboardingWithNameRequest& onboardingRequest,
            std::unique_ptr<OnboardingResponse>& uptr_onboardingResponse) override;
    ReturnValue requestOnboardingWithCsr(
            const OnboardingWithCsrRequest& onboardingRequest,
            std::unique_ptr<OnboardingResponse>& uptr_onboardingResponse) override;

    //Device Registry Interface
    ReturnValue getDeviceRegistryEcho() override;
    ReturnValue queryDevice(
            const DeviceQueryForm& deviceQueryForm,
            std::unique_ptr<DeviceQueryList>& uptr_deviceQueryList) override;
    ReturnValue registerDevice(
            const DeviceRegistrationForm& deviceRegistrationForm,
            std::unique_ptr<DeviceRegistryEntry>& uptr_deviceRegistryEntry) override;
    ReturnValue registerOnboardingDeviceWithName(
            const DeviceOnboardingWithNameRequest& deviceOnboardingRequest,
            std::unique_ptr<DeviceOnboardingResponse>& uptr_deviceOnboardingResponse) override;
    ReturnValue registerOnboardingDeviceWithCsr(
            const DeviceOnboardingWithCsrRequest& deviceOnboardingRequest,
            std::unique_ptr<DeviceOnboardingResponse>& uptr_deviceOnboardingResponse) override;
    ReturnValue unregisterDevice(
            const DeviceRegistryEntry& deviceRegistryEntry) override;

    //System Registry Interface
    ReturnValue getSystemRegistryEcho() override;
    ReturnValue querySystem(
            const SystemQueryForm& systemQueryForm,
            std::unique_ptr<SystemQueryList>& uptr_systemQueryList) override;
    ReturnValue registerSystem(
            const SystemRegistrationForm& systemRegistrationForm,
            std::unique_ptr<SystemRegistryEntry>& uptr_systemRegistryEntry) override;
    ReturnValue registerOnboardingSystemWithName(
            const SystemOnboardingWithNameRequest& systemOnboardingRequest,
            std::unique_ptr<SystemOnboardingResponse>& uptr_systemOnboardingResponse) override;
    ReturnValue registerOnboardingSystemWithCsr(
            const SystemOnboardingWithCsrRequest& systemOnboardingRequest,
            std::unique_ptr<SystemOnboardingResponse>& uptr_systemOnboardingResponse) override;
    ReturnValue unregisterSystem(
            const SystemRegistryEntry& systemRegistryEntry) override;

    HttpClient& getHttpClient(){
        return httpClient;
    }

private:
    HttpClient httpClient;
};

}


#endif // QARROWHEADHTTPCLIENTDEVICEADAPTER_H
