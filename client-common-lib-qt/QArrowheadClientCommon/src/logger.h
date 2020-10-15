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
#ifndef LOGGER_H
#define LOGGER_H

#include "qarrowheadclientcommon_global.h"
#include <QObject>
#include <QTextStream>
#include <QFile>

namespace arrowhead {

class QARROWHEADCLIENTCOMMON_EXPORT Logger : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Logger
     * @param logFileName: specifying the file to log to (absolute path)
     * @param parent: QObject parent for integrating with QObject memory management
     *
     * @details Explicit constructor for Logger
     */
    explicit Logger(QString logFileName, QObject *parent = nullptr);

    /**
    * @brief ~Logger
    *
    * @details Destructor for Logger
    */
   ~Logger();

   /**
    * @brief myMessageHandler
    * @param type: type of the message (Debug, Info, Warning, Critical, Fatal)
    * @param context: context of the message
    * @param msg: message text
    *
    * @details The message handler is a function that prints out debug messages, warnings,
    * critical and fatal error messages. Must be static.
    */
   static void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

   /**
    * @brief getLogToFile
    * @return The value of the member variable 'logToFile'
    *
    * @details Getter function for member variable 'logToFile'
    */
   bool getLogToFile() const;

   /**
    * @brief setLogToFile
    * @param value: setter value for member variable 'logToFile'
    *
    * @detials Setter function for member variable 'logToFile'
    */
   void setLogToFile(bool value);

   /**
    * @brief getLogFileName
    * @return The value of the member variable 'logFileName'
    *
    * @details Getter function for member variable 'logFileName'
    */
   QString getLogFileName() const;

   /**
    * @brief setLogFileName
    * @param value: setter value for member variable 'logFileName'
    *
    * @details Setter function for member variable 'logFileName'
    */
   void setLogFileName(const QString &value);

   QtMsgType getLogLevel() const;
   void setLogLevel(const QtMsgType &value);

signals:

public slots:

private:
   /**
    * @brief logToFile
    *
    * @details Indicates whether message output is directed to file
    * (true) or to standrad output (false)
    */
   bool logToFile;

   /**
    * @brief logFileName
    *
    * @details name of the log file
    */
   QString logFileName;

   /**
    * @brief logFile
    *
    * @details The reference to the logFile, which is used to store
    * log messages.
    */
   QFile logFile;

   /**
    * @brief logStream
    *
    * @details The output stream (file or stdout). Must be static.
    */
   static QTextStream logStream;

   /**
    * @brief logLevel
    */
   static QtMsgType logLevel;
};

}

#endif // LOGGER_H
