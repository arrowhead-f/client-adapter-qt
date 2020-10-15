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

#include "arrowheadserverexception.h"
#include "httpclient.h"
#include <QFile>
#include <QSslKey>
#include <QEventLoop>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QJsonDocument>

namespace arrowhead {

HttpClient::AuthenticationMethod HttpClient::authMethodFromString(std::string &typeString){
    if(typeString == "basic authentication"){
        return AuthenticationMethod::BasicAuthentication;
    }
    else if(typeString == "client certificate"){
        return AuthenticationMethod::ClientCertificate;
    }
    else {
        return AuthenticationMethod::Invalid;
    }
}

const std::string HttpClient::typeToString(const AuthenticationMethod &type){
    switch(type){
    case HttpClient::AuthenticationMethod::BasicAuthentication:
        return "basic authentication";
    case HttpClient::AuthenticationMethod::ClientCertificate:
        return "client certificate";
    default:
        return "";
    }
}

void HttpClient::handleSSLErrors() {

    QObject::connect(&qnam, &QNetworkAccessManager::sslErrors,
                     [] (QNetworkReply *reply, const QList<QSslError> &errors) {
        qDebug() << "HttpClient: errors=" << errors.toStdList();
        for(int i=0; i< errors.size(); i++){
            qDebug() << "HttpClient: errors[" << i << "]= " << errors.at(i).error() << " cert=" << errors.at(i).certificate();
        }
    });
}

ReturnValue HttpClient::setSslConfig(std::string p12FileName, std::string password, bool disableHostnameVerification){
    // Local cert provided, this implies authentication via client certificate
    authMethod = AuthenticationMethod::ClientCertificate;

    QFile p12File (p12FileName.data());
    if(p12File.open(QFile::ReadOnly)){
        qDebug() << "HttpClient p12FileName=" << p12File.fileName();
        QSslCertificate::importPkcs12(&p12File, &key, &cert, &caCertificates, password.data());
        p12File.close();
        if(!key.isNull()){
            sslConfig.setPrivateKey(key);
        }
        else{
            qCritical() << "HttpClient SSL private key is NULL.";
            return ReturnValue::InvalidValue;
        }
        if(!caCertificates.isEmpty()){
            sslConfig.setCaCertificates(caCertificates);
        }
        else{
            qCritical() << "HttpClient: SSL CA certs is empty.";
            return ReturnValue::InvalidValue;
        }
        if(!cert.isNull()){
            sslConfig.setLocalCertificate(cert);
        }
        else{
            qCritical() << "HttpClient SSL Local cert is NULL.";
            return ReturnValue::InvalidValue;
        }
    }
    else{
        qCritical() << "HttpClient: Failed to open CA Cert File.";
        return ReturnValue::UnknownError;
    }

    qDebug() << "HttpClient: caCerts=" << sslConfig.caCertificates() << "size=" << sslConfig.caCertificates().size();

    this->disableHostnameVerification = disableHostnameVerification;
    qDebug() << "HttpClient: disableHostnameVerification=" << disableHostnameVerification;
    if(disableHostnameVerification){
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    }

    handleSSLErrors();

    return ReturnValue::Ok;
}

ReturnValue HttpClient::setSslConfig(std::string caCertFileName, bool disableHostnameVerification){
    // Local cert not provided, this implies basic authentication
    authMethod = AuthenticationMethod::BasicAuthentication;

    QFile caCertFile (caCertFileName.data());
    if(caCertFile.open(QFile::ReadOnly)){
        qDebug() << "HttpClient caCertFile=" << caCertFile.fileName() << " opened successfully.";
        caCertificates = QSslCertificate::fromDevice(&caCertFile);
        caCertFile.close();
        if(!caCertificates.isEmpty()){
            sslConfig.setCaCertificates(caCertificates);
        }
        else{
            qCritical() << "HttpClient: SSL CA certs is empty.";
            return ReturnValue::InvalidValue;
        }
    }
    else{
        qCritical() << "HttpClient: Failed to open CA Cert File: " << caCertFileName.data();
        return ReturnValue::UnknownError;
    }

    qDebug() << "HttpClient: caCerts=" << sslConfig.caCertificates() << "size=" << sslConfig.caCertificates().size();

    this->disableHostnameVerification = disableHostnameVerification;
    qDebug() << "HttpClient: disableHostnameVerification=" << disableHostnameVerification;
    if(disableHostnameVerification){
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    }



    QObject::connect(&qnam, &QNetworkAccessManager::authenticationRequired,
                     [&] (QNetworkReply *reply, QAuthenticator *authenticator){
        qDebug() << "HttpClient::performHttpOperation: " <<
                    "authentication required at: " << reply->url();
        if(reply->url().path().contains("onboarding")){
            qDebug() << "HttpClient::performHttpOperation: " <<
                        "setting credentials: (sharedsecret, " << sharedSecret.data() << ")";
            authenticator->setUser("sharedsecret");
            authenticator->setPassword(sharedSecret.data());
        }
    });

    handleSSLErrors();

    return ReturnValue::Ok;
}

ReturnValue HttpClient::setSslConfig(
        std::string caCertFileName, std::string certFileName, std::string keyFileName,
        std::string password, bool disableHostnameVerification){
    // Local cert provided, this implies authentication via client certificate
    authMethod = AuthenticationMethod::ClientCertificate;

    QFile certFile (certFileName.data());
    if(certFile.open(QFile::ReadOnly)){
        qDebug() << "HttpClient certFile=" << certFile.fileName() << " opened successfully.";
        cert = QSslCertificate(&certFile);
        certFile.close();
        if(!cert.isNull()){
            sslConfig.setLocalCertificate(cert);
        }
        else{
            qCritical() << "HttpClient SSL Local cert is NULL.";
            return ReturnValue::InvalidValue;
        }
    }
    else{
        qCritical() << "HttpClient: Failed to open Cert File.";
        return ReturnValue::UnknownError;
    }

    QFile keyFile(keyFileName.data());
    if(keyFile.open(QFile::ReadOnly)){
        qDebug() << "HttpClient keyFile=" << keyFile.fileName() << " opened successfully.";
        key = QSslKey(&keyFile, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, password.data());
        keyFile.close();
        if(!key.isNull()){
            sslConfig.setPrivateKey(key);
        }
        else{
            qCritical() << "HttpClient SSL private key is NULL.";
            return ReturnValue::InvalidValue;
        }
    }
    else{
        qCritical() << "HttpClient: Failed to open Key File.";
        return ReturnValue::UnknownError;
    }

    return setSslConfig(caCertFileName, disableHostnameVerification);
}

ReturnValue HttpClient::setLocalCert(std::string certificate, std::string privateKey, bool fromFile) {
    if(fromFile){
        QFile certFile (certificate.data());
        if(certFile.open(QFile::ReadOnly)){
            qDebug() << "HttpClient certFile=" << certFile.fileName() << " opened successfully.";
            cert = QSslCertificate(&certFile);
            certFile.close();
            if(!cert.isNull()){
                sslConfig.setLocalCertificate(cert);
            }
            else{
                qCritical() << "HttpClient SSL Local cert is NULL.";
                return ReturnValue::InvalidValue;
            }
        }
        else{
            qCritical() << "HttpClient: Failed to open Cert File.";
            return ReturnValue::UnknownError;
        }
        QFile keyFile(privateKey.data());
        if(keyFile.open(QFile::ReadOnly)){
            qDebug() << "HttpClient keyFile=" << keyFile.fileName() << " opened successfully.";
            key = QSslKey(&keyFile, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey);
            keyFile.close();
            if(!key.isNull()){
                sslConfig.setPrivateKey(key);
            }
            else{
                qCritical() << "HttpClient SSL private key is NULL.";
                return ReturnValue::InvalidValue;
            }
        }
        else{
            qCritical() << "HttpClient: Failed to open Key File.";
            return ReturnValue::UnknownError;
        }
    }
    else{
        sslConfig.setLocalCertificate(QSslCertificate(QByteArray(certificate.data())));
        sslConfig.setPrivateKey(QSslKey(QByteArray(privateKey.data()), QSsl::Rsa));
    }

    qnam.clearAccessCache();

    return ReturnValue::Ok;
}

ReturnValue HttpClient::setSharedSecret(std::string sharedSecret){
    if(authMethod != AuthenticationMethod::BasicAuthentication){
        qWarning() << "HttpClient::setSharedSecret: " <<
                      "authentication method is: " << typeToString(authMethod).data() <<
                      " and not basic authentication. Shared secret discarded.";
        return ReturnValue::InvalidValue;
    }
    qDebug() << "HttpClient::setSharedSecret: " <<
                "setting shared secret to: " << sharedSecret.data();
    this->sharedSecret = sharedSecret;
    return ReturnValue::Ok;
}

ReturnValue HttpClient::performHttpOperation(QNetworkAccessManager::Operation operationType,
        std::unique_ptr<QNetworkRequest> &request, std::unique_ptr<QNetworkReply> &reply,
        std::shared_ptr<const QByteArray> payload){

    ReturnValue retVal;

    qDebug() << "HttpClient::performHttpOperation: "
                "performing HTTP operation <" << typeToString(operationType) << "> "
                "to the URL <" << request->url() << ">";
    if(payload){
        qDebug() << " with payload <" << *payload << ">";
    }

    if(authMethod == AuthenticationMethod::BasicAuthentication){
        auto basicAuthPwd = ("sharedSecret:" + QString(sharedSecret.data())).toUtf8().toBase64();
        qDebug() << "HttpClient::performHttpOperation: " <<
                    "setting header: " << "(Authorization, " <<
                    "Basic " + basicAuthPwd << ")";
        request->setRawHeader("Authorization", "Basic " + basicAuthPwd);
    }


    if(!sslConfig.isNull()){
        request->setSslConfiguration(sslConfig);
    }

    qDebug() << "HttpClient performOperation request cert: " << request->sslConfiguration().localCertificate();

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
            qCritical() << "HttpClient::performHttpOperation payload is empty";
            return ReturnValue::InvalidValue;
        }
        reply.reset(qnam.put(*request, *payload));
        break;
    case QNetworkAccessManager::Operation::PostOperation:
        if(payload->isEmpty()){
            qCritical() << "HttpClient::performHttpOperation payload is empty";
            return ReturnValue::InvalidValue;
        }
        reply.reset(qnam.post(*request, *payload));
        break;
    default:
        qWarning() << "HttpClient::performHttpOperation HTTP operation <"
                   << operationType << "> unimplemented.";
        return ReturnValue::Unimplemented;
    }
    eventLoop.exec();

    if(reply != nullptr){
        if(reply->error() == QNetworkReply::NoError){
            retVal = ReturnValue::Ok;
        }
        else{
            //Create error
            auto responseBody = reply->readAll();

            qWarning() << "HttpClient::performHttpOperation ERROR " << reply->errorString();
            for(quint8 i=0; i<reply->rawHeaderList().length(); i++){
                qDebug() << "HttpClient::performHttpOperation header: "
                         << reply->rawHeaderList().at(i);
            }
            qWarning() << "HttpClient::performHttpOperation reply " << responseBody;


           if(!responseBody.isEmpty())
                throw ArrowheadServerException::fromJsonObject(QJsonDocument::fromJson(responseBody).object());

            retVal = ReturnValue::UnknownError;
        }
    }
    else{
        retVal = ReturnValue::UnknownError;
    }

    return retVal;
}

const QString HttpClient::typeToString(const QNetworkAccessManager::Operation &operationType) const{
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
}
