/********************** ETUDE 3 CART 360 2019 *******************************
   WELCOME!
   THE PURPOSE OF THIS EXERCISE IS TO DESIGN A VERY SIMPLE KEYBOARD (5 KEYS)
   WHICH ALLOWS YOU TO PLAY LIVE, RECORD, AND PLAYBACK SINGLE NOTES (NO CHORDS).
   THERE WILL BE A BUTTON WHICH WHEN PRESSED WILL TAKE THE USER TO THE NEXT MODE:
   THERE ARE 5 POSSIBLE MODES
   0 ==  reset    ==   led off == also resets
   1 ==  live     ==   led BLUE
   2 ==  record   ==   led RED
   3 ==  play     ==   led GREEN
   4 ==  loopMODE ==   led PURPLE

   ADDITIONALLY - WHEN THE USER CHANGES MODE,
   THE RGB LED WILL CHANGE COLOR (ACCORDING TO THE SPECIFICATIONS)

   PLEASE FOLLOW THE INSTRUCTIONS IN THE COMMENTS:
   DO NOT ADD ANY MORE FUNCTION DEFINITIONS
   ONLY IMPLEMENT THE FUNCTION DEFINITIONS SUPPLIED
   THERE IS NO NEED TO ADD ANY NEW VARIABLES OR CONSTANTS
   PROVIDE COMMENTS FOR ANY OF THE CODE IMPLEMENTED
   GOOD LUCK!
*/
/**** CONSTANTS ********************************************************/

#define BUTTON_MODE_PIN 2 // Button to change the mode

// constants for RGB LED
#define LED_PIN_R 5 // R PIN
#define LED_PIN_G 9 // G PIN
#define LED_PIN_B 6 // B PIN

// constant for note in (button-resistor ladder on breadboard)
# define NOTE_IN_PIN A0

//constant for max number of notes in array
#define MAX_NOTES 16

// a constant for duration
const int duration = 200;

// constant for pin to output for buzzer
#define BUZZER_PIN 3 // PWM

/******** VARIABLES *****************************************************/
// counter for how many notes we have
int countNotes = 0;
// array to hold the notes played (for record/play mode)
int notes [MAX_NOTES];
int mode = 0; // start at off
int prevButtonState = LOW;

/*************************************************************************/


/**********************SETUP() DO NOT CHANGE*******************************/
// Declare pin mode for the single digital input
void setup()
{
  pinMode(BUTTON_MODE_PIN, INPUT);
}

/**********************LOOP() DO NOT CHANGE *******************************
   INSTRUCTIONS:
   There is NO NEED to change the loop - it establishes the flow of the program
   We call here 3 functions repeatedly:
   1: chooseMode(): this function will determine the mode that your program is in
   based on if the button (linked to the BUTTON_MODE_PIN) was pressed
   2: setRGB(): will set the color of the RGB LED based on the value of the mode variable
   3: selectMode(): will determine which function to call based on the value of the mode variable

**************************************************************************/
void loop()
{
  chooseMode();
  setRGB();
  selectMode();
}
/******************CHOOSEMODE(): IMPLEMENT *********************************
   INSTRUCTIONS:
   Read the value from the Button (linked to the BUTTON_MODE_PIN) and
   if is being pressed then change the mode variable.
   REMEMBER:
   mode == 0 is reset
   mode == 1 is live
   mode == 2 is record
   mode == 3 is play
   mode == 4 is loopMode
   Every time the user presses the button, the program will go to the next mode,
   once it reaches 4, it should go back to mode == 0.
   (i.e. if mode ==2 and we press, then mode ==3) ...
**************************************************************************/
void chooseMode() {
  //check if the button is pressed once
  if (digitalRead(BUTTON_MODE_PIN) != prevButtonState && digitalRead(BUTTON_MODE_PIN) == HIGH) {
    //if the mode is not equal to 4 then incresase mode by one else reset to 0
    mode != 4 ? mode++ : mode = 0;
  }
  prevButtonState = digitalRead(BUTTON_MODE_PIN);

}

/******************SETRGB(): IMPLEMENT *********************************
   INSTRUCTIONS:
   Depending on the value of the mode variable:
   if the mode is 0 - RGB LED IS OFF
   if the mode is 1 - RGB LED IS BLUE
   if mode is 2 - RGB LED IS RED
   if mode is 3 - RGB LED IS GREEN
   if mode is 4 - RGB LED iS PURPLE
   YOU MUST USE A SWITCH CASE CONSTRUCT (NOT A SERIES OF IF / ELSE STATEMENTS
**************************************************************************/
void setRGB()
{
  //switch statment for different modes affect the colour of the RGB LED
  switch (mode) {
    case 1:
      //BLUE
      analogWrite(LED_PIN_R, 0);
      analogWrite(LED_PIN_G, 0);
      analogWrite(LED_PIN_B, 255);
      break;
    case 2:
      //RED
      analogWrite(LED_PIN_R, 255);
      analogWrite(LED_PIN_G, 0);
      analogWrite(LED_PIN_B, 0);
      break;
    case 3:
      //GREEN
      analogWrite(LED_PIN_R, 0);
      analogWrite(LED_PIN_G, 255);
      analogWrite(LED_PIN_B, 0);
      break;
    case 4:
      //PURPLE
      analogWrite(LED_PIN_R, 200);
      analogWrite(LED_PIN_G, 0);
      analogWrite(LED_PIN_B, 255);
      break;
    default:
      //OFF
      analogWrite(LED_PIN_R, 0);
      analogWrite(LED_PIN_G, 0);
      analogWrite(LED_PIN_B, 0);
  }
}
/**********************SELECTMODE() DO NOT CHANGE *******************************
   INSTRUCTIONS:
   There is NO NEED to change this function - it selects WHICH function should run based on the mode variable
   There are 4 possibilities
   1: reset(): this function will reset any arrays etc, and will do nothing else
   2: live(): this function will play the corresponding notes
   to the user pressing the respective buttons.
   NOTE:: the notes played are NOT stored
   3: record(): this function will play the corresponding notes
   to the user pressing the respective buttons
   AND will STORE up to 16 consecutive notes in an array.
   4: play(): this function will playback any notes stored in the array that were recorded
   in the previous mode
   5: loopMode(): this function will playback any notes stored in the array that were recorded,
   BUT unlike the previous mode, you can choose in which sequence the notes are played.
   REQUIRED: only play notes from the array (no live stuff)

******************************************************************************/
void selectMode()
{
  if (mode == 0) {
    reset();
  }
  else if (mode == 1) {
    live();
  }
  else if (mode == 2) {
    record();
  }

  else if (mode == 3) {
    play();
  }

  else if (mode == 4) {
    loopMode();
  }
}
/******************RESET(): IMPLEMENT **************************************
   INSTRUCTIONS:
   this function should ensure that any notes recorded are no longer available
**************************************************************************/
void reset()
{
  //rest countNotes to 0
  countNotes = 0;

  //loops through notes array and sets all values in array to 0
  for (int i = 0; i < MAX_NOTES;  ++i ) {
    notes[i] = 0;
  }

}
/******************LIVE(): IMPLEMENT **************************************
   INSTRUCTIONS:
   this function will play the corresponding notes
   to the user pressing the respective buttons
   NOTE:: the notes played are NOT stored
   SO: you need read in the input from the analog input (linked to the button-resistor ladder combo)
   THEN - output the note to the buzzer using the tone() function
**************************************************************************/
void live()
{
  //Output not to buzzer using the tone() function
  tone(BUZZER_PIN, analogRead(NOTE_IN_PIN), duration);
}
/******************RECORD(): IMPLEMENT **********************************
   INSTRUCTIONS:
   this function will play the corresponding notes
   to the user pressing the respective buttons
   AND will STORE up to 16 consecutive notes in an array.
   SO:you need read in the input from the analog input (linked to the button-resistor ladder combo)
   AND - output the note to the buzzer using the tone() function
   THEN store that note in the array  - BE CAREFUL - you can only allow for up to MAX_NOTES to be stored
**************************************************************************/
void record()
{
  // check if countNotes is less than MAX_NOTES
  if (countNotes < MAX_NOTES) {
    //Output not to buzzer using the tone() function
    tone(BUZZER_PIN, analogRead(NOTE_IN_PIN), duration);
    //if the NOTE_IN_PIN is pressed (bigger than 10)
    if (analogRead(NOTE_IN_PIN) > 10) {
      //make the current note equal the value of the NOTE_IN_PIN
      notes[countNotes] = analogRead(NOTE_IN_PIN);
      // if the current note value is not equal to 0 and the button is not being pressed
    } else if (notes[countNotes] != 0 && analogRead(NOTE_IN_PIN) == 0) {
      //increment the countNotes by one
      countNotes++;
    }
  }
}
/******************PLAY(): IMPLEMENT ************************************
   INSTRUCTIONS:
   this function will playback any notes stored in the array that were recorded
   in the previous mode
   SO: you need to go through the array of values (be careful - the user may not have put in MAX_NOTES)
   READ each value IN ORDER
   AND output each note to the buzzer using the tone() function
   ALSO: as long as we are in this mode, the notes are played over and over again
   BE CAREFUL: make sure you allow for the user to get to another mode from the mode button...
**************************************************************************/
void play()
{
  // loop throught the notes array in asending order
  for ( int i = 0; i < countNotes;  ++i ) {
    //if the button_mode is pressed break from the loop
    if (digitalRead(BUTTON_MODE_PIN) == HIGH) {
      break;
    }
    //Output not to buzzer using the tone() function
    tone(BUZZER_PIN, notes[i], duration);
    delay(duration);
  }
}
/******************LOOPMODE(): IMPLEMENT *********************************
   INSTRUCTIONS:
   this function will playback any notes stored in the array that were recorded
   in the previous mode
   SO: you need to go through the array of values (be careful - the user may not have put in MAX_NOTES)
   READ values IN ANY ORDERING (You MUST use the array - but you can determine your own sequence)
   AND output each note to the buzzer using the tone() function
   ALSO: as long as we are in this mode, the notes are played over and over again
   BE CAREFUL: make sure you allow for the user to get to another mode from the mode button...
**************************************************************************/
void loopMode()
{
  // loop throught the notes array in desending order
  for ( int i = countNotes - 1; i >= 0;  --i ) {
    //if the button_mode is pressed break from the loop
    if (digitalRead(BUTTON_MODE_PIN) == HIGH) {
      break;
    }
    //Output not to buzzer using the tone() function
    tone(BUZZER_PIN, notes[i], duration);
    delay(duration);
  }
}

/**************************************************************************/
