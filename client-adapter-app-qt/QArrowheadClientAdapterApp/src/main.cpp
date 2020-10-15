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
#include "consoleapp.h"
#include <string>
#include <QCoreApplication>
#include <QSettings>
#include <QDebug>
#include <QFile>
#include <QProcess>
#include <QThread>
#include <QObject>
#include <QMetaEnum>

static constexpr const char *LOG_FILE_PATH = "qarrowheadclientadapterapp.log";

int main(int argc, char *argv[])
{
    auto store = std::make_unique<DataStore>();
    Record<SettingsParam> settings = store->getSettings();

    std::string logFileNameString = settings.getString(SettingsParam::log_file);
    QString logFileName = logFileNameString.empty() ? LOG_FILE_PATH : logFileNameString.c_str();

    QtMsgType logLevel = QtMsgType::QtInfoMsg;
    std::string logLevelString = settings.getString(SettingsParam::log_level);
    if(logLevelString == "debug"){
        logLevel = QtMsgType::QtDebugMsg;
    }
    else if(logLevelString == "info"){
        logLevel = QtMsgType::QtInfoMsg;
    }
    else if(logLevelString == "warning"){
        logLevel = QtMsgType::QtWarningMsg;
    }


    // Set up logging
    auto logger = std::make_unique<arrowhead::Logger>(logFileName);
    logger->setLogLevel(logLevel);
    qInstallMessageHandler(logger->myMessageHandler);


#ifdef CREATE_DOCUMENTATION
    FormattedText::setUsingFormattedTexts(false);
    ConsoleApp app{argc, argv};

    app.createUserDocumentation();
#else

    QCoreApplication a(argc, argv);
    ConsoleApp app{argc, argv};

    app.start();
#endif

    return 0;
}

