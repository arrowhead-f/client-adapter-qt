#include "servicequerylist.h"
#include <QJsonArray>

namespace arrowhead {

ServiceQueryList ServiceQueryList::fromJsonObject(const QJsonObject &jsonObj)
{
    std::vector<ServiceRegistryEntry> srEntryVector;
    int unfilteredHits = jsonObj["unfilteredHits"].toInt();

    QJsonArray serviceQueryData = jsonObj["serviceQueryData"].toArray();
    for(auto srEntry : serviceQueryData){
        srEntryVector.push_back(ServiceRegistryEntry::fromJsonObject(
                                    srEntry.toObject()));
    }

    return ServiceQueryList(srEntryVector, unfilteredHits);
}

}
