#include <Wire.h>
#include <math.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
BLEDescriptor *pDescr;
BLE2902 *pBLE2902_1;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


void writemytext (String com){
  display.clearDisplay();
  display.display();
  display.setCursor(0,28);
  display.println(com);
  display.display();
}

void scrollpls (int time){
  display.startscrollleft(0x00, 0x0F);
  delay(time);
  display.stopscroll();
}

  void scrollmytext(String com){

    display.clearDisplay();
    display.display();
    display.setCursor(0,28);
    int stringAmount = com.length() % 20;
    Serial.println("COMAMOUNT: " + com.length());
    Serial.println("STRINGAMOUNT: " + stringAmount);
    stringAmount = floor(stringAmount);
    Serial.println("STRINGAMOUNT: " + stringAmount);

    switch(stringAmount){
      case 1:
        String line1 = com.substring(0,20);
        Serial.println("Line1: " + line1);
        String line2 = com.substring(21,com.length());
        Serial.println("Line2: " + line2);
        writemytext(line1);
        scrollpls(8000);
        writemytext(line2);
        scrollpls(8000);
      break;
    }
  }

void onwritelogic(){
  Serial.begin(115200);
  String command = pCharacteristic->getValue();
  if (command == "demo"){
    pCharacteristic->setValue("Displaying Demo");
    writemytext("Displaying Demo");
    delay(2000);
    textDemo();
    delay(2000);
    pCharacteristic->setValue("Waiting for command");
    writemytext("Waiting for command");
  }
  else if (command == "comoot"){
    pCharacteristic->setValue("Work in Progress");
    writemytext("Work in Progress");
  }
  else if (command.length() > 21){
    scrollmytext(command);
  }
  else {
    writemytext(command);
  }
}

class CharacteristicCallBack: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pChar) override {
    String pChar2_value_stdstr = pChar -> getValue();
    String pChar2_value_string = String(pChar2_value_stdstr.c_str());
    Serial.println(pChar2_value_string);
    onwritelogic();
  }
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  BLEDevice::init("Arons TestServer owo");
  pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic =
    pService->createCharacteristic(CHARACTERISTIC_UUID,
                                   BLECharacteristic::PROPERTY_READ |
                                   BLECharacteristic::PROPERTY_WRITE
                                   );
  
pCharacteristic->setValue("waiting for command");
  
  //Descriptor time
  pDescr = new BLEDescriptor((uint16_t)0x2901);
  pDescr->setValue("Console");
  pCharacteristic->addDescriptor(pDescr);

  pBLE2902_1 = new BLE2902();
  pBLE2902_1->setNotifications(true);                 
  pCharacteristic->addDescriptor(pBLE2902_1);

  //time for the callback Function
  pCharacteristic->setCallbacks(new CharacteristicCallBack());

  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

//pCharacteristics is for user input / user info
//pCharacteristics_1 is for the Data on the Display
void loop() {
  
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
  display.clearDisplay();
  display.display();
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