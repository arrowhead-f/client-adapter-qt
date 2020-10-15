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

#ifndef SERVICEREQUESTFORM_H
#define SERVICEREQUESTFORM_H

#include "qarrowheadclientsystemadapter_global.h"
#include "common.h"
#include "arrowheadbasetypes.h"
#include "arrowheadservicerequirement.h"
#include <vector>
#include <map>
#include <QJsonObject>

namespace arrowhead {

struct QAHSYSADAPTER_EXPORT ServiceRequestForm
{
    struct PreferredProvider{
        ArrowheadCloud providerCloud;
        ArrowheadSystem providerSystem;

        PreferredProvider(ArrowheadCloud providerCloud, ArrowheadSystem providerSystem) :
            providerCloud(providerCloud), providerSystem(providerSystem) {}

        QJsonObject toJsonObject() const;
    };

    enum class OrchestrationFlagKey{
        Invalid,
        MatchMaking,
        MetadataSearch,
        OnlyPreferred,
        PingProviders,
        OverrideStore,
        EnableInterCloud,
        TriggerInterCloud
    };

    ArrowheadServiceRequirement requestedService;
    ArrowheadSystem requesterSystem;
    std::vector<PreferredProvider> preferredProviders;
    std::map<OrchestrationFlagKey,bool> orchestrationFlags;

    struct builder;

    QJsonObject toJsonObject() const;

private:
    ServiceRequestForm(ArrowheadServiceRequirement requestedService,
                       ArrowheadSystem requesterSystem,
                       std::vector<PreferredProvider> preferredProviders,
                       std::map<OrchestrationFlagKey,bool> orchestrationFlags) :
        requestedService(requestedService),
        requesterSystem(requesterSystem),
        preferredProviders(preferredProviders),
        orchestrationFlags(orchestrationFlags) {}
};

struct QAHSYSADAPTER_EXPORT ServiceRequestForm::builder{
private:
    ArrowheadServiceRequirement requestedService;
    ArrowheadSystem requesterSystem;
    std::vector<PreferredProvider> preferredProviders;
    std::map<OrchestrationFlagKey,bool> orchestrationFlags;

public:
    builder();
    builder& setRequestedService(ArrowheadServiceRequirement value);
    builder& setRequesterSystem(ArrowheadSystem value);
    builder& setPreferredProviders(std::vector<PreferredProvider> value);
    builder& setOrchestrationFlags(std::map<OrchestrationFlagKey,bool> value);
    ServiceRequestForm build() const;
};

std::string typeToString(const ServiceRequestForm::OrchestrationFlagKey& type);
ServiceRequestForm::OrchestrationFlagKey orchestrationFlagKeyFromString(const std::string& typeString);

}

#endif // SERVICEREQUESTFORM_H
