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

#include "testsuite.h"
#include <QtTest/QtTest>

int main(int argc, char *argv[])
{
    int status = 0;

    auto runTest = [&status, argc, argv](QObject* obj) {
        status |= QTest::qExec(obj, argc, argv);
    };

    auto &suite = TestSuite::suite();
    for(auto it = suite.begin(); it != suite.end(); ++it){
        runTest(*it);
    }

    if (0 == status) {
        qInfo() << "--------- All tests PASSED ---------";
    } else {
        qCritical() << "\x1B[1;31m--------- Unit test FAILED ---------\x1B[0m";
    }

    return status;
}
