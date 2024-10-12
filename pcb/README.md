### Hardware configuration

#### before powering the device
Since the 

#### used buses on the PCB

|bus name |acces it via…| used for… |
|--|--|--|
| UART0 | J102 | serial communication, debugging |
| UART1 | J101 | GPS-Chip|
| I²C 0 | J103 | Gyro/Acceleration, Temperature/Environment-Sensors|
| SPI0  | see figure below | SD-Card |
| SPI1  | see figure below | e-Ink-Display |

The wires used for SPI communication are located and marked on the backside of the PCB.
![](assets/spi_access_location.png)

#### power management

TODO:
- [ ] table of power rails, how to configure on PCB
- [ ] document power draw of the components when finished