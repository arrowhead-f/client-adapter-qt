#ifndef TESTSUITE_H
#define TESTSUITE_H

#include <QObject>
#include <vector>

class TestSuite : public QObject
{
    Q_OBJECT
public:
    TestSuite(){
        suite().push_back(this);
    }

    static std::vector<QObject*> & suite(){
        static std::vector<QObject*> objects;
        return objects;
    }
};

#endif // TESTSUITE_H
