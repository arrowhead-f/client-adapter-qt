/****************************************************************************
**
** Copyright 2020 evopro Innovation Kft.
**
** This file is part of arrowhead-f/client-adapter-qt.
**
** arrowhead-f/client-adapter-qt is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**	
** arrowhead-f/client-adapter-qt is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU Lesser General Public License for more details.
**	
** You should have received a copy of the GNU Lesser General Public License
** along with arrowhead-f/client-adapter-qt. If not, see <https://www.gnu.org/licenses/>.
**
****************************************************************************/

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
