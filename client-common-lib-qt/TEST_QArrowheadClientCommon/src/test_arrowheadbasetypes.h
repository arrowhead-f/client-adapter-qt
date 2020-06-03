#ifndef TESTARROWHEADBASETYPES_H
#define TESTARROWHEADBASETYPES_H

#include "arrowheadbasetypes.h"
#include "testsuite.h"
#include <QObject>

class TestArrowheadCloud : public TestSuite
{
    Q_OBJECT
public:
    using TestSuite::TestSuite;

private slots:
    void initTestCase();
    void toJsonObject();
    void fromJsonObject();

private:
    const std::string operatorName = "evoInno";
    const std::string cloudName = "evoCloud";
    QJsonObject cloudJsonObject;
};

class TestArrowheadSystem : public TestSuite
{
    Q_OBJECT
public:
    using TestSuite::TestSuite;

private slots:
    void initTestCase();
    void toJsonObject();
    void fromJsonObject();

private:
    const std::string systemName = "CanFilter";
    const std::string address = "192.168.0.1";
    const int port = 443;
    const std::string authenticationInfo =
            "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAxzYuc22QSs"
            "t/dS7geYYK5l5kLxU0tayNdixkEQ17ix+CUcUbKIsnyftZxaCYT46r"
            "QtXgCaYRdJcbB3hmyrOavkhTpX79xJZnQmfuamMbZBqitvscxW9zRR"
            "9tBUL6vdi/0rpoUwPMEh8+Bw7CgYR0FK0DhWYBNDfe9HKcyZEv3max"
            "8Cdq18htxjEsdYO0iwzhtKRXomBWTdhD5ykd/fACVTr4+KEY+IeLvu"
            "bHVmLUhbE5NgWXxrRpGasDqzKhCTmsa2Ysf712rl57SlH0Wz/Mr3F7"
            "aM9YpErzeYLrl0GhQr9BVJxOvXcVd4kmY+XkiCcrkyS1cnghnllh+L"
            "CwQu1sYwIDAQAB";
    QJsonObject sysJsonObject;
};

class TestArrowheadService : public TestSuite
{
    Q_OBJECT
public:
    using TestSuite::TestSuite;

private slots:
    void initTestCase();
    void toJsonObject();
    void fromJsonObject();

private:
    const std::string serviceDefinition = "BatteryStatus";
    const std::vector<std::string> interfaces = {"JSON", "XML"};
    const arrowhead::SecurityType securityType = arrowhead::SecurityType::Certificate;
    const std::map<std::string, std::string> metadata = {
        {"refreshInterval", "60"},
        {"schema", "/schema/batterystatus.schema.json"}
    };
    const int version = 2;
    const std::string serviceUri = "/batterystatus";
    const std::string endOfValidity = "2030-05-11T15:55:13Z";
    QJsonObject serviceJsonObjectTo;
    QJsonObject serviceJsonObjectFrom;
};

#endif // TESTARROWHEADBASETYPES_H
