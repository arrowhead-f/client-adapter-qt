#ifndef SERVICEREGISTRATIONFORM_H
#define SERVICEREGISTRATIONFORM_H

#include "arrowheadbasetypes.h"
#include <QJsonObject>

namespace arrowhead {

struct ServiceRegistrationForm
{
    ArrowheadService service;
    ArrowheadSystem provider;

    ServiceRegistrationForm(ArrowheadService service, ArrowheadSystem provider) :
        service(service), provider(provider) {}

    QJsonObject toJsonObject() const;
};

}

#endif // SERVICEREGISTRATIONFORM_H
