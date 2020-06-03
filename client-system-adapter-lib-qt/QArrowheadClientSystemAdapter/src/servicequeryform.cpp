#include "servicequeryform.h"
#include <QJsonArray>

namespace arrowhead {

QJsonObject ServiceQueryForm::toJsonObject() const
{
    // Mandatory fields: serviceDefinitionRequirement
    QJsonObject jsonObject = requestedService.toJsonObject();
    jsonObject.insert("pingProviders", pingProviders);

    return jsonObject;
}

ServiceQueryForm::builder::builder() :
    requestedService(ArrowheadServiceRequirement::builder().build()),
    pingProviders(false) {}

ServiceQueryForm::builder& ServiceQueryForm::builder::setRequestedService(ArrowheadServiceRequirement value){
    requestedService = value;
    return *this;
}

ServiceQueryForm::builder& ServiceQueryForm::builder::setPingProviders(bool value){
    pingProviders = value;
    return *this;
}

ServiceQueryForm ServiceQueryForm::builder::build() const{
    return ServiceQueryForm(requestedService,
                            pingProviders);
}

}


