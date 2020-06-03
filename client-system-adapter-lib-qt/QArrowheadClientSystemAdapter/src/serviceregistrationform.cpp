#include "serviceregistrationform.h"

namespace arrowhead {

QJsonObject ServiceRegistrationForm::toJsonObject() const
{
    QJsonObject jsonObject = service.toJsonObject();
    jsonObject.insert("providerSystem", provider.toJsonObject());
    return jsonObject;
}

}


