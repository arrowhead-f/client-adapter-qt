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
#include "logger.h"
#include <QtTest/QtTest>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Processing arguments
    QStringList arguments = app.arguments();
    QString logFileName = app.applicationName() + ".log";
    QtMsgType logLevel = QtMsgType::QtInfoMsg;

    for(quint8 i=0; i<arguments.size()-1; i++){
        if(arguments.at(i) == "--logFile"){
            logFileName = arguments.at(i+1);
        }
        else if(arguments.at(i) == "--logLevel"){
            if(arguments.at(i+1) == "debug"){
                logLevel = QtMsgType::QtDebugMsg;
            }
            else if(arguments.at(i+1) == "info"){
                logLevel = QtMsgType::QtInfoMsg;
            }
            else if(arguments.at(i+1) == "warning"){
                logLevel = QtMsgType::QtWarningMsg;
            }
        }
    }

    logFileName = app.applicationDirPath() + "/" + logFileName;

    // Set up logging
    auto logger = std::make_unique<arrowhead::Logger>(logFileName, &app);
    logger->setLogLevel(logLevel);
    qInstallMessageHandler(logger->myMessageHandler);

    qInfo() << "Application: " << app.applicationName() << " started at: " << QDateTime::currentDateTime() <<
               ", logging to: " << logFileName;

    int status = 0;

    auto runTest = [&status, &arguments](QObject* obj) {
        if(arguments.contains("--logFile")){
            arguments.replaceInStrings("--logFile", "-o");
        }
        if(arguments.contains("--logLevel")){
            int index = arguments.indexOf("--logLevel");
            QString logLevelArg = arguments.at(index+1);
            if(logLevelArg == "debug"){
                arguments.replaceInStrings("--logLevel", "-vs");
            }
            else if(logLevelArg == "warning"){
                arguments.replaceInStrings("--logLevel", "-silent");
            }
            else{
                arguments.replaceInStrings("--logLevel", "-v2");
            }
            arguments.removeAt(index+1);
        }
        status |= QTest::qExec(obj, arguments);
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

    app.exit(status);
}
