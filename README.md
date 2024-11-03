# SMbus bq20z9xx reader
Reading data from a bq20z9xx smart battery controller via the SMBus (version 1.1) protocol. Using an ESP32
The serial output can be displayed via an Ansi terminal (f.e. putty)
# Putty Settings
![putty_screen_1](pictures/putty_screen_1.jpg)

![putty_screen_1](pictures/putty_screen_2.jpg)
# Connections
GND
SDA (SDI) (GPIO21)
SCL (SCK) (GPIO22)
The power of the bq20z9xx is supplied by the SCL line of the I2C. No need for additional power line. Wait 10 seconds after connectting before reading data. Use 4.7K or 10k resistors as pull up for SDA and SCL lines.

