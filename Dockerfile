FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        gcc-7 \
        g++-7 \
        git \
        cmake \
        build-essential \
        bluez \
        libbluetooth-dev \
    && rm -f /usr/bin/hcitool \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 100 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 100 && \
    update-alternatives --install /usr/bin/cc  cc  /usr/bin/gcc-7 100 && \
    update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++-7 100

WORKDIR /workspace
COPY . /workspace
CMD ["bash"]
