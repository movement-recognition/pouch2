## Base specifications

#### sensors
- The box shall have an IMU, measuring acceleration (`m*s^-2`) and rotations (`°*s^-1`).
- The IMU shall be capable of sampling with least 200Hz sample rate.
- The IMU should have as little jitter as possible.
- The box may use multiple Chips for realizing the IMU-functionality.

- The box should have an atmospheric sensor measuring temperature, humidity.
- The atmospheric sensor may also measure gases.
- The box may have a microphone
- The box shall have the option to communicate other sensors.
- The interface for communicating with other sensors shall be open and well documented. It should support at least one of the following protocols: I²C, UART, SPI. It may also support well known buses (CAN, ASI, IO-Link).

- The box shall have a GPS-receiver. The receiver should support multiple satellite systems (NAVSTAR, GLONASS, Galileo, Beidou).
The gain should be as high as possible for indoor use. 
- The box should have a real-time-clock (RTC). The RTC can be integrated into the GPS-receiver.
- The GPS-receiver shall be able to output location data with at least 0,5Hz. 
- The GPS-receiver may have a power saving mode whilst not losing it's fix.

#### connectivity
- The box shall be able to communicate with the Internet. 
- Internet access (TCP/IP) shall be provided via `IEEE 802.11b,g,n`. Newer standards like `ac`, `ax`, `ad` may be used instead.
- Internet access (TCP/IP) shall be provided through a wireless broadband connection. The device should use at least LTE or newer generations. The device may use older standards as GSM or UMTS.
- The box may also send data packets through LoRaWAN. If realized, the box shall use the european SRD-Band. Usage of the North-american or Asian frequency bands should be possible. 

- The box shall be able to communicate with a 1R-Server, directly or indirectly. For indirect communication, the box communicates with a seperate server which receives those messages and forwards them to the 1R-Server. The server may reformulate (e.g. decompress, reformulate) the data packets or add some to comply with the 1R-standard.
- Sufficient signal power for the protocols will be availiable in the environment.

#### hardware, processing
- The box shall power itself. It will need a battery.
- The battery inside the box shall be big enough to power the device for at least 7 days.  
- The battery may not have a charging circuit on board. The battery can be charged with a seperate device.
- The box shall be able to store the aquired data (IMU, atmospheric and other sensors) at their corresponding design rates for at least 7 days. The storage space can be bigger.
- The box shall be able to process captured data (live and from the storage) at least in a 2-second interval. The processing consists of multiple FFTs (Fast fourier transformation) on the captured data and calculation of multiple statistical features (mean, median, min, max, about 20 features in total, calculated for each IMU-channel seperately). 
- The box should also be able to run the data through a small neuronal network (EdgeAI). This process will not run all the time. Exact specifications are not known yet.

- The box shall have a casing. It should be stable enough to withstand typical loads/stresses in the logistics context.
- The box should be shaped like a rectangular cuboid.
- The box should consume less than 1 Liter in volume.
- The box shall have a simple user interface (UI). 
- The UI shall enable the user to start/stop data recording sessions.
- The UI shall give feedback to the user if a recording is running.
- The UI shall give feedback to the user if an error occured. 
- The UI may use a display or similar to show more detailed information.
- A display can fulfill the user-feedback-requirements.
- The box can have a buzzer/loudspeaker to send out acoustical markers. 
- A buzzer/loudspeaker can fulfill the user-feedback-requirements. 


#### software

- The box shall run custom software. Custom software may be replaced by a highly configurable/scriptable system. It shall support realization of custom detection, event and EdgeAI-features.
- The software shall be expandable for future applications. 


## Design choices
| criterion | reasoning |
| --------- | --------- |
| base-sensors | see research of Pouch1. 200 Hz sample-rate are deviated from |
| connectivity | planned integration into 1R-ecosystem |
| ui-features  | experience from testing pouch1 |
| edge-ai capability | future use |
| battery life | maximal duration of a testing session |