#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <QObject>
#include <QJsonObject>

namespace arrowhead {

enum class ReturnValue{
    Ok = 0,
    InvalidValue = 1,
    InvalidArgNum = 2,
    NetworkError = 3,
    PeerError = 4,
    UnknownError = 5,
    Unimplemented = 6
};

enum class SecurityType{
    Invalid,
    NotSecure,
    Certificate,
    Token
};

std::string typeToString(const SecurityType& type);
SecurityType securityTypeFromString(const std::string& typeString);

struct RegistryTag{
    int id {-1};
    std::string createdAt;
    std::string updatedAt;

    RegistryTag() = default;
    RegistryTag(int id, std::string createdAt, std::string updatedAt) :
        id(id), createdAt(createdAt), updatedAt(updatedAt) {}

    QJsonObject toJsonObject() const;
    static RegistryTag fromJsonObject(const QJsonObject& jsonObject);
};

}

Q_DECLARE_METATYPE(arrowhead::SecurityType)

#endif // COMMON_H
