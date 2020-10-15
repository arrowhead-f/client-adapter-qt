FROM alpine AS qtbuilder

RUN apk update && apk upgrade \
  && apk add --update alpine-sdk qt5-qtbase-dev \
  && rm -rf /var/cache/apk/*

WORKDIR /arrowhead

COPY . .

RUN qmake-qt5 -r && make

FROM alpine

RUN apk update && apk upgrade \
  && apk add --update qt5-qtbase \
  && rm -rf /var/cache/apk/*

WORKDIR /arrowhead

# TODO: variables
COPY --from=0 /arrowhead/lib/libQArrowheadClientCommon.so.1.0.0 \
              /arrowhead/lib/libQArrowheadClientDeviceAdapter.so.1.0.0 \
              /arrowhead/lib/libQArrowheadClientSystemAdapter.so.1.0.0 /usr/local/lib/

COPY --from=0 /arrowhead/client-common-lib-qt/QArrowheadClientCommon/src/*.h /usr/local/include/QArrowheadClientCommon/
COPY --from=0 /arrowhead/client-system-adapter-lib-qt/QArrowheadClientSystemAdapter/src/*.h /usr/local/include/QArrowheadClientSystemAdapter/
COPY --from=0 /arrowhead/client-device-adapter-lib-qt/QArrowheadClientDeviceAdapter/src/*.h /usr/local/include/QArrowheadClientDeviceAdapter/

RUN ln -s libQArrowheadClientCommon.so.1.0.0 /usr/local/lib/libQArrowheadClientCommon.so && \
    ln -s libQArrowheadClientCommon.so.1.0.0 /usr/local/lib/libQArrowheadClientCommon.so.1 && \
    ln -s libQArrowheadClientCommon.so.1.0.0 /usr/local/lib/libQArrowheadClientCommon.so.1.0 && \
    ln -s libQArrowheadClientDeviceAdapter.so.1.0.0 /usr/local/lib/libQArrowheadClientDeviceAdapter.so && \
    ln -s libQArrowheadClientDeviceAdapter.so.1.0.0 /usr/local/lib/libQArrowheadClientDeviceAdapter.so.1 && \
    ln -s libQArrowheadClientDeviceAdapter.so.1.0.0 /usr/local/lib/libQArrowheadClientDeviceAdapter.so.1.0 && \
    ln -s libQArrowheadClientSystemAdapter.so.1.0.0 /usr/local/lib/libQArrowheadClientSystemAdapter.so && \
    ln -s libQArrowheadClientSystemAdapter.so.1.0.0 /usr/local/lib/libQArrowheadClientSystemAdapter.so.1 && \
    ln -s libQArrowheadClientSystemAdapter.so.1.0.0 /usr/local/lib/libQArrowheadClientSystemAdapter.so.1.0
