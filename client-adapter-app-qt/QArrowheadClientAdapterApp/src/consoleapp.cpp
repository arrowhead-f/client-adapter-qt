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
#include "consoleapp.h"
#include <memory>

ConsoleApp::ConsoleApp(int argc, char* argv[], QObject *parent)
    : QObject(parent),
    controller{argc != 0},
    highlighted_text{FormattedText::blue, FormattedText::bold},
    welcome_message {
        FormattedText::formatText(highlighted_text, "Welcome to the Arrowhead Adapter App! ;)\n") +
        "It provides an easy-to-use command line interface for registering/unregistering device/system/service, requesting orchestration, and handling certificates.\n"
        "\n"
        "If you need help just use "+FormattedText::formatText(highlighted_text,"'?'")+" or "+FormattedText::formatText(highlighted_text,"'help'")+" anywhere you want!\n"
        ""
        +FormattedText::formatText(highlighted_text,"For example:")+" register ?"
    },
    commands    {
            /* description */
             FormattedText::formatText(highlighted_text, "The Arrowhead Adapter App provides an easy-to-use command line interface for:\n")
            +FormattedText::formatText(highlighted_text, "  - ")+"onboarding with name based on certificate\n"
            +FormattedText::formatText(highlighted_text, "  - ")+"system and device registration/deregistration/query\n"
            +FormattedText::formatText({FormattedText::cyan}, "  - ")+"service registration/deregistration\n"
            +FormattedText::formatText(highlighted_text, "  - ")+"orchestration request\n"
            +FormattedText::formatText(highlighted_text, "  - ")+"certificate handling\n\n"
            +FormattedText::formatText(highlighted_text, "To learn more about Arrowhead, click on the following link: ")+"https://www.arrowhead.eu/\n"
            "\n"
            +FormattedText::formatText(highlighted_text,"The basic syntax of the commands is the following:\n")+
            "<command> <subcommand> <parameter1>[<value1>] <parameter2>[<value2>] ...\n"
            "\n"
            +FormattedText::formatText(highlighted_text, "For example: \n")+
            FormattedText::formatText(highlighted_text, "❯ ")+"orchestrate system name[MyOrchestration] interfaces[HTTPS-SECURE-JSON] service-name[my-service] on[MyDevice1.MySystem1]\n"
            "\n"
            +FormattedText::formatText(highlighted_text, "The parameter values have one of the following types:\n")
            +FormattedText::formatText(highlighted_text, "  - text:")+"   <parameter-name>[<parameter-value>]  eg. name[MyFavouriteDevice]\n"
            +FormattedText::formatText(highlighted_text, "  - number:")+" <parameter-name>[<parameter-value>]  eg. system-onboarding-port[8435]\n"
            +FormattedText::formatText(highlighted_text, "  - map:")+"    <parameter-name>[<key1>:<value1>][<key2>:<value2>]...  eg. metadata[property1:value1][property2:value2]\n"
            +FormattedText::formatText(highlighted_text, "  - list")+"    <parameter-name>[<value1>][<value2>][<value3>]...      eg. flags[onlyPreferred][pingProviders]\n"
            "\n"
            +FormattedText::formatText(highlighted_text, "Initialize the app to work right\n")+
            "After registering a device, you can register systems on that device and services can be registered on previously registered systems.\n (register device/system/service)\n"
            "On registered systems you can also request orchestrations.\n\n"
            "In order to register a system on a certain device, you have to select that device first (select device name[NameOfMyDevice])\n"
            "The"
            "Alternatively, you can use the \'on\' parameter to define the context of the registration.\n\n"+
            FormattedText::formatText(highlighted_text, "For example: \n")+
            FormattedText::formatText(highlighted_text, "❯ ")+"register device name[MyDevice1] address[192.168.1.1] mac[ff-ff-ff-ff-ff-ff]\n"+
            FormattedText::formatText(highlighted_text, "❯ ")+"select device name[MyDevice1]\n"+
            FormattedText::formatText(highlighted_text, "MyDevice1❯ ")+"register system name[MySystem1] port[8080]\n"+
            FormattedText::formatText(highlighted_text, "MyDevice1❯ ")+"select system name[MySystem1]\n"+
            FormattedText::formatText(highlighted_text, "MyDevice1.MySystem1❯ ")+"register service name[MyService1] interfaces[HTTPS-SECURE-JSON] security-type[CERTIFICATE]\n"+
            FormattedText::formatText(highlighted_text, "MyDevice1.MySystem1❯ ")+"orchestrate system name[MyOrchestration] interfaces[HTTPS-SECURE-JSON] service-name[RequestedService]\n"+
            "\n"+
            FormattedText::formatText(highlighted_text, "Alternatively:\n")+
            FormattedText::formatText(highlighted_text, "❯ ")+"register device name[MyDevice1] address[192.168.1.1] mac[ff-ff-ff-ff-ff-ff]\n"+
            FormattedText::formatText(highlighted_text, "❯ ")+"register system name[MySystem1] port[8080] on[MyDevice1]\n"+
            FormattedText::formatText(highlighted_text, "❯ ")+"register service name[MyService1] interfaces[HTTPS-SECURE-JSON] security-type[CERTIFICATE] on[MyDevice1.MySystem1]\n"+
            FormattedText::formatText(highlighted_text, "❯ ")+"orchestrate system name[MyOrchestration] interfaces[HTTPS-SECURE-JSON] service-name[RequestedService] on[MyDevice1.MySystem1]\n"
            "\n"
            "Have fun! ;)",

            /* commands */ {
                {
                    "exit",
                    std::make_shared<SingleCommand>(
                        /* description*/
                        "Use this command to exit from a selection or exit from the program.",

                        /*mandatoryParameters*/ std::map<std::string, ParameterInfo>{},

                        /*furtherParameters*/ std::map<std::string, ParameterInfo>{},

                        /* action */
                        [this](std::map<std::string, Parameter> parameters)->CommandEffect{
                            return controller.exitFromContext() ? CommandEffect::Continue : CommandEffect::Exit;
                        }
                    )
                },
                {
                    "clear",
                    std::make_shared<SingleCommand>(
                        /* description*/
                        "Use this command to clear the console.",

                        /*mandatoryParameters*/ std::map<std::string, ParameterInfo>{},

                        /*furtherParameters*/ std::map<std::string, ParameterInfo>{},

                        /* action */
                        [this](std::map<std::string, Parameter> parameters)->CommandEffect{
                            #if defined _WIN32
                                system("cls");
                            #elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
                                system("clear");
                            #elif defined (__APPLE__)
                                system("clear");
                            #endif

                            std::cout << welcome_message << std::endl;

                            return CommandEffect::Continue;
                        }
                    )
                },
                {
                    "register",
                    std::make_shared<MultipleCommand>(
                        /* description*/
                        "Use this command to register a device, system, or service.",
                        std::map<std::string, std::shared_ptr<Command>>{
                            {
                                "device",
                                std::make_shared<SingleCommand>(
                                    /* Description: */
                                    "Use this command to register a device.",

                                    /* Mandatory parameters: */ std::map<std::string, ParameterInfo>{
                                        {"name",                {Text,      "name of the device to be registered"}},
                                        {"mac",                 {Text,      "mac address of the device to be registered, the correct format is xx-xx-xx-xx-xx-xx"}},
                                        {"address",             {Text,      "address of the device to be registered (IPv4, IPv6 or URL)"}}
                                    },

                                    /* Further parameters: */    std::map<std::string, ParameterInfo>{
                                        {"authentication-info", {Text,      "base64 encoded public key of the certificate for this device"}},
                                        {"end-of-validity",     {Text,      "the device is available until this UTC timestamp"            }},
                                        {"metadata",            {TextMap,   "various meta information as map"                             }},
                                        {"version",             {Number,    "the version of this registry entry"                          }}
                                    },

                                    /* Action: */
                                    [this](std::map<std::string, Parameter> parameters)->CommandEffect{
                                        controller.registerDevice(getRecordParameters<DeviceParam>(parameters));
                                        return CommandEffect::Continue;
                                    }
                                )
                            },
                            {
                                "system",
                                std::make_shared<SingleCommand>(
                                    /* Description: */
                                    "Use this command to register a system.",

                                   /* Mandatory parameters: */ std::map<std::string, ParameterInfo>{
                                       {"name",                {Text, "name of the system to be registered"                    }},
                                       {"port",                {Number, "port under which this system's services are available"}}
                                   },

                                   /* Further parameters: */    std::map<std::string, ParameterInfo>{
                                       {"address",             {Text, "address of the system to be registered (IPv4, IPv6 or URL)"}},
                                       {"authentication-info", {Text, "base64 encoded public key of the certificate this device/system"}},
                                       {"end-of-validity",     {Text, "the system is available until this UTC timestamp"}},
                                       {"metadata",            {TextMap, "various meta information as map"}},
                                       {"version",             {Number, "the version of this registry entry"}},
                                       {"on",                  {Text, "you can define which device you would like to register the system on"}},
                                   },

                                   /* Action: */
                                   [this](std::map<std::string, Parameter> parameters)->CommandEffect{
                                        controller.registerSystem(getRecordParameters<SystemParam>(parameters), getContext(parameters));
                                        return CommandEffect::Continue;
                                   }
                                )
                            },
                            {
                                "service",
                                std::make_shared<SingleCommand>(
                                    /* Description: */
                                     "Use this command to register a service.",

                                     /* Mandatory parameters: */ std::map<std::string, ParameterInfo>{
                                         {"name",                {Text   , "the definition of service to be registered"}},
                                         {"interfaces",          {TextList, "list of the interfaces the service supports (pattern: <protocol>-SECURE/INSECURE-<format>, e.g.: HTTPS-SECURE-JSON)"}},
                                         {"security-type",       {Text   , "the authentication type for the service to be used (TOKEN/CERTIFICATE/NOT_SECURE)"}}
                                     },

                                     /* Further parameters: */    std::map<std::string, ParameterInfo>{
                                         {"uri",                 {Text   , "the URI which the service can be accessed on"}},
                                         {"end-of-validity",     {Text   , "the service is available until this UTC timestamp"}},
                                         {"metadata",            {TextMap, "various meta information as map"}},
                                         {"version",             {Number , "the version of this registry entry"}},
                                         {"on",                  {Text   , "you can define which device and system you would like to register the service on, e.g.: MyDevice.MySystem"}}
                                     },

                                     /* Action: */
                                     [this](std::map<std::string, Parameter> parameters)->CommandEffect{
                                        controller.registerService(getRecordParameters<ServiceParam>(parameters), getContext(parameters));
                                        return CommandEffect::Continue;
                                     }
                                )
                            }
                        }
                    )
                },{
                "select",
                std::make_shared<MultipleCommand>(
                    /* description*/
                    "Use this command to select a device or system to be able to handle it.",
                    std::map<std::string, std::shared_ptr<Command>>{
                        {
                            "device",
                            std::make_shared<SingleCommand>(
                                /* Description: */
                                "Use this command to select a device to be able to handle it.",

                                /* Mandatory parameters: */ std::map<std::string, ParameterInfo>{
                                    {"name",                {Text,"the name of the device to be selected"}}
                                },

                                /* Further parameters: */    std::map<std::string, ParameterInfo>{},

                                /* Action: */
                                [this](std::map<std::string, Parameter> parameters)->CommandEffect{
                                   controller.selectDevice(std::get<Text>(parameters["name"]));
                                   return CommandEffect::Continue;
                                }
                            )
                        },
                        {
                            "system",
                            std::make_shared<SingleCommand>(
                                /* Description: */
                                "Use this command to select a system to be able to handle it.",

                               /* Mandatory parameters: */ std::map<std::string, ParameterInfo>{
                                   {"name",                {Text,"the name of the system to be selected"}},
                               },

                               /* Further parameters: */    std::map<std::string, ParameterInfo>{
                                   {"on",                  {Text,"you can define which device you would like to select the system on"}}
                               },

                               /* Action: */
                               [this](std::map<std::string, Parameter> parameters)->CommandEffect{
                                    controller.selectSystem(std::get<Text>(parameters["name"]), getContext(parameters));
                                    return CommandEffect::Continue;
                               }
                            )
                        }
                    }
                )
                },
                {
                    "unregister",
                    std::make_shared<MultipleCommand>(
                        /* description*/
                        "Use this command to unregister a device, system or service.",
                        std::map<std::string, std::shared_ptr<Command>>{
                            {
                                "device",
                                std::make_shared<SingleCommand>(
                                    /* Description: */
                                    "Use this command to unregister a device.",

                                    /* Mandatory parameters: */ std::map<std::string, ParameterInfo>{
                                        {"name",                {Text,"name of the device to be unregistered"}}
                                    },

                                    /* Further parameters: */    std::map<std::string, ParameterInfo>{},

                                    /* Action: */
                                    [this](std::map<std::string, Parameter> parameters)->CommandEffect{
                                       controller.unregisterDevice(std::get<Text>(parameters["name"]));
                                       return CommandEffect::Continue;
                                    }
                                )
                            },
                            {
                                "system",
                                std::make_shared<SingleCommand>(
                                    /* Description: */
                                    "Use this command to unregister a system.",

                                   /* Mandatory parameters: */ std::map<std::string, ParameterInfo>{
                                       {"name",                {Text,"name of the system to be unregistered"}},
                                   },

                                   /* Further parameters: */    std::map<std::string, ParameterInfo>{
                                        {"on",                  {Text,"you can define which device you would like to unregister the system on"}}
                                    },

                                   /* Action: */
                                   [this](std::map<std::string, Parameter> parameters)->CommandEffect{
                                        controller.unregisterSystem(std::get<Text>(parameters["name"]), getContext(parameters));
                                        return CommandEffect::Continue;
                                   }
                                )
                            },
                            {
                                "service",
                                std::make_shared<SingleCommand>(
                                    /* Description: */
                                     "Use this command to unregister a service.",

                                     /* Mandatory parameters: */ std::map<std::string, ParameterInfo>{
                                         {"name",                {Text,"name of the service to be unregistered"}}
                                     },

                                     /* Further parameters: */   std::map<std::string, ParameterInfo>{
                                        {"on",                  {Text,"you can define which device and system you would like to unregister the service on, e.g.: MyDevice.MySystem"}}
                                     },

                                     /* Action: */
                                     [this](std::map<std::string, Parameter> parameters)->CommandEffect{
                                        controller.unregisterService(std::get<Text>(parameters["name"]), getContext(parameters));
                                        return CommandEffect::Continue;
                                     }
                                )
                            }
                        }
                    )
                },
                {
                    "show",
                    std::make_shared<MultipleCommand>(
                        /* description*/
                        "Use this command to show the data of a device, system or service, or the actual status of the systems.",
                        std::map<std::string, std::shared_ptr<Command>>{
                            {
                                "status",
                                std::make_shared<SingleCommand>(
                                    /* description*/
                                    "Use this command to get the actual status. If some core systems are not available, error messages are shown.",

                                    /*mandatoryParameters*/ std::map<std::string, ParameterInfo>{},

                                    /*furtherParameters*/ std::map<std::string, ParameterInfo>{},

                                    /* action */
                                    [this](std::map<std::string, Parameter> parameters)->CommandEffect{
                                        std::cout << controller.getStatusMessage() << std::endl;
                                        return CommandEffect::Continue;
                                    }
                                )
                            },
                            {
                                "device",
                                std::make_shared<SingleCommand>(
                                    /* Description: */
                                    "Use this command to show the data of a device.",

                                    /* Mandatory parameters: */ std::map<std::string, ParameterInfo>{
                                        {"name",                {Text,"name of the device to show the data of"}}
                                    },

                                    /* Further parameters: */    std::map<std::string, ParameterInfo>{},

                                    /* Action: */
                                    [this](std::map<std::string, Parameter> parameters)->CommandEffect{
                                        showRecord(controller.getDevice(std::get<Text>(parameters["name"])));
                                        return CommandEffect::Continue;
                                    }
                                )
                            },
                            {
                                "system",
                                std::make_shared<SingleCommand>(
                                    /* Description: */
                                    "Use this command to show the data of a system.",

                                   /* Mandatory parameters: */ std::map<std::string, ParameterInfo>{
                                       {"name",                {Text,"name of the system to show the data of"}},
                                   },

                                   /* Further parameters: */   std::map<std::string, ParameterInfo>{
                                       {"on",                  {Text,"you can define which device you would like to show the data of a system on"}}
                                   },

                                   /* Action: */
                                   [this](std::map<std::string, Parameter> parameters)->CommandEffect{
                                        showRecord(controller.getSystem(std::get<Text>(parameters["name"]), getContext(parameters)));
                                        return CommandEffect::Continue;
                                   }
                                )
                            },
                            {
                                "service",
                                std::make_shared<SingleCommand>(
                                    /* Description: */
                                     "Use this command to show the data of a service.",

                                     /* Mandatory parameters: */ std::map<std::string, ParameterInfo>{
                                         {"name",                {Text,"name of the service to show the data of"}}
                                     },

                                     /* Further parameters: */    std::map<std::string, ParameterInfo>{
                                         {"on",                  {Text,"you can define which device and system you would like to show the data of a service on, e.g.: MyDevice.MySystem"}}
                                     },

                                     /* Action: */
                                     [this](std::map<std::string, Parameter> parameters)->CommandEffect{
                                         showRecord(controller.getService(std::get<Text>(parameters["name"]), getContext(parameters)));
                                         return CommandEffect::Continue;
                                     }
                                )
                            },
                            {
                                "settings",
                                std::make_shared<SingleCommand>(
                                    /* Description: */
                                     "Use this command to show the settings.",

                                     /* Mandatory parameters: */ std::map<std::string, ParameterInfo>{},

                                     /* Further parameters: */   std::map<std::string, ParameterInfo>{},

                                     /* Action: */
                                     [this](std::map<std::string, Parameter> parameters)->CommandEffect{
                                        showRecord(controller.getSettings());
                                        return CommandEffect::Continue;
                                     }
                                )
                            },
                            {
                                "devices",
                                std::make_shared<SingleCommand>(
                                    /* Description: */
                                    "Use this command to show data of all (or filtered) devices.",

                                    /* Mandatory parameters: */ std::map<std::string, ParameterInfo>{},

                                    /* Further parameters: */   std::map<std::string, ParameterInfo>{
                                        {"filter",              {Text,"a regex filter which all returned devices should contain"}    },
                                        {"parameters",          {TextList,"the list of device parameters which will be shown on the output"}}
                                    },

                                    /* Action: */
                                    [this](std::map<std::string, Parameter> parameters)->CommandEffect{
                                        std::string filter = parameters.count("filter") ? std::get<Text>(parameters["filter"]) : "";
                                        std::vector<std::string> parameters_list = parameters.count("parameters") ? std::get<TextList>(parameters["parameters"]) : std::vector<std::string>{};

                                        if(parameters_list.size()==1 && parameters_list[0] == "all")
                                            showRecords(controller.getDevices(filter, DeviceParam::getParameters()));
                                        else
                                            showRecords(controller.getDevices(filter, toParameterList<DeviceParam>(parameters_list)));

                                        return CommandEffect::Continue;
                                    }
                                )
                            },
                            {
                                "systems",
                                std::make_shared<SingleCommand>(
                                    /* Description: */
                                    "Use this command to show data of all (or filtered) systems.",

                                    /* Mandatory parameters: */ std::map<std::string, ParameterInfo>{},

                                    /* Further parameters: */   std::map<std::string, ParameterInfo>{
                                        {"filter",              {Text,"a regex filter which all returned systems should contain"}    },
                                        {"parameters",          {TextList,"the list of system parameters which will be shown on the output"}},
                                        {"on",                  {Text,"you can define which device you would like to show the data of the systems on"}    }
                                    },

                                    /* Action: */
                                    [this](std::map<std::string, Parameter> parameters)->CommandEffect{
                                        std::string filter = parameters.count("filter") ? std::get<Text>(parameters["filter"]) : "";
                                        std::vector<std::string> parameters_list = parameters.count("parameters") ? std::get<TextList>(parameters["parameters"]) : std::vector<std::string>{};

                                        if(parameters_list.size()==1 && parameters_list[0] == "all")
                                            showRecords(controller.getSystems(filter, SystemParam::getParameters(), getContext(parameters)));
                                        else
                                            showRecords(controller.getSystems(filter, toParameterList<SystemParam>(parameters_list), getContext(parameters)));
                                        return CommandEffect::Continue;
                                    }
                                )
                            },
                            {
                                "services",
                                std::make_shared<SingleCommand>(
                                    /* Description: */
                                    "Use this command to show data of all (or filtered) services.",

                                    /* Mandatory parameters: */ std::map<std::string, ParameterInfo>{},

                                    /* Further parameters: */   std::map<std::string, ParameterInfo>{
                                        {"filter",              {Text,"a regex filter which all returned services should contain"}    },
                                        {"parameters",          {TextList,"the list of service parameters which will be shown on the output"}},
                                        {"on",                  {Text,"you can define which device and system you would like to show the data of the services on, e.g.: MyDevice.MySystem"}    }
                                    },

                                    /* Action: */
                                    [this](std::map<std::string, Parameter> parameters)->CommandEffect{
                                        std::string filter = parameters.count("filter") ? std::get<Text>(parameters["filter"]) : "";
                                        std::vector<std::string> parameters_list = parameters.count("parameters") ? std::get<TextList>(parameters["parameters"]) : std::vector<std::string>{};

                                        if(parameters_list.size()==1 && parameters_list[0] == "all")
                                            showRecords(controller.getServices(filter, ServiceParam::getParameters(), getContext(parameters)));
                                        else
                                            showRecords(controller.getServices(filter, toParameterList<ServiceParam>(parameters_list), getContext(parameters)));
                                        return CommandEffect::Continue;
                                    }
                                )
                            },
                            {
                                "orchestration",
                                std::make_shared<SingleCommand>(
                                    /* Description: */
                                    "Use this command to show data of an orchestration.",

                                    /* Mandatory parameters: */ std::map<std::string, ParameterInfo>{
                                        {"name",                {Text,"the previously given name of the orchestration (the orchestrations are numbered automatically)"}}
                                    },

                                    /* Further parameters: */    std::map<std::string, ParameterInfo>{
                                        {"on", {Text, "you can define which device and system you would like to show the data of an orchestration on, e.g.: MyDevice.MySystem"}}
                                    },

                                    /* Action: */
                                    [this](std::map<std::string, Parameter> parameters)->CommandEffect{
                                        showRecord(controller.getService(std::get<Text>(parameters["name"]), getContext(parameters)));
                                        return CommandEffect::Continue;
                                    }
                                )
                            },
                            {
                                "orchestrations",
                                std::make_shared<SingleCommand>(
                                    /* Description: */
                                    "Use this command to show all the orechestrations.",

                                    /* Mandatory parameters: */  std::map<std::string, ParameterInfo>{
                                    },

                                    /* Further parameters: */   std::map<std::string, ParameterInfo>{
                                        {"on",                  {Text,"you can define which device and system you would like to show the data of the orchestrations on, e.g.: MyDevice.MySystem"}}
                                    },

                                    /* Action: */
                                    [this](std::map<std::string, Parameter> parameters)->CommandEffect{
                                        std::string filter = parameters.count("filter") ? std::get<Text>(parameters["filter"]) : "";
                                        std::vector<std::string> parameters_list = parameters.count("parameters") ? std::get<TextList>(parameters["parameters"]) : std::vector<std::string>{};

                                        if(parameters_list.size()==1 && parameters_list[0] == "all")
                                            showRecords(controller.getOrchestrations(filter, OrchestrationParam::getParameters(), getContext(parameters)));
                                        else
                                            showRecords(controller.getOrchestrations(filter, toParameterList<OrchestrationParam>(parameters_list), getContext(parameters)));
                                        return CommandEffect::Continue;
                                    }
                                )
                            }
                        }
                    )
                },
                {
                    "set",
                    std::make_shared<MultipleCommand>(
                        /* description*/
                        "Use this command to the set the settings.",
                        std::map<std::string, std::shared_ptr<Command>>{
                            {
                                "settings",
                                std::make_shared<SingleCommand>(
                                    /* Description: */
                                     "Use this command to the set the settings.",

                                     /* Mandatory parameters: */ std::map<std::string, ParameterInfo>{},

                                     /* Further parameters: */   std::map<std::string, ParameterInfo>{
                                         {"onboarding-system-address",   {Text, "IPv4/IPv6 address of the onboarding system"}},
                                         {"onboarding-system-port",      {Number, "port of the onboarding system"}},
                                         {"authentication-method",       {Text, "the authentication method used for Arrowhead core system requests (\'client-certificate\'/\'basic-authentication\')"}},
                                         {"shared-secret",               {Text, "the password for basic-authentication method"}},
                                         {"key-store",                   {Text, "the key store which will be used in the beginning of device onboarding and which must contain an Arrowhead-compatible certificate"}},
                                         {"key-store-password",          {Text, "the password for the given key store"}},
                                         {"private-key",                 {Text, "the private key used for the onboarding process"}},
                                         {"public-key",                  {Text, "the public key used for the onboarding process"}},
                                         {"log-file",                    {Text, "the log file name to write all the logs to, if empty, logs will be written to the standard output"}},
                                         {"log-level",                   {Text, "the log level of the application (\'debug\'/\'info\'/warning\')"}},
                                     },

                                     /* Action: */
                                     [this](std::map<std::string, Parameter> parameters)->CommandEffect{
                                        controller.setSettings(getRecordParameters<SettingsParam>(parameters));
                                        return CommandEffect::Continue;
                                     }
                                )
                            }
                        }
                    )
                },
                {
                    "orchestrate",
                    std::make_shared<MultipleCommand>(
                        /* description*/
                        "Use this command request orchestration for a system.",
                        std::map<std::string, std::shared_ptr<Command>>{
                            {
                                "system",
                                std::make_shared<SingleCommand>(
                                    /* Description: */
                                     "Use this command request orchestration for a system.",

                                     /* Mandatory parameters: */ std::map<std::string, ParameterInfo>{
                                        {"name",                {Text,"the identifier name of the orchestration reponses returned (the response names are numbered from 1)"}},
                                        {"service-name",        {Text, "the definition of service requested"}},
                                        {"interfaces",          {TextList, "list of the interfaces the requested service should support (pattern: <protocol>-SECURE/INSECURE-<format>, e.g.: HTTPS-SECURE-JSON)"}}
                                     },

                                     /* Further parameters: */ std::map<std::string, ParameterInfo>{
                                        {"security-types",      {TextList, "the authentication types for the requested service should use (\'TOKEN\'/\'CERTIFICATE\'/\'NOT_SECURE\')"}},
                                        {"metadata",            {TextMap, "various meta information of the requested service as map"}},
                                        {"version",             {Number, "the version of the requested service registry entry"}},
                                        {"max-version",         {Number, "the maximal version of the requested service registry entry"}},
                                        {"min-version",         {Number, "the minimal version of the requested service registry entry"}},
                                        {"preferred-provider",  {TextMap, "parameters of the preferred provider for the requested service, the keys for this map: \'operator-name\', \'cloud-name\',\'system-name\',\'system-address\',\'system-port\',\'authentication-info\'"}},
                                        {"flags",               {TextList, "the orchestration flags which should be set true for this orchestration (matchmaking/metadataSearch/onlyPreferred/pingProviders/overrideStore/enableInterCloud/triggerInterCloud)"}},
                                        {"on",                  {Text, "you can define which device and system you would like to request the orchestration for, e.g.: MyDevice.MySystem"}}
                                     },

                                     /* Action: */
                                     [this](std::map<std::string, Parameter> parameters)->CommandEffect{
                                        controller.orchestrateSystem(getRecordParameters<OrchestrationRequestParam>(parameters), getContext(parameters));
                                        return CommandEffect::Continue;
                                     }
                                )
                            }
                        }
                    )
                }
            }
        }{
            //ARGUMENTS TO STRINGSTREAM
            for(int i=0; i<argc; i++)
                ss.push_back(argv[i]);

        }
