<a name="mantis"></a>
# Mantis AR Glasses
An open source smart glasses

![alt text](https://i.imgur.com/jm8B8wC.png)

<a name="features"></a>
### Features


- Use a bluetooth multimeter and display the obtained data on the screen of the glasses.
- View the temperatures of electronic circuits with the MLX90640 thermal camera.

With these features, the Mantis AR Glasses wearer will be able to use a multimeter from the glasses and monitor circuit temperatures.

> ***This project has been realized by the students of the department of electronics of the institute CIFP Don Bosco LHII.***


- [ Mantis AR Glasses ](#mantis)
- [ Features ](#features)
- [ How did it start? ](#history)
- [ What do we need? ](#whatweneed)
	- [ List of materials ](#listofmaterials)
	- [ Circuits and connections ](#circuits)
		- [ ST7735 Display ](#st7735)
		- [ MLX90640 ](#mlx90640)
		- [ Touch sensors ](#touchsensors)
		- [ Power management ](#power)
	- [ Software ](#software)
		- [ Install libraries ](#libraries)
		- [ Select the driver of our display ](#selectdriver)
- [ Bugs and error ](#bugserror)
- [ Disclaimer ](#disclaimer)

<a name="history"></a>
# How did it start?

We had the opportunity to test the Vuzix smart glasses and we thought it would be a good idea to do something similar but open source and cheaper.

-poner fotos de las vuzix y nuestras gafas


<a name="whatweneed"></a>
# What do we need?
<a name="listofmaterials"></a>
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

<a name="circuits"></a>
## Circuits and connections
<a name="st7735"></a>
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

<a name="mlx90640"></a>
### MLX90640
![alt text](https://i.imgur.com/uXUjABv.png)

| MLX90640 PIN      | ESP32 PIN |
| :--------- | :-----:|
| GND      | GND                    |
| VIN      | 3V3                       |   
| SCL | 22        |
| SDA      | 21        |   
| PS | GND        |

<a name="touchsensors"></a>
### Touch sensors
![alt text](https://i.imgur.com/2C5GvKA.png)

| TOUCH PIN      | ESP32 PIN |
| :--------- | :-----:|
| GND      | GND                    |
| VIN      | 3V3                       |   
| LEFT | 26        |
| GO BACK      | 14        |   
| SELECT | 27        |
| RIGHT | 25        |

<a name="power"></a>
### Power management
![alt text](https://i.imgur.com/TlzLC9l.png)

| BATTERY PIN      | ESP32 PIN |
| :--------- | :-----:|
| BATT-      | GND                    |
| BAT+      | 3V3                       |   
| VOLTAGE DIVIDER OUTPUT  | 34        |


<a name="software"></a>
## Software

<a name="libraries"></a>
### Install libraries
To install a library in arduino we have to go to :
- sketch->include library->manage libraries
![alt text](https://i.imgur.com/vbDBNBz.png)
And a window will open where we can look for the name of the library:
- TFT_eSPI
![alt text](https://i.imgur.com/Cnmn5hQ.png)

<a name="selectdriver"></a>
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

<a name="bugserror"></a>
## Bugs and error

- When you are inside an option (multimeter, battery, info...), if you click left or right, you move through the menu without the need to click back.
- When you are in the thermal camera, if you click back, the program crashes and you need to reset the glasses.
- To connect the multimeter, the glasses must be switched off, first the multimeter is switched on and then the glasses, otherwise they do not connect.

<a name="disclaimer"></a>
## Disclaimer
![alt text](https://i.imgur.com/LufqfRG.png)

The glasses are not designed for use in devices or situations where there may be physical injury if the software has errors.




