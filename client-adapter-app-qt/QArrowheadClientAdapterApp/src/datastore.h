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
#ifndef DATASTORE_H
#define DATASTORE_H

#include "record.h"
#include "enums/deviceparam.h"
#include "enums/orchestrationparam.h"
#include "enums/orchestrationrequestparam.h"
#include "enums/serviceparam.h"
#include "enums/settingsparam.h"
#include "enums/systemparam.h"

#include <vector>
#include <string>
#include <QFile>


class DataStore
{
    /**
     * etc
     *   settings.json
     *   devices
     *      MyDevice1
     *      MyDevice2
     *          device_certificate.crt
     *          onboarding_certificate.crt
     *          parameters.json
     *          systems
     *              MySystem1
     *                  system_certificate.crt
     *                  parameters.json
     *                  services
     *                      MyService1
     *                          parameters.json
     *                      MyService2
     *                          parameters.json
     *                  orchestrations
     *                      MyOrchestration1
     *                          parameters.json
     *              MySystem2
     */


    static constexpr const char* settingsparam_filename = "etc/settings.json";
    static constexpr const char* params_filename = "parameters.json";
    static constexpr const char* devicecert_filename = "device_certificate.crt";
    static constexpr const char* device_private_key_filename = "device_private_key.pem";
    static constexpr const char* device_public_key_filename = "device_public_key.pem";
    static constexpr const char* onboardingcert_filename = "onboarding_certificate.crt";
    static constexpr const char* onboarding_private_key_filename = "onboarding_private_key.pem";
    static constexpr const char* onboarding_public_key_filename = "onboarding_public_key.pem";
    static constexpr const char* systemcert_filename = "system_certificate.crt";
    static constexpr const char* system_private_key_filename = "system_private_key.pem";
    static constexpr const char* system_public_key_filename = "system_public_key.pem";

    std::vector<std::string> getFilteredSubfolderNames(const std::string& path, const std::string& filter_regex);

    std::string getDevicePath(const std::string& device);
    std::string getSystemPath(const std::string& device, const std::string& system);
    std::string getServicePath(const std::string& device, const std::string& system, const std::string& service);
    std::string getOrchestrationPath(const std::string& device, const std::string& system, const std::string& orchestration);

    QByteArray readFile(const QString& path);
    void writeFile(const QString& path, const QByteArray& data);

    template<typename T>
    Record<T> openRecord(const std::string& path);


    template<typename T>
    void saveRecord(Record<T> record, const std::string& path);

public:

    bool isValid(const std::string& device, const std::string& system = "", const std::string& service = "");


    Record<SettingsParam> getSettings();
    void setSettings(const Record<SettingsParam>& settingsToSet);

    void saveOnboardingCertificate(const std::string& certificate, const std::string& device);
    void saveOnboardingPrivateKey(const std::string& privateKey, const std::string& device);
    void saveOnboardingPublicKey(const std::string& publicKey, const std::string& device);
    void saveDeviceCertificate(const std::string& certificate, const std::string& device);
    void saveDevicePrivateKey(const std::string& privateKey, const std::string& device);
    void saveDevicePublicKey(const std::string& publicKey, const std::string& device);
    void saveSystemCertificate(const std::string& certificate, const std::string& device, const std::string& system);
    void saveSystemPrivateKey(const std::string& privateKey, const std::string& device, const std::string& system);
    void saveSystemPublicKey(const std::string& publicKey, const std::string& device, const std::string& system);

    std::string getOnboardingCertificate(const std::string& device);
    std::string getOnboardingCertificateFile(const std::string& device);
    std::string getOnboardingPrivateKeyFile(const std::string& device);
    std::string getOnboardingPublicKeyFile(const std::string& device);
    std::string getDeviceCertificate(const std::string& device);
    std::string getDeviceCertificateFile(const std::string& device);
    std::string getDevicePrivateKeyFile(const std::string& device);
    std::string getDevicePublicKeyFile(const std::string& device);
    std::string getSystemCertificate(const std::string& device, const std::string& system);
    std::string getSystemCertificateFile(const std::string& device, const std::string& system);
    std::string getSystemPrivateKeyFile(const std::string& device, const std::string& system);
    std::string getSystemPublicKeyFile(const std::string& device, const std::string& system);

    std::string getPublicKey();
    std::string getPrivateKey();

    void saveDevice(const Record<DeviceParam>& deviceToSave);
    void saveSystem(const Record<SystemParam>& systemToSave, const std::string& device);
    void saveService(const Record<ServiceParam>& serviceToSave, const std::string& device, const std::string& system);
    void saveOrchestration(const Record<OrchestrationParam>& orchestrationToSave, const std::string& device, const std::string& system);

    void removeDevice(const std::string& name);
    void removeSystem(const std::string& name, const std::string& device);
    void removeService(const std::string& name, const std::string& device, const std::string& system);

    std::vector<Record<DeviceParam>> getDevices(const std::vector<DeviceParam>& parameters, const std::string& filter_regex);
    std::vector<Record<SystemParam>> getSystems(const std::vector<SystemParam>& parameters, const std::string& filter_regex, const std::string& device);
    std::vector<Record<ServiceParam>> getServices(const std::vector<ServiceParam>& parameters, const std::string& filter_regex, const std::string& device, const std::string& system);

    std::vector<Record<OrchestrationParam>> getOrchestrations(const std::vector<OrchestrationParam>& parameters, const std::string& filter_regex, const std::string& device, const std::string& system);


};

#endif // DATASTORE_H
