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

  drawcenterlines(); //draw centering lines to help with positioning

  drawarrowup(); //makes an arrow up

  display.display();
}

void loop() {
  // put your main code here, to run repeatedly:
}

void drawcenterlines(void){
  display.drawLine(0, 0, 127, 63, WHITE);
  display.drawLine(127, 0, 0, 63, WHITE);
}

void drawarrowup(void){
  display.drawTriangle(10, 0, 0, 5, 20, 5, WHITE);
  display.drawRect(5, 5, 5, 5, WHITE);
}