#ifndef TESTCOMMON_H
#define TESTCOMMON_H

#include "common.h"
#include "testsuite.h"
#include <QJsonObject>

class TestCommon : public TestSuite
{
    Q_OBJECT
public:
    using TestSuite::TestSuite;

private slots:
    void typeToString_data();
    void typeToString();
};

class TestRegistryTag : public TestSuite
{
    Q_OBJECT
public:
    using TestSuite::TestSuite;

private slots:
    void initTestCase();
    void toJsonObject();
    void fromJsonObject();

private:
    const int id=1;
    const std::string createdAt="2020-05-11T15:55:13Z";
    const std::string updatedAt="2020-05-11T18:01:23Z";
    QJsonObject regTagJsonObject;
};

#endif // TESTCOMMON_H
