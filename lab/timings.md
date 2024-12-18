## findings of 2024-12-01: debugging with gdb and "freertos"-addon
![Task view](asset/freertos_task_view.png)

## findings of 2024-11-21: debugging SD-SPI0:
SPI behaves weirdly with `pico_fatfs`-library.
![weird behaviour](asset/spi_weird_behaviour.png)

=> solved by resoldering

## findings of 2024-11-13
- 10kHz freeRTOS Tick frequency
- 400kHz I²C
#### poll_imu_sensor()

- 534us @ 10kHz tick frequency
- 528us @ 1kHz tick frequency

![](asset/timing_poll_imu_sensor_raw.png)
- channel 0/1 (pink, blue)= I²C bus
- channel 2 (green) = GPIO 22

#### poll_env_sensor()
- 8.6ms @ 10kHz tick frequency
- 3.0ms @ 1kHz tick frequency
I²C-part takes the smallest amount of time, calculations afterwards take multiple milliseconds (!)
![](asset/timing_poll_env_sensor_raw.png)
- channel 0/1 (pink, blue)= I²C bus
- channel 2 (green) = GPIO 22
