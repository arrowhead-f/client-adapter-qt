#ifndef SERVICEQUERYFORM_H
#define SERVICEQUERYFORM_H

#include "arrowheadservicerequirement.h"
#include <QJsonObject>

namespace arrowhead{

struct ServiceQueryForm
{
    ArrowheadServiceRequirement requestedService;
    bool pingProviders;

    struct builder;

    QJsonObject toJsonObject() const;

private:
    ServiceQueryForm(ArrowheadServiceRequirement requestedService,
                    bool pingProviders) :
        requestedService(requestedService),
        pingProviders(pingProviders) {}
};

struct ServiceQueryForm::builder{
private:
    ArrowheadServiceRequirement requestedService;
    bool pingProviders = false;

public:
    builder();
    builder& setRequestedService(ArrowheadServiceRequirement value);
    builder& setPingProviders(bool value);
    ServiceQueryForm build() const;
};

}

#endif // SERVICEQUERYFORM_H
