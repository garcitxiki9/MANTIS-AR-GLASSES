


// The remote service UUID we wish to connect to.
#define  UUID_S "0000fff0-0000-1000-8000-00805f9b34fb" 
// The characteristic UUID of the remote service we are interested in=DATA= Handle 9
#define  UUID_C "0000FFF4-0000-1000-8000-00805F9B34FB"  

static   BLEUUID UUID_SERV(UUID_S);
static   BLEUUID UUID_CHAR(UUID_C);

static   boolean b_doConnect = false;
static   boolean b_connected = false;
static   boolean b_doScan    = false;

static   BLERemoteCharacteristic* pRemoteCharacteristic;
static   BLEAdvertisedDevice* myDevice;
int      ai_XOR[] = {0x41,0x21,0x73,0x55,0xa2,0xc1,0x32,0x71,0x66,0xaa,0x3b,0xd0,0xe2};
int      ai_DMM[10];       // Speicher fuer die 10 Bytes des DMM
long     l_ms0;            // Store the beginning of Scan for BLE Devices

String   s_SevSeg="";
String   s_DMM="No connected"; // Value
String   s_PRE="";         // Prefix M k _ m mikro n
String   s_UNIT="";        // 'C 'F V A Ohm Hz
String   s_OPT="";         // Optionen BT,HOLD,DC,AC


int  msb(int lsb)
{
 int mm=128,ml=1,b=0; 
  for(int i=0;i<8;i++){if((lsb & mm)==mm){b+=ml;} mm=mm/2; ml=ml*2;} 
  return b;
}

void digit(int dig)
{
  s_SevSeg+=String(dig,HEX)+" ";
  if(dig>127){s_DMM+=".";}
  int d7=dig & 0x7f;
  if(d7==0)   {s_DMM+=" "; return;}
  if(d7==0x7d){s_DMM+="0"; return;}
  if(d7==0x05){s_DMM+="1"; return;}
  if(d7==0x5b){s_DMM+="2"; return;}
  if(d7==0x1f){s_DMM+="3"; return;}
  if(d7==0x27){s_DMM+="4"; return;}
  if(d7==0x3e){s_DMM+="5"; return;}
  if(d7==0x7e){s_DMM+="6"; return;}
  if(d7==0x15){s_DMM+="7"; return;}
  if(d7==0x7f){s_DMM+="8"; return;}
  if(d7==0x3f){s_DMM+="9"; return;}
  if(d7==0x77){s_DMM+="A"; return;}
  if(d7==0x4c){s_DMM+="u"; return;}
  if(d7==0x6a){s_DMM+="t"; return;}
  if(d7==0x4e){s_DMM+="o"; return;}
  if(d7==0x68){s_DMM+="L"; return;}
  s_DMM+="_"+String(d7,HEX)+"_";
}

void decode_DMM()
{
  s_DMM=""; s_SevSeg=""; s_OPT="BT "; s_UNIT=""; s_PRE="";
  digit(16*(ai_DMM[3]%16)+ai_DMM[4]/16);
  digit(16*(ai_DMM[4]%16)+ai_DMM[5]/16);
  digit(16*(ai_DMM[5]%16)+ai_DMM[6]/16);
  digit(16*(ai_DMM[6]%16)+ai_DMM[7]/16);
  Serial.print(/*s_SevSeg+"=>"+*/s_DMM + s_UNIT);
  
  if(ai_DMM[3] & 0x40){s_OPT+= "HOLD "; }
  
  if(ai_DMM[8] & 0x80){s_PRE+= "nano";  }
  if(ai_DMM[8] & 0x40){s_UNIT+="Volt";  } 
  if(ai_DMM[8] & 0x20){s_OPT+= "( )";   } // ?
  if(ai_DMM[8] & 0x10){s_OPT+= "BT ";   } // ?
  if(ai_DMM[8] & 0x08){s_UNIT+="Farad"; }
  if(ai_DMM[8] & 0x04){s_OPT+= " -|>- ";}
  if(ai_DMM[8] & 0x02){s_OPT+= " :) ";  } // ? Speaker
  if(ai_DMM[8] & 0x01){s_PRE+= "micro"; }
  
  if(ai_DMM[9] & 0x80){s_UNIT+="Ohm";   }
  if(ai_DMM[9] & 0x40){s_PRE+= "kilo";  }
  if(ai_DMM[9] & 0x20){s_PRE+= "milli"; } // ?
  if(ai_DMM[9] & 0x10){s_PRE+= "Mega";  } 
  if(ai_DMM[9] & 0x08){s_UNIT+="Ampere";} // ?
  if(ai_DMM[9] & 0x04){s_UNIT+="Hz";    }
  if(ai_DMM[9] & 0x02){s_UNIT+="'F";    }
  if(ai_DMM[9] & 0x01){s_UNIT+="'C";    }
}

static void notifyCallback
(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify)
{
  //Serial.printf("%8.3f Notify for UUID_C Data[%2d] RAW[]=[",millis()/1000.0,length);
  for(int i=0;i<length;i++){/*Serial.printf("%02X,",pData[i]);*/}
  //Serial.print("] XOR[]=[");
  for(int i=0;i<length;i++){ ai_DMM[i]=msb(pData[i]^ai_XOR[i]); /*Serial.printf("%02X,",ai_DMM[i]);*/}
  //Serial.print("] ");
  decode_DMM();
  //Serial.println();
}

class MyClientCallback : public BLEClientCallbacks
{
  void onConnect(BLEClient* pclient)
  {
    //Serial.println("CallBack.onConnect()");
  }

  void onDisconnect(BLEClient* pclient)
  {
    /*Serial.println("CallBack.onDisconnect()");*/  b_connected = false;
    //digitalWrite(pin_M5_LED,LOW);
  }
};

bool connectToServer()
{
  //Serial.printf("Forming a connection to %s\r\n",myDevice->getAddress().toString().c_str());
  BLEClient*  pClient  = BLEDevice::createClient();
 // Serial.println(" - Created client");
  pClient->setClientCallbacks(new MyClientCallback());
  pClient->connect(myDevice);  // Connect to the remove BLE Server.
  //Serial.println(" - Connected to server");

  BLERemoteService* pRemoteService = pClient->getService(UUID_SERV);
  if (pRemoteService == nullptr)
  {
    //Serial.print("Failed to find our service UUID: ");
    //Serial.println(UUID_SERV.toString().c_str());
    pClient->disconnect();
    return false;
  }
  //Serial.printf(" - Found our service %s\r\n",UUID_S);

  //Serial.printf(" - Check for UUID_C   %s\r\n",UUID_C);
  pRemoteCharacteristic = pRemoteService->getCharacteristic(UUID_CHAR);
  if (pRemoteCharacteristic == nullptr)
  {
    //Serial.println("Failed to find our characteristic UUID: ");
    pClient->disconnect(); return false;
  }
  //Serial.println(" - Found our UUID_C");

  if(pRemoteCharacteristic->canRead()) // Read the value of the characteristic.
  {
    std::string value = pRemoteCharacteristic->readValue(); 
    //Serial.printf("The characteristic value was:%s\r\n",value.c_str());
  }

  if(pRemoteCharacteristic->canNotify())
  {
    //Serial.println("Device can NOTIFY!! Register Notify-Callback for it"); delay(1000);
    pRemoteCharacteristic->registerForNotify(notifyCallback);
  }
  b_connected = true;
}

// Scan for BLE servers and find the first one that advertises the service we are looking for.
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    //Serial.print("BLE Advertised Device found: ");
    //Serial.println(advertisedDevice.toString().c_str());
//    We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(UUID_SERV))
    {
      BLEDevice::getScan()->stop();
      myDevice    = new BLEAdvertisedDevice(advertisedDevice);
      b_doConnect = true;
      b_doScan    = true;
      //Serial.printf("UUID_S found, therefore Scan stopped! after %d ms",millis()-l_ms0);
      //digitalWrite(pin_M5_LED,HIGH); // LED off !!
    }
  }
};

void multimeter_inizialization()
{
 // M5.begin(); pinMode(pin_M5_LED,OUTPUT); digitalWrite(pin_M5_LED,LOW); // LED is lowactiv
  //Serial.println("Starting ESP32 as Bluetooth Low Energie Client ..");
  //Serial.printf("Scan for Service UUID=%s\r\n",UUID_S);
  l_ms0=millis();
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);  // Specify that we want active scanning
  pBLEScan->start(5, false);      // AutoStop SCAN  after 5 seconds.
}

void connecting_multimeter()
{
  if(b_doConnect == true)
  {
    if(connectToServer())
      { /*Serial.println("Now connected to the BLE Server.");*/    }
    else{ /*Serial.println("FAILED to connect to the server."); */}
    b_doConnect = false;
  }
  if(!b_connected & b_doScan)
  { l_ms0=millis(); BLEDevice::getScan()->start(0);}  // SCAN if NOT Connected

}
