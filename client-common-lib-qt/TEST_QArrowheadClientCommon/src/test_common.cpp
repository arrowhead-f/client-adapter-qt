#include "test_common.h"
#include <QtTest/QtTest>

Q_DECLARE_METATYPE(std::string)

static TestCommon TEST_COMMON;
static TestRegistryTag TEST_REGISTRYTAG;

void TestCommon::typeToString_data(){
    using namespace arrowhead;

    QTest::addColumn<SecurityType>("sourceType");
    QTest::addColumn<std::string>("resultString");

    QTest::newRow("not secure") << SecurityType::NotSecure <<
                                   std::string("NOT_SECURE");
    QTest::newRow("certificate") << SecurityType::Certificate <<
                                    std::string("CERTIFICATE");
    QTest::newRow("token") << SecurityType::Token <<
                              std::string("TOKEN");
    QTest::newRow("invalid") << SecurityType::Invalid <<
                                std::string("");
}

void TestCommon::typeToString(){
    using namespace arrowhead;
    QFETCH(SecurityType, sourceType);
    QFETCH(std::string, resultString);
    QCOMPARE(arrowhead::typeToString(sourceType),
             resultString);
}

void TestRegistryTag::initTestCase(){
    QMap<QString,QVariant> kvPairs;
    kvPairs.insert("id", id);
    kvPairs.insert("createdAt", createdAt.data());
    kvPairs.insert("updatedAt", updatedAt.data());
    regTagJsonObject = QJsonObject::fromVariantMap(kvPairs);
}

void TestRegistryTag::toJsonObject(){
    using namespace arrowhead;
    RegistryTag registryTag(id, createdAt, updatedAt);

    QCOMPARE(registryTag.toJsonObject(), regTagJsonObject);
}

void TestRegistryTag::fromJsonObject(){
    using namespace arrowhead;
    RegistryTag registryTag = RegistryTag::fromJsonObject(regTagJsonObject);

    QCOMPARE(registryTag.id, id);
    QCOMPARE(registryTag.createdAt, createdAt);
    QCOMPARE(registryTag.updatedAt, updatedAt);
}
