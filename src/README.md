### setup of the toolchain
- Make sure all submodules, especially those of the pico-sdk are cloned as well. Try to run `git submodule update --init` inside of the `/src/lib/pico-sdk`-folder to fix this.

- You need to install at least the following packages (see the [SDK-Documentation](https://github.com/raspberrypi/pico-sdk) as well for further essential packages): `gcc gcc-libs jsoncpp arm-none-eabi-gcc arm-none-eabi-newlib`.

- prepare the build-environment using cmake: `mkdir build; cd build; cmake -S .. -B `
- build with `make` inside of the build-directory
- after successful compilation, keep the "BOOTSEL"-button on the Pi Pico pressed and copy the `.uf2`-file from the build-directory onto the microcontroller via your file explorer.
- un/replug the pico/PCB.


### troubleshooting

- `ldconfig` after installing system-side-libraries
- check if freeRTOS is in the right `smp`-branch

### further literature
- [Connecting to the internet with Pico-W](https://datasheets.raspberrypi.com/picow/connecting-to-the-internet-with-pico-w.pdf)
- [WiFi-Scan demo code](https://github.com/raspberrypi/pico-examples/blob/master/pico_w/wifi/wifi_scan)
- [Multicore blog](https://ghubcoder.github.io/posts/using-multiple-cores-pico-freertos/)
- [Multicore demo code](https://github.com/raspberrypi/pico-examples/blob/master/multicore/multicore_runner_queue/multicore_runner_queue.c)
- [fatfs-library](https://github.com/elehobica/pico_fatfs)