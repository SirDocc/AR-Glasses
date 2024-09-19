#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BluetoothSerial.h>
//this needs to be checked for a connection

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

#define BT_DISCOVER_TIME 10000
esp_spp_sec_t sec_mask = ESP_SPP_SEC_NONE;  // or ESP_SPP_SEC_ENCRYPT|ESP_SPP_SEC_AUTHENTICATE to request pincode confirmation
esp_spp_role_t role = ESP_SPP_ROLE_SLAVE;   // or ESP_SPP_ROLE_MASTER

// std::map<BTAddress, BTAdvertisedDeviceSet> btDeviceList;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  if (!SerialBT.begin("ESP32test", true)) {
    Serial.println("========== serialBT failed!");
    abort();
  }
  // SerialBT.setPin("1234"); // doesn't seem to change anything
  // SerialBT.enableSSP(); // doesn't seem to change anything

  Serial.println("Starting discoverAsync...");
  BTScanResults *btDeviceList = SerialBT.getScanResults();  // maybe accessing from different threads!
  if (SerialBT.discoverAsync([](BTAdvertisedDevice *pDevice) {
        // BTAdvertisedDeviceSet*set = reinterpret_cast<BTAdvertisedDeviceSet*>(pDevice);
        // btDeviceList[pDevice->getAddress()] = * set;
        Serial.printf(">>>>>>>>>>>Found a new device asynchronously: %s\n", pDevice->toString().c_str());
      })) {
    delay(BT_DISCOVER_TIME);
    Serial.print("Stopping discoverAsync... ");
    SerialBT.discoverAsyncStop();
    Serial.println("discoverAsync stopped");
    delay(5000);
    if (btDeviceList->getCount() > 0) {
      BTAddress addr;
      int channel = 0;
      Serial.println("Found devices:");
      for (int i = 0; i < btDeviceList->getCount(); i++) {
        BTAdvertisedDevice *device = btDeviceList->getDevice(i);
        Serial.printf(" ----- %s  %s %d\n", device->getAddress().toString().c_str(), device->getName().c_str(), device->getRSSI());
        std::map<int, std::string> channels = SerialBT.getChannels(device->getAddress());
        Serial.printf("scanned for services, found %d\n", channels.size());
        for (auto const &entry : channels) {
          Serial.printf("     channel %d (%s)\n", entry.first, entry.second.c_str());
        }
        if (channels.size() > 0) {
          addr = device->getAddress();
          channel = channels.begin()->first;
        }
      }
      if (addr) {
        Serial.printf("connecting to %s - %d\n", addr.toString().c_str(), channel);
        SerialBT.connect(addr, channel, sec_mask, role);
      }
    } else {
      Serial.println("Didn't find any devices");
    }
  } else {
    Serial.println("Error on discoverAsync f.e. not workin after a \"connect\"");
  }
}

void loop() {
    if (!SerialBT.isClosed() && SerialBT.connected()) {
    if (SerialBT.write((const uint8_t *)sendData.c_str(), sendData.length()) != sendData.length()) {
      Serial.println("tx: error");
    } else {
      Serial.printf("tx: %s", sendData.c_str());
    }
    if (SerialBT.available()) {
      Serial.print("rx: ");
      while (SerialBT.available()) {
        int c = SerialBT.read();
        if (c >= 0) {
          Serial.print((char)c);
        }
      }
      Serial.println();
    }
  } else {
    Serial.println("not connected");
  }
  delay(1000);
}

void drawcenterlines(void){ //drawing a debug X for offset finding
  display.drawLine(0, 0, 127, 63, WHITE);
  display.drawLine(127, 0, 0, 63, WHITE);
}

void drawArrowUp(void){ // Draws an arrow up
  int offsetX = 52; //offset X
  int offsetY = 20; //offset Y
  display.fillTriangle(10 + offsetX,  0 + offsetY,
                        0 + offsetX, 10 + offsetY, 
                       20 + offsetX, 10 + offsetY, WHITE);

  display.fillRect(5+offsetX, 10+offsetY, 10, 10, WHITE);
  //Triangle Size: 10, 0, 0, 10, 20, 10
  //Rectangle Size: 5, 10, 10, 10
}

void drawArrowDown(void){ //Draws an arrow down
  int offsetX = 52; //offset X
  int offsetY = 20; //offset Y
  display.fillTriangle(10 + offsetX, 20 + offsetY, 
                        0 + offsetX, 10 + offsetY, 
                       20 + offsetX, 10 + offsetY, WHITE);

  display.fillRect(5+offsetX, 0+offsetY, 10, 10, WHITE);
} 

void drawArrowLeft(void){
  int offsetX = 52; //offset X
  int offsetY = 20; //offset Y
  display.fillTriangle(0 + offsetX, 10 + offsetY, 
                      10 + offsetX,  0 + offsetY, 
                      10 + offsetX, 20 + offsetY, WHITE);

  display.fillRect(10 + offsetX, 5 + offsetY, 10, 10, WHITE);
}

void drawArrowRight(void){
  int offsetX = 52; //offset X
  int offsetY = 20; //offset Y
  display.fillTriangle(20 + offsetX, 10 + offsetY, 
                       10 + offsetX,  0 + offsetY, 
                       10 + offsetX, 20 + offsetY, WHITE);

  display.fillRect(0 + offsetX, 5 + offsetY, 10, 10, WHITE);
}

void textDemo(void){ //showing off Text and Arrows feature via this Demo
  display.setCursor(45,28);
  display.println("Hallo!");
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setCursor(45,28);
  display.println("Banane");
  display.display();
  delay(1000);
  display.clearDisplay();
  drawArrowUp();
  display.display();
  delay(1000);
  display.clearDisplay();
  drawArrowDown();
  display.display();
  delay(1000);
  display.clearDisplay();
  drawArrowLeft();
  display.display();
  delay(1000);
  display.clearDisplay();
  drawArrowRight();
  display.display();
  delay(1000);
  display.clearDisplay();
}

void teleprompter(void){ //Code to scroll up the Text on screen
  display.clearDisplay();
  int pos = 80;
  while(pos > -50){
    display.setCursor(0, pos);
    display.println("BEEMOVIE SCRIPT HELP MEEE");
    display.display();
    delay(100);
    pos = pos - 1;
    display.clearDisplay();
    display.setCursor(0, pos);
    display.println("BEEMOVIE SCRIPT HELP MEEE");
    display.display();
  }
}