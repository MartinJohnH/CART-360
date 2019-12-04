#include <Adafruit_NeoPixel.h>

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2);
Adafruit_GPS GPS(&mySerial);
#define GPSECHO  true

byte neopix_gamma[] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
  144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
  215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN_5          5
#define PIN_6          6
#define PIN_7          7

#define NUMPIXELS_1      16
#define NUMPIXELS_2      3

Adafruit_NeoPixel pixels_1 = Adafruit_NeoPixel(NUMPIXELS_1, PIN_6, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel pixels_2 = Adafruit_NeoPixel(NUMPIXELS_2, PIN_5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_3 = Adafruit_NeoPixel(NUMPIXELS_2, PIN_7, NEO_GRB + NEO_KHZ800);

const int HR_RX = 4;
byte oldSample, sample;

unsigned long interval = 10;  // the time we need to wait
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;

unsigned int i = 0;
boolean cycleFinished = false;

void setup() {
  pixels_1.begin();
  pixels_1.setBrightness(80);
  pixels_2.begin();
  pixels_2.setBrightness(80);
  pixels_3.begin();
  pixels_3.setBrightness(80);

  //green on
  for (int i = 0; i < NUMPIXELS_1; i++) {
    pixels_1.setPixelColor(i, pixels_1.Color(0, 200, 0));
    pixels_1.show();
    delay(50);
  }
  for (int i = 0; i < NUMPIXELS_1; i++) {
    pixels_1.setPixelColor(i, pixels_1.Color(0, 0, 0));
    pixels_1.show();
    delay(50);
  }
  //Serial.begin(9600);
  Serial.begin(115200);
  delay(5000);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);

  //heart rate monitor
  pinMode (HR_RX, INPUT);
  Serial.println("Waiting for heart beat...");

  // Wait until a heart beat is detected
  while (!digitalRead(HR_RX)) {};
  Serial.println ("Heart beat detected!");

}

void heartBeat () {
  Serial.println("heartbeat");
  for (volatile int i = 0; i < pixels_2.numPixels(); i++) {
    pixels_2.setPixelColor(i, pixels_2.Color(236, 64, 50)); //orange: #EC4032
    pixels_3.setPixelColor(i, pixels_3.Color(236, 64, 50)); //orange: #EC4032
  }
  pixels_2.show();
  pixels_3.show();
  delay(10);
}

void speedIndicator(float delayTime) {
  if (i >= NUMPIXELS_1) {
    cycleFinished = !cycleFinished;
    i = 0;
  }
  unsigned long currentMillis = millis();
  if (!cycleFinished && currentMillis - previousMillis > delayTime) {
    pixels_1.setPixelColor(i, pixels_1.Color(236, 64, 50));
    pixels_1.show();
    i += 1;
    previousMillis = millis();
  }
  if (cycleFinished && currentMillis - previousMillis2 > delayTime) {
    pixels_1.setPixelColor(i, pixels_1.Color(221, 137, 131));
    pixels_1.show();
    i += 1;
    previousMillis2 = millis();
  }
}

uint32_t timer = millis();
float recordedSpeed = 4.0;

void loop() {
  sample = digitalRead(HR_RX); //Store signal output
  if (sample && (oldSample != sample)) {
    Serial.println("Beat");
    heartBeat();
  }
  oldSample = sample; //Store last signal received

  for (volatile int i = 0; i < pixels_2.numPixels(); i++) {
    pixels_2.setPixelColor(i, pixels_2.Color(20, 0, 0));
    pixels_3.setPixelColor(i, pixels_3.Color(20, 0, 0));
  }
  pixels_2.show();
  pixels_3.show();

  char c = GPS.read();

  if ((c) && (GPSECHO))
    Serial.write(c);

  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }

  if (timer > millis())  timer = millis();
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
    Serial.print("\nTime: ");
    if (GPS.hour < 10) {
      Serial.print('0');
    }
    Serial.print(GPS.hour, DEC); Serial.print(':');
    if (GPS.minute < 10) {
      Serial.print('0');
    }
    Serial.print(GPS.minute, DEC); Serial.print(':');
    if (GPS.seconds < 10) {
      Serial.print('0');
    }
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    if (GPS.milliseconds < 10) {
      Serial.print("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      Serial.print("0");
    }
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      recordedSpeed = GPS.speed;
    }
  }
   //speedIndicator((1.0 / recordedSpeed * 200));

}
