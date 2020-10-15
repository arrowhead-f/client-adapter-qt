# Build instructions

## Requirements:

* [docker buildx](https://docs.docker.com/buildx/working-with-buildx/)

## Multiarch(amd64, arm64) build and release:

```
export DOCKER_USER='your-docker-username'
docker login -u $DOCKER_USER
docker buildx build -t ${DOCKER_USER}/arrowhead-client-adapter --platform linux/arm64/v8,linux/amd64 --push .
```

## Test on target arch:

```
root@apalis-imx8:~# docker run ${DOCKER_USER}/arrowhead-client-adapter
********* Start testing of TestCommon *********
Config: Using QtTest library 5.14.2, Qt 5.14.2 (arm64-little_endian-lp64 shared (dynamic) release build; by GCC 9.3.0)
PASS   : TestCommon::initTestCase()
PASS   : TestCommon::typeToString(not secure)
PASS   : TestCommon::typeToString(certificate)
PASS   : TestCommon::typeToString(token)
PASS   : TestCommon::typeToString(invalid)
PASS   : TestCommon::cleanupTestCase()
Totals: 6 passed, 0 failed, 0 skipped, 0 blacklisted, 1ms
********* Finished testing of TestCommon *********
...
```
