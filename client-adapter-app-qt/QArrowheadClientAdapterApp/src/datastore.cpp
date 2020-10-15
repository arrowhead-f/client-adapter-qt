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
#include "datastore.h"
#include <iostream>
#include <algorithm>
#include <regex>
#include <QDir>
#include <QRegularExpression>

std::vector<std::string> DataStore::getFilteredSubfolderNames(const std::string& path, const std::string& filter_regex) {
    QDir dir(path.c_str());
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    auto fileList = dir.entryInfoList();


    std::vector<std::string> filteredNames;

    if (!fileList.empty()){
        std::vector<std::string> names;
        std::transform(fileList.begin(), fileList.end(), std::back_inserter(names), [](const QFileInfo& file) { return file.fileName().toStdString(); });

        std::copy_if(names.begin(), names.end(), std::back_inserter(filteredNames), [&](const std::string& name) { std::smatch m; return std::regex_search(name, m, std::regex{filter_regex}); });
    }

    return filteredNames;
}

std::string DataStore::getDevicePath(const std::string& device = "") {
    return std::string("devices/") + device;// + '/';
}

std::string DataStore::getSystemPath(const std::string& device, const std::string& system = "") {
    return getDevicePath(device) + "/systems/" + system;//+ '/';
}

std::string DataStore::getServicePath(const std::string& device, const std::string& system, const std::string& service = "") {
    return getSystemPath(device, system) + "/services/" + service;// + '/';
}

std::string DataStore::getOrchestrationPath(const std::string& device, const std::string& system, const std::string& orchestration = "") {
    return getSystemPath(device, system) + "/orchestrations/" + orchestration;// + '/';
}

QByteArray DataStore::readFile(const QString& path) {
    QFile file(path);

    if(!file.exists()){
        file.open(QIODevice::WriteOnly);
        file.close();
    }

    if (!file.open(QIODevice::ReadOnly))
        throw std::runtime_error("Couldn't open file \'" + path.toStdString() + '\'');

    return file.readAll();
}

void DataStore::writeFile(const QString& path, const QByteArray& data) {
    QFile file(path);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        throw std::runtime_error("Couldn't open file " + path.toStdString());

    file.write(data);
}

template<typename T>
Record<T> DataStore::openRecord(const std::string& path) {


    Record<T> record;
    record.updateFromByteArray(readFile(path.c_str()));

    return record;
}


template<typename T>
void DataStore::saveRecord(Record<T> record, const std::string& path){
    record.updateFromByteArray(readFile(path.c_str()));

    writeFile(path.c_str(), record.toByteArray());
}

bool DataStore::isValid(const std::string& device, const std::string& system, const std::string& service) {
    std::string path;

    if (!service.empty())
        path = getServicePath(device, system, service);
    else if (!system.empty())
        path = getSystemPath(device, system);
    else
        path = getDevicePath(device);

    return QDir(path.c_str()).exists();
}

Record<SettingsParam> DataStore::getSettings(){

    Record<SettingsParam> settings = openRecord<SettingsParam>(settingsparam_filename);

    settings.addField(SettingsParam::onboarding_system_address,     std::string(""));
    settings.addField(SettingsParam::onboarding_system_port,        0);
    settings.addField(SettingsParam::authentication_method,         std::string(""));
    settings.addField(SettingsParam::shared_secret,                 std::string(""));
    settings.addField(SettingsParam::key_store,                     std::string(""));
    settings.addField(SettingsParam::key_store_password,            std::string(""));
    settings.addField(SettingsParam::private_key,                   std::string(""));
    settings.addField(SettingsParam::public_key,                    std::string(""));
    settings.addField(SettingsParam::log_file,                      std::string(""));
    settings.addField(SettingsParam::log_level,                     std::string(""));

    return settings;
}

void DataStore::setSettings(const Record<SettingsParam>& settingsToSet) {
    saveRecord<SettingsParam>(settingsToSet, settingsparam_filename);
}


void DataStore::saveOnboardingCertificate(const std::string& certificate, const std::string &device) {
    QString fqCert = "-----BEGIN CERTIFICATE-----\n" + QString::fromStdString(certificate) + "\n-----END CERTIFICATE-----";
    writeFile(std::string(getDevicePath(device) +'/'+ onboardingcert_filename).c_str(), fqCert.toStdString().c_str());
}

void DataStore::saveOnboardingPrivateKey(const std::string& privateKey, const std::string &device){
    QString fqKey = "-----BEGIN PRIVATE KEY-----\n" + QString::fromStdString(privateKey) + "\n-----END PRIVATE KEY-----";
    writeFile(std::string(getDevicePath(device) +'/'+ onboarding_private_key_filename).c_str(), fqKey.toStdString().c_str());
}

void DataStore::saveOnboardingPublicKey(const std::string& publicKey, const std::string &device){
    QString fqKey = "-----BEGIN PUBLIC KEY-----\n" + QString::fromStdString(publicKey) + "\n-----END PUBLIC KEY-----";
    writeFile(std::string(getDevicePath(device) +'/'+ onboarding_public_key_filename).c_str(), fqKey.toStdString().c_str());
}

void DataStore::saveDeviceCertificate(const std::string& certificate, const std::string& device) {
    QString fqCert = "-----BEGIN CERTIFICATE-----\n" + QString::fromStdString(certificate) + "\n-----END CERTIFICATE-----";
    writeFile(std::string(getDevicePath(device) +'/'+ devicecert_filename).c_str(), fqCert.toStdString().c_str());
}

void DataStore::saveDevicePrivateKey(const std::string& privateKey, const std::string& device){
    QString fqKey = "-----BEGIN PRIVATE KEY-----\n" + QString::fromStdString(privateKey) + "\n-----END PRIVATE KEY-----";
    writeFile(std::string(getDevicePath(device) +'/'+ device_private_key_filename).c_str(), fqKey.toStdString().c_str());
}

void DataStore::saveDevicePublicKey(const std::string& publicKey, const std::string& device){
    QString fqKey = "-----BEGIN PUBLIC KEY-----\n" + QString::fromStdString(publicKey) + "\n-----END PUBLIC KEY-----";
    writeFile(std::string(getDevicePath(device) +'/'+ device_public_key_filename).c_str(), fqKey.toStdString().c_str());
}

void DataStore::saveSystemCertificate(const std::string& certificate, const std::string& device, const std::string& system) {
    QString fqCert = "-----BEGIN CERTIFICATE-----\n" + QString::fromStdString(certificate) + "\n-----END CERTIFICATE-----";
    writeFile(std::string(getSystemPath(device, system) +'/'+ systemcert_filename).c_str(), fqCert.toStdString().c_str());
}

void DataStore::saveSystemPrivateKey(const std::string& privateKey, const std::string& device, const std::string& system){
    QString fqKey = "-----BEGIN PRIVATE KEY-----\n" + QString::fromStdString(privateKey) + "\n-----END PRIVATE KEY-----";
    writeFile(std::string(getSystemPath(device, system) +'/'+ system_private_key_filename).c_str(), fqKey.toStdString().c_str());
}

void DataStore::saveSystemPublicKey(const std::string& publicKey, const std::string& device, const std::string& system){
    QString fqKey = "-----BEGIN PUBLIC KEY-----\n" + QString::fromStdString(publicKey) + "\n-----END PUBLIC KEY-----";
    writeFile(std::string(getSystemPath(device, system) +'/'+ system_public_key_filename).c_str(), fqKey.toStdString().c_str());
}


std::string DataStore::getOnboardingCertificate(const std::string& device) {
    return readFile(std::string(getDevicePath(device) +'/'+ onboardingcert_filename).c_str()).toStdString();
}

std::string DataStore::getOnboardingCertificateFile(const std::string& device) {
    return std::string(getDevicePath(device) +'/'+ onboardingcert_filename);
}

std::string DataStore::getOnboardingPrivateKeyFile(const std::string& device){
    return std::string(getDevicePath(device) +'/'+ onboarding_private_key_filename);
}
std::string DataStore::getOnboardingPublicKeyFile(const std::string& device){
    return std::string(getDevicePath(device) +'/'+ onboarding_public_key_filename);
}

std::string DataStore::getDeviceCertificate(const std::string& device) {
    return readFile(std::string(getDevicePath(device) +'/'+ devicecert_filename).c_str()).toStdString();
}

std::string DataStore::getDeviceCertificateFile(const std::string& device) {
    return std::string(getDevicePath(device) +'/'+ devicecert_filename);
}

std::string DataStore::getDevicePrivateKeyFile(const std::string& device){
    return std::string(getDevicePath(device) +'/'+ device_private_key_filename);
}
std::string DataStore::getDevicePublicKeyFile(const std::string& device){
    return std::string(getDevicePath(device) +'/'+ device_public_key_filename);
}

std::string DataStore::getSystemCertificate(const std::string& device, const std::string& system) {
    return readFile(std::string(getSystemPath(device, system) +'/'+ systemcert_filename).c_str()).toStdString();
}

std::string DataStore::getSystemCertificateFile(const std::string& device, const std::string& system) {
    return std::string(getSystemPath(device, system) +'/'+ systemcert_filename);
}

std::string DataStore::getSystemPrivateKeyFile(const std::string& device, const std::string& system) {
    return std::string(getSystemPath(device, system) +'/'+ system_private_key_filename);
}

std::string DataStore::getSystemPublicKeyFile(const std::string& device, const std::string& system) {
    return std::string(getSystemPath(device, system) +'/'+ system_public_key_filename);
}


std::string DataStore::getPublicKey() {
    Record<SettingsParam> settingsRecord = getSettings();
    return QString(readFile(settingsRecord.getString(SettingsParam::public_key).c_str())).remove(QRegularExpression(".*-{5}\n*"))
                   .remove(QRegularExpression("[\\n\\t\\r]")).toStdString();
}

std::string DataStore::getPrivateKey() {
    Record<SettingsParam> settingsRecord = getSettings();
    return QString(readFile(settingsRecord.getString(SettingsParam::private_key).c_str())).remove(QRegularExpression(".*-{5}\n*"))
                   .remove(QRegularExpression("[\\n\\t\\r]")).toStdString();
}


void DataStore::saveDevice(const Record<DeviceParam>& deviceToSave) {
    std::string dir = getDevicePath(deviceToSave.getString(DeviceParam::name));
    QDir().mkpath(dir.c_str());
    saveRecord<DeviceParam>(deviceToSave, dir + '/' + params_filename);
}

void DataStore::saveSystem(const Record<SystemParam>& systemToSave, const std::string& device) {
    std::string dir = getSystemPath(device, systemToSave.getString(SystemParam::name));
    QDir().mkpath(dir.c_str());
    saveRecord<SystemParam>(systemToSave, dir + '/' + params_filename);
}

void DataStore::saveService(const Record<ServiceParam>& serviceToSave, const std::string& device, const std::string& system) {
    std::string dir = getServicePath(device, system, serviceToSave.getString(ServiceParam::name));
    QDir().mkpath(dir.c_str());
    saveRecord<ServiceParam>(serviceToSave, dir + '/' + params_filename);
}

void DataStore::saveOrchestration(const Record<OrchestrationParam>& orchestrationToSave, const std::string& device, const std::string& system) {
    std::string dir = getOrchestrationPath(device, system, orchestrationToSave.getString(OrchestrationParam::name));
    QDir().mkpath(dir.c_str());
    saveRecord<OrchestrationParam>(orchestrationToSave, dir + '/' + params_filename);

}


void DataStore::removeDevice(const std::string& name) {
    QDir(getDevicePath(name).c_str()).removeRecursively();
}

void DataStore::removeSystem(const std::string& name, const std::string& device) {
    QDir(getSystemPath(device, name).c_str()).removeRecursively();
}

void DataStore::removeService(const std::string& name, const std::string& device, const std::string& system) {
    QDir(getServicePath(device, system, name).c_str()).removeRecursively();

}


std::vector<Record<DeviceParam>> DataStore::getDevices(const std::vector<DeviceParam>& parameters, const std::string& filter_regex) {
    std::vector<std::string> deviceNames = getFilteredSubfolderNames(getDevicePath(), filter_regex);
    std::vector<Record<DeviceParam>> devices;
    for (const auto& deviceName : deviceNames) {
        Record<DeviceParam> device = openRecord<DeviceParam>(getDevicePath(deviceName) + '/' + params_filename);
        device.projectFields(parameters);
        devices.push_back(device);
    }
    return devices;
}

std::vector<Record<SystemParam>> DataStore::getSystems(const std::vector<SystemParam>& parameters, const std::string& filter_regex, const std::string& device) {
    std::vector<std::string> systemNames = getFilteredSubfolderNames(getSystemPath(device), filter_regex);
    std::vector<Record<SystemParam>> systems;
    for (const auto& systemName : systemNames) {
        Record<SystemParam> system = openRecord<SystemParam>(getSystemPath(device, systemName) + '/' + params_filename);
        system.projectFields(parameters);
        systems.push_back(system);
    }
    return systems;
}

std::vector<Record<ServiceParam>> DataStore::getServices(const std::vector<ServiceParam>& parameters, const std::string& filter_regex, const std::string& device, const std::string& system) {
    std::vector<std::string> serviceNames = getFilteredSubfolderNames(getServicePath(device, system), filter_regex);
    std::vector<Record<ServiceParam>> services;
    for (const auto& serviceName : serviceNames) {
        Record<ServiceParam> service = openRecord<ServiceParam>(getServicePath(device, system, serviceName) + '/' + params_filename);
        service.projectFields(parameters);
        services.push_back(service);
    }
    return services;
}

std::vector<Record<OrchestrationParam>> DataStore::getOrchestrations(const std::vector<OrchestrationParam>& parameters, const std::string& filter_regex, const std::string& device, const std::string& system) {
    std::vector<std::string> orchestrationNames = getFilteredSubfolderNames(getOrchestrationPath(device, system), filter_regex);
    std::vector<Record<OrchestrationParam>> orchestrations;
    for (const auto& orchestrationName : orchestrationNames) {
        Record<OrchestrationParam> orchestration = openRecord<OrchestrationParam>(getOrchestrationPath(device, system, orchestrationName) + '/' + params_filename);
        orchestration.projectFields(parameters);
        orchestrations.push_back(orchestration);
    }
    return orchestrations;
}
