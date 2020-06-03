#ifndef SERVICEQUERYLIST_H
#define SERVICEQUERYLIST_H

#include "serviceregistryentry.h"
#include <vector>
#include <QJsonObject>

namespace arrowhead {

struct ServiceQueryList
{
    std::vector<ServiceRegistryEntry> srEntryVector;
    int unfilteredHits;

    ServiceQueryList(std::vector<ServiceRegistryEntry> srEntryVector,
                     int unfilteredHits) :
        srEntryVector(srEntryVector),
        unfilteredHits(unfilteredHits) {}

    static ServiceQueryList fromJsonObject(
            const QJsonObject &jsonObj);
};

}



#endif // SERVICEQUERYLIST_H
