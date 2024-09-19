#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

#define BT_DISCOVER_TIME 10000

static bool btScanAsync = true;
static bool btScanSync = true;

void btAdvertisedDeviceFound(BTAdvertisedDevice *pDevice) {
  Serial.printf("Found a device asynchronously: %s\n", pDevice->toString().c_str());
}

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SerialBT.begin("ESP32test");  //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  if (btScanAsync) {
    Serial.print("Starting asynchronous discovery... ");
    if (SerialBT.discoverAsync(btAdvertisedDeviceFound)) {
      Serial.println("Findings will be reported in \"btAdvertisedDeviceFound\"");
      delay(10000);
      Serial.print("Stopping discoverAsync... ");
      SerialBT.discoverAsyncStop();
      Serial.println("stopped");
    } else {
      Serial.println("Error on discoverAsync f.e. not working after a \"connect\"");
    }

    if (btScanSync) {
    Serial.println("Starting synchronous discovery... ");
    BTScanResults *pResults = SerialBT.discover(BT_DISCOVER_TIME);
      if (pResults) {
        pResults->dump(&Serial);
      } else {
        Serial.println("Error on BT Scan, no result!");
      }
    }
  }
}

void loop() {
  delay(100);
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