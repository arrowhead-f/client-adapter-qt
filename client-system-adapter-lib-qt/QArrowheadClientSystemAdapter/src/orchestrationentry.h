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
