FROM ubuntu:24.04

# Install essential tools inside Docker
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        nasm \
        make \
        qemu-system-x86 \
        grub-pc-bin \
        grub-common \
        xorriso \
        wget \
        ca-certificates \
        mtools && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

