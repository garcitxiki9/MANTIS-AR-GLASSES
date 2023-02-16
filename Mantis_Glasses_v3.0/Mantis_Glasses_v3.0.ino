#include "variables.h"
#include "ble_multimeter.h"
#include "mlx90640_cam.h"

void setup() {
  //Serial.begin(115200);// only for test
  //Set the data manually without the RTC module
  //rtc.setTime(15, 30, 10, 9, 2, 2023); //seconds; minutes; hour; day; month; year  
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  
  display_configuration();
  spr_creation();
  multimeter_inizialization();
  mlx90640_inizialization();
  //declare the buttons as inputs
  pinMode(button_left, INPUT_PULLUP);
  pinMode(button_right, INPUT_PULLUP);
  pinMode(button_select, INPUT_PULLUP);
  pinMode(button_goback, INPUT_PULLUP);
}

void loop() {
  //debugging(); //only for test (this will print the variables states (buttons and menu_states) on serial monitor
  main_page();//all the menu and submenus
  connecting_multimeter();
}

//  ===========================================================================================================
// ||                                         CONFIGURATION OF OUR DISPLAY                                    ||
//  ===========================================================================================================

void display_configuration() {
  tft.init();
  tft.setRotation(3); //landscape = 3
  tft.writecommand(TFT_MADCTL); //access to display memory
  tft.writedata(0xE0); // send data to change to mirror effect
  tft.fillScreen(TFT_BLACK);
  //loading screen
  tft.setSwapBytes(true);
  tft.pushImage(0, 0, 160, 80,  load_page_160x80);
  delay(1250);
  //tft.fillScreen(TFT_BLACK);
}
//  ===========================================================================================================
// ||                                         CREATING SPRITES                                                ||
//  ===========================================================================================================

//By creating sprites, we avoid screen flickering
void spr_creation() {
  spr_background.createSprite(display_width, display_height);
  //spr_mlx90640_background.createSprite(display_width, display_height);
  spr_thermal_camera35x35.createSprite(35, 35);
  spr_multimeter35x35.createSprite(35, 35);
  spr_battery35x35.createSprite(35, 35);
  spr_info35x35.createSprite(35, 35);

  //To correctly display the sprites, we set setSwapBytes to true
  spr_multimeter35x35.setSwapBytes(true);
  spr_thermal_camera35x35.setSwapBytes(true);
  spr_battery35x35.setSwapBytes(true);
  spr_info35x35.setSwapBytes(true);

}

//  ===========================================================================================================
// ||                                  READING BUTTONS TO INCREASE OR DICREASE THE VARIABLE                   ||
//  ===========================================================================================================

void buttons() {
  // read the state of the pushbutton value:
  byte left_state = digitalRead(button_left);
  byte right_state = digitalRead(button_right);

  // check if the pushbutton is pressed. If it is, the button state is HIGH:
  if ((left_state == HIGH) && (right_state == LOW)) {
    p_index = m_index;
    m_index--;
    delay(190);
  }
  if ((right_state == HIGH) && (left_state == LOW)) {
    p_index = m_index;
    m_index++;
    delay(190);
  }
  if( (m_index <= 0)) {
    m_index = 5;
  }
  else if ((m_index > 5)) {
    m_index = 1;
  }
}


//  ===========================================================================================================
// ||                      CALLING THIS WILL SELECT THE ICON AND CHANGE THE SCREEN                            ||
//  ===========================================================================================================

void select_option(int index) {
  // read the state of the pushbutton value:
  byte left_state = digitalRead(button_left);
  byte right_state = digitalRead(button_right);
  byte select_state = digitalRead(button_select);
  byte goback_state = digitalRead(button_goback);

  c_index = index;
  if(p_index != c_index) {
    p_index = c_index;
  }
  //select multimeter
  if(index == 1) {
    x_multimeter = 68;
    x_thermalcamera = 140;
    x_battery = 180;
    x_info = 180;
    str_info = "";
    str_time_hour = "";
    str_time_minutes = "";
    str_time_date = "";
    str_volts = "";
    str_volts_number = "";
    str_option = "Use a blueooth multimeter and check the voltages on the display";
    y_str = 45;
    x_str--;
    if (x_str < -500){
      x_str = 160;
    }
    //if you push select button, the menu_states will change to 1
    if (select_state == HIGH){
      menu_states = 1;
    }
    if (menu_states == 1){
      multimeter_page();
    }
    //if you push goback button, the menu_states will change to 0
    if (goback_state == HIGH){
      menu_states = 0;
    }
    if (menu_states == 0){
      index = 1;
    }

  }
  //select thermal camera
  else if(index == 2) {
    x_multimeter = -10;
    x_thermalcamera = 68;
    x_battery = 140;
    x_info = 180;
    str_info = "";
    str_time_hour = "";
    str_time_minutes = "";
    str_time_date = "";
    str_volts = "";
    str_volts_number = "";
    str_option = "Use the MLX90640 to check temperatures of the circuit";
    y_str = 45;
    x_str--;
    if (x_str < -400){
      x_str = 160;
    }
    //if you push select button, the menu_states will change to 2
    if (select_state == HIGH){
      tft.fillScreen(TFT_BLACK);
      spr_background.deleteSprite();
      menu_states = 2;
    }
    if (menu_states == 2){
      thermalcam_page();
    }
    //if you push goback button, the menu_states will change to 0
    if (goback_state == HIGH){
      menu_states = 0;
    }
    if (menu_states == 0){
      index = 2;
    }
  }
  //select battery icon
  else if(index == 3) {
    x_multimeter = -40;
    x_thermalcamera = -10;
    x_battery = 65;
    x_info = 140;
    y_str = 45;
    str_info = "";
    str_time_hour = "";
    str_time_minutes = "";
    str_time_date = "";
    str_volts = "";
    str_volts_number = "";
    str_option = "Check the battery charge";
    x_str--;
    if (x_str < -200){
      x_str = 160;
    }
    if (select_state == HIGH){
      menu_states = 3;
    }
    if (menu_states == 3){
      battery_page();
    }
    if (goback_state == HIGH){
      menu_states = 0;
    }
    if (menu_states == 0){
      index = 3;
    }
  }
  //info
  else if(index == 4) {
    x_multimeter = -120;
    x_thermalcamera = -120;
    x_battery =  -10;
    x_info =  65;
    y_str = 45;
    str_info = "";
    str_time_hour = "";
    str_time_minutes = "";
    str_time_date = "";
    str_volts = "";
    str_volts_number = "";
    str_option = "Read about this project";
    x_str--;
    if (x_str < -200){
      x_str = 160;
    }
    if (select_state == HIGH){
      menu_states = 4;
    }
    if (menu_states == 4){
      info_page();
    }
    if (goback_state == HIGH){
      menu_states = 0;
    }
    if (menu_states == 0){
      index = 4;
    }
  }
  //menu item not selected
  else if(index == 5) {
    x_multimeter = 145;
    x_thermalcamera = 180;
    x_battery = 180;
    x_info =  180;
    x_str = 30;
    y_str = 60;
    x_str_time_hour = 25;
    y_str_time_hour = 10;
    x_str_time_minutes = 50;
    y_str_time_minutes = 10;
    x_str_time_date = 15;
    y_str_time_date = 35;
    str_volts = "";
    str_volts_number = "";
    str_info = "";
    str_option = "MANTIS GLASSES";
    str_time_hour = rtc.getHour(true);
    str_time_minutes = rtc.getMinute();
    str_time_date = rtc.getDate();
  }

  if(menu_states == 1 || menu_states == 2 || menu_states == 3 || menu_states == 4){
    x_str_time_hour = 800;
    y_str_time_hour = 800;
    x_str_time_minutes = 800;
    y_str_time_minutes = 800;
    x_str_time_date = 800;
    y_str_time_date = 800;
  }
  
  
}

//  ===========================================================================================================
// ||                                   FUNCTION TO DRAW MAIN PAGE                                            ||
//  ===========================================================================================================

void main_page() {

  buttons();
  select_option(m_index);
  spr_background.fillSprite(background_color);
  spr_multimeter35x35.pushImage(0, 0, 35, 35,  multimeter_35x35);
  spr_thermal_camera35x35.pushImage(0, 0, 35, 35,  thermal_camera_35x35);
  spr_battery35x35.pushImage(0, 0, 35, 35,  battery_35x35);
  spr_info35x35.pushImage(0, 0, 35, 35,  info_35x35);

  spr_multimeter35x35.pushToSprite(&spr_background, x_multimeter, 5, TFT_BLACK);
  spr_thermal_camera35x35.pushToSprite(&spr_background, x_thermalcamera, 5, TFT_BLACK);
  spr_battery35x35.pushToSprite(&spr_background, x_battery, 5, TFT_BLACK);
  spr_info35x35.pushToSprite(&spr_background, x_info, 5, TFT_BLACK);
  
  spr_background.drawString(str_option, x_str, y_str, 2);
  spr_background.drawString(str_info, x_str_info, y_str_info, 2);
  //spr_background.drawString(str_time_hour, x_str_time_hour, y_str_time_hour, 2);
  //spr_background.drawString(str_time_minutes, x_str_time_minutes, y_str_time_minutes, 2);
  //spr_background.drawString(str_time_date, x_str_time_date, y_str_time_date, 2);

 //EXPERIMENTO
 
  //spr_background.drawString(&timeinfo, "%H", x_str_time_hour, y_str_time_hour, 2);
  spr_background.drawString("", x_str_time_minutes, y_str_time_minutes, 2);
  spr_background.drawString(str_time_date, x_str_time_date, y_str_time_date, 2);

  
  spr_background.drawString(str_volts_number, x_str_volts, y_str_volts, 2);
  spr_background.drawString(str_volts, x_str_volts_level, y_str_volts_level, 2);

  spr_background.pushSprite(0, 0);
}

//  ===========================================================================================================
// ||                                   PAGES OF OUR MENU                                                     ||
//  ===========================================================================================================

void multimeter_page() {
  x_multimeter = 600;
  x_thermalcamera = 600;
  x_battery = 600;
  x_info =  600;
  x_str = 20;
  str_option = s_DMM;
}

void thermalcam_page() {
  x_multimeter = 600;
  x_thermalcamera = 600;
  x_battery = 600;
  x_info =  600;
  x_str = 30;
  str_option = "";

  
String myT;
uint16_t mlx90640Frame[834];

    MLX90640_GetFrameData(MLX90640_address, mlx90640Frame);
    float vdd = MLX90640_GetVdd(mlx90640Frame, &mlx90640);
    float Ta = MLX90640_GetTa(mlx90640Frame, &mlx90640);
    float tr = Ta - TA_SHIFT;
    float emissivity = 0.95;
    MLX90640_CalculateTo(mlx90640Frame, &mlx90640, emissivity, tr, mlx90640To);

  for (int i1 = 0 ; i1 < 32 ; i1++) {
   for (int i2 = 0 ; i2 < 24 ; i2++)  {
    float tT = mlx90640To[i1+i2*32];
    float mycolor;
    
    //get Iron Palette 
    mycolor = 0;//Black
    mycolor = (tT>=30?RGB565(0x20, 0x00, 0x8C):mycolor);//Blue
    mycolor = (tT>=80?RGB565(0xCC, 0x00, 0x77):mycolor);//Purple
    mycolor = (tT>=130?RGB565(0xFF, 0xD7, 0x00):mycolor);//Gold
    mycolor = (tT>=180?RGB565(0xFF, 0xFF, 0xFF):mycolor);//White

    tft.fillRect(1+i1*3, tft.height()-i2*3, 3,3, mycolor);
    if (i1+i2*32==768/2+15) {myT=String(tT,0); tft.fillRect(1+i1*3, tft.height()-i2*3, 3,3, RGB565(0x00, 0x00, 0xff));} //Center
  }
 }

  tft.fillRect(100,1, 60,60, TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(100, 30);
  tft.print(myT+"C");
  
}

void battery_page() {
  x_multimeter = 600;
  x_thermalcamera = 600;
  x_battery = 600;
  x_info =  600;
  y_str_info = 600;
  x_str = 600;
  x_str_volts = 5;
  y_str_volts = 30;
  x_str_volts_level = 5;
  y_str_volts_level = 15;
  if (battery_volts >= battery_level[2]){
    str_volts = "High charge level";
    str_volts_number = battery_volts;
    
    }
  else if (battery_volts < battery_level[2] && battery_volts > battery_level[1]){
    str_volts = "Medium charge level";
    str_volts_number = battery_volts;
    }
  else if (battery_volts < battery_level[1] && battery_volts > battery_level[0]){
    str_volts = "Low charge level";
    str_volts_number = battery_volts;
    }
  
}
void info_page() {
  x_multimeter = 600;
  x_thermalcamera = 600;
  x_battery = 600;
  x_info =  600;
  y_str_info = 10;
  str_option = "";
  str_info = "This project has been made by the students of the Electronics Department of Don Bosco";
  x_str_info--;
  if (x_str_info < -630){
    x_str_info = 160;
  }
}


//  ===========================================================================================================
// ||                              PRINT INFORMATION OF THE VARIABLES IN SERIAL MONITOR                       ||
//  ===========================================================================================================

void debugging() {
  /*
  byte left_state = digitalRead(left);
  byte right_state = digitalRead(right);
  byte select_state = digitalRead(select);
  byte goback_state = digitalRead(goback);
  Serial.println("");
  Serial.println("left\tright\tselect\tgoback\tm_index\tp_index\tc_index\tmenu_states");
  Serial.print(left_state);  Serial.print("\t");  Serial.print(right_state);  Serial.print("\t");  Serial.print(select_state); Serial.print("\t");  Serial.print(goback_state); Serial.print("\t");  Serial.print(m_index); Serial.print("\t");  Serial.print(p_index);  Serial.print("\t");  Serial.print(c_index); Serial.print("\t");  Serial.print(menu_states);
  delay(100);
  */
}
