### setup of the toolchain
- Make sure all submodules, especially those of the pico-sdk are cloned as well. Try to run `git submodule update --init` inside of the `/src/lib/pico-sdk`-folder to fix this.

- You need to install at least the following packages (see the [SDK-Documentation](https://github.com/raspberrypi/pico-sdk) as well for further essential packages): `gcc gcc-libs jsoncpp arm-none-eabi-gcc arm-none-eabi-newlib`.

- compile the software using cmake: `mkdir build; cd build; cmake -S .. -B .`


### troubleshooting

- `ldconfig` after installing system-side-libraries

### further literature
- [Connecting to the internet with Pico-W](https://datasheets.raspberrypi.com/picow/connecting-to-the-internet-with-pico-w.pdf)