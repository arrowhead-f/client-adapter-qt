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

#ifndef QARROWHEADCLIENTDEVICEADAPTER_H
#define QARROWHEADCLIENTDEVICEADAPTER_H

#include "common.h"
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
#include <memory>

namespace arrowhead {

class QArrowheadClientDeviceAdapter{
public:
    QArrowheadClientDeviceAdapter(
            std::string onboardingControllerBaseUrl) :
        onboardingControllerBaseUrl(onboardingControllerBaseUrl) {}

    QArrowheadClientDeviceAdapter(
            std::string onboardingControllerBaseUrl,
            std::string deviceRegistryBaseUrl,
            std::string systemRegistryBaseUrl) :
        onboardingControllerBaseUrl(onboardingControllerBaseUrl),
        deviceRegistryBaseUrl(deviceRegistryBaseUrl),
        systemRegistryBaseUrl(systemRegistryBaseUrl) {}

    virtual ~QArrowheadClientDeviceAdapter() = default;

    void setDeviceRegistryBaseUrl(std::string url) { deviceRegistryBaseUrl = url; }

    void setSystemRegistryBaseUrl(std::string url) { systemRegistryBaseUrl = url; }


    // Onboarding Controller Interface
    virtual ReturnValue getOnboardingControllerEcho() = 0;
    virtual ReturnValue requestOnboardingWithName(
            const OnboardingWithNameRequest& onboardingRequest,
            std::unique_ptr<OnboardingResponse>& uptr_onboardingResponse) = 0;
    virtual ReturnValue requestOnboardingWithCsr(
            const OnboardingWithCsrRequest& onboardingRequest,
            std::unique_ptr<OnboardingResponse>& uptr_onboardingResponse) = 0;

    //Device Registry Interface
    virtual ReturnValue getDeviceRegistryEcho() = 0;
    virtual ReturnValue queryDevice(
            const DeviceQueryForm& deviceQueryForm,
            std::unique_ptr<DeviceQueryList>& uptr_deviceQueryList) = 0;
    virtual ReturnValue registerDevice(
            const DeviceRegistrationForm& deviceRegistrationForm,
            std::unique_ptr<DeviceRegistryEntry>& uptr_deviceRegistryEntry) = 0;
    virtual ReturnValue registerOnboardingDeviceWithName(
            const DeviceOnboardingWithNameRequest& deviceOnboardingRequest,
            std::unique_ptr<DeviceOnboardingResponse>& uptr_deviceOnboardingResponse) = 0;
    virtual ReturnValue registerOnboardingDeviceWithCsr(
            const DeviceOnboardingWithCsrRequest& deviceOnboardingRequest,
            std::unique_ptr<DeviceOnboardingResponse>& uptr_deviceOnboardingResponse) = 0;
    virtual ReturnValue unregisterDevice(
            const DeviceRegistryEntry& deviceRegistryEntry) = 0;

    //System Registry Interface
    virtual ReturnValue getSystemRegistryEcho() = 0;
    virtual ReturnValue querySystem(
            const SystemQueryForm& systemQueryForm,
            std::unique_ptr<SystemQueryList>& uptr_systemQueryList) = 0;
    virtual ReturnValue registerSystem(
            const SystemRegistrationForm& systemRegistrationForm,
            std::unique_ptr<SystemRegistryEntry>& uptr_systemRegistryEntry) = 0;
    virtual ReturnValue registerOnboardingSystemWithName(
            const SystemOnboardingWithNameRequest& systemOnboardingRequest,
            std::unique_ptr<SystemOnboardingResponse>& uptr_systemOnboardingResponse) = 0;
    virtual ReturnValue registerOnboardingSystemWithCsr(
            const SystemOnboardingWithCsrRequest& systemOnboardingRequest,
            std::unique_ptr<SystemOnboardingResponse>& uptr_systemOnboardingResponse) = 0;
    virtual ReturnValue unregisterSystem(
            const SystemRegistryEntry& systemRegistryEntry) = 0;
protected:
    std::string onboardingControllerBaseUrl;
    std::string deviceRegistryBaseUrl;
    std::string systemRegistryBaseUrl;

};

}

#endif // QARROWHEADCLIENTDEVICEADAPTER_H
