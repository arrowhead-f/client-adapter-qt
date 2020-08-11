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

#include "qarrowheadhttpclientdeviceadapter.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QByteArray>
#include <QUrlQuery>
#include <string>

namespace arrowhead {

// Onboarding Controller Interface
ReturnValue QArrowheadHttpClientDeviceAdapter::getOnboardingControllerEcho(){
    auto request = std::make_unique<QNetworkRequest>(QUrl(QString(onboardingControllerBaseUrl.data()) + "/echo"));
    std::unique_ptr<QNetworkReply> reply;

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::GetOperation, request, reply);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientDeviceAdapter::getOnboardingConrtollerEcho SUCCESS";
        if(reply){
            qDebug() << "QArrowheadHttpClientDeviceAdapter::getOnboardingConrtollerEcho reply: " << reply->readAll();
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientDeviceAdapter::getOnboardingConrtollerEcho FAILURE";
    }

    return retVal;
}

ReturnValue QArrowheadHttpClientDeviceAdapter::requestOnboardingWithName(
        const OnboardingWithNameRequest &onboardingRequest,
        std::unique_ptr<OnboardingResponse> &uptr_onboardingResponse){

    auto request = std::make_unique<QNetworkRequest>();
    request->setRawHeader("Content-Type", "application/json");
    QUrl requestUrl;
    if(httpClient.getAuthMethod() == HttpClient::AuthenticationMethod::BasicAuthentication){
        requestUrl = QUrl(QString(onboardingControllerBaseUrl.data()) + "/sharedsecret/name");
    }
    else if(httpClient.getAuthMethod() == HttpClient::AuthenticationMethod::ClientCertificate){
        requestUrl = QUrl(QString(onboardingControllerBaseUrl.data()) + "/certificate/name");
    }
    else{
        qWarning() << "QArrowheadHttpClientDeviceAdapter::requestOnboardingWithName: " <<
                      "authentication method invalid: " << HttpClient::typeToString(httpClient.getAuthMethod()).data();
        return ReturnValue::InvalidValue;
    }
    request->setUrl(requestUrl);

    std::unique_ptr<QNetworkReply> reply;
    auto payload = std::make_shared<QByteArray>(
                QJsonDocument(onboardingRequest.toJsonObject()).toJson(QJsonDocument::Compact));

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::PostOperation, request, reply, payload);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientDeviceAdapter::requestOnboardingWithName SUCCESS";
        if(reply){
            QByteArray responseBody = reply->readAll();
            uptr_onboardingResponse = std::make_unique<OnboardingResponse>(
                        OnboardingResponse::fromJsonObject(QJsonDocument::fromJson(responseBody).object()));
            qDebug() << "QArrowheadHttpClientDeviceAdapter::requestOnboardingWithName reply: " << responseBody;
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientDeviceAdapter::requestOnboardingWithName FAILURE";
    }

    return retVal;
}

ReturnValue QArrowheadHttpClientDeviceAdapter::requestOnboardingWithCsr(
        const OnboardingWithCsrRequest& onboardingRequest,
        std::unique_ptr<OnboardingResponse>& uptr_onboardingResponse){
    auto request = std::make_unique<QNetworkRequest>();
    request->setRawHeader("Content-Type", "application/json");

    QUrl requestUrl;
    if(httpClient.getAuthMethod() == HttpClient::AuthenticationMethod::BasicAuthentication){
        requestUrl = QUrl(QString(onboardingControllerBaseUrl.data()) + "/sharedsecret/csr");
    }
    else if(httpClient.getAuthMethod() == HttpClient::AuthenticationMethod::ClientCertificate){
        requestUrl = QUrl(QString(onboardingControllerBaseUrl.data()) + "/certificate/csr");
    }
    else{
        qWarning() << "QArrowheadHttpClientDeviceAdapter::requestOnboardingWithCsr: " <<
                      "authentication method invalid: " << HttpClient::typeToString(httpClient.getAuthMethod()).data();
        return ReturnValue::InvalidValue;
    }
    request->setUrl(requestUrl);

    std::unique_ptr<QNetworkReply> reply;
    auto payload = std::make_shared<QByteArray>(
                QJsonDocument(onboardingRequest.toJsonObject()).toJson(QJsonDocument::Compact));

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::PostOperation, request, reply, payload);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientDeviceAdapter::requestOnboardingWithCsr SUCCESS";
        if(reply){
            QByteArray responseBody = reply->readAll();
            uptr_onboardingResponse = std::make_unique<OnboardingResponse>(
                        OnboardingResponse::fromJsonObject(QJsonDocument::fromJson(responseBody).object()));
            qDebug() << "QArrowheadHttpClientDeviceAdapter::requestOnboardingWithCsr reply: " << responseBody;
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientDeviceAdapter::requestOnboardingWithCsr FAILURE";
    }

    return retVal;
}


//Device Registry Interface
ReturnValue QArrowheadHttpClientDeviceAdapter::getDeviceRegistryEcho(){
    auto request = std::make_unique<QNetworkRequest>(QUrl(QString(deviceRegistryBaseUrl.data()) + "/echo"));
    std::unique_ptr<QNetworkReply> reply;

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::GetOperation, request, reply);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientDeviceAdapter::getDeviceRegistryEcho SUCCESS";
        if(reply){
            qDebug() << "QArrowheadHttpClientDeviceAdapter::getDeviceRegistryEcho reply: " << reply->readAll();
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientDeviceAdapter::getDeviceRegistryEcho FAILURE";
    }

    return retVal;
}

ReturnValue QArrowheadHttpClientDeviceAdapter::queryDevice(
        const DeviceQueryForm& deviceQueryForm,
        std::unique_ptr<DeviceQueryList>& uptr_deviceQueryList){
    auto request = std::make_unique<QNetworkRequest>();
    request->setRawHeader("Content-Type", "application/json");

    request->setUrl(QUrl(QString(deviceRegistryBaseUrl.data()) + "/query"));

    std::unique_ptr<QNetworkReply> reply;
    auto payload = std::make_shared<QByteArray>(
                QJsonDocument(deviceQueryForm.toJsonObject()).toJson(QJsonDocument::Compact));

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::PostOperation, request, reply, payload);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientDeviceAdapter::queryDevice SUCCESS";
        if(reply){
            QByteArray responseBody = reply->readAll();
            uptr_deviceQueryList = std::make_unique<DeviceQueryList>(
                        DeviceQueryList::fromJsonObject(QJsonDocument::fromJson(responseBody).object()));
            qDebug() << "QArrowheadHttpClientDeviceAdapter::queryDevice reply: " << responseBody;
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientDeviceAdapter::queryDevice FAILURE";
    }

    return retVal;
}

ReturnValue QArrowheadHttpClientDeviceAdapter::registerDevice(
        const DeviceRegistrationForm& deviceRegistrationForm,
        std::unique_ptr<DeviceRegistryEntry>& uptr_deviceRegistryEntry){

    auto request = std::make_unique<QNetworkRequest>();
    request->setRawHeader("Content-Type", "application/json");

    request->setUrl(QUrl(QString(deviceRegistryBaseUrl.data()) + "/register"));

    std::unique_ptr<QNetworkReply> reply;
    auto payload = std::make_shared<QByteArray>(
                QJsonDocument(deviceRegistrationForm.toJsonObject()).toJson(QJsonDocument::Compact));

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::PostOperation, request, reply, payload);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientDeviceAdapter::registerDevice SUCCESS";
        if(reply){
            QByteArray responseBody = reply->readAll();
            uptr_deviceRegistryEntry = std::make_unique<DeviceRegistryEntry>(
                        DeviceRegistryEntry::fromJsonObject(QJsonDocument::fromJson(responseBody).object()));
            qDebug() << "QArrowheadHttpClientDeviceAdapter::registerDevice reply: " << responseBody;
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientDeviceAdapter::registerDevice FAILURE";
    }

    return retVal;
}

ReturnValue QArrowheadHttpClientDeviceAdapter::registerOnboardingDeviceWithName(
        const DeviceOnboardingWithNameRequest& deviceOnboardingRequest,
        std::unique_ptr<DeviceOnboardingResponse>& uptr_deviceOnboardingResponse){

    auto request = std::make_unique<QNetworkRequest>();
    request->setRawHeader("Content-Type", "application/json");

    request->setUrl(QUrl(QString(deviceRegistryBaseUrl.data()) + "/onboarding/name"));

    std::unique_ptr<QNetworkReply> reply;
    auto payload = std::make_shared<QByteArray>(
                QJsonDocument(deviceOnboardingRequest.toJsonObject()).toJson(QJsonDocument::Compact));

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::PostOperation, request, reply, payload);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientDeviceAdapter::registerOnboardingDeviceWithName SUCCESS";
        if(reply){
            QByteArray responseBody = reply->readAll();
            uptr_deviceOnboardingResponse = std::make_unique<DeviceOnboardingResponse>(
                        DeviceOnboardingResponse::fromJsonObject(QJsonDocument::fromJson(responseBody).object()));
            qDebug() << "QArrowheadHttpClientDeviceAdapter::registerOnboardingDeviceWithName reply: " << responseBody;
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientDeviceAdapter::registerOnboardingDeviceWithName FAILURE";
    }

    return retVal;
}

ReturnValue QArrowheadHttpClientDeviceAdapter::registerOnboardingDeviceWithCsr(
        const DeviceOnboardingWithCsrRequest& deviceOnboardingRequest,
        std::unique_ptr<DeviceOnboardingResponse>& uptr_deviceOnboardingResponse){

    auto request = std::make_unique<QNetworkRequest>();
    request->setRawHeader("Content-Type", "application/json");

    request->setUrl(QUrl(QString(deviceRegistryBaseUrl.data()) + "/onboarding/csr"));

    std::unique_ptr<QNetworkReply> reply;
    auto payload = std::make_shared<QByteArray>(
                QJsonDocument(deviceOnboardingRequest.toJsonObject()).toJson(QJsonDocument::Compact));

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::PostOperation, request, reply, payload);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientDeviceAdapter::registerOnboardingDeviceWithCsr SUCCESS";
        if(reply){
            QByteArray responseBody = reply->readAll();
            uptr_deviceOnboardingResponse = std::make_unique<DeviceOnboardingResponse>(
                        DeviceOnboardingResponse::fromJsonObject(QJsonDocument::fromJson(responseBody).object()));
            qDebug() << "QArrowheadHttpClientDeviceAdapter::registerOnboardingDeviceWithCsr reply: " << responseBody;
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientDeviceAdapter::registerOnboardingDeviceWithCsr FAILURE";
    }

    return retVal;
}

ReturnValue QArrowheadHttpClientDeviceAdapter::unregisterDevice(
        const DeviceRegistryEntry& deviceRegistryEntry){

    auto request = std::make_unique<QNetworkRequest>();

    QUrl url = QUrl(QString(deviceRegistryBaseUrl.data()) + "/unregister");

    QUrlQuery query;
    query.addQueryItem("device_name", deviceRegistryEntry.device.deviceName.data());
    query.addQueryItem("mac_address", deviceRegistryEntry.device.macAddress.data());
    url.setQuery(query);

    request->setUrl(url);

    std::unique_ptr<QNetworkReply> reply;

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::DeleteOperation, request, reply);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientDeviceAdapter::unregisterDevice SUCCESS";
        if(reply){
            QByteArray responseBody = reply->readAll();
            qDebug() << "QArrowheadHttpClientDeviceAdapter::unregisterDevice reply: " << responseBody;
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientDeviceAdapter::unregisterDevice FAILURE";
    }

    return retVal;
}


//System Registry Interface
ReturnValue QArrowheadHttpClientDeviceAdapter::getSystemRegistryEcho(){
    auto request = std::make_unique<QNetworkRequest>(QUrl(QString(systemRegistryBaseUrl.data()) + "/echo"));

    std::unique_ptr<QNetworkReply> reply;

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::GetOperation, request, reply);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientDeviceAdapter::getSystemRegistryEcho SUCCESS";
        if(reply){
            qDebug() << "QArrowheadHttpClientDeviceAdapter::getSystemRegistryEcho reply: " << reply->readAll();
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientDeviceAdapter::getSystemRegistryEcho FAILURE";
    }

    return retVal;
}

ReturnValue QArrowheadHttpClientDeviceAdapter::querySystem(
        const SystemQueryForm& systemQueryForm,
        std::unique_ptr<SystemQueryList>& uptr_systemQueryList){
    auto request = std::make_unique<QNetworkRequest>();
    request->setRawHeader("Content-Type", "application/json");

    request->setUrl(QUrl(QString(systemRegistryBaseUrl.data()) + "/query"));

    std::unique_ptr<QNetworkReply> reply;
    auto payload = std::make_shared<QByteArray>(
                QJsonDocument(systemQueryForm.toJsonObject()).toJson(QJsonDocument::Compact));

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::PostOperation, request, reply, payload);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientDeviceAdapter::querySystem SUCCESS";
        if(reply){
            QByteArray responseBody = reply->readAll();
            uptr_systemQueryList = std::make_unique<SystemQueryList>(
                        SystemQueryList::fromJsonObject(QJsonDocument::fromJson(responseBody).object()));
            qDebug() << "QArrowheadHttpClientDeviceAdapter::querySystem reply: " << responseBody;
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientDeviceAdapter::querySystem FAILURE";
    }

    return retVal;
}

ReturnValue QArrowheadHttpClientDeviceAdapter::registerSystem(
        const SystemRegistrationForm& systemRegistrationForm,
        std::unique_ptr<SystemRegistryEntry>& uptr_systemRegistryEntry){

    auto request = std::make_unique<QNetworkRequest>();
    request->setRawHeader("Content-Type", "application/json");

    request->setUrl(QUrl(QString(systemRegistryBaseUrl.data()) + "/register"));

    std::unique_ptr<QNetworkReply> reply;
    auto payload = std::make_shared<QByteArray>(
                QJsonDocument(systemRegistrationForm.toJsonObject()).toJson(QJsonDocument::Compact));

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::PostOperation, request, reply, payload);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientDeviceAdapter::registerSystem SUCCESS";
        if(reply){
            QByteArray responseBody = reply->readAll();
            uptr_systemRegistryEntry = std::make_unique<SystemRegistryEntry>(
                        SystemRegistryEntry::fromJsonObject(QJsonDocument::fromJson(responseBody).object()));
            qDebug() << "QArrowheadHttpClientDeviceAdapter::registerSystem reply: " << responseBody;
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientDeviceAdapter::registerSystem FAILURE";
    }

    return retVal;
}

ReturnValue QArrowheadHttpClientDeviceAdapter::registerOnboardingSystemWithName(
        const SystemOnboardingWithNameRequest& systemOnboardingRequest,
        std::unique_ptr<SystemOnboardingResponse>& uptr_systemOnboardingResponse){

    auto request = std::make_unique<QNetworkRequest>();
    request->setRawHeader("Content-Type", "application/json");

    request->setUrl(QUrl(QString(systemRegistryBaseUrl.data()) + "/onboarding/name"));

    std::unique_ptr<QNetworkReply> reply;
    auto payload = std::make_shared<QByteArray>(
                QJsonDocument(systemOnboardingRequest.toJsonObject()).toJson(QJsonDocument::Compact));

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::PostOperation, request, reply, payload);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientDeviceAdapter::registerOnboardingSystemWithName SUCCESS";
        if(reply){
            QByteArray responseBody = reply->readAll();
            uptr_systemOnboardingResponse = std::make_unique<SystemOnboardingResponse>(
                        SystemOnboardingResponse::fromJsonObject(QJsonDocument::fromJson(responseBody).object()));
            qDebug() << "QArrowheadHttpClientDeviceAdapter::registerOnboardingSystemWithName reply: " << responseBody;
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientDeviceAdapter::registerOnboardingSystemWithName FAILURE";
    }

    return retVal;
}

ReturnValue QArrowheadHttpClientDeviceAdapter::registerOnboardingSystemWithCsr(
        const SystemOnboardingWithCsrRequest& systemOnboardingRequest,
        std::unique_ptr<SystemOnboardingResponse>& uptr_systemOnboardingResponse){

    auto request = std::make_unique<QNetworkRequest>();
    request->setRawHeader("Content-Type", "application/json");

    request->setUrl(QUrl(QString(systemRegistryBaseUrl.data()) + "/onboarding/csr"));

    std::unique_ptr<QNetworkReply> reply;
    auto payload = std::make_shared<QByteArray>(
                QJsonDocument(systemOnboardingRequest.toJsonObject()).toJson(QJsonDocument::Compact));

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::PostOperation, request, reply, payload);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientDeviceAdapter::registerOnboardingSystemWithCsr SUCCESS";
        if(reply){
            QByteArray responseBody = reply->readAll();
            uptr_systemOnboardingResponse = std::make_unique<SystemOnboardingResponse>(
                        SystemOnboardingResponse::fromJsonObject(QJsonDocument::fromJson(responseBody).object()));
            qDebug() << "QArrowheadHttpClientDeviceAdapter::registerOnboardingSystemWithCsr reply: " << responseBody;
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientDeviceAdapter::registerOnboardingSystemWithCsr FAILURE";
    }

    return retVal;
}

ReturnValue QArrowheadHttpClientDeviceAdapter::unregisterSystem(
        const SystemRegistryEntry& systemRegistryEntry){

    auto request = std::make_unique<QNetworkRequest>();

    QUrl url = QUrl(QString(systemRegistryBaseUrl.data()) + "/unregister");

    QUrlQuery query;
    query.addQueryItem("system_name", systemRegistryEntry.system.systemName.data());
    query.addQueryItem("address", systemRegistryEntry.system.address.data());
    query.addQueryItem("port", std::to_string(systemRegistryEntry.system.port).data());
    url.setQuery(query);

    request->setUrl(url);

    std::unique_ptr<QNetworkReply> reply;

    ReturnValue retVal = httpClient.performHttpOperation(QNetworkAccessManager::Operation::DeleteOperation, request, reply);

    if(retVal == ReturnValue::Ok){
        qDebug() << "QArrowheadHttpClientDeviceAdapter::unregisterSystem SUCCESS";
        if(reply){
            QByteArray responseBody = reply->readAll();
            qDebug() << "QArrowheadHttpClientDeviceAdapter::unregisterSystem reply: " << responseBody;
        }
    }
    else{
        qWarning() << "QArrowheadHttpClientDeviceAdapter::unregisterSystem FAILURE";
    }

    return retVal;
}



}
