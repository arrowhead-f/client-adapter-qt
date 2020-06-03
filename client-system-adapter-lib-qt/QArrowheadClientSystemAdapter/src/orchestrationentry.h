#ifndef ORCHESTRATIONENTRY_H
#define ORCHESTRATIONENTRY_H

#include "common.h"
#include "arrowheadbasetypes.h"
#include <vector>
#include <QJsonObject>

namespace arrowhead {

struct OrchestrationEntry
{
    enum class Warning{
        FromOtherCloud,
        TtlExpired,
        TtlExpiring,
        TtlUnknown,
        Invalid
    };

    struct ServiceEntry{
        ArrowheadService service;
        RegistryTag entryTag;

        ServiceEntry(ArrowheadService service, RegistryTag entryTag) :
            service(service), entryTag(entryTag) {}
    };

    struct ProviderEntry{
        ArrowheadSystem provider;
        RegistryTag entryTag;

        ProviderEntry(ArrowheadSystem provider, RegistryTag entryTag) :
            provider(provider), entryTag(entryTag) {}
    };

    ServiceEntry orchestratedService;
    ProviderEntry orchestratedProvider;
    stringmap authorizationTokens;
    std::vector<Warning> warnings;

    OrchestrationEntry(ServiceEntry orchestratedService,
                       ProviderEntry orchestratedProvider,
                       stringmap authorizationTokens,
                       std::vector<Warning> warnings) :
      orchestratedService(orchestratedService),
      orchestratedProvider(orchestratedProvider),
      authorizationTokens(authorizationTokens),
      warnings(warnings) {}

    static OrchestrationEntry fromJsonObject(const QJsonObject &jsonObj);
};

std::string typeToString(const OrchestrationEntry::Warning& type);
OrchestrationEntry::Warning orchestrationWarningFromString(const std::string &typeString);

}



#endif // ORCHESTRATIONENTRY_H
