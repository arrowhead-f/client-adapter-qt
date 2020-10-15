# arrowhead-client-base installation manual

1. Install docker from [here](https://docs.docker.com/get-docker/)

2. Download arrowhead-client-base docker image from [docker-hub](https://hub.docker.com/r/davidjenei/arrowhead-client-base)

   ```dockerfile
   docker pull davidjenei/arrowhead-client-base
   ```

3. Create container from arrowhead-client-base image and run the container:

   ```bash
   $ docker run \
     -it \
     --name arrowhead-base \
     davidjenei/arrowhead-client-base
   ```

   After executing the docker run command presented above, you should get a bash terminal within the container.

4. The arrowhead _client-adapter-qt_ shared libraries can be found in the following folders:

   - Library files: /usr/local/lib
   - Library header files: /usr/local/include

5. Use the container for linking the libraries to your legacy app. (The documentation of the libraries is available [here](../README.md).)