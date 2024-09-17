#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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

}

void loop() {
  textDemo();
}

void drawcenterlines(void){ //drawing a debug X for offset finding
  display.drawLine(0, 0, 127, 63, WHITE);
  display.drawLine(127, 0, 0, 63, WHITE);
}

void drawArrowUp(void){ // Draws an arrow up
  int ox = 52; //offset X
  int oy = 20; //offset Y
  display.fillTriangle(10+ox, 0+oy, 0+ox, 10+oy, 20+ox, 10+oy, WHITE);
  display.fillRect(5+ox, 10+oy, 10, 10, WHITE);
  //Triangle Size: 10, 0, 0, 10, 20, 10
  //Rectangle Size: 5, 10, 10, 10
}

void drawArrowDown(void){ //Draws an arrow down
  int ox = 52; //offset X
  int oy = 20; //offset Y
  display.fillTriangle(10+ox, 20+oy, 0+ox, 10+oy, 20+ox, 10+oy, WHITE);
  display.fillRect(5+ox, 10+oy, 10, 10, WHITE);
} 

void drawArrowLeft(void){
  int ox = 52; //offset X
  int oy = 20; //offset Y
  display.fillTriangle(0+ox, 10+oy, 10+ox, 0+oy, 10+ox, 20+oy, WHITE);
  display.fillRect(10+ox, 5+oy, 10, 10, WHITE);
}

void drawArrowRight(void){
  int ox = 52; //offset X
  int oy = 20; //offset Y
  display.fillTriangle(20+ox, 10+oy, 10+ox, 0+oy, 10+ox, 20+oy, WHITE);
  display.fillRect(0+ox, 5+oy, 10, 10, WHITE);
}

void textDemo(void){
  display.setCursor(45,28);
  display.println("Hallo!");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setCursor(45,28);
  display.println("Banane");
  display.display();
  delay(2000);
  display.clearDisplay();
}