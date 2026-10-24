FROM ubuntu:20.04

# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install required packages (no bluez / bluez-tools)
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        gcc-7 \
        g++-7 \
        git \
        cmake \
        build-essential \
        libbluetooth-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Make gcc-7 / g++-7 the default compilers
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 100 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 100 && \
    update-alternatives --install /usr/bin/cc  cc  /usr/bin/gcc-7 100 && \
    update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++-7 100

WORKDIR /workspace

COPY . /workspace

# Default entrypoint: bash
CMD ["bash"]