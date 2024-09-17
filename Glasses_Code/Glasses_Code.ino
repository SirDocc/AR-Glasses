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

  drawcenterlines();

  drawarrowup();

  display.display();
}

void loop() {
  // put your main code here, to run repeatedly:
}

void drawcenterlines(void){ //drawing a debug X for offset finding
  display.drawLine(0, 0, 127, 63, WHITE);
  display.drawLine(127, 0, 0, 63, WHITE);
}

void drawarrowup(void){ // Draws an arrow up
  int ox = 53; //offset X
  int oy = 20; //offset Y
  display.fillTriangle(10+ox, 0+oy, 0+ox, 10+oy, 20+ox, 10+oy, WHITE);
  display.fillRect(5+ox, 10+oy, 10, 10, WHITE);
  //Triangle Size: 10, 0, 0, 10, 20, 10
  //Rectangle Size: 5, 10, 10, 10
}

void drawarrowdown(void){ //Draws an arrow down
  int ox = 53; //offset X
  int oy = 20; //offset Y
  display.fillTriangle(10, 20, 0, 10, 20, 10, WHITE);
  display.fillRect(5, 0, 10, 10, WHITE);
} 