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
#include <QtTest/QtTest>
#include <QFile>
#include <controller.h>

#include "datastore.h"


class TEST_QArrowheadClientAdapterApp : public QObject
{
    Q_OBJECT

public:
    TEST_QArrowheadClientAdapterApp();
    ~TEST_QArrowheadClientAdapterApp();

private slots:
    //DATASTORE

    void dataStore_settingsHandling();
    void dataStore_deviceHandling();
    void dataStore_systemHandling();
    void dataStore_serviceHandling();
    void dataStore_orchestrationHandling();

    //CONTROLLER

    //Set settings
    void controller_setSettings();

    //Device
    void controller_registerDevice();
    void controller_selectDevice();

    //System
    void controller_registerSystem();
    void controller_contextCheck();

    //Service
    void controller_registerService();

    //Orchestration
    void controller_orchestrateSystem();

    //Unregistrations
    void controller_unregisterService();
    void controller_unregisterSystem();
    void controller_unregisterDevice();

private:
    QString rmHeaderFooter(std::string content){
        return QString::fromStdString(content).remove(QRegularExpression(".*-{5}\n*"))
                .remove(QRegularExpression("[\\n\\t\\r]"));
    }
};

//DATASTORE

TEST_QArrowheadClientAdapterApp::TEST_QArrowheadClientAdapterApp()
{

}

TEST_QArrowheadClientAdapterApp::~TEST_QArrowheadClientAdapterApp()
{

}

void TEST_QArrowheadClientAdapterApp::dataStore_settingsHandling()
{
    DataStore dataStore;
    QDir("").mkdir("etc");
    QFile prkFile("etc/testOnboardingPrivateKey.pem");
    QFile pukFile("etc/testOnboardingPublicKey.pem");

    if (!prkFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
            throw std::runtime_error("Couldn't open file testOnboardingPrivateKey.pem");
    if (!pukFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
            throw std::runtime_error("Couldn't open file testOnboardingPublicKey.pem");

    const char* prkData = "testdata626=testdata626=testdata626=testdata626=testdata626=testdata626=testdata626=";
    const char* pukData = "anothertestdata449/anothertestdata449/anothertestdata449/anothertestdata449/anothertestdata449/";
    prkFile.write(prkData);
    pukFile.write(pukData);

    prkFile.close();
    pukFile.close();

    Record<SettingsParam> settings;
    settings.addField(SettingsParam::onboarding_system_address,     std::string("127.0.0.1"));
    settings.addField(SettingsParam::onboarding_system_port,        8435);
    settings.addField(SettingsParam::authentication_method,         std::string("client certificate"));
    settings.addField(SettingsParam::shared_secret,                 std::string("hidden_secret"));
    settings.addField(SettingsParam::key_store,                     std::string("etc/eitg.p12"));
    settings.addField(SettingsParam::key_store_password,            std::string("password"));
    settings.addField(SettingsParam::private_key,                   std::string("etc/testOnboardingPrivateKey.pem"));
    settings.addField(SettingsParam::public_key,                    std::string("etc/testOnboardingPublicKey.pem"));
    settings.addField(SettingsParam::log_level,                     std::string("debug"));

    dataStore.setSettings(settings);

    Record<SettingsParam> readSettings = dataStore.getSettings();

    QCOMPARE(readSettings.getString(SettingsParam::onboarding_system_address),     std::string("127.0.0.1"));
    QCOMPARE(readSettings.getInteger(SettingsParam::onboarding_system_port),       8435);
    QCOMPARE(readSettings.getString(SettingsParam::authentication_method),         std::string("client certificate"));
    QCOMPARE(readSettings.getString(SettingsParam::shared_secret),                 std::string("hidden_secret"));
    QCOMPARE(readSettings.getString(SettingsParam::key_store),                     std::string("etc/eitg.p12"));
    QCOMPARE(readSettings.getString(SettingsParam::key_store_password),            std::string("password"));
    QCOMPARE(readSettings.getString(SettingsParam::private_key),                   std::string("etc/testOnboardingPrivateKey.pem"));
    QCOMPARE(readSettings.getString(SettingsParam::public_key),                    std::string("etc/testOnboardingPublicKey.pem"));
    QCOMPARE(readSettings.getString(SettingsParam::log_level),                      std::string("debug"));

    QCOMPARE(dataStore.getPrivateKey(), std::string(prkData));
    QCOMPARE(dataStore.getPublicKey(), std::string(pukData));

    prkFile.remove();
    pukFile.remove();
}

void TEST_QArrowheadClientAdapterApp::dataStore_deviceHandling() {
    DataStore dataStore;

    std::map<std::string, std::string> metadata = {{"prop1", "val1"}, {"prop2", "val2"}, {"prop3", "val3"}};

    Record<DeviceParam> device1, device2;
    device1.addField(DeviceParam::name, "MyArrowheadDevice1");
    device1.addField(DeviceParam::address, "192.168.41.3");
    device1.addField(DeviceParam::mac, "aa-aa-aa-aa-aa-aa");
    device1.addField(DeviceParam::version, 2);
    device1.addField(DeviceParam::metadata, metadata);

    device2.addField(DeviceParam::name, "MyArrowheadDevice2");
    device2.addField(DeviceParam::address, "192.168.41.4");
    device2.addField(DeviceParam::mac, "bb-bb-bb-bb-bb-bb");
    device2.addField(DeviceParam::version, 4);

    dataStore.saveDevice(device1);
    dataStore.saveDevice(device2);
    dataStore.saveOnboardingCertificate("testonboardingcertificate1", "MyArrowheadDevice1");
    dataStore.saveDeviceCertificate("testdevicecertificate1", "MyArrowheadDevice1");

    QCOMPARE(dataStore.getDevices({}, "Device").size(), 2u);

    Record<DeviceParam> device1read = dataStore.getDevices({DeviceParam::name, DeviceParam::mac, DeviceParam::address, DeviceParam::metadata, DeviceParam::version}, "MyArrowheadDevice1").at(0);
    Record<DeviceParam> device2read = dataStore.getDevices({DeviceParam::name, DeviceParam::mac, DeviceParam::address, DeviceParam::version}, "MyArrowheadDevice2").at(0);

    QCOMPARE(device1read.getString(DeviceParam::name), "MyArrowheadDevice1");
    QCOMPARE(device1read.getMap(DeviceParam::metadata).at("prop2"), metadata.at("prop2"));
    QCOMPARE(device2read.getInteger(DeviceParam::version), 4);

    QCOMPARE(rmHeaderFooter(dataStore.getOnboardingCertificate("MyArrowheadDevice1")), "testonboardingcertificate1");
    QCOMPARE(rmHeaderFooter(dataStore.getDeviceCertificate("MyArrowheadDevice1")), "testdevicecertificate1");

    dataStore.removeDevice("MyArrowheadDevice1");
    QCOMPARE(dataStore.getDevices({}, "Device").size(), 1u);
    dataStore.removeDevice("MyArrowheadDevice2");
    QCOMPARE(dataStore.getDevices({}, "Device").size(), 0u);
}


void TEST_QArrowheadClientAdapterApp::dataStore_systemHandling() {
    DataStore dataStore;

    std::map<std::string, std::string> metadata = {{"prop1", "val1"}, {"prop2", "val2"}, {"prop3", "val3"}};

    Record<DeviceParam> device1;
    device1.addField(DeviceParam::name, "MyArrowheadDevice1");

    Record<SystemParam> system1, system2;
    system1.addField(SystemParam::name, "MyArrowheadSystem1");
    system1.addField(SystemParam::address, "192.168.41.3");
    system1.addField(SystemParam::authentication_info, "asdasdasd");
    system1.addField(SystemParam::port, 441);
    system1.addField(SystemParam::metadata, metadata);

    system2.addField(SystemParam::name, "MyArrowheadSystem2");
    system2.addField(SystemParam::address, "192.168.41.4");
    system2.addField(SystemParam::authentication_info, "asdasdasd2");
    system2.addField(SystemParam::port, 443);

    dataStore.saveSystem(system1, "MyArrowheadDevice1");
    dataStore.saveSystem(system2, "MyArrowheadDevice1");
    dataStore.saveSystemCertificate("testsystemcertificate1", "MyArrowheadDevice1", "MyArrowheadSystem1");

    QCOMPARE(dataStore.getSystems({}, "System", "MyArrowheadDevice1").size(), 2u);

    Record<SystemParam> system1read = dataStore.getSystems({SystemParam::name, SystemParam::port, SystemParam::address, SystemParam::metadata, SystemParam::authentication_info}, "MyArrowheadSystem1", "MyArrowheadDevice1").at(0);
    Record<SystemParam> system2read = dataStore.getSystems({SystemParam::name, SystemParam::port, SystemParam::address, SystemParam::authentication_info}, "MyArrowheadSystem2", "MyArrowheadDevice1").at(0);

    QCOMPARE(system1read.getString(SystemParam::name), "MyArrowheadSystem1");
    QCOMPARE(system1read.getMap(SystemParam::metadata).at("prop2"), metadata.at("prop2"));
    QCOMPARE(system2read.getInteger(SystemParam::port), 443);

    QCOMPARE(rmHeaderFooter(dataStore.getSystemCertificate("MyArrowheadDevice1", "MyArrowheadSystem1")), "testsystemcertificate1");

    dataStore.removeSystem("MyArrowheadSystem1", "MyArrowheadDevice1");
    QCOMPARE(dataStore.getSystems({}, "System", "MyArrowheadDevice1").size(), 1u);
    dataStore.removeDevice("MyArrowheadDevice1");
    QCOMPARE(dataStore.getSystems({}, "System", "MyArrowheadDevice1").size(), 0u);
}

void TEST_QArrowheadClientAdapterApp::dataStore_serviceHandling() {

    DataStore dataStore;

    std::map<std::string, std::string> metadata = {{"prop1", "val1"}, {"prop2", "val2"}, {"prop3", "val3"}};
    std::vector<std::string> interfaces = {"HTTP-SECURE-JSON", "HTTP-INSECURE-JSON"};

    Record<DeviceParam> device1;
    device1.addField(DeviceParam::name, "MyArrowheadDevice1");
    Record<SystemParam> system1;
    system1.addField(SystemParam::name, "MyArrowheadSystem1");

    Record<ServiceParam> service1, service2;
    service1.addField(ServiceParam::name, "MyArrowheadService1");
    service1.addField(ServiceParam::uri, "https://localhost:8765/service");
    service1.addField(ServiceParam::interfaces, interfaces);
    service1.addField(ServiceParam::metadata, metadata);

    service2.addField(ServiceParam::name, "MyArrowheadService2");

    dataStore.saveDevice(device1);
    dataStore.saveSystem(system1, "MyArrowheadDevice1");

    dataStore.saveService(service1, "MyArrowheadDevice1", "MyArrowheadSystem1");
    dataStore.saveService(service2, "MyArrowheadDevice1", "MyArrowheadSystem1");

    QCOMPARE(dataStore.getServices({}, "Service", "MyArrowheadDevice1", "MyArrowheadSystem1").size(), 2u);

    Record<ServiceParam> service1read = dataStore.getServices({ServiceParam::name, ServiceParam::uri, ServiceParam::interfaces, ServiceParam::metadata}, "MyArrowheadService1", "MyArrowheadDevice1", "MyArrowheadSystem1").at(0);
    Record<ServiceParam> service2read = dataStore.getServices({ServiceParam::name}, "MyArrowheadService2", "MyArrowheadDevice1", "MyArrowheadSystem1").at(0);

    QCOMPARE(service1read.getString(ServiceParam::name), "MyArrowheadService1");
    QCOMPARE(service1read.getMap(ServiceParam::metadata).at("prop2"), metadata.at("prop2"));
    QCOMPARE(service1read.getList(ServiceParam::interfaces).at(1), interfaces.at(1));
    QCOMPARE(service1read.getString(ServiceParam::uri), "https://localhost:8765/service");
    QCOMPARE(service2read.getString(ServiceParam::name), "MyArrowheadService2");

    dataStore.removeService("MyArrowheadService1", "MyArrowheadDevice1", "MyArrowheadSystem1");
    QCOMPARE(dataStore.getServices({}, "Service", "MyArrowheadDevice1", "MyArrowheadSystem1").size(), 1u);
    dataStore.removeDevice("MyArrowheadDevice1");
    QCOMPARE(dataStore.getServices({}, "Service", "MyArrowheadDevice1", "MyArrowheadSystem1").size(), 0u);
}

void TEST_QArrowheadClientAdapterApp::dataStore_orchestrationHandling() {

    DataStore dataStore;

    std::vector<std::string> interfaces = {"HTTP-SECURE-JSON", "HTTP-INSECURE-JSON"};

    Record<DeviceParam> device1;
    device1.addField(DeviceParam::name, "MyArrowheadDevice1");
    Record<SystemParam> system1;
    system1.addField(SystemParam::name, "MyArrowheadSystem1");

    Record<OrchestrationParam> orchestration1, orchestration2;
    orchestration1.addField(OrchestrationParam::name, "MyArrowheadOrchestration1");
    orchestration1.addField(OrchestrationParam::service_id, 4);
    orchestration1.addField(OrchestrationParam::interfaces, interfaces);

    orchestration2.addField(OrchestrationParam::name, "MyArrowheadOrchestration2");

    dataStore.saveDevice(device1);
    dataStore.saveSystem(system1, "MyArrowheadDevice1");

    dataStore.saveOrchestration(orchestration1, "MyArrowheadDevice1", "MyArrowheadSystem1");
    dataStore.saveOrchestration(orchestration2, "MyArrowheadDevice1", "MyArrowheadSystem1");

    QCOMPARE(dataStore.getOrchestrations({}, "Orchestration", "MyArrowheadDevice1", "MyArrowheadSystem1").size(), 2);

    Record<OrchestrationParam> orchestration1read = dataStore.getOrchestrations({OrchestrationParam::name, OrchestrationParam::service_id, OrchestrationParam::interfaces}, "MyArrowheadOrchestration1", "MyArrowheadDevice1", "MyArrowheadSystem1").at(0);
    Record<OrchestrationParam> orchestration2read = dataStore.getOrchestrations({OrchestrationParam::name}, "MyArrowheadOrchestration2", "MyArrowheadDevice1", "MyArrowheadSystem1").at(0);

    QCOMPARE(orchestration1read.getString(OrchestrationParam::name), "MyArrowheadOrchestration1");
    QCOMPARE(orchestration1read.getList(OrchestrationParam::interfaces).at(1), interfaces.at(1));
    QCOMPARE(orchestration1read.getInteger(OrchestrationParam::service_id), 4);
    QCOMPARE(orchestration2read.getString(OrchestrationParam::name), "MyArrowheadOrchestration2");

    dataStore.removeDevice("MyArrowheadDevice1");
    QCOMPARE(dataStore.getOrchestrations({}, "Orchestration", "MyArrowheadDevice1", "MyArrowheadSystem1").size(), 0);

}

//CONTROLLER

//Set settings
void TEST_QArrowheadClientAdapterApp::controller_setSettings(){
    Controller controller;

    Record<SettingsParam> settings;
    settings.addField(SettingsParam::onboarding_system_address,     std::string("127.0.0.1"));
    settings.addField(SettingsParam::onboarding_system_port,        8435);
    settings.addField(SettingsParam::authentication_method,         std::string("client certificate"));
    settings.addField(SettingsParam::shared_secret,                 std::string("hidden_secret"));
    settings.addField(SettingsParam::key_store,                     CONF_DIR + std::string("eitg.p12"));
    settings.addField(SettingsParam::key_store_password,            std::string("password"));
    settings.addField(SettingsParam::private_key,                   CONF_DIR + std::string("onboardingPrivateKey.pem"));
    settings.addField(SettingsParam::public_key,                    CONF_DIR + std::string("onboardingPublicKey.pem"));

    controller.setSettings(settings);

    Record<SettingsParam> readSettings = controller.getSettings();

    QCOMPARE(readSettings.getString(SettingsParam::onboarding_system_address),     std::string("127.0.0.1"));
    QCOMPARE(readSettings.getInteger(SettingsParam::onboarding_system_port),       8435);
    QCOMPARE(readSettings.getString(SettingsParam::authentication_method),         std::string("client certificate"));
    QCOMPARE(readSettings.getString(SettingsParam::shared_secret),                 std::string("hidden_secret"));
    QCOMPARE(readSettings.getString(SettingsParam::key_store),                     CONF_DIR + std::string("eitg.p12"));
    QCOMPARE(readSettings.getString(SettingsParam::key_store_password),            std::string("password"));
    QCOMPARE(readSettings.getString(SettingsParam::private_key),                   CONF_DIR + std::string("onboardingPrivateKey.pem"));
    QCOMPARE(readSettings.getString(SettingsParam::public_key),                    CONF_DIR + std::string("onboardingPublicKey.pem"));
}


//Device
void TEST_QArrowheadClientAdapterApp::controller_registerDevice(){
    int argc;
    QCoreApplication a(argc, nullptr);

    Controller controller;

    Record<DeviceParam> device;
    device.addField(DeviceParam::name,                  "MyArrowheadDevice");
    device.addField(DeviceParam::address,               "192.138.1.1");
    device.addField(DeviceParam::mac,                   "ab-ba-cd-aa-11-22");
    device.addField(DeviceParam::metadata,              stringmap{{"key1","value1"}, {"key2","value2"}});
    device.addField(DeviceParam::end_of_validity,       "2021-08-30 18:22:23");
    device.addField(DeviceParam::version,               8);

    controller.registerDevice(device);

    Record<DeviceParam> registered_device = controller.getDevice(device.getString(DeviceParam::name));

    QCOMPARE(device.getString(DeviceParam::name),     device.getString(DeviceParam::name));
    QCOMPARE(device.getString(DeviceParam::address),  device.getString(DeviceParam::address));
    QCOMPARE(device.getString(DeviceParam::mac),      device.getString(DeviceParam::mac));
    QCOMPARE(device.getMap(DeviceParam::metadata),    device.getMap(DeviceParam::metadata));
    QCOMPARE(device.getString(DeviceParam::authentication_info),   device.getString(DeviceParam::authentication_info));
    QCOMPARE(device.getString(DeviceParam::end_of_validity),       device.getString(DeviceParam::end_of_validity));
    QCOMPARE(device.getInteger(DeviceParam::version),              device.getInteger(DeviceParam::version));
}

void TEST_QArrowheadClientAdapterApp::controller_selectDevice(){
    int argc;
    QCoreApplication a(argc, nullptr);

    Controller controller;
    std::string device_name = "MyArrowheadDevice";

    controller.selectDevice(device_name);

    QCOMPARE(controller.getContext(), device_name);
}

//System
void TEST_QArrowheadClientAdapterApp::controller_registerSystem(){
    int argc;
    QCoreApplication a(argc, nullptr);

    Controller controller;

    Record<SystemParam> system;
    system.addField(SystemParam::name,              "eitg");
    system.addField(SystemParam::address,           "192.168.1.2");
    system.addField(SystemParam::port,              8080);
    system.addField(SystemParam::metadata,          stringmap{{"key1", "value1"}, {"key2", "value2"}});
    system.addField(SystemParam::end_of_validity,   "2021-08-30 18:22:23");
    system.addField(SystemParam::version,           8);

    controller.registerSystem(system, "MyArrowheadDevice");

    Record<SystemParam> registered_system = controller.getSystem(system.getString(SystemParam::name), "MyArrowheadDevice");

    QCOMPARE(system.getString(SystemParam::name),     system.getString(SystemParam::name));
    QCOMPARE(system.getString(SystemParam::address),  system.getString(SystemParam::address));
    QCOMPARE(system.getInteger(SystemParam::port),    system.getInteger(SystemParam::port));
    QCOMPARE(system.getMap(SystemParam::metadata),    system.getMap(SystemParam::metadata));
    QCOMPARE(system.getString(SystemParam::end_of_validity),       system.getString(SystemParam::end_of_validity));
    QCOMPARE(system.getInteger(SystemParam::version),              system.getInteger(SystemParam::version));
}

void TEST_QArrowheadClientAdapterApp::controller_contextCheck(){
    int argc;
    QCoreApplication a(argc, nullptr);

    Controller controller;

    std::string device_name = "MyArrowheadDevice";
    std::string system_name = "eitg";

    controller.selectSystem(system_name, device_name);
    QCOMPARE(controller.getContext(), device_name + '.' + system_name);

    controller.exitFromContext();
    QCOMPARE(controller.getContext(), device_name);

    controller.exitFromContext();
    QCOMPARE(controller.getContext(), "");
}

//Service
void TEST_QArrowheadClientAdapterApp::controller_registerService(){
    int argc;
    QCoreApplication a(argc, nullptr);

    Controller controller;

    Record<ServiceParam> service;
    service.addField(ServiceParam::name,                "MyArrowheadService");
    service.addField(ServiceParam::uri,                 "https://www.arrowhead.eu/");
    service.addField(ServiceParam::security_type,       "CERTIFICATE");
    service.addField(ServiceParam::metadata,            stringmap{{"key1", "value1"}, {"key2", "value2"}});
    service.addField(ServiceParam::interfaces,          std::vector<std::string>{"HTTPS-SECURE-JSON"});
    service.addField(ServiceParam::end_of_validity,     "2021-08-30 18:22:23");
    service.addField(ServiceParam::version,             8);

    controller.registerService(service, "MyArrowheadDevice.eitg");

    Record<ServiceParam> registered_service = controller.getService(service.getString(ServiceParam::name), "MyArrowheadDevice.eitg");

    QCOMPARE(service.getString(ServiceParam::name),             service.getString(ServiceParam::name));
    QCOMPARE(service.getString(ServiceParam::uri),              service.getString(ServiceParam::uri));
    QCOMPARE(service.getString(ServiceParam::security_type),    service.getString(ServiceParam::security_type));
    QCOMPARE(service.getMap(ServiceParam::metadata),            service.getMap(ServiceParam::metadata));
    QCOMPARE(service.getList(ServiceParam::interfaces),         service.getList(ServiceParam::interfaces));
    QCOMPARE(service.getString(ServiceParam::end_of_validity),  service.getString(ServiceParam::end_of_validity));
    QCOMPARE(service.getInteger(ServiceParam::version),         service.getInteger(ServiceParam::version));
}

//Orchestration
void TEST_QArrowheadClientAdapterApp::controller_orchestrateSystem(){
    int argc;
    QCoreApplication a(argc, nullptr);

    Controller controller;

    Record<OrchestrationRequestParam> orchestration;
    orchestration.addField(OrchestrationRequestParam::name,                "MyArrowheadOrchestration");
    orchestration.addField(OrchestrationRequestParam::service_name,        "service-register");
    orchestration.addField(OrchestrationRequestParam::interfaces,          std::vector<std::string>{"HTTPS-SECURE-JSON"});
    orchestration.addField(OrchestrationRequestParam::security_types,      std::vector<std::string>{"CERTIFICATE"});
    orchestration.addField(OrchestrationRequestParam::metadata,            stringmap{{"key1", "value1"}, {"key2", "value2"}});
    orchestration.addField(OrchestrationRequestParam::flags,               std::vector<std::string>{""});
    orchestration.addField(OrchestrationRequestParam::preferred_provider,  stringmap{
                               {"operator-name",        "ArrowheadOperator"},
                               {"cloud-name",           "ArrowheadCloud"},
                               {"system-name",          "ArrowheadSystem"},
                               {"system-address",       "https://www.arrowhead.eu/"},
                               {"system-port",          "8080"},
                               {"authentication-info",  ""}
                           });
    orchestration.addField(OrchestrationRequestParam::max_version,         10);
    orchestration.addField(OrchestrationRequestParam::min_version,         1);
    orchestration.addField(OrchestrationRequestParam::version,             8);

    controller.orchestrateSystem(orchestration, "MyArrowheadDevice.eitg");
}

//Unregistrations
void TEST_QArrowheadClientAdapterApp::controller_unregisterService(){
    int argc;
    QCoreApplication a(argc, nullptr);

    Controller controller;
    controller.unregisterService("MyArrowheadService", "MyArrowheadDevice.eitg");

    QCOMPARE(controller.getServices("", ServiceParam::getParameters(), "MyArrowheadDevice.eitg").size(), 0);
}

void TEST_QArrowheadClientAdapterApp::controller_unregisterSystem(){
    int argc;
    QCoreApplication a(argc, nullptr);

    Controller controller;
    controller.unregisterSystem("eitg", "MyArrowheadDevice");

    QCOMPARE(controller.getSystems("", SystemParam::getParameters(), "MyArrowheadDevice").size(), 0);
}

void TEST_QArrowheadClientAdapterApp::controller_unregisterDevice(){
    int argc;
    QCoreApplication a(argc, nullptr);

    Controller controller;
    controller.unregisterDevice("MyArrowheadDevice");

    QCOMPARE(controller.getDevices("", DeviceParam::getParameters()).size(), 0);
}


QTEST_APPLESS_MAIN(TEST_QArrowheadClientAdapterApp)

#include "tst_test_qarrowheadclientadapterapp.moc"
