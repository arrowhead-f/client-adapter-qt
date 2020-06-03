#include "serviceregistryentry.h"

namespace arrowhead {

ServiceRegistryEntry ServiceRegistryEntry::fromJsonObject(const QJsonObject &jsonObj)
{
    RegistryTag entryTag = RegistryTag::fromJsonObject(jsonObj);

    ArrowheadService service = ArrowheadService::fromJsonObject(jsonObj);
    RegistryTag serviceTag = RegistryTag::fromJsonObject(jsonObj["serviceDefinition"].toObject());

    ArrowheadSystem provider = ArrowheadSystem::fromJsonObject(jsonObj["provider"].toObject());
    RegistryTag providerTag = RegistryTag::fromJsonObject(jsonObj["provider"].toObject());

    return ServiceRegistryEntry(ServiceEntry(service, serviceTag), ProviderEntry(provider, providerTag),
                                entryTag);
}

}
