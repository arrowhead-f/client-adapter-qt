#ifndef SERVICEREQUESTFORM_H
#define SERVICEREQUESTFORM_H

#include "common.h"
#include "arrowheadbasetypes.h"
#include "arrowheadservicerequirement.h"
#include <vector>
#include <map>
#include <QJsonObject>

namespace arrowhead {

struct ServiceRequestForm
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

struct ServiceRequestForm::builder{
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
