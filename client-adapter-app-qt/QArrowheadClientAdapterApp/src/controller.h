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
#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <sstream>
#include <string>
#include <iostream>
#include <qfile.h>
#include <QRegularExpression>
#include <QtNetwork/QSslKey>
#include <httpclient.h>
#include <qarrowheadhttpclientdeviceadapter.h>
#include <qarrowheadhttpclientdeviceadapter.h>
#include <qdebug.h>
#include <regex>
#include <QDir>

#include "enums/deviceparam.h"
#include "enums/orchestrationparam.h"
#include "enums/orchestrationrequestparam.h"
#include "enums/serviceparam.h"
#include "enums/settingsparam.h"
#include "enums/systemparam.h"
#include "record.h"
#include "datastore.h"
#include "orchestrationentry.h"

#include "qarrowheadhttpclientsystemadapter.h"
#include "logger.h"

using namespace arrowhead;

struct Data{
    arrowhead::HttpClient::AuthenticationMethod authMethod;
    std::string sharedSecret;

    std::string keyStore;
    std::string keyStorePwd;

    std::string privateKey;
    std::string publicKey;

    std::unique_ptr<OnboardingResponse> uptr_onboardingResponse;
    std::unique_ptr<arrowhead::QArrowheadHttpClientDeviceAdapter> ahAdapter;
};

class Controller{
    bool use_state;
    std::string context;
    enum SelectedContext{Empty, Device, System} selectedContext = Empty;

    DataStore store;

    std::string findUri(std::string uri, std::string name){
        std::regex rgx("https:\\/\\/.*\\/"+name);
        std::smatch match;

        if(std::regex_search(uri, match, rgx))
            uri = match[0];

        return uri;
    }


   Data createAdapter(std::string common_name) {
        Data data;

        Record<SettingsParam> settings = store.getSettings();

        std::string address = settings.getString(SettingsParam::onboarding_system_address);
        if(address.empty()){
            throw std::runtime_error("There is not given onboarding-system address, please add it (example: set settings onboarding-system-address[127.0.0.0])");
        }

        std::string onboardingControllerBaseUrl =
                "https://" + address + ":"
                + std::to_string(settings.getInteger(SettingsParam::onboarding_system_port))
                + "/onboarding";


        std::string method = settings.getString(SettingsParam::authentication_method);
        std::replace(method.begin(), method.end(), '-', ' ');
        data.authMethod = HttpClient::authMethodFromString(method);

        if(data.authMethod == HttpClient::AuthenticationMethod::BasicAuthentication){
            data.sharedSecret = settings.getString(SettingsParam::shared_secret);
        }else if(data.authMethod == HttpClient::AuthenticationMethod::ClientCertificate) {
            data.keyStore = settings.getString(SettingsParam::key_store);
            data.keyStorePwd = settings.getString(SettingsParam::key_store_password);
        }

        data.privateKey = store.getPrivateKey();
        data.publicKey = store.getPublicKey();


        data.ahAdapter = std::make_unique<QArrowheadHttpClientDeviceAdapter>(onboardingControllerBaseUrl);


        if(data.authMethod == HttpClient::AuthenticationMethod::BasicAuthentication){
            data.ahAdapter->getHttpClient().setSharedSecret(data.sharedSecret);
        } else {
            data.ahAdapter->getHttpClient().setSslConfig(data.keyStore, data.keyStorePwd, true); //TODO false, if not from localhost
        }

        //Check ONBOARDING SYSTEM AVAILABLE
        if(data.ahAdapter->getOnboardingControllerEcho() != ReturnValue::Ok)
            throw std::runtime_error("Onboarding System is not available, make sure the settings are correct for the onboarding-system-address and onboarding-system-port, use the show/set settings!");

        //Onboarding for the uri-es
        KeyPair keyPair("", "PKCS#8", data.privateKey, data.publicKey);

        OnboardingWithNameRequest onboardingRequest(CertificateCreationRequest(common_name, keyPair));

        if(ReturnValue::Ok != data.ahAdapter->requestOnboardingWithName(onboardingRequest, data.uptr_onboardingResponse)){
            throw std::runtime_error("Onboarding System not working:(");
        }



        //DEVICE REGISTRY BASE URL
        std::string uri = data.uptr_onboardingResponse->deviceRegistry.serviceUri;
        data.ahAdapter->setDeviceRegistryBaseUrl(findUri(uri, "deviceregistry"));

        if(data.ahAdapter->getDeviceRegistryEcho() != ReturnValue::Ok){
            throw std::runtime_error("Device Registry System is not available:(");
        }

        //SYSTEM REGISTRY BASE URL
        uri = data.uptr_onboardingResponse->systemRegistry.serviceUri;
        data.ahAdapter->setSystemRegistryBaseUrl(findUri(uri, "systemregistry"));

        if(data.ahAdapter->getSystemRegistryEcho() != ReturnValue::Ok)
            throw std::runtime_error("System Registry System is not available:(");

        return data;
    }

   auto getSystemAdapter(Data& data, std::string systemName, std::string address, int port){
       std::string service = findUri(data.uptr_onboardingResponse->serviceRegistry.serviceUri, "serviceregistry");
       std::string orchest = findUri(data.uptr_onboardingResponse->orchestrationService.serviceUri, "orchestrator");

       auto clientSystem = std::make_unique<ArrowheadSystem>(
                   systemName, address, port);

       auto ahSystemAdapter = std::make_unique<QArrowheadHttpClientSystemAdapter>(
                   service, ""/* Authorization base url? */, orchest,
                   std::move(clientSystem));

        ahSystemAdapter->getHttpClient().setSslConfig(data.keyStore, data.keyStorePwd, true); //TODO false, if not from localhost

       if(ahSystemAdapter->getServiceRegistryEcho() != ReturnValue::Ok){
           throw std::runtime_error("Service Registry is not available:(");
       }

       if(ahSystemAdapter->getOrchestratorEcho() != ReturnValue::Ok){
           throw std::runtime_error("Orchestratoris not available:(");
       }


       return ahSystemAdapter;
   }

   std::string getDeviceFromContext(std::string on, std::string name = ""){
       on = (on.empty() ? context : on);
       on += (name.empty() ? "" : "." + name);

       std::regex rgx("([\\w-:\\\\/ ]+)(.([\\w-:\\\\/ ]+))?");
       std::smatch match;

       if(!std::regex_match(on, match, rgx)){
           throw std::runtime_error("Invalid context!");
       }

       if(!store.isValid(match[1])){
           throw std::runtime_error("Invalid context!");
       }

       return match[1];
   }

   std::string getSystemFromContext(std::string on, std::string name = ""){
       on = (on.empty() ? context : on);
       on += (name.empty() ? "" : "." + name);

       std::regex rgx("([\\w-:\\\\/ ]+).([\\w-:\\\\/ ]+)");
       std::smatch match;

       if(!std::regex_match(on, match, rgx)){
           throw std::runtime_error("Invalid context!");
       }

       if(!store.isValid(match[1], match[2])){
           throw std::runtime_error("Invalid context!");
       }

       return match[2];
   }


public:

   Controller(bool use_state = true) : use_state{use_state}{}

   bool exitFromContext(){
       if(!use_state){
           throw std::runtime_error("Invalid request");
       }

       switch(selectedContext){
            case Device:
                selectedContext = Empty;
                context = "";
                return true;
            case System:
                selectedContext = Device;
                std::getline(std::stringstream{context}, context, '.');
                return true;
           case Empty:
                return false;
       }

       return false;
   }

   void selectDevice(std::string name){
       if(!use_state){
           throw std::runtime_error("Invalid request");
       }

       if(store.isValid(name)){
           context = name;
           selectedContext = Device;
       }else{
           throw std::runtime_error("Not valid device name!");
       }
   }

    void selectSystem(std::string name, std::string on = ""){
        if(!use_state){
            throw std::runtime_error("Invalid request");
        }

        on = (on == "" ? context : on);

        if(on == ""){
            throw std::runtime_error("You need a device where you can select your system!");
        }

        if(!store.isValid(on)){
            throw std::runtime_error("The device name ("+on+") what on you want select system is not valid!");
        }

        if(store.isValid(on, name)){
            context = on + "." +name;
            selectedContext = System;
        }else{
            throw std::runtime_error("Not valid system name!");
        }
    }

    std::string getContext(){
        return context;
    }

    std::string getStatusMessage(){
        Data data = createAdapter("get_uris");
        return "Everything is fine!:P";
    }

    void registerDevice(Record<DeviceParam> deviceToRegister){
        if(!deviceToRegister.containsAllMandatoryFields()){
            throw std::runtime_error{"Mandatory field missing"};
        }

        std::string commonName = deviceToRegister.getString(DeviceParam::name);
        Data data = createAdapter(commonName);

        QDir("").mkpath("devices/" + QString::fromStdString(commonName));

        store.saveOnboardingCertificate(data.uptr_onboardingResponse->onboardingCertificate.certificate, commonName);
        store.saveOnboardingPrivateKey(data.uptr_onboardingResponse->onboardingCertificate.keyPair.privateKey, commonName);
        store.saveOnboardingPublicKey(data.uptr_onboardingResponse->onboardingCertificate.keyPair.publicKey, commonName);

        std::unique_ptr<DeviceOnboardingResponse> uptr_onboardingResponse;

        KeyPair keyPair("", "PKCS#8", data.privateKey, data.publicKey);

        DeviceOnboardingWithNameRequest onboardingRequest(
                    CertificateCreationRequest{commonName, keyPair},
                    arrowhead::ArrowheadDevice{
                        deviceToRegister.getString(DeviceParam::name),
                        deviceToRegister.getString(DeviceParam::mac),
                        deviceToRegister.getString(DeviceParam::address),

                        deviceToRegister.getString(DeviceParam::authentication_info)
                    },
                    deviceToRegister.getString(DeviceParam::end_of_validity), deviceToRegister.getMap(DeviceParam::metadata), deviceToRegister.getInteger(DeviceParam::version));                                               //version

        data.ahAdapter->getHttpClient().setLocalCert(store.getOnboardingCertificateFile(commonName),
                                                     store.getOnboardingPrivateKeyFile(commonName));

        if(ReturnValue::Ok != data.ahAdapter->registerOnboardingDeviceWithName(onboardingRequest, uptr_onboardingResponse)){
            throw std::runtime_error("Not valid device registration!:(");
        }

        deviceToRegister.addField(DeviceParam::version, uptr_onboardingResponse->deviceRegistryEntry.version);
        deviceToRegister.addField(DeviceParam::end_of_validity, uptr_onboardingResponse->deviceRegistryEntry.endOfValidity);
        deviceToRegister.addField(DeviceParam::id, uptr_onboardingResponse->deviceRegistryEntry.deviceTag.id);
        deviceToRegister.addField(DeviceParam::created_at, uptr_onboardingResponse->deviceRegistryEntry.deviceTag.createdAt);
        deviceToRegister.addField(DeviceParam::updated_at, uptr_onboardingResponse->deviceRegistryEntry.deviceTag.updatedAt);
        deviceToRegister.addField(DeviceParam::authentication_info, uptr_onboardingResponse->deviceRegistryEntry.device.authenticationInfo);
        deviceToRegister.addField(DeviceParam::metadata, uptr_onboardingResponse->deviceRegistryEntry.metadata);

        store.saveDevice(deviceToRegister);
        store.saveDeviceCertificate(uptr_onboardingResponse->certificateResponse.certificate, commonName);
        store.saveDevicePrivateKey(uptr_onboardingResponse->certificateResponse.keyPair.privateKey, commonName);
        store.saveDevicePublicKey(uptr_onboardingResponse->certificateResponse.keyPair.publicKey, commonName);
    }



    void registerSystem(Record<SystemParam> systemToRegister, std::string on = ""){
        if(!systemToRegister.containsAllMandatoryFields()){
            throw std::runtime_error{"Mandatory field missing"};
        }

        Data data = createAdapter(systemToRegister.getString(SystemParam::name));

        std::string device = getDeviceFromContext(on);
        auto device_data =   getDevice(device);

        std::unique_ptr<SystemOnboardingResponse> uptr_onboardingResponse;

        KeyPair keyPair("", "PKCS#8", store.getPrivateKey(), store.getPublicKey());
        std::string commonName = systemToRegister.getString(SystemParam::name);

        std::string address = systemToRegister.getString(SystemParam::address);
        int port = systemToRegister.getInteger(SystemParam::port);

        if(address.empty()){
            address = device_data.getString(DeviceParam::address);
        }

        systemToRegister.addField(SystemParam::address, address);

        SystemOnboardingWithNameRequest onboardingRequest(
                    CertificateCreationRequest(commonName, keyPair),                                     //csr
                    ArrowheadDevice{
                        device,
                        device_data.getString(DeviceParam::mac),
                        device_data.getString(DeviceParam::address)
                    },      //device
                    ArrowheadSystem{
                        systemToRegister.getString(SystemParam::name),
                        address,
                        port
                    },      //system
                    systemToRegister.getString(SystemParam::end_of_validity),                            //endOfValidity
                    systemToRegister.getMap(SystemParam::metadata),                                      //metadata
                    systemToRegister.getInteger(SystemParam::version));                                  //version

        data.ahAdapter->getHttpClient().setLocalCert(store.getDeviceCertificateFile(device),
                                                     store.getDevicePrivateKeyFile(device));

        if(ReturnValue::Ok != data.ahAdapter->registerOnboardingSystemWithName(onboardingRequest, uptr_onboardingResponse)){
            throw std::runtime_error{"System registration fault! :("};
        }

        systemToRegister.addField(SystemParam::id, uptr_onboardingResponse->systemRegistryEntry.systemTag.id);
        systemToRegister.addField(SystemParam::authentication_info, uptr_onboardingResponse->systemRegistryEntry.system.authenticationInfo);
        systemToRegister.addField(SystemParam::created_at, uptr_onboardingResponse->systemRegistryEntry.systemTag.createdAt);
        systemToRegister.addField(SystemParam::updated_at, uptr_onboardingResponse->systemRegistryEntry.systemTag.updatedAt);
        systemToRegister.addField(SystemParam::end_of_validity, uptr_onboardingResponse->systemRegistryEntry.endOfValidity);
        systemToRegister.addField(SystemParam::metadata, uptr_onboardingResponse->systemRegistryEntry.metadata);
        systemToRegister.addField(SystemParam::version, uptr_onboardingResponse->systemRegistryEntry.version);

        store.saveSystem(systemToRegister, device);
        store.saveSystemCertificate(uptr_onboardingResponse->certificateResponse.certificate, device, systemToRegister.getString(SystemParam::name));
        store.saveSystemPrivateKey(uptr_onboardingResponse->certificateResponse.keyPair.privateKey, device, systemToRegister.getString(SystemParam::name));
        store.saveSystemPublicKey(uptr_onboardingResponse->certificateResponse.keyPair.publicKey, device, systemToRegister.getString(SystemParam::name));
    }

    void registerService(Record<ServiceParam> serviceToRegister, std::string on = ""){
        if(!serviceToRegister.containsAllMandatoryFields()){
            throw std::runtime_error{"Mandatory field missing"};
        }

        std::string name = serviceToRegister.getString(ServiceParam::name);

        std::string device = getDeviceFromContext(on);
        std::string system = getSystemFromContext(on);

        Data data = createAdapter(device);

        auto system_data = getSystem(system, device);

        std::unique_ptr<arrowhead::ServiceRegistryEntry> uptr_serviceRegistryEntry;

        auto ahSystemAdapter = getSystemAdapter(
                    data,
                    system_data.getString(SystemParam::name),
                    system_data.getString(SystemParam::address),
                    system_data.getInteger(SystemParam::port));

        ArrowheadService service = ArrowheadService::builder()
                .setServiceDefinition(serviceToRegister.getString(ServiceParam::name))
                .setInterfaces(serviceToRegister.getList(ServiceParam::interfaces))
                .setServiceUri(serviceToRegister.getString(ServiceParam::uri))
                .setSecurityType(securityTypeFromString(serviceToRegister.getString(ServiceParam::security_type)))
                .setEndOfValidity(serviceToRegister.getString(ServiceParam::end_of_validity))
                .setMetadata(serviceToRegister.getMap(ServiceParam::metadata))
                .setVersion(serviceToRegister.getInteger(ServiceParam::version))
                .build();

        ArrowheadSystem provider(system_data.getString(SystemParam::name), system_data.getString(SystemParam::address), system_data.getInteger(SystemParam::port));
        ServiceRegistrationForm serviceRegistrationForm(service, provider);

        ahSystemAdapter->getHttpClient().setLocalCert(store.getSystemCertificateFile(device, system),
                                                      store.getSystemPrivateKeyFile(device, system));

        if(ReturnValue::Ok != ahSystemAdapter->registerService(serviceRegistrationForm, uptr_serviceRegistryEntry)){
            throw std::runtime_error{"The service registration fault! :("};
        }

        serviceToRegister.addField(ServiceParam::id, uptr_serviceRegistryEntry->registeredService.entryTag.id);
        serviceToRegister.addField(ServiceParam::uri, uptr_serviceRegistryEntry->registeredService.service.serviceUri);
        serviceToRegister.addField(ServiceParam::created_at, uptr_serviceRegistryEntry->registeredService.entryTag.createdAt);
        serviceToRegister.addField(ServiceParam::updated_at, uptr_serviceRegistryEntry->registeredService.entryTag.updatedAt);
        serviceToRegister.addField(ServiceParam::end_of_validity, uptr_serviceRegistryEntry->registeredService.service.endOfValidity);
        serviceToRegister.addField(ServiceParam::metadata, uptr_serviceRegistryEntry->registeredService.service.metadata);
        serviceToRegister.addField(ServiceParam::version, uptr_serviceRegistryEntry->registeredService.service.version);

        store.saveService(serviceToRegister, device, system);
    }

    void unregisterDevice(std::string name){
        std::string device = getDeviceFromContext(name);

        Data data = createAdapter("unregister_device");

        auto device_data = getDevice(device);

        DeviceRegistryEntry entry;
        entry.device.deviceName = device;
        entry.device.address      = device_data.getString(DeviceParam::address);
        entry.device.macAddress    = device_data.getString(DeviceParam::mac);

        data.ahAdapter->getHttpClient().setLocalCert(store.getOnboardingCertificateFile(device),
                                                     store.getOnboardingPrivateKeyFile(device));

        if(ReturnValue::Ok != data.ahAdapter->unregisterDevice(entry)){
            throw std::runtime_error{"Unregister device fault! :("};
        }

        store.removeDevice(name);

        context = "";
        selectedContext = Empty;
    }

    void unregisterSystem(std::string name, std::string on = ""){
        std::string device = getDeviceFromContext(on);
        std::string system = name;

        Data data = createAdapter("unregister_system");

        auto system_data = getSystem(system, device);

        SystemRegistryEntry entry;
        entry.system.systemName = name;
        entry.system.address = system_data.getString(SystemParam::address);
        entry.system.port    = system_data.getInteger(SystemParam::port);

        data.ahAdapter->getHttpClient().setLocalCert(store.getDeviceCertificateFile(device),
                                                     store.getDevicePrivateKeyFile(device));
        if(ReturnValue::Ok != data.ahAdapter->unregisterSystem(entry)){
            throw std::runtime_error{"Unregister system fault! :("};
        }

        store.removeSystem(name, device);

        if(selectedContext == System){
            exitFromContext();
        }
    }

    void unregisterService(std::string name, std::string on = ""){
        std::string device = getDeviceFromContext(on);
        std::string system = getSystemFromContext(on);

        Data data = createAdapter("unregister_service");

        auto system_data = getSystem(system, device);
        auto service_data = getService(name, device +"."+system);

        auto ahSystemAdapter = getSystemAdapter(
                    data,
                    name,
                    system_data.getString(SystemParam::address),
                    system_data.getInteger(SystemParam::port));

        ArrowheadService::builder builder;
        builder.setServiceDefinition(service_data.getString(ServiceParam::name));

        ahSystemAdapter->getHttpClient().setLocalCert(store.getSystemCertificateFile(device, system),
                                                      store.getSystemPrivateKeyFile(device, system));

        auto ret_value = ahSystemAdapter->unregisterService({
           {builder.build(), {}}, {{
                   system,
                   system_data.getString(SystemParam::address),
                   system_data.getInteger(SystemParam::port)
                }, {}
           }, {}
        });

        if(ReturnValue::Ok != ret_value){
            throw std::runtime_error{"Unregister service fault! :("};
        }

        store.removeService(name, device, system);
    }

    void orchestrateSystem(Record<OrchestrationRequestParam> orchestrationRequest, std::string on = ""){
        if(!orchestrationRequest.containsAllMandatoryFields())
            throw std::runtime_error{"Mandatory field missing"};

        std::string name = orchestrationRequest.getString(OrchestrationRequestParam::name);

        std::string device = getDeviceFromContext(on);
        std::string system = getSystemFromContext(on);

        Data data = createAdapter(device);

        auto system_data = getSystem(system, device);

        auto ahSystemAdapter = getSystemAdapter(
                    data,
                    system_data.getString(SystemParam::name),
                    system_data.getString(SystemParam::address),
                    system_data.getInteger(SystemParam::port));

        std::vector<SecurityType> security_types;
        for(auto& security_type : orchestrationRequest.getList(OrchestrationRequestParam::security_types))
            security_types.push_back(securityTypeFromString(security_type));

        std::unique_ptr<OrchestrationResponse> uptr_orchestrationResponse;
        ArrowheadServiceRequirement requestedService =
                ArrowheadServiceRequirement::builder()
                .setServiceDefinitionRequirement(orchestrationRequest.getString(OrchestrationRequestParam::service_name))
                .setInterfaceRequirements(orchestrationRequest.getList(OrchestrationRequestParam::interfaces))
                .setSecurityRequirements(security_types)
                .setMetadataRequirements(orchestrationRequest.getMap(OrchestrationRequestParam::metadata))
                .setVersionRequirement(orchestrationRequest.getInteger(OrchestrationRequestParam::version))
                .setMaxVersionRequirement(orchestrationRequest.getInteger(OrchestrationRequestParam::max_version))
                .setMinVersionRequirement(orchestrationRequest.getInteger(OrchestrationRequestParam::min_version))
                .build();

        std::map<ServiceRequestForm::OrchestrationFlagKey, bool> flags;
        for(auto& flag : orchestrationRequest.getList(OrchestrationRequestParam::flags)){
            flags.insert({orchestrationFlagKeyFromString(flag), true});
        }

        stringmap provider = orchestrationRequest.getMap(OrchestrationRequestParam::preferred_provider);

        std::vector<ServiceRequestForm::PreferredProvider> preffered_providers;
        if(!provider.empty())
            preffered_providers = {{
                ArrowheadCloud{provider["operator-name"], provider["cloud-name"]},
                ArrowheadSystem{
                    provider["system-name"],
                    provider["system-address"],
                    provider["system-port"].empty() ? 0 : std::stoi(provider["system-port"]),
                    provider["authentication-info"]}
            }};

        ServiceRequestForm serviceRequestForm = ServiceRequestForm::builder()
                .setOrchestrationFlags(flags)
                .setRequestedService(requestedService)
                .setPreferredProviders(preffered_providers)
                .setRequesterSystem(ArrowheadSystem(system_data.getString(SystemParam::name), system_data.getString(SystemParam::address), system_data.getInteger(SystemParam::port)))
                .build();

        ahSystemAdapter->getHttpClient().setLocalCert(store.getSystemCertificateFile(device, system),
                                                      store.getSystemPrivateKeyFile(device, system));

        if(ReturnValue::Ok != ahSystemAdapter->requestOrchestration(serviceRequestForm, uptr_orchestrationResponse))
            throw std::runtime_error{"Orchestration was not valid!"};


        int id = 0;
        for(auto& record : uptr_orchestrationResponse->orchEntryVector){
            std::vector<std::string> warnings;
            for(auto& warnig : record.warnings)
                warnings.push_back(typeToString(warnig));

            Record<OrchestrationParam> params;
            params.addField(OrchestrationParam::name, orchestrationRequest.getString(OrchestrationRequestParam::name) + std::to_string(id));
            params.addField(OrchestrationParam::authorization_tokens, record.authorizationTokens);
            params.addField(OrchestrationParam::interfaces, record.orchestratedService.service.interfaces);
            params.addField(OrchestrationParam::metadata, record.orchestratedService.service.metadata);
            params.addField(OrchestrationParam::provider_address, record.orchestratedProvider.provider.address);
            params.addField(OrchestrationParam::provider_authentication_info, record.orchestratedProvider.provider.authenticationInfo);
            params.addField(OrchestrationParam::provider_created_at, record.orchestratedProvider.entryTag.createdAt);
            params.addField(OrchestrationParam::provider_id, record.orchestratedProvider.entryTag.id);
            params.addField(OrchestrationParam::provider_port, record.orchestratedProvider.provider.port);
            params.addField(OrchestrationParam::provider_system_name, record.orchestratedProvider.provider.systemName);
            params.addField(OrchestrationParam::provider_updated_at, record.orchestratedProvider.entryTag.updatedAt);
            params.addField(OrchestrationParam::secure, typeToString(record.orchestratedService.service.securityType));
            params.addField(OrchestrationParam::service_created_at, record.orchestratedService.entryTag.createdAt);
            params.addField(OrchestrationParam::service_id, record.orchestratedService.entryTag.id);
            params.addField(OrchestrationParam::service_name, record.orchestratedService.service.serviceDefinition);
            params.addField(OrchestrationParam::service_updated_at, record.orchestratedService.entryTag.updatedAt);
            params.addField(OrchestrationParam::service_uri, record.orchestratedService.service.serviceUri);
            params.addField(OrchestrationParam::version, record.orchestratedService.service.version);
            params.addField(OrchestrationParam::warnings, warnings);
            store.saveOrchestration(params, device, system);
            id++;
        }
    }

    void setSettings(Record<SettingsParam> settings_param){
        store.setSettings(settings_param);
    }

    Record<SettingsParam> getSettings(){
        return store.getSettings();
    }

    Record<DeviceParam> getDevice(std::string name){
        std::vector<Record<DeviceParam>> list = getDevices("^" + name + "$", DeviceParam::getParameters());
        if(list.size() == 0){
            throw std::runtime_error("There is no " + name);
        }

        return list[0];
    }

    Record<SystemParam> getSystem(std::string name, std::string on = ""){
        std::vector<Record<SystemParam>> list = getSystems("^" + name + "$", SystemParam::getParameters(), on);
        if(list.size() == 0){
            throw std::runtime_error("There is no " + name);
        }

        return list[0];
    }

    Record<ServiceParam> getService(std::string name, std::string on = ""){
        std::vector<Record<ServiceParam>> list = getServices("^" + name + "$", ServiceParam::getParameters(), on);
        if(list.size() == 0){
            throw std::runtime_error("There is no " + name);
        }

        return list[0];
    }

    Record<OrchestrationParam> getOrchestration(std::string name="", std::string on = ""){
        std::vector<Record<OrchestrationParam>> list = getOrchestrations("^" + name + "$", OrchestrationParam::getParameters(), on);
        if(list.size() == 0){
            throw std::runtime_error("There is no " + name);
        }

        return list[0];
    }

    std::vector<Record<DeviceParam>> getDevices(std::string filter="", std::vector<DeviceParam> parameters={}){
        if(parameters.empty()){
            parameters = {
                DeviceParam::name,
                DeviceParam::mac,
                DeviceParam::address
            };

        }

        return store.getDevices(parameters, filter);
    }

    std::vector<Record<SystemParam>> getSystems(std::string filter="", std::vector<SystemParam> parameters={}, std::string on = ""){
        if(parameters.empty()){
            parameters = {
                SystemParam::name,
                SystemParam::address,
                SystemParam::port
            };
        }

        return store.getSystems(parameters, filter, getDeviceFromContext(on.empty() ? context : on));
    }

    std::vector<Record<ServiceParam>> getServices(std::string filter="", std::vector<ServiceParam> parameters={}, std::string on = ""){
        if(parameters.empty()){
            parameters = {
                ServiceParam::name,
                ServiceParam::interfaces,
                ServiceParam::security_type
            };
        }

        return store.getServices(parameters, filter, getDeviceFromContext(on), getSystemFromContext(on));
    }

    std::vector<Record<OrchestrationParam>> getOrchestrations(std::string filter="", std::vector<OrchestrationParam> parameters={}, std::string on = ""){
        if(parameters.empty()){
            parameters = {
                OrchestrationParam::name,
                OrchestrationParam::provider_system_name,
                OrchestrationParam::service_name,
                OrchestrationParam::service_uri,
                OrchestrationParam::secure,
                OrchestrationParam::version,
            };
        }

        return store.getOrchestrations(parameters, filter, getDeviceFromContext(on), getSystemFromContext(on));
    }
};

#endif // CONTROLLER_H
