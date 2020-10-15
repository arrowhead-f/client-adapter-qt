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
#include "logger.h"
#include <stdio.h>
#include <QDateTime>
#include <QDebug>

namespace arrowhead {

QTextStream Logger::logStream(stdout);
QtMsgType Logger::logLevel=QtMsgType::QtInfoMsg;

Logger::Logger(QString logFileName, QObject *parent) : QObject(parent),
    logToFile(false), logFileName(logFileName)
{
    logFile.setFileName(logFileName);
    if(logFile.open(QIODevice::WriteOnly | QIODevice::Append) == true){
        logStream.setDevice(&logFile);
        logStream << "Logger: stream device set to " << logFileName << "\n";
        logToFile = true;
    }
    else{
        qCritical() << "Unable to open logFile: "<< logFileName << "\n";
    }

}


Logger::~Logger(){
    logStream.flush();
    if(logFile.isOpen()){
        logFile.close();
    }
}


// Use the following line instead if the context line and function is more useful:
// logStream << "[" << dateTime.toString("yyyy-MM-dd HH:mm:ss.zzz").toLocal8Bit().constData() << " | DEBUG |" <<
// " | " << context.line << " | " << context.function << "] " << localMsg.constData() << "\n";
void Logger::myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg){

    QStringList pathItems = QString(context.file).split('/');
    QString fileName = pathItems.last();

    if(type < logLevel){
        return;
    }

    QByteArray localMsg = msg.toLocal8Bit();
    QDateTime dateTime(QDateTime::currentDateTime());
    switch (type) {
    case QtDebugMsg:
       logStream << "[" << dateTime.toString("yyyy-MM-dd HH:mm:ss.zzz").toLocal8Bit().constData() << " | DEBUG   " <<
                     "] " << fileName << ":" << context.line << ":" << ": " << localMsg.constData() << "\n";
       break;
    case QtInfoMsg:
       logStream << "[" << dateTime.toString("yyyy-MM-dd HH:mm:ss.zzz").toLocal8Bit().constData() << " | INFO    " <<
                    "] " << fileName << ":" << context.line << ":" << ": " << localMsg.constData() << "\n";
       break;
    case QtWarningMsg:
       logStream << "[" << dateTime.toString("yyyy-MM-dd HH:mm:ss.zzz").toLocal8Bit().constData() << " | WARNING " <<
                    "] " << fileName << ":" << context.line << ":" << ": " << localMsg.constData() << "\n";
       break;
    case QtCriticalMsg:
       logStream << "[" << dateTime.toString("yyyy-MM-dd HH:mm:ss.zzz").toLocal8Bit().constData() << " | CRITICAL" <<
                    "] " << fileName << ":" << context.line << ":" << ": " << localMsg.constData() << "\n";
       break;
    case QtFatalMsg:
       logStream << "[" << dateTime.toString("yyyy-MM-dd HH:mm:ss.zzz").toLocal8Bit().constData() << " | FATAL   " <<
                    "] " << fileName << ":" << context.line << ":" << ": " << localMsg.constData() << "\n";
       abort();
    }
    logStream.flush();
}

bool Logger::getLogToFile() const
{
    return logToFile;
}

void Logger::setLogToFile(bool value)
{
    if(logToFile != value){
       logToFile = value;
       logStream.flush();
       logFile.close();

       // If we want to log to file, than set logStream to logFile.
       if(logToFile == true){
           logFile.setFileName(logFileName);
           if(logFile.open(QIODevice::WriteOnly | QIODevice::Append) == true){
               logStream.setDevice(&logFile);
               qDebug() << "Logger: stream device set to " << logFileName << "\n";
           }
           else{
               qCritical() << "Unable to open logFile: " << logFileName << "\n";
           }
       }

       // If we want to log to stdout, than set logStream to stdout
       else{
           logFile.close();
           if(logFile.open(stdout, (QIODevice::WriteOnly | QIODevice::Append)) == true){
               logStream.setDevice(&logFile);
               qDebug() << "Logger: stream device set to stdout.\n";
           }
           else{
               qCritical() << "Unable to open stdout.\n";
           }
       }
    }

}

QString Logger::getLogFileName() const
{
    return logFileName;
}

void Logger::setLogFileName(const QString &value)
{
    logFileName = value;
}

QtMsgType Logger::getLogLevel() const
{
    return logLevel;
}

void Logger::setLogLevel(const QtMsgType &value)
{
    logLevel = value;
}

}
