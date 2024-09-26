#include <Wire.h>
#include <math.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

BLEServer *pServer = NULL; //globaler leerer pointer für pServer
BLECharacteristic *pCharacteristic = NULL; //globaler leerer pointer für pCharacteristic
BLEDescriptor *pDescr; //globaler leerer pointer für pDecriptor
BLE2902 *pBLE2902_1; //global definierter pointer für BLE2902_1

#define SCREEN_WIDTH 128 //screen länge
#define SCREEN_HEIGHT 64 //screen höhe

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); //initialisierung des Displays

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b" 
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

void  WriteMyText (String command){ //diese Methode schreibt den Parameter auf den screen.
  display.clearDisplay();
  display.display();
  display.setCursor(0,28);
  display.println(command);
  display.display();
}

void scrollPls (int time){ //diese Methode scrollt den Text auf dem Screen für eine Parameter definierte Dauer nach links
  display.startscrollleft(0x00, 0x0F);
  delay(time);
  display.stopscroll();
}

void fitMyText(String command){ //diese Methode teilt den String des Nutzers in kleinere Substrings, sonst passt der Text nicht auf den Screen
  Serial.println("This is my String: " + command);
  display.clearDisplay();
  display.display();
  display.setCursor(0,28);
  String onDisplay;
  for (int i = 0; i <= command.length(); i+=20){
    if(i+19 <= command.length()){
      onDisplay = command.substring(i,i+20);
      WriteMyText(onDisplay);
      Serial.println("This is on Display rn: " + onDisplay);
      delay(1000);
      scrollPls(8000);
    }
    else {
      onDisplay = command.substring(i,command.length());
      WriteMyText(onDisplay);
      Serial.println("This is on final Display rn: " + onDisplay);
      delay(1000);
      scrollPls(8000);
    }
  }
}

void onWriteLogic(String command){ //Dieser Code wird ausgeführt wenn der Nutzer etwas an den Server schreibt. Hier wird unterschieden ob es ein Kommando ist und ob gescrollt werden soll.
  if (command == "demo"){
    WriteMyText("Displaying Demo");
    delay(2000);
    textDemo();
    delay(2000);
    WriteMyText("Waiting for command");
  }
  else if (command == "komoot"){
    WriteMyText("Work in Progress");
  }
  else if (command.length() > 21){
    pCharacteristic->setValue("Writing Text");
    fitMyText(command);
  }
  else {
    WriteMyText(command);
  }
}

class CharacteristicCallBack: public BLECharacteristicCallbacks { //Diese Funktion überschreibt das Verhalten des Servers.
  void onWrite(BLECharacteristic *pChar) override { //Statt ständigem aussenden der Info, führt der Server nur Code aus wenn er vom Nutzer Info bekommt
    String pChar2_value_stdstr = pChar -> getValue();
    String pChar2_value_string = String(pChar2_value_stdstr.c_str());
    Serial.println(pChar2_value_string);
    onWriteLogic(pChar2_value_string);
  }
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  Serial.println("Starting BLE work!");
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { //failsafe für den Display
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.display(); //setup für den Display

  BLEDevice::init("Benchie Brille"); //Hier passiert das ganze Setup für die BLE Technik
  pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic =
    pService->createCharacteristic(CHARACTERISTIC_UUID,
                                   BLECharacteristic::PROPERTY_READ |
                                   BLECharacteristic::PROPERTY_WRITE
                                   );
  
  //Descriptor time
  pDescr = new BLEDescriptor((uint16_t)0x2901); //Hier wird eine Description hinzugefügt
  pDescr->setValue("Console");
  pCharacteristic->addDescriptor(pDescr);

  pBLE2902_1 = new BLE2902();
  pBLE2902_1->setNotifications(true);                 
  pCharacteristic->addDescriptor(pBLE2902_1);

  //time for the callback Function
  pCharacteristic->setCallbacks(new CharacteristicCallBack()); //Hier wird die Callback function hinzugefügt

  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising(); //Mit diesem Code wird die Brille gefunden
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

void drawArrowUp(void){ // Hier wird ein Pfeil nach oben gemalt
  int offsetX = 52; //offset X
  int offsetY = 20; //offset Y
  display.fillTriangle(10 + offsetX,  0 + offsetY,
                        0 + offsetX, 10 + offsetY, 
                       20 + offsetX, 10 + offsetY, WHITE);

  display.fillRect(5+offsetX, 10+offsetY, 10, 10, WHITE);
  //Triangle Size: 10, 0, 0, 10, 20, 10
  //Rectangle Size: 5, 10, 10, 10
}

void drawArrowDown(void){ // Hier wird ein Pfeil nach unten gemalt
  int offsetX = 52; //offset X
  int offsetY = 20; //offset Y
  display.fillTriangle(10 + offsetX, 20 + offsetY, 
                        0 + offsetX, 10 + offsetY, 
                       20 + offsetX, 10 + offsetY, WHITE);

  display.fillRect(5+offsetX, 0+offsetY, 10, 10, WHITE);
} 

void drawArrowLeft(void){ // Hier wird ein Pfeil nach links gemalt
  int offsetX = 52; //offset X
  int offsetY = 20; //offset Y
  display.fillTriangle(0 + offsetX, 10 + offsetY, 
                      10 + offsetX,  0 + offsetY, 
                      10 + offsetX, 20 + offsetY, WHITE);

  display.fillRect(10 + offsetX, 5 + offsetY, 10, 10, WHITE);
}

void drawArrowRight(void){ // Hier wird ein Pfeil nach rechts gemalt
  int offsetX = 52; //offset X
  int offsetY = 20; //offset Y
  display.fillTriangle(20 + offsetX, 10 + offsetY, 
                       10 + offsetX,  0 + offsetY, 
                       10 + offsetX, 20 + offsetY, WHITE);

  display.fillRect(0 + offsetX, 5 + offsetY, 10, 10, WHITE);
}

void textDemo(void){ //Das ist die Demo welche man mit "demo" in der Konsole ausführen kann
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