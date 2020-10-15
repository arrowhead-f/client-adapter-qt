
# client-adapter-app-qt
# QArrowheadClientAdapterApp
## User manual
### Overview

_QArrowheadClientAdapterApp_ is a command line tool, which can be used by client devices and systems to get registered into an Arrowhead local cloud, lookup for other Arrowhead-enabled parties within the network, and request orchestration to start communicating with them. The app can be used via two different interfaces:

1. **Interactive terminal:** human operators can directly type in commands and visually check command execution in an interactive way, 
2. **Scripting interface:** the control commands can be invoked programmatically from other processes. In this mode the tool starts, executes the invoked command with the indicated parameters passed in as command line arguments, and then it quits immediately. 

The main functions of _QArrowheadClientAdapterApp_ are the followings:

  - onboarding with name using a preloaded client certificate,
  - system and device registration/deregistration/query,
  - service registration/deregistration,
  - orchestration request,
  - certificate handling.

The supported Arrowhead version is 4.1.3. The documentation of the Arrowhead framework is available [here](https://github.com/arrowhead-f/core-java-spring).

### Basic usage

The basic syntax of the commands is the following:

```bash
<command> <subcommand> <parameter-name1>[<value1>] <parameter-name2>[<value2>] ...
```

For example:

```
> orchestrate system name[MyOrchestration] interfaces[HTTPS-SECURE-JSON] service-name[my-service] on[MyDevice1.MySystem1]
```

The parameter values have one of the following types:
|Parameter type | Syntax|
|--|--|
| **Text**      | ```<parameter-name>[<parameter-value>]``` |
| **Number**    | ```<parameter-name>[<parameter-value>]``` |
| **Map**       | ```<parameter-name>[<key1>:<value1>][<key2>:<value2>]... ``` |
| **List**      | ```<parameter-name>[<value1>][<value2>]...```

A few examples:

  - **Text**:	 ```name[MyFavouriteDevice]```
  - **Number**:	```system-onboarding-port[8435]```
  - **Map**:    ```metadata[property1:value1][property2:value2]```
  - **List**:    ```flags[onlyPreferred][pingProviders]```

### Detailed walkthrough

#### Initialization

When you use the app for the first time, you must set a few settings parameter for the Arrowhead requests to work properly (*set settings*). These are 
- the *address* and *port* of the *Arrowhead Onboarding Controller Core System* ('onboarding-system-address', 'onboarding-system-port'),
- the *authentication method* ('authentication-method') - currently only the client certificate method is supported
- the *keystore*\* and *password* for the onboarding requests ('key-store', 'key-store-password')
- the *private*\* and *public key*\* *pair* for the onboarding process ('private_key', 'public_key')


\* These parameters represent the *file path* or *file name* in the application folder

See the [Set settings](#set-settings) section.

#### Registering the client to the Arrowhead local cloud

Once we have successfully added the parameters for accessing the *Arrowhead Onboarding Controller Core System* using the command *set settings*,  we have the option to integrate our custom system with the desired services in the Arrowhead Local cloud. To do this, we must first register our *Arrowhead Device* (*register device*), on which we can register our *System* (*register system*), and finally our *Services* (*register service*).   

In addition to registering services for the systems, we also have the option to request orchestration (*orchestrate system*).

In order to register a system on a certain device, you have to select that device first (select device name[NameOfMyDevice])
Alternatively, you can use the 'on' parameter to define the context of the registration.

#### An example through a specific case

First of all, we have to initialize our application. For this we have to give the address and port of the Arrowhead Onboarding Core System

Assuming that the system is available on the URI *192.168.1.100:8435*, we can inform our application like this:
```
set settings onboarding-system-address[192.168.1.100] onboarding-system-port[8435]
```
We should then set the authentication method as client-certificate:
```
set settings authentication-method[client-certificate]
```
Now the keystore file is the *keystore.p12* in our project directory, and the corresponding password is *mypassword*:
```
set settings key-store[keystore.p12] key-store-password[mypassword] 
```
We have to give private and public key for the onboarding, which are stored in the files *privateKey.pem* and *publicKey.pem* in our application folder.
```
set settings private-key[privateKey.pem] public-key[publicKey.pem]
```
The previous settings can also be set in one command.

Now let's register a device with *MyDevice1* name, that has the IP *192.168.1.1* and MAC address *f9-a3-f8-bb-32-3d*:
```
> register device name[MyDevice1] address[192.168.1.1] mac[f9-a3-f8-bb-32-3d]
```
To register a system we should select our device:
```
> select device name[MyDevice1]
```
Now on this device we can register a system with name *MySystem1* and port *8080*:
```
MyDevice1> register system name[MySystem1] port[8080]
```
We then select our system:
```
MyDevice1> select system name[MySystem1]
```
And register a service named *MyService1* with the desired parameters:
```
MyDevice1.MySystem1> register service name[MyService1] interfaces[HTTPS-SECURE-JSON] security-type[CERTIFICATE]
```
It is also possible to orchestrate our system with the service *RequestedService* (The requested orchestration data will be later available as *MyOrchestration*):
```
MyDevice1.MySystem1> orchestrate system name[MyOrchestration] interfaces[HTTPS-SECURE-JSON] service-name[RequestedService]
```
Now if the orchestration was successful, its data will be shown when listing all the orchestrations:
```
MyDevice1.MySystem1> show orchestrations
```
If we want, we can unregister everything, and exit from our application:
```
MyDevice1.MySystem1> unregister service name[MyService1]
MyDevice1> unregister system name[MySystem1]
MyDevice1> unregister device name[MyDevice1]
> exit
```

**Alternatively:**
If you do not want to constantly select devices and systems, you have the option to specify the *on* parameter where the command should be issued. You must enter either a device name or a system path in the following format:
'on[\<device-name>. \<system-name>]'.

```
> set settings onboarding-system-address[192.168.1.100] onboarding-system-port[8435]
> set settings authentication-method[client-certificate]
> set settings key-store[keystore.p12] key-store-password[mypassword] 
> set settings private-key[privateKey.pem] public-key[publicKey.pem]
> register device name[MyDevice1] address[192.168.1.1] mac[f9-a3-f8-bb-32-3d]
> register system name[MySystem1] port[8080] on[MyDevice1]
> register service name[MyService1] interfaces[HTTPS-SECURE-JSON] security-type[CERTIFICATE] on[MyDevice1.MySystem1]
> orchestrate system name[MyOrchestration] interfaces[HTTPS-SECURE-JSON] service-name[RequestedService] on[MyDevice1.MySystem1]
> unregister service name[MyService1] on[MyDevice1.MySystem1]
> unregister system name[MySystem1] on[MyDevice1]
> unregister device[MyDevice1]
> exit
```



### Command reference

|Command Name             |Parameters                                                                                                                                                            |
|-------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| **set settings**        | authentication-method, key-store, key-store-password, log-file, log-level, onboarding-system-address, onboarding-system-port, private-key, public-key, shared-secret |
| **register device**     | **address**, **mac**, **name**, authentication-info, end-of-validity, metadata, version                                                                              |
| **register system**     | **name**, **port**, address, authentication-info, end-of-validity, metadata, on, version                                                                             |
| **register service**    | **interfaces**, **name**, **security-type**, end-of-validity, metadata, on, uri, version                                                                             |
| **orchestrate system**  | **interfaces**, **name**, **service-name**, flags, max-version, metadata, min-version, on, preferred-provider, security-types, version                               |
| **select device**       | **name**                                                                                                                                                             |
| **select system**       | **name**, on                                                                                                                                                         |
| **show device**         | **name**                                                                                                                                                             |
| **show settings**       |                                                                                                                                                                      |
| **show status**         |                                                                                                                                                                      |
| **show devices**        | filter, parameters                                                                                                                                      |
| **show system**         | **name**, on                                                                                                                                                         |
| **show systems**        | filter, on, parameters                                                                                                                                               |
| **show service**        | **name**, on                                                                                                                                                         |
| **show services**       | filter, on, parameters                                                                                                                                               |
| **show orchestration**  | **name**, on                                                                                                                                           |
| **show orchestrations** | on                                                                                                                                                                   |
| **unregister device**   | **name**                                                                                                                                                             |
| **unregister system**   | **name**, on                                                                                                                                                         |
| **unregister service**  | **name**, on                                                                                                                                                         |
| **clear**               |                                                                                                                                                                      |
| **exit**                |                                                                                                                                                                      |

:point_up: **Mandatory commands/parameters are bold**


#### Set
Use this command to the set the settings.


##### Set settings
Use this command to the set the settings.

|Parameter                    |Type  |Description                                                                                                                      |
|-----------------------------|------|---------------------------------------------------------------------------------------------------------------------------------|
|**authentication-method**    |text  |the authentication method used for Arrowhead core system requests ('client-certificate'/'basic-authentication')                  |
|**key-store**                |text  |the key store which will be used in the beginning of device onboarding and which must contain an Arrowhead-compatible certificate|
|**key-store-password**       |text  |the password for the given key store                                                                                             |
|**log-file**                 |text  |the log file name to write all the logs to, if empty, logs will be written to the standard output                                |
|**log-level**                |text  |the log level of the application ('debug'/'info'/warning')                                                                       |
|**onboarding-system-address**|text  |IPv4/IPv6 address of the onboarding system                                                                                       |
|**onboarding-system-port**   |number|port of the onboarding system                                                                                                    |
|**private-key**              |text  |the private key used for the onboarding process                                                                                  |
|**public-key**               |text  |the public key used for the onboarding process                                                                                   |
|**shared-secret**            |text  |the password for basic-authentication method                                                                                     |



#### Register
Use this command to register a device, system, or service.


##### Register device
Use this command to register a device.

|Parameter              |Type  |Description                                                                        |
|-----------------------|------|-----------------------------------------------------------------------------------|
|**address**            |text  |address of the device to be registered (IPv4, IPv6 or URL)                         |
|**mac**                |text  |mac address of the device to be registered, the correct format is xx-xx-xx-xx-xx-xx|
|**name**               |text  |name of the device to be registered                                                |
|**authentication-info**|text  |base64 encoded public key of the certificate for this device                       |
|**end-of-validity**    |text  |the device is available until this UTC timestamp                                   |
|**metadata**           |map   |various meta information as map                                                    |
|**version**            |number|the version of this registry entry                                                 |

##### Register system
Use this command to register a system.

|Parameter              |Type  |Description                                                         |
|-----------------------|------|--------------------------------------------------------------------|
|**name**               |text  |name of the system to be registered                                 |
|**port**               |number|port under which this system's services are available               |
|**address**            |text  |address of the system to be registered (IPv4, IPv6 or URL)          |
|**authentication-info**|text  |base64 encoded public key of the certificate this device/system     |
|**end-of-validity**    |text  |the system is available until this UTC timestamp                    |
|**metadata**           |map   |various meta information as map                                     |
|**on**                 |text  |you can define which device you would like to register the system on|
|**version**            |number|the version of this registry entry                                  |

##### Register service
Use this command to register a service.

|Parameter          |Type  |Description                                                                                                        |
|-------------------|------|-------------------------------------------------------------------------------------------------------------------|
|**interfaces**     |list  |list of the interfaces the service supports (pattern: <protocol>-SECURE/INSECURE-<format>, e.g.: HTTPS-SECURE-JSON)|
|**name**           |text  |the definition of service to be registered                                                                         |
|**security-type**  |text  |the authentication type for the service to be used (TOKEN/CERTIFICATE/NOT_SECURE)                                  |
|**end-of-validity**|text  |the service is available until this UTC timestamp                                                                  |
|**metadata**       |map   |various meta information as map                                                                                    |
|**on**             |text  |you can define which device and system you would like to register the service on, e.g.: MyDevice.MySystem          |
|**uri**            |text  |the URI which the service can be accessed on                                                                       |
|**version**        |number|the version of this registry entry                                                                                 |

#### Orchestrate
Use this command request orchestration for a system.


##### Orchestrate system
Use this command request orchestration for a system.

|Parameter             |Type  |Description                                                                                                                                                                            |
|----------------------|------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|**interfaces**        |list  |list of the interfaces the requested service should support (pattern: <protocol>-SECURE/INSECURE-<format>, e.g.: HTTPS-SECURE-JSON)                                                    |
|**name**              |text  |the identifier name of the orchestration reponses returned (the response names are numbered from 1)                                                                                    |
|**service-name**      |text  |the definition of service requested                                                                                                                                                    |
|**flags**             |list  |the orchestration flags which should be set true for this orchestration (matchmaking/metadataSearch/onlyPreferred/pingProviders/overrideStore/enableInterCloud/triggerInterCloud)      |
|**max-version**       |number|the maximal version of the requested service registry entry                                                                                                                            |
|**metadata**          |map   |various meta information of the requested service as map                                                                                                                               |
|**min-version**       |number|the minimal version of the requested service registry entry                                                                                                                            |
|**on**                |text  |you can define which device and system you would like to request the orchestration for, e.g.: MyDevice.MySystem                                                                        |
|**preferred-provider**|map   |parameters of the preferred provider for the requested service, the keys for this map: 'operator-name', 'cloud-name','system-name','system-address','system-port','authentication-info'|
|**security-types**    |list  |the authentication types for the requested service should use ('TOKEN'/'CERTIFICATE'/'NOT_SECURE')                                                                                     |
|**version**           |number|the version of the requested service registry entry                                                                                                                                    |
#### Select
Use this command to select a device or system to be able to handle it.


##### Select device
Use this command to select a device to be able to handle it.

|Parameter|Type|Description                          |
|---------|----|-------------------------------------|
|**name** |text|the name of the device to be selected|

##### Select system
Use this command to select a system to be able to handle it.

|Parameter|Type|Description                                                       |
|---------|----|------------------------------------------------------------------|
|**name** |text|the name of the system to be selected                             |
|**on**   |text|you can define which device you would like to select the system on|

#### Show
Use this command to show the data of a device, system or service, or the actual status of the systems.

##### Show settings
Use this command to show the settings.


##### Show status
Use this command to get the actual status. If some core systems are not available, error messages are shown.

##### Show device
Use this command to show the data of a device.

|Parameter|Type|Description                           |
|---------|----|--------------------------------------|
|**name** |text|name of the device to show the data of|

##### Show devices
Use this command to show data of all (or filtered) devices.

|Parameter     |Type|Description                                                    |
|--------------|----|---------------------------------------------------------------|
|**filter**    |text|a regex filter which all returned devices should contain       |
|**parameters**|list|the list of device parameters which will be shown on the output|

##### Show system
Use this command to show the data of a system.

|Parameter|Type|Description                                                               |
|---------|----|--------------------------------------------------------------------------|
|**name** |text|name of the system to show the data of                                    |
|**on**   |text|you can define which device you would like to show the data of a system on|

##### Show systems
Use this command to show data of all (or filtered) systems.

|Parameter     |Type|Description                                                                  |
|--------------|----|-----------------------------------------------------------------------------|
|**filter**    |text|a regex filter which all returned systems should contain                     |
|**on**        |text|you can define which device you would like to show the data of the systems on|
|**parameters**|list|the list of system parameters which will be shown on the output              |

##### Show service
Use this command to show the data of a service.

|Parameter|Type|Description                                                                                                    |
|---------|----|---------------------------------------------------------------------------------------------------------------|
|**name** |text|name of the service to show the data of                                                                        |
|**on**   |text|you can define which device and system you would like to show the data of a service on, e.g.: MyDevice.MySystem|


##### Show services
Use this command to show data of all (or filtered) services.

|Parameter     |Type|Description                                                                                                       |
|--------------|----|------------------------------------------------------------------------------------------------------------------|
|**filter**    |text|a regex filter which all returned services should contain                                                         |
|**on**        |text|you can define which device and system you would like to show the data of the services on, e.g.: MyDevice.MySystem|
|**parameters**|list|the list of service parameters which will be shown on the output                                                  |




##### Show orchestration
Use this command to show data of an orchestration.

|Parameter|Type|Description                                                                                                           |
|---------|----|----------------------------------------------------------------------------------------------------------------------|
|**name** |text|the previously given name of the orchestration (the orchestrations are numbered automatically)                        |
|**on**   |text|you can define which device and system you would like to show the data of an orchestration on, e.g.: MyDevice.MySystem|

##### Show orchestrations
Use this command to show all the orechestrations.

|Parameter|Type|Description                                                                                                             |
|---------|----|------------------------------------------------------------------------------------------------------------------------|
|**on**   |text|you can define which device and system you would like to show the data of the orchestrations on, e.g.: MyDevice.MySystem|

##### Show service
Use this command to show the data of a service.

|Parameter|Type|Description                                                                                                    |
|---------|----|---------------------------------------------------------------------------------------------------------------|
|**name** |text|name of the service to show the data of                                                                        |
|**on**   |text|you can define which device and system you would like to show the data of a service on, e.g.: MyDevice.MySystem|

#### Unregister
Use this command to unregister a device, system or service.


##### Unregister device
Use this command to unregister a device.

|Parameter|Type|Description                          |
|---------|----|-------------------------------------|
|**name** |text|name of the device to be unregistered|

##### Unregister system
Use this command to unregister a system.

|Parameter|Type|Description                                                           |
|---------|----|----------------------------------------------------------------------|
|**name** |text|name of the system to be unregistered                                 |
|**on**   |text|you can define which device you would like to unregister the system on|

##### Unregister service
Use this command to unregister a service.

|Parameter|Type|Description                                                                                                |
|---------|----|-----------------------------------------------------------------------------------------------------------|
|**name** |text|name of the service to be unregistered                                                                     |
|**on**   |text|you can define which device and system you would like to unregister the service on, e.g.: MyDevice.MySystem|

#### Clear
Use this command to clear the console.


#### Exit
Use this command to exit from a selection or exit from the program.