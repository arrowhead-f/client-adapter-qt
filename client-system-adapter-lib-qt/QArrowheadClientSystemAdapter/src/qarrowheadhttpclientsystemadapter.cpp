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

#include "qarrowheadhttpclientsystemadapter.h"
#include <QFile>
#include <QSslKey>
#include <QEventLoop>
#include <QJsonDocument>
#include <QUrlQuery>
#include <arrowheadserverexception.h>

namespace arrowhead {

ReturnValue QArrowheadHttpClientSystemAdapter::getServiceRegistryEcho()
{
    auto request = std::make_unique<QNetworkRequest>(QUrl(QString(serviceRegsitryBaseUrl.data()) + "/echo"));
    std::unique_ptr<QNetworkReply> reply;

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::GetOperation, request, reply);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientSystemAdapter::getServiceRegistryEcho SUCCESS";
        if(reply){
            qDebug() << "QArrowheadHttpClientSystemAdapter::getServiceRegistryEcho reply: " << reply->readAll();
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientSystemAdapter::getServiceRegistryEcho FAILURE";
    }

    return retVal;
}


ReturnValue QArrowheadHttpClientSystemAdapter::registerService(
        const ServiceRegistrationForm& serviceRegistrationForm, std::unique_ptr<ServiceRegistryEntry>& uptr_ServiceRegistryEntry)
{
    auto request = std::make_unique<QNetworkRequest>(QUrl(QString(serviceRegsitryBaseUrl.data()) + "/register"));
    std::unique_ptr<QNetworkReply> reply;
    auto payload = std::make_shared<QByteArray>(
                QJsonDocument(serviceRegistrationForm.toJsonObject()).toJson(QJsonDocument::Compact));
    request->setRawHeader("Content-Type", "application/json");

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::PostOperation, request, reply, payload);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientSystemAdapter::registerService SUCCESS";
        if(reply){
            QByteArray responseBody = reply->readAll();
            uptr_ServiceRegistryEntry = std::make_unique<ServiceRegistryEntry>(
                        ServiceRegistryEntry::fromJsonObject(QJsonDocument::fromJson(responseBody).object()));
            qDebug() << "QArrowheadHttpClientSystemAdapter::registerService reply: " << responseBody;
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientSystemAdapter::registerService FAILURE";
    }

    return retVal;
}


ReturnValue QArrowheadHttpClientSystemAdapter::unregisterService(
        const ServiceRegistryEntry& serviceRegistryEntry)
{
    ArrowheadService service = serviceRegistryEntry.registeredService.service;
    ArrowheadSystem provider = serviceRegistryEntry.registeredProvider.provider;

    QUrl deregistrationUrl(QString(serviceRegsitryBaseUrl.data()) + "/unregister");
    QUrlQuery query;
    query.addQueryItem("service_definition", service.serviceDefinition.data());
    query.addQueryItem("system_name", provider.systemName.data());
    query.addQueryItem("address", provider.address.data());
    query.addQueryItem("port", QString::number(provider.port));
    deregistrationUrl.setQuery(query);

    auto request = std::make_unique<QNetworkRequest>(deregistrationUrl);
    std::unique_ptr<QNetworkReply> reply;

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::DeleteOperation, request, reply);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientSystemAdapter::unregisterService SUCCESS";
        if(reply){
            qDebug() << "QArrowheadHttpClientSystemAdapter::registerService reply: " << reply->readAll();
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientSystemAdapter::unregisterService FAILURE";
    }

    return retVal;
}


ReturnValue QArrowheadHttpClientSystemAdapter::queryService(
        const ServiceQueryForm& serviceQueryForm, std::unique_ptr<ServiceQueryList>& uptr_ServiceQueryList)
{
    auto request = std::make_unique<QNetworkRequest>(QUrl(QString(serviceRegsitryBaseUrl.data()) + "/query"));
    std::unique_ptr<QNetworkReply> reply;
    auto payload = std::make_shared<QByteArray>(
                QJsonDocument(serviceQueryForm.toJsonObject()).toJson(QJsonDocument::Compact));
    request->setRawHeader("Content-Type", "application/json");

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::PostOperation, request, reply, payload);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientSystemAdapter::queryService SUCCESS";
        if(reply){
            QByteArray responseBody = reply->readAll();
            uptr_ServiceQueryList = std::make_unique<ServiceQueryList>(
                        ServiceQueryList::fromJsonObject(QJsonDocument::fromJson(responseBody).object()));
            qDebug() << "QArrowheadHttpClientSystemAdapter::queryService reply: " << responseBody;
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientSystemAdapter::queryService FAILURE";
    }

    return retVal;
}


ReturnValue QArrowheadHttpClientSystemAdapter::getAuthorizationSystemEcho()
{
    auto request = std::make_unique<QNetworkRequest>(QUrl(QString(authorizationBaseUrl.data()) + "/echo"));
    std::unique_ptr<QNetworkReply> reply;

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::GetOperation, request, reply);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientSystemAdapter::getAuthorizationSystemEcho SUCCESS";
        if(reply){
            qDebug() << "QArrowheadHttpClientSystemAdapter::getAuthorizationSystemEcho reply: " << reply->readAll();
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientSystemAdapter::getAuthorizationSystemEcho FAILURE";
    }

    return retVal;
}


ReturnValue QArrowheadHttpClientSystemAdapter::getPublicKey(std::string &publicKey)
{
    auto request = std::make_unique<QNetworkRequest>(QUrl(QString(authorizationBaseUrl.data()) + "/publickey"));
    std::unique_ptr<QNetworkReply> reply;

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::GetOperation, request, reply);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientSystemAdapter::getPublicKey SUCCESS";
        if(reply){
            QByteArray responseBody = reply->readAll();
            publicKey = responseBody.toStdString();
            qDebug() << "QArrowheadHttpClientSystemAdapter::getPublicKey reply: " << responseBody;
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientSystemAdapter::getPublicKey FAILURE";
    }

    return retVal;
}


ReturnValue QArrowheadHttpClientSystemAdapter::getOrchestratorEcho()
{
    auto request = std::make_unique<QNetworkRequest>(QUrl(QString(orchestratorBaseUrl.data()) + "/echo"));
    std::unique_ptr<QNetworkReply> reply;

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::GetOperation, request, reply);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientSystemAdapter::getOrchestratorEcho SUCCESS";
        if(reply){
            qDebug() << "QArrowheadHttpClientSystemAdapter::getOrchestratorEcho reply: " << reply->readAll();
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientSystemAdapter::getOrchestratorEcho FAILURE";
    }

    return retVal;
}


ReturnValue QArrowheadHttpClientSystemAdapter::requestOrchestration(const ServiceRequestForm& serviceRequestForm, std::unique_ptr<OrchestrationResponse>& uptr_orchestrationResponse){

    auto request = std::make_unique<QNetworkRequest>(QUrl(QString(orchestratorBaseUrl.data()) + "/orchestration"));
    std::unique_ptr<QNetworkReply> reply;
    auto payload = std::make_shared<QByteArray>(
                QJsonDocument(serviceRequestForm.toJsonObject()).toJson(QJsonDocument::Compact));
    request->setRawHeader("Content-Type", "application/json");

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::PostOperation, request, reply, payload);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientSystemAdapter::requestOrchestration SUCCESS";
        if(reply){
            QByteArray responseBody = reply->readAll();
            uptr_orchestrationResponse = std::make_unique<OrchestrationResponse>(
                        OrchestrationResponse::fromJsonObject(QJsonDocument::fromJson(responseBody).object()));
            qDebug() << "QArrowheadHttpClientSystemAdapter::requestOrchestration reply: " << responseBody;
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientSystemAdapter::requestOrchestration FAILURE";
    }

    return retVal;
}

ReturnValue QArrowheadHttpClientSystemAdapter::requestOrchestrationById(const int &storeEntryId, std::unique_ptr<OrchestrationResponse> &uptr_orchestrationResponse)
{
    auto request = std::make_unique<QNetworkRequest>(
                QUrl(QString(orchestratorBaseUrl.data()) + "/orchestration/" + QString::number(storeEntryId)));
    std::unique_ptr<QNetworkReply> reply;

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::GetOperation, request, reply);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientSystemAdapter::requestOrchestrationById SUCCESS";
        if(reply){
            QByteArray responseBody = reply->readAll();
            uptr_orchestrationResponse = std::make_unique<OrchestrationResponse>(
                        OrchestrationResponse::fromJsonObject(QJsonDocument::fromJson(responseBody).object()));
            qDebug() << "QArrowheadHttpClientSystemAdapter::requestOrchestrationById reply: " << responseBody;
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientSystemAdapter::requestOrchestrationById FAILURE";
    }

    return retVal;
}
}

