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

#include "keypair.h"

namespace arrowhead {

QJsonObject KeyPair::toJsonObject() const{
    QJsonObject keyPairJson;
    keyPairJson.insert("keyAlgorithm", keyAlgorithm.data());
    keyPairJson.insert("keyFormat", keyFormat.data());
    keyPairJson.insert("privateKey", privateKey.data());
    keyPairJson.insert("publicKey", publicKey.data());
    return keyPairJson;
}

KeyPair KeyPair::fromJsonObject(const QJsonObject &keyPairJson){
    auto keyAlgorithm = keyPairJson["keyAlgorithm"].toString().toStdString();
    auto keyFormat = keyPairJson["keyFormat"].toString().toStdString();
    auto privateKey = keyPairJson["privateKey"].toString().toStdString();
    auto publicKey = keyPairJson["publicKey"].toString().toStdString();
    return KeyPair(keyAlgorithm, keyFormat, privateKey, publicKey);
}

}
