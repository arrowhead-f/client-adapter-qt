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

#include <QCoreApplication>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QString>
#include <QDebug>

#include <iostream>

#include "arrowheadclient.h"
#include "arrowheadservice.h"
#include "securearrowheadclient.h"
#include "insecurearrowheadclient.h"


void printHelp(){
    std::cout << "QArrowheadAdaptor: sw adaptor library to integrate legacy systems into an Arrowhead Cloud" << std::endl;
    std::cout << "HELP" << std::endl;

}

bool appConfigValid(QSettings *appConfig){
    bool retVal = true;

    //TODO validation conditions check

    return retVal;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QStringList args = app.arguments();
    bool secureClient = false;
    QString configFilePath("adaptor.ini");

    if(args.size() > 1){
        bool skip = false;
        for(int i=1; i<args.size();++i){
            if(skip){
                skip = false;
                continue;
            }
            //secure
            if((args.at(i) == "-s") || (args.at(i) == "--secure")){
                std::cout << "Secure client" << std::endl;
                secureClient = true;
            }
            //config file path
            else if((args.at(i) == "-c") || (args.at(i) == "--config")){
                if(args.size()>(i+1)){
                    configFilePath = args.at(i+1);
                    std::cout << "Configuration file path set to: " << configFilePath.toStdString()<< std::endl;
                    skip = true;
                }
                else{
                    std::cout << "Missing missing configuration file path!" << std::endl;
                    return 0;
                }
            }
            //help
            else if((args.at(i) == "-h") || (args.at(i) == "--help")){
                printHelp();
                return 0;
            }
            else{
                std::cout << "Unknown command option: " << args.at(i).toStdString() << std::endl;
                return 0;
            }
        }
    }
    else{
        printHelp();
        return 0;
    }

    QSettings appConfig(configFilePath, QSettings::IniFormat, &app);
    if(appConfigValid(&appConfig)){
        ArrowheadClient *ahClient;
        if(secureClient){
            qDebug() << "Creating secure arrowhead client...";
            ahClient = new SecureArrowheadClient(&appConfig, &app);
        }
        else{
            qDebug() << "Creating insecure arrowhead client...";
            ahClient = new InsecureArrowheadClient(&appConfig, &app);
        }
    }
    // Load the configuration file.
    // Warning: applicationDirPath returns the directory of the executable file!
//    QString arrowheadConfigFileName(app.applicationDirPath() + "/etc/arrowhead.ini");
//    qDebug() << "arrowheadConfigFileName: " << arrowheadConfigFileName;
//    QSettings *arrowheadSettings=new QSettings(arrowheadConfigFileName, QSettings::IniFormat, &app);

    // Create the Arrowhead Client, which manages the communication with the Arrowhead core systems
//    ArrowheadClient *ahClient = new ArrowheadClient(arrowheadSettings, &app);
//    ahClient->pingServiceRegistry();

    // Register provided services (Service Regsitry)
//    ArrowheadService *providedService = new ArrowheadService(arrowheadSettings, "myService", &app);
//    QString serviceURI("/chargingProfile");
    //counter = ahClient->registerService(providedService, serviceURI);
    //qCritical() << "main Number of successful registrations: " << counter;

    // Request services to consume (Orchestrator)
//    QJsonDocument orchResponse;
//    ArrowheadService *consumedService = new ArrowheadService(arrowheadSettings, "myService", &app);
//    QMap<QString, bool> orchestrationFlags;
//    QString QoS("");
//    ahClient->setOrchestrationFlags(orchestrationFlags);
    //ahClient->requestService(consumedService, orchestrationFlags, QoS, orchResponse);
    //qCritical() << "main Orchestration response: " << orchResponse;

    // Deregister provided services (Service Registry)
//    ahClient->deregisterService(providedService, serviceURI);

    return app.exec();
}

