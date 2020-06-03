#include "test_arrowheadbasetypes.h"
#include <QtTest/QtTest>
#include <QMap>
#include <QVariant>
#include <QVector>

static TestArrowheadCloud TEST_ARROWHEADCLOUD;
static TestArrowheadSystem TEST_ARROWHEADSYSTEM;
static TestArrowheadService TEST_ARROWHEADSERVICE;

void TestArrowheadCloud::initTestCase(){
    QMap<QString,QVariant> kvPairs;
    kvPairs.insert("operator", operatorName.data());
    kvPairs.insert("name", cloudName.data());
    cloudJsonObject = QJsonObject::fromVariantMap(kvPairs);
}

void TestArrowheadCloud::toJsonObject(){
    using namespace arrowhead;
    ArrowheadCloud ahCloud(operatorName, cloudName);

    QCOMPARE(ahCloud.toJsonObject(), cloudJsonObject);
}

void TestArrowheadCloud::fromJsonObject(){
    using namespace arrowhead;
    ArrowheadCloud arrowheadCloud = ArrowheadCloud::fromJsonObject(cloudJsonObject);

    QCOMPARE(arrowheadCloud.operatorName, operatorName);
    QCOMPARE(arrowheadCloud.cloudName, cloudName);
}

void TestArrowheadSystem::initTestCase(){
    QMap<QString,QVariant> kvPairs;
    kvPairs.insert("systemName", systemName.data());
    kvPairs.insert("address", address.data());
    kvPairs.insert("port", port);
    kvPairs.insert("authenticationInfo", authenticationInfo.data());
    sysJsonObject = QJsonObject::fromVariantMap(kvPairs);
}

void TestArrowheadSystem::toJsonObject(){
    using namespace arrowhead;
    ArrowheadSystem arrowheadSystem(systemName, address, port,
                                    authenticationInfo);

    QCOMPARE(arrowheadSystem.toJsonObject(), sysJsonObject);
}

void TestArrowheadSystem::fromJsonObject(){
    using namespace arrowhead;
    ArrowheadSystem arrowheadSystem = ArrowheadSystem::fromJsonObject(sysJsonObject);

    QCOMPARE(arrowheadSystem.systemName, systemName);
    QCOMPARE(arrowheadSystem.address, address);
    QCOMPARE(arrowheadSystem.port, port);
    QCOMPARE(arrowheadSystem.authenticationInfo, authenticationInfo);
}

void TestArrowheadService::initTestCase(){
    QMap<QString,QVariant> kvPairs;
    kvPairs.insert("serviceDefinition", serviceDefinition.data());
    kvPairs.insert("secure", arrowhead::typeToString(securityType).data());
    kvPairs.insert("version", version);
    kvPairs.insert("serviceUri", serviceUri.data());
    kvPairs.insert("endOfValidity", endOfValidity.data());
    serviceJsonObjectTo = QJsonObject::fromVariantMap(kvPairs);

    QStringList ifaceList;
    for(auto iface : interfaces){
        ifaceList.append(iface.data());
    }
    serviceJsonObjectTo.insert("interfaces", QJsonArray::fromStringList(ifaceList));

    QMap<QString,QVariant> mdPairs;
    for(auto mdPair : metadata){
        mdPairs.insert(mdPair.first.data(),
                       mdPair.second.data());
    }
    serviceJsonObjectTo.insert("metadata", QJsonObject::fromVariantMap(mdPairs));

    serviceJsonObjectFrom = serviceJsonObjectTo;
    QMap<QString,QVariant> servDefKvPairs;
    servDefKvPairs.insert("serviceDefinition", serviceDefinition.data());
    serviceJsonObjectFrom["serviceDefinition"] =
            QJsonObject::fromVariantMap(servDefKvPairs);

}

void TestArrowheadService::toJsonObject(){
    using namespace arrowhead;
    ArrowheadService builtService =
            ArrowheadService::builder()
            .setServiceDefinition(serviceDefinition)
            .setInterfaces(interfaces)
            .setSecurityType(securityType)
            .setMetadata(metadata)
            .setVersion(version)
            .setServiceUri(serviceUri)
            .setEndOfValidity(endOfValidity)
            .build();

    QCOMPARE(builtService.toJsonObject(), serviceJsonObjectTo);
}

void TestArrowheadService::fromJsonObject(){
    using namespace arrowhead;
    ArrowheadService arrowheadService = ArrowheadService::fromJsonObject(serviceJsonObjectFrom);

    QCOMPARE(arrowheadService.serviceDefinition, serviceDefinition);
    QCOMPARE(arrowheadService.interfaces, interfaces);
    QCOMPARE(arrowheadService.securityType, securityType);
    QCOMPARE(arrowheadService.metadata, metadata);
    QCOMPARE(arrowheadService.version, version);
    QCOMPARE(arrowheadService.serviceUri, serviceUri);
    QCOMPARE(arrowheadService.endOfValidity, endOfValidity);
}
