docker build -t i386-kernel-env-full .
docker run --rm -it \
    -v $(pwd):/workspace \
    -w /workspace \
    i386-kernel-env-full \
    bash -c "
        mkdir -p iso/boot/grub && \
        cp build/kernel.bin iso/boot/ && \
        echo 'set timeout=0
menuentry \"My Kernel\" {
    multiboot /boot/kernel.bin
    boot
}' > iso/boot/grub/grub.cfg && \
        grub-mkrescue -o kernel.iso iso
    "

