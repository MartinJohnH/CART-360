#include <Keyboard.h>

int JoyStick_X = A0; //x
int JoyStick_Y = A1; //y
int JoyStick_Z = 10; //key

#define SensorINPUT 5 

boolean prevState = true;

void setup()
{
  pinMode(SensorINPUT, INPUT);
  Keyboard.begin();
  Serial.begin(9600);
}
void loop()
{
  int rawX, rawY, rawZ;
  rawX = analogRead(JoyStick_X);
  rawY = analogRead(JoyStick_Y);
  rawZ = digitalRead(JoyStick_Z);

 Serial.println(digitalRead(SensorINPUT));
  if (rawY < 200 && (rawX > 200 && rawX < 700)) {
    //up
    if(prevState == true){
       //Serial.println("UP");
       if(!digitalRead(SensorINPUT)){
         //up arrow
         Keyboard.press(218);
       }else {
         //down arrow
         Keyboard.press(217);
       }
       Keyboard.releaseAll();
       prevState = false;
    }
  }
  else if (rawY > 700 && (rawX > 200 && rawX < 700)) {
    //down
    if(prevState == true){
       //Serial.println("DOWN");
       if(!digitalRead(SensorINPUT)){
         //down arrow
         Keyboard.press(217);
       }else {
         //up arrow
         Keyboard.press(218);
       }
       Keyboard.releaseAll();
       prevState = false;
    }
    //Keyboard.press(217);
  } else if (rawX < 200 && (rawY > 200 && rawY < 700)) {
    //left
     if(prevState == true){
       //Serial.println("LEFT");
       if(!digitalRead(SensorINPUT)){
         //left arrow
         Keyboard.press(216);
       }else {
         //right arrow
         Keyboard.press(215);
       }
       Keyboard.releaseAll();
       prevState = false;
    }
  } else if (rawX > 800 && (rawY > 200 && rawY < 700)) {
    //right
     if(prevState == true){
       //Serial.println("RIGHT");
        if(!digitalRead(SensorINPUT)){
         //right arrow
         Keyboard.press(215);
       }else {
         //left arrow
         Keyboard.press(216);
       }
       Keyboard.releaseAll();
       prevState = false;
    }
  }else {
    prevState = true;
  }
  delay(100);
}
