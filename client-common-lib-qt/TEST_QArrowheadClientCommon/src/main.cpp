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
