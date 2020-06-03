#include "orchestrationresponse.h"
#include <QJsonArray>

namespace arrowhead {

OrchestrationResponse OrchestrationResponse::fromJsonObject(const QJsonObject &jsonObj){

    std::vector<OrchestrationEntry> orchEntryVector;

    QJsonArray response = jsonObj["response"].toArray();
    for(const auto orchEntry : response){
        orchEntryVector.push_back(OrchestrationEntry::fromJsonObject(orchEntry.toObject()));
    }

    return OrchestrationResponse(orchEntryVector);
}

}
