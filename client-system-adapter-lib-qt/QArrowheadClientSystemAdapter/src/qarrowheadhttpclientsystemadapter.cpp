#include "qarrowheadhttpclientsystemadapter.h"
#include <QFile>
#include <QSslKey>
#include <QEventLoop>
#include <QJsonDocument>
#include <QUrlQuery>

namespace arrowhead {

ReturnValue QArrowheadHttpClientSystemAdapter::setSslConfig(std::string p12FileName, std::string password, bool disableHostnameVerification){
    ReturnValue retVal = ReturnValue::Ok;

    QFile p12File (p12FileName.data());
    if(p12File.open(QFile::ReadOnly)){
        qDebug() << "QArrowheadHttpClientSystemAdapter p12FileName=" << p12File.fileName();
        QSslKey key;
        QSslCertificate cert;
        QList<QSslCertificate> caCertificates;
        QSslCertificate::importPkcs12(&p12File, &key, &cert, &caCertificates, password.data());
        p12File.close();
        if(!key.isNull()){
            sslConfig.setPrivateKey(key);
        }
        else{
            qCritical() << "QArrowheadHttpClientSystemAdapter SSL private key is NULL.";
            retVal = ReturnValue::InvalidValue;
        }
        if(!caCertificates.isEmpty()){
            sslConfig.setCaCertificates(caCertificates);
        }
        else{
            qCritical() << "QArrowheadHttpClientSystemAdapter: SSL CA certs is empty.";
            retVal = ReturnValue::InvalidValue;
        }
        if(!cert.isNull()){
            clientSystem->systemName = cert.subjectDisplayName().toStdString();
            clientSystem->authenticationInfo = cert.publicKey().toPem(password.data()).toStdString();
            sslConfig.setLocalCertificate(cert);
        }
        else{
            qCritical() << "QArrowheadHttpClientSystemAdapter SSL Local cert is NULL.";
            retVal = ReturnValue::InvalidValue;
        }
    }
    else{
        qCritical() << "QArrowheadHttpClientSystemAdapter: Failed to open CA Cert File.";
        retVal = ReturnValue::UnknownError;
    }

    qDebug() << "QArrowheadHttpClientSystemAdapter: caCerts=" << sslConfig.caCertificates() << "size=" << sslConfig.caCertificates().size();

    this->disableHostnameVerification = disableHostnameVerification;
    qDebug() << "QArrowheadHttpClientSystemAdapter: disableHostnameVerification=" << disableHostnameVerification;
    if(disableHostnameVerification){
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    }

    return retVal;
}

ReturnValue QArrowheadHttpClientSystemAdapter::performHttpOperation(QNetworkAccessManager::Operation operationType,
        std::unique_ptr<QNetworkRequest> &request, std::unique_ptr<QNetworkReply> &reply,
        std::shared_ptr<const QByteArray> payload){

    ReturnValue retVal;

    qDebug() << "QArrowheadHttpClientSystemAdapter::performHttpOperation: "
                "performing HTTP operation <" << typeToString(operationType) << "> "
                "to the URL <" << request->url() << ">";
    if(payload){
        qDebug() << " with payload <" << *payload << ">";
    }

    if(!sslConfig.isNull()){
        QObject::connect(&qnam, &QNetworkAccessManager::sslErrors,
                         [] (QNetworkReply *reply, const QList<QSslError> &errors) {
            qDebug() << "QArrowheadHttpClientSystemAdapter: errors=" << errors.toStdList();
            for(int i=0; i< errors.size(); i++){
                qDebug() << "QArrowheadHttpClientSystemAdapter: errors[" << i << "]= " << errors.at(i).error() << " cert=" << errors.at(i).certificate();
            }
        });
        request->setSslConfiguration(sslConfig);
    }

    QEventLoop eventLoop;
    QObject::connect(&qnam, &QNetworkAccessManager::finished, &eventLoop,
                     &QEventLoop::quit);

    switch(operationType){
    case QNetworkAccessManager::Operation::GetOperation:
        reply.reset(qnam.get(*request));
        break;
    case QNetworkAccessManager::Operation::DeleteOperation:
        reply.reset(qnam.deleteResource(*request));
        break;
    case QNetworkAccessManager::Operation::PutOperation:
        if(payload->isEmpty()){
            qCritical() << "QArrowheadHttpClientSystemAdapter::performHttpOperation payload is empty";
            return ReturnValue::InvalidValue;
        }
        reply.reset(qnam.put(*request, *payload));
        break;
    case QNetworkAccessManager::Operation::PostOperation:
        if(payload->isEmpty()){
            qCritical() << "QArrowheadHttpClientSystemAdapter::performHttpOperation payload is empty";
            return ReturnValue::InvalidValue;
        }
        reply.reset(qnam.post(*request, *payload));
        break;
    default:
        qWarning() << "QArrowheadHttpClientSystemAdapter::performHttpOperation HTTP operation <"
                   << operationType << "> unimplemented.";
        return ReturnValue::Unimplemented;
    }
    eventLoop.exec();

    if(reply != nullptr){
        if(reply->error() == QNetworkReply::NoError){
            retVal = ReturnValue::Ok;
        }
        else{
            qWarning() << "QArrowheadHttpClientSystemAdapter::performHttpOperation ERROR " << reply->errorString();
            for(quint8 i=0; i<reply->rawHeaderList().length(); i++){
                qDebug() << "QArrowheadHttpClientSystemAdapter::performHttpOperation header: "
                         << reply->rawHeaderList().at(i);
            }
            qWarning() << "QArrowheadHttpClientSystemAdapter::performHttpOperation reply " << reply->readAll();
            retVal = ReturnValue::UnknownError;
        }
    }
    else{
        retVal = ReturnValue::UnknownError;
    }

    return retVal;
}

const QString QArrowheadHttpClientSystemAdapter::typeToString(const QNetworkAccessManager::Operation &operationType) const{
    switch(operationType){
    case QNetworkAccessManager::Operation::GetOperation:
        return "GET";
    case QNetworkAccessManager::Operation::PutOperation:
        return "PUT";
    case QNetworkAccessManager::Operation::HeadOperation:
        return "HEAD";
    case QNetworkAccessManager::Operation::PostOperation:
        return "POST";
    case QNetworkAccessManager::Operation::CustomOperation:
        return "CUSTOM";
    case QNetworkAccessManager::Operation::DeleteOperation:
        return "DELETE";
    case QNetworkAccessManager::Operation::UnknownOperation:
        return "UNKNOWN";
    }
    return "";
}

ReturnValue QArrowheadHttpClientSystemAdapter::getServiceRegistryEcho()
{
    auto request = std::make_unique<QNetworkRequest>(QUrl(QString(serviceRegsitryBaseUrl.data()) + "/echo"));
    std::unique_ptr<QNetworkReply> reply;

    ReturnValue retVal = performHttpOperation(QNetworkAccessManager::Operation::GetOperation, request, reply);

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

    ReturnValue retVal = performHttpOperation(QNetworkAccessManager::Operation::PostOperation, request, reply, payload);

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

    ReturnValue retVal = performHttpOperation(QNetworkAccessManager::Operation::DeleteOperation, request, reply);

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

    ReturnValue retVal = performHttpOperation(QNetworkAccessManager::Operation::PostOperation, request, reply, payload);

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

    ReturnValue retVal = performHttpOperation(QNetworkAccessManager::Operation::GetOperation, request, reply);

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

    ReturnValue retVal = performHttpOperation(QNetworkAccessManager::Operation::GetOperation, request, reply);

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

    ReturnValue retVal = performHttpOperation(QNetworkAccessManager::Operation::GetOperation, request, reply);

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

    ReturnValue retVal = performHttpOperation(QNetworkAccessManager::Operation::PostOperation, request, reply, payload);

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

    ReturnValue retVal = performHttpOperation(QNetworkAccessManager::Operation::GetOperation, request, reply);

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

