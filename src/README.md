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
- printing with freertos on Core1 does *not* work properly. Just don't to avoid isr_hardfault-crashes.

#### debugging
- use the [guide](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf) to setup debugging either with the hardware-box or through another Pi Pico (page 19). 
- use openOCD to flash a binary to the DUT: `openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program pouch_two.elf verify reset exit"`
- to open a debug-session in gdb:
  1. `openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000"`
  2. `gdb pouch_two.elf`
  3. inside gdb, execute `> target remote localhost:3333`
  4. `> monitor reset init`
  5. `> continue`
- other possible commands from inside gdb are: `load`, `b main` and many more. 

##### freertos inside of gdb
- `pip install freertos-gdb` (see https://github.com/espressif/freertos-gdb)
- run inside of gdb: `python import freertos_gdb`

### further literature
- [Connecting to the internet with Pico-W](https://datasheets.raspberrypi.com/picow/connecting-to-the-internet-with-pico-w.pdf)
- [WiFi-Scan demo code](https://github.com/raspberrypi/pico-examples/blob/master/pico_w/wifi/wifi_scan)
- [Multicore blog](https://ghubcoder.github.io/posts/using-multiple-cores-pico-freertos/)
- [Multicore demo code](https://github.com/raspberrypi/pico-examples/blob/master/multicore/multicore_runner_queue/multicore_runner_queue.c)
- [fatfs-library](https://github.com/elehobica/pico_fatfs)
- [TFLite-lib for pico](https://github.com/raspberrypi/pico-tflmicro/)
