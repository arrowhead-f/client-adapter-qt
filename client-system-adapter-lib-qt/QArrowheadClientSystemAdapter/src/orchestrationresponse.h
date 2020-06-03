#ifndef ORCHESTRATIONRESPONSE_H
#define ORCHESTRATIONRESPONSE_H

#include "orchestrationentry.h"
#include <vector>
#include <QJsonObject>

namespace arrowhead {

struct OrchestrationResponse
{
    std::vector<OrchestrationEntry> orchEntryVector;

    OrchestrationResponse(std::vector<OrchestrationEntry> orchEntryVector) :
        orchEntryVector(orchEntryVector) {}

    static OrchestrationResponse fromJsonObject(const QJsonObject &jsonObj);
};

}


#endif // ORCHESTRATIONRESPONSE_H
