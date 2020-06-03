#ifndef ARROWHEADSERVICEREQUIREMENT_H
#define ARROWHEADSERVICEREQUIREMENT_H

#include "common.h"
#include "arrowheadbasetypes.h"
#include <string>
#include <vector>
#include <QJsonObject>

namespace arrowhead {

struct ArrowheadServiceRequirement
{
    std::string serviceDefinitionRequirement;
    stringvector interfaceRequirements;
    std::vector<SecurityType> securityRequirements;
    stringmap metadataRequirements;
    int versionRequirement;
    int minVersionRequirement;
    int maxVersionRequirement;

    struct builder;

    QJsonObject toJsonObject() const;

private:
    ArrowheadServiceRequirement(std::string serviceDefinitionRequirement,
                                stringvector interfaceRequirements,
                                std::vector<SecurityType> securityRequirements,
                                stringmap metadataRequirements,
                                int versionRequirement,
                                int minVersionRequirement,
                                int maxVersionRequirement) :
                    serviceDefinitionRequirement(serviceDefinitionRequirement),
                    interfaceRequirements(interfaceRequirements),
                    securityRequirements(securityRequirements),
                    metadataRequirements(metadataRequirements),
                    versionRequirement(versionRequirement),
                    minVersionRequirement(minVersionRequirement),
                    maxVersionRequirement(maxVersionRequirement) {}
};

struct ArrowheadServiceRequirement::builder{
private:
    std::string serviceDefinitionRequirement;
    stringvector interfaceRequirements;
    std::vector<SecurityType> securityRequirements;
    stringmap metadataRequirements;
    int versionRequirement = 0;
    int minVersionRequirement = 0;
    int maxVersionRequirement = 0;

public:
    builder& setServiceDefinitionRequirement(std::string value);
    builder& setInterfaceRequirements(stringvector value);
    builder& setSecurityRequirements(std::vector<SecurityType> value);
    builder& setMetadataRequirements(stringmap value);
    builder& setVersionRequirement(int value);
    builder& setMinVersionRequirement(int value);
    builder& setMaxVersionRequirement(int value);
    ArrowheadServiceRequirement build() const;
};

}

#endif // ARROWHEADSERVICEREQUIREMENT_H
