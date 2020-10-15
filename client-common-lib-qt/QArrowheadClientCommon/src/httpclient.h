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

#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include "qarrowheadclientcommon_global.h"
#include "common.h"
#include <memory>
#include <QNetworkAccessManager>
#include <QSslKey>

namespace arrowhead {

class QARROWHEADCLIENTCOMMON_EXPORT HttpClient
{
public:
    enum class AuthenticationMethod{
        Invalid,
        BasicAuthentication,
        ClientCertificate
    };

    static AuthenticationMethod authMethodFromString(std::string &typeString);
    static const std::string typeToString(const AuthenticationMethod &type);

    void handleSSLErrors();

    ReturnValue setSslConfig(
            std::string p12FileName,
            std::string password,
            bool disableHostnameVerification=false);
    ReturnValue setSslConfig(
            std::string caCertFileName,
            bool disableHostnameVerification=false);
    ReturnValue setSslConfig(
            std::string caCertFileName,
            std::string certFileName,
            std::string keyFileName,
            std::string password,
            bool disableHostnameVerification=false);
    ReturnValue setLocalCert(std::string certificate, std::string privateKey, bool fromFile=true);
    ReturnValue setSharedSecret(std::string sharedSecret);
    ReturnValue performHttpOperation(QNetworkAccessManager::Operation operationType,
                                     std::unique_ptr<QNetworkRequest> &request, std::unique_ptr<QNetworkReply> &reply,
                                     std::shared_ptr<const QByteArray> payload = nullptr);
    const QString typeToString(const QNetworkAccessManager::Operation &operationType) const;
    AuthenticationMethod getAuthMethod() const {
        return authMethod;
    }
    QSslKey getKey() const {
        return key;
    }
    QSslCertificate getCertificate() const {
        return cert;
    }
    std::string getSharedSecret() const {
        return sharedSecret;
    }


private:
    QNetworkAccessManager qnam;
    QSslConfiguration sslConfig;
    std::string sharedSecret;
    QSslKey key;
    QSslCertificate cert;
    QList<QSslCertificate> caCertificates;
    bool disableHostnameVerification=false;
    AuthenticationMethod authMethod;
};
}


#endif // HTTPCLIENT_H
