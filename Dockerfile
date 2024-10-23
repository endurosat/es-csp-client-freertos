FROM ubuntu
ARG DEBIAN_FRONTEND=noninteractive
MAINTAINER "Iliya Iliev"
# Install required packages
RUN apt-get update && \
     apt-get install --no-install-recommends --no-install-suggests -y build-essential git clang-14 cmake gcc-arm-none-eabi build-essential gdb-multiarch libnewlib-arm-none-eabi pip

# Build
RUN mkdir /home/esclient_freertos && \
     cd /home/esclient_freertos && \
     git config --global http.sslVerify false && \
     git clone https://github.com/endurosat/es-csp-client-freertos.git && \
     cd es-csp-client-freertos && \
     cmake -B ./build -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain.cmake && \
     cmake --build ./build --config Release

# Build Docs
RUN cd /home/esclient_freertos/es-csp-client-freertos && \
     cd docs && \
     pip3 install -r requirements.txt --break-system-packages && \
     cmake -B ./build && \
     cmake --build ./build

ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib64/
RUN ldconfig

# Set working directory
# WORKDIR /home/esclient_freertos
CMD /bin/bash
