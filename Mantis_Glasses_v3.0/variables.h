//  ===========================================================================================================
// ||                                            LIBRARIES AND ICONS                                          ||
//  ===========================================================================================================

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include <Wire.h>
#include "MLX90640_API.h" //github.com/sparkfun/SparkFun_MLX90640_Arduino_Example
#include "MLX90640_I2C_Driver.h"
#include <EEPROM.h>
#include <ESP32Time.h>
#include <BLEDevice.h>
#include <Pangodream_18650_CL.h>
#include "load_screen_160x80.h"
#include "multimeter35x35.h"
#include "thermal_camera35x35.h"
#include "battery35x35.h"
#include "info35x35.h"


//  ===========================================================================================================
// ||                                            SIZE OF OUR DISPLAY                                          ||
//  ===========================================================================================================

#define display_width  160
#define display_height 80
//  ===========================================================================================================
// ||MISO = 2                                                                                                 ||
// ||SCLK = 15                                                                                                ||
// ||CS = 17                                                                                                  ||
// ||DC = 16                                                                                                  ||
// ||RST = 4                                                                                                  ||
//  ===========================================================================================================

//  ===========================================================================================================
// ||                              SPRITES                                                                    ||
//  ===========================================================================================================
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
TFT_eSprite spr_multimeter35x35 = TFT_eSprite(&tft);  
TFT_eSprite spr_thermal_camera35x35 = TFT_eSprite(&tft); 
TFT_eSprite spr_battery35x35 = TFT_eSprite(&tft);
TFT_eSprite spr_info35x35 = TFT_eSprite(&tft);
TFT_eSprite spr_background = TFT_eSprite(&tft); 
 


//  ===========================================================================================================
// ||                              BUTTON COUNTER VARIABLES                                                   ||
//  ===========================================================================================================
byte m_index = 5; // menu index
byte p_index = 5; // previous page
byte c_index = 5; // current page

//  ===========================================================================================================
// ||                             BUTTONS GPIO                                                                ||
//  ===========================================================================================================
const int button_left = 26;
const int button_right = 25;
const int button_select = 27;
const int button_goback = 14;
//  ===========================================================================================================
// ||                              MENU STATES                                                                ||
//  ===========================================================================================================
byte menu_states = 0; //main menu
//  ===========================================================================================================
// ||menu_states = 0 -> main menu                                                                             ||
// || menu_states = 1 -> multimeter                                                                           ||
// || menu_states = 2 -> thermal camera                                                                       ||
// ||menu_states = 3 -> battery info                                                                          ||
// ||menu_states = 4 -> info                                                                                  ||
//  ===========================================================================================================

//  ===========================================================================================================
// ||                             BATTERY LEVEL                                                               ||
//  ===========================================================================================================
Pangodream_18650_CL BL;
float battery_volts = BL.getBatteryVolts();
float battery_level[] = {2.3, 3.0, 3.6};
//ADC 34 pin for read the volts

//  ===========================================================================================================
// ||                     VARIABLES TO CONTROL THE MOVEMENT AND POSITIONING OF SPRITES AND STRINGS            ||
//  ===========================================================================================================
int x_multimeter = 68;
int x_thermalcamera = 140;
int x_battery = 200;
int x_info = 260;

int x_str = 45;
int y_str = 45;

int x_str_info = 0;
int y_str_info = 0;

int x_str_volts = 0;
int y_str_volts = 0;

int x_str_volts_level = 0;
int y_str_volts_level = 0;

int x_str_time_hour = 0;
int y_str_time_hour = 0;

int x_str_time_minutes = 0;
int y_str_time_minutes = 0;

int x_str_time_date = 0;
int y_str_time_date = 0;

int background_color = TFT_BLACK;


//  ===========================================================================================================
// ||                              VARIABLES TO CHANGE THE DISPLAYED INFORMATION                              ||
//  ===========================================================================================================
String str_option = "";
String str_info = "";
String str_volts_number = "";
String str_volts = "";
String str_time_hour = "";
String str_time_minutes = "";
String str_time_date = "";


//  ===========================================================================================================
// ||                              TIME VARIABLES                                                             ||
//  ===========================================================================================================
// ||     Serial.println(rtc.getTime());          //  (String) 15:24:38                                       ||
// ||     Serial.println(rtc.getDate());          //  (String) Sun, Jan 17 2021                               ||
// ||     Serial.println(rtc.getDate(true));      //  (String) Sunday, January 17 2021                        ||
// ||     Serial.println(rtc.getDateTime());      //  (String) Sun, Jan 17 2021 15:24:38                      ||
// ||     Serial.println(rtc.getDateTime(true));  //  (String) Sunday, January 17 2021 15:24:38               ||
// ||     Serial.println(rtc.getTimeDate());      //  (String) 15:24:38 Sun, Jan 17 2021                      ||
// ||     Serial.println(rtc.getTimeDate(true));  //  (String) 15:24:38 Sunday, January 17 2021               ||
// ||     Serial.println(rtc.getMicros());        //  (long)    723546                                        ||
// ||     Serial.println(rtc.getMillis());        //  (long)    723                                           ||
// ||     Serial.println(rtc.getEpoch());         //  (long)    1609459200                                    ||
// ||     Serial.println(rtc.getSecond());        //  (int)     38    (0-59)                                  ||
// ||     Serial.println(rtc.getMinute());        //  (int)     24    (0-59)                                  ||
// ||     Serial.println(rtc.getHour());          //  (int)     3     (0-12)                                  ||
// ||     Serial.println(rtc.getHour(true));      //  (int)     15    (0-23)                                  ||
// ||     Serial.println(rtc.getAmPm());          //  (String)  pm                                            ||
// ||     Serial.println(rtc.getAmPm(true));      //  (String)  PM                                            ||
// ||     Serial.println(rtc.getDay());           //  (int)     17    (1-31)                                  ||
// ||     Serial.println(rtc.getDayofWeek());     //  (int)     0     (0-6)                                   ||
// ||     Serial.println(rtc.getDayofYear());     //  (int)     16    (0-365)                                 ||
// ||     Serial.println(rtc.getMonth());         //  (int)     0     (0-11)                                  ||
// ||     Serial.println(rtc.getYear());          //  (int)     2021                                          ||
// ||     rtc.setTime(20, 43, 21, 27, 1, 2023); //seconds; minutes; hour; day; month; year                    ||
// ============================================================================================================

ESP32Time rtc(0);  // offset in seconds (GMT+1 = 3600)
