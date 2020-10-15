# arrowhead-client-cli installation manual

1. Install docker from [here](https://docs.docker.com/get-docker/)

2. Download arrowhead-client-cli docker image from [docker-hub](https://hub.docker.com/r/davidjenei/arrowhead-client-cli)

   ```dockerfile
   docker pull davidjenei/arrowhead-client-cli
   ```

3. Create an empty folder on your host. (In the followings, you will create configuration files for arrowhead-client-cli and store them in this folder. Finally, you will mount this folder to your container.)

   1. Create Arrowhead _client_ key-pair and key store and save them to the config folder you've just created: 
      Follow the instructions available [here](https://github.com/arrowhead-f/core-java-spring#certificates).

   2. Create a json settings file for the client adapter application and save it to the config folder you've created previously. 
      The definition of the required fields can be found in the [user manual](../client-adapter-app-qt/QArrowheadClientAdapterApp/doc/ah_cli_app_user_manual.md) of the client adapter application (see Section "Set settings"). The fields "key-store", "private-key" and "public-key" must point to the appropriate files created in step 3.1. Note that the config folder containing this settings file will be mounted to the "etc" folder relative to the application executable inside the Docker container, so the file path must be "etc/\_filename\_". 

   3. Examples:

      content of the config folder:

         - settings.json (settings file)
         - eitg.p12 (key store)
         - onboardingPrivateKey.pem (private key)
      - onboardingPublicKey.pem (public key)

      settings.json:

      ```json
      {
          "authentication-method": "client certificate",
          "key-store": "etc/eitg.p12",
          "key-store-password": "password",
          "onboarding-system-address": "127.0.0.1",
          "onboarding-system-port": 8435,
          "private-key": "etc/onboardingPrivateKey.pem",
          "public-key": "etc/onboardingPublicKey.pem",
          "shared-secret": "hidden_secret",
          "log-file": "etc/arrowhead-client.log",
         	"log-level": "debug"
      } 
      ```

4. Create container from arrowhead-client-cli image, mount config folder to the new container (mounting point must be: /arrowhead/etc) and run the container:

   ```bash
   $ docker run \
     -it \
     --name arrowhead-client \
     --mount type=bind,source=_path_to_config_folder_on_host,target=/arrowhead/etc \
     davidjenei/arrowhead-client-cli
   ```

   After executing the docker run command presented above, you should get the interactive arrowhead adapter command line interface.

5. Use the arrowhead client adapter app interactively. User manual can be found [here](../client-adapter-app-qt/QArrowheadClientAdapterApp/doc/ah_cli_app_user_manual.md).