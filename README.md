# Mantis AR Glasses
An open source smart glasses

![alt text](https://i.imgur.com/jm8B8wC.png)

### Features


- Use a bluetooth multimeter and display the obtained data on the screen of the glasses.
- View the temperatures of electronic circuits with the MLX90640 thermal camera.

With these features, the Mantis AR Glasses wearer will be able to use a multimeter from the glasses and monitor circuit temperatures.

> ***This project has been realized by the students of the department of electronics of the institute CIFP Don Bosco LHII.***


# How did it start?

We had the opportunity to test the Vuzix smart glasses and we thought it would be a good idea to do something similar but open source and cheaper.

-poner fotos de las vuzix y nuestras gafas


# What do we need?

## List of materials
Here is a detailed list of what we need to build our own smart glasses

- Microcontroller board (e.g. Arduino or ESP32)
- Display module (e.g. OLED or TFT)
- Micro-USB cable for programming and powering the board
- Lithium-ion battery or power bank
- 3D printer to print the frame
- Light reflective material to display the screen
- Power management circuit
- Wi-Fi or Bluetooth module
- Sensors (e.g. accelerometer, gyroscope, proximity)
- Buttons or touchpad for user input
- Wiring and cables to connect the components
- A software development kit and libraries to program the glasses.

In our case, we will use an ESP32 because it already has integrated WiFi and Bluetooth, and our sensor will be a thermal camera and a TFT RGB IPS screen because it offers higher resolution for using the thermal camera.


## Circuits and connections
### ST7735 Display
![alt text](https://i.imgur.com/MXfNOGr.png)

| ST7735 PIN      | ESP32 PIN |
| :--------- | :-----:|
| GND      | GND                    |
| VCC      | 3V3                       |   
| SCL | 15        |
| SDA      | 2        |   
| RES | 4        |
| DC      | 16        |   
| CS | 17        |
| BLK | No connected        |

### MLX90640
![alt text](https://i.imgur.com/uXUjABv.png)

| MLX90640 PIN      | ESP32 PIN |
| :--------- | :-----:|
| GND      | GND                    |
| VIN      | 3V3                       |   
| SCL | 22        |
| SDA      | 21        |   
| PS | GND        |

### Touch sensors

### Power management

## Software
### Install libraries
To install a library in arduino we have to go to :
- sketch->include library->manage libraries
![alt text](https://i.imgur.com/vbDBNBz.png)
And a window will open where we can look for the name of the library:
- TFT_eSPI
![alt text](https://i.imgur.com/Cnmn5hQ.png)

### Select the driver of our display
Once we have installed the TFT_eSPI library, we need to locate the library folder, which is located in the Arduino folder.
`C:\Users\Carlos\This PC\Documents\Arduino\libraries\TFT_eSPI`
Next, we need to edit the file named
`User_Setup_Select.h`
By default, it will contain the line
`#include <User_Setup.h>`
We need to comment out this line and locate the driver for our screen, in our case
`#include <User_Setups/Setup43_ST7735.h>`
and uncomment it.

It should look something like this:

```c++
...
//#include <User_Setup.h>           // Default setup is root library folder
...
#include <User_Setups/Setup43_ST7735.h>            // Setup file for ESP8266 & ESP32 configured for my ST7735S 80x160
...
```
