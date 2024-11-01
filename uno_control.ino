#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C LCD(0x27, 16, 2);

void setup() {
  LCD.init();
  LCD.backlight();

  LCD.noDisplay();

  LCD.display();

  pinMode(PIN_A0, OUTPUT);
  pinMode(PIN_A1, OUTPUT);

  digitalWrite(PIN_A0, HIGH);
  digitalWrite(PIN_A1, HIGH);
}

void loop() {
  delay(1000);
  tick();
}

uint32_t disableWaterAt = 0;
void enableWater(uint8_t duration) {
  if (disableWaterAt != 0) {
    return;
  }
  digitalWrite(PIN_A0, LOW);
  disableWaterAt = millis() + ((uint32_t)duration) * 1000;
  LCD.setCursor(0, 1);
  LCD.print("water enabled");
}

void disableWater() {
  if (disableWaterAt == 0) {
    return;
  }
  if (millis() > disableWaterAt) {
    digitalWrite(PIN_A0, HIGH);
    disableWaterAt = 0;
    LCD.clear();
    printTime();
  }
}


void tick() {
  printTime();
  processWater();
}

void processWater() {

  uint32_t seconds_in_minute = 60;
  uint32_t seconds_in_hour = seconds_in_minute * 60;
  uint32_t runWaterEverySeconds = seconds_in_minute * 20;
  uint32_t runWaterDuration = 30;
  uint32_t runWaterThreshold = 5;
  if (((millis() / 1000)) % runWaterEverySeconds <= runWaterThreshold) {
    enableWater(runWaterDuration);
  }

  disableWater();
}

void printTime() {
  uint32_t seconds = (millis() / 1000);
  uint8_t vseconds = seconds % 60;

  uint32_t minutes = seconds / 60;
  uint8_t vminutes = minutes % 60;

  uint32_t hours = minutes / 60;
  uint8_t vhours = hours % 24;

  uint8_t days = hours / 24;
  char line[17];
  sprintf(line, "%02dd %02d:%02d:%02d", days, vhours, vminutes, vseconds);
  LCD.setCursor(0, 0);
  LCD.print(line);
}