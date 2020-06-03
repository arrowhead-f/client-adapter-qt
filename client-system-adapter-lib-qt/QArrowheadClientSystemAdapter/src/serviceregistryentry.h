#ifndef SERVICEREGISTRYENTRY_H
#define SERVICEREGISTRYENTRY_H

#include "common.h"
#include "arrowheadbasetypes.h"
#include <QJsonObject>

namespace arrowhead {

struct ServiceRegistryEntry
{
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

    ServiceEntry registeredService;
    ProviderEntry registeredProvider;
    RegistryTag entryTag;

    ServiceRegistryEntry(
            ServiceEntry registeredService, ProviderEntry registeredProvider, RegistryTag entryTag) :
        registeredService(registeredService), registeredProvider(registeredProvider), entryTag(entryTag) {}

    static ServiceRegistryEntry fromJsonObject(const QJsonObject &jsonObj);
};

}

#endif // SERVICEREGISTRYENTRY_H
