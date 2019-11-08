// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
//heart rate sensor code: //code from: https://create.arduino.cc/projecthub/Johan_Ha/from-ky-039-to-heart-rate-0abfca

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN_5          5
#define PIN_6          6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS_1      16
#define NUMPIXELS_2      1

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels_1 = Adafruit_NeoPixel(NUMPIXELS_1, PIN_5, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel pixels_2 = Adafruit_NeoPixel(NUMPIXELS_2, PIN_6, NEO_GRB + NEO_KHZ800);

#define samp_siz 4
#define rise_threshold 5
int sensorPin = 0;

int delayval = 500; // delay for half a second
float avgHeartRate;

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
  Serial.begin(9600);
  pixels_1.begin(); // This initializes the NeoPixel library.
  pixels_2.begin();
}

void loop() {
  turnOnNeoPixelRing();
  turnOnNeoPixelStrip();
  heartRate();
}

void turnOnNeoPixelRing() {
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  for (int i = 0; i < NUMPIXELS_1; i++) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels_1.setPixelColor(i, pixels_1.Color(0, 0, 150)); // Moderately bright blue color.
    pixels_1.show(); // This sends the updated pixel color to the hardware.
    //delay(delayval); // Delay for a period of time (in milliseconds).
  }
}

void turnOnNeoPixelStrip() {
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  for (int i = 0; i < NUMPIXELS_2; i++) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels_2.setPixelColor(i, pixels_2.Color(150, 0, 150)); // Moderately bright purple color.
    pixels_2.show(); // This sends the updated pixel color to the hardware.
    //delay(delayval); // Delay for a period of time (in milliseconds).
  }
}

void heartRate(){
  float reads[samp_siz], sum;
   long int now, ptr;
   float last, reader, start;
   float first, second, third, before;
   bool rising;
   int rise_count;
   int n;
   long int last_beat;
   for (int i = 0; i < samp_siz; i++)
     reads[i] = 0;
   sum = 0;
   ptr = 0;
   while(1)
   {
     // calculate an average of the sensor
     // during a 20 ms period (this will eliminate
     // the 50 Hz noise caused by electric light
     n = 0;
     start = millis();
     reader = 0.;
     do
     {
       reader += analogRead (sensorPin);
       n++;
       now = millis();
     }
     while (now < start + 20);  
     reader /= n;  // we got an average
     // Add the newest measurement to an array
     // and subtract the oldest measurement from the array
     // to maintain a sum of last measurements
     sum -= reads[ptr];
     sum += reader;
     reads[ptr] = reader;
     last = sum / samp_siz;
     // now last holds the average of the values in the array
     // check for a rising curve (= a heart beat)
     if (last > before)
     {
       rise_count++;
       if (!rising && rise_count > rise_threshold)
       {
         // Ok, we have detected a rising curve, which implies a heartbeat.
         // Record the time since last beat, keep track of the two previous
         // times (first, second, third) to get a weighed average.
         // The rising flag prevents us from detecting the same rise 
         // more than once.
         rising = true;
         first = millis() - last_beat;
         last_beat = millis();
         // Calculate the weighed average of heartbeat rate
         // according to the three last beats
         avgHeartRate = 60000. / (0.4 * first + 0.3 * second + 0.3 * third);
         Serial.print(avgHeartRate);
         Serial.print('\n');
         third = second;
         second = first;
       }
     }
     else
     {
       // Ok, the curve is falling
       rising = false;
       rise_count = 0;
     }
     before = last;
     ptr++;
     ptr %= samp_siz;
   }
}
