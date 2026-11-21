
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

// I2C pins
#define SDA_PIN 21
#define SCL_PIN 22

// Initialize OLED
Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

// Initialize RTC
RTC_DS3231 rtc;

const char* daysOfWeek[] = {
  "Sunday", "Monday", "Tuesday",
  "Wednesday", "Thursday",
  "Friday", "Saturday"
};

void setup() {

  Serial.begin(115200);

  // Initialize I2C
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("OLED initialization failed!");
    while (1);
  }

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("RTC not found!");

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 20);
    display.println("RTC NOT FOUND!");
    display.display();

    while (1);
  }

  // Set RTC time if battery was lost
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time!");

    // Set RTC to compilation time
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  Serial.println("RTC Clock Started!");
}

void loop() {

  // Read RTC time
  DateTime now = rtc.now();

  // Serial Monitor output
  Serial.print(now.year());
  Serial.print("/");
  Serial.print(now.month());
  Serial.print("/");
  Serial.print(now.day());

  Serial.print(" ");

  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.println(now.second());

  // OLED display
  display.clearDisplay();

  // Header
  display.setTextSize(1);
  display.setCursor(25, 0);
  display.println("ESP32 RTC CLOCK");

  display.drawLine(0, 12, 127, 12, SSD1306_WHITE);

  // Time
  display.setTextSize(2);
  display.setCursor(10, 20);

  if (now.hour() < 10) display.print("0");
  display.print(now.hour());
  display.print(":");

  if (now.minute() < 10) display.print("0");
  display.print(now.minute());
  display.print(":");

  if (now.second() < 10) display.print("0");
  display.print(now.second());

  // Date
  display.setTextSize(1);
  display.setCursor(15, 45);

  display.print(now.day());
  display.print("/");
  display.print(now.month());
  display.print("/");
  display.print(now.year());

  // Day of week
  display.setCursor(35, 56);
  display.print(daysOfWeek[now.dayOfTheWeek()]);

  display.display();

  delay(1000);
}
