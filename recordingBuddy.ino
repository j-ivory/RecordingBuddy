#include <pitchToFrequency.h>
#include <pitchToNote.h>
#include <frequencyToNote.h>
#include <MIDIUSB_Defs.h>
#include <MIDIUSB.h>

#include <Encoder.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#include <USB-MIDI.h>
#define KICK 36
#define HH 42
#define SNARE 38
USBMIDI_CREATE_DEFAULT_INSTANCE();

using namespace MIDI_NAMESPACE;

// Encoder **begin**
Encoder myEnc(8, 9);   //   avoid using pins with LEDs attached
Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();
// Encoder  **end**

// Button States  **begin**
const int buttonPin[] = {4,5,6,10,11,12}; 
int buttonPushCounter[] = {0,0,0,0,0,0};   // counter for the number of button presses
int buttonState[] = {0,0,0,0,0,0};         // current state of the button
int lastButtonState[] = {0,0,0,0,0,0};
int i = 6;
volatile byte state = LOW;
const byte interruptPin = 7;
// Button States  **end**

void setup() {
  Serial.begin(11520);
  //Serial.begin(9600);
  MIDI.begin(MIDI_CHANNEL_OMNI);

  //init display
  alpha4.begin(0x70);

  //setup button pins
  for(int j=0 ; j<i; j++)
  {  pinMode(buttonPin[j], INPUT); }
  //pinMode(A0, INPUT);

  //setup button on interrupt pin
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), fncButtonStateChange, CHANGE);

  MIDI.setHandleContinue(OnContinue);
}

static void OnContinue() {
  Serial.println(F("Continue"));
}

long oldPosition  = -999;


char plus1[5] = "  +1";
char zero[5] = "   0";
char minus1[5] = "  -1";
char left[5] = "LEFT";
char right[5] = "RIGH";


char intToChar(int x) 
{
  char S = x;
  return S;
}


void setDisplay(char string[]) {
    
    alpha4.writeDigitAscii(0, string[0]);
    alpha4.writeDigitAscii(1, string[1]);
    alpha4.writeDigitAscii(2, string[2]);
    alpha4.writeDigitAscii(3, string[3]);
    alpha4.writeDisplay();
}

void setDisplay(char c1, char c2, char c3, char c4) {
    alpha4.writeDigitAscii(0, c1);
    alpha4.writeDigitAscii(1, c2);
    alpha4.writeDigitAscii(2, c3);
    alpha4.writeDigitAscii(3, c4);
    alpha4.writeDisplay();
}



long previous;

long lastPosition = 0;
int value = 0;
void loop() {
  checkButtonStates();
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition);
    Serial.println(oldPosition);
    
    
    
    if(newPosition > lastPosition)
    {
      if(!state){
        setDisplay('V','o','l','+');
        MIDI.sendControlChange(7,oldPosition,1);
      }
      else{
        setDisplay('P','a','n','R');
        MIDI.sendControlChange(10,oldPosition,1);
      }
      
    }
//    if(newPosition == 0)
//    {
//      setDisplay(zero);
//    }
    if(newPosition < lastPosition)
    {
      if(!state){
        setDisplay('V','o','l','-');
        MIDI.sendControlChange(7,oldPosition,1);
      }
      else{
        setDisplay('P','a','n','L');
        MIDI.sendControlChange(10,oldPosition,1);
      }
    }
    lastPosition = newPosition;
    
  } 
}

char charIFY(int x) {
  char c = '.';
  if(x==0)
    c = '0';
  if(x==1)
    c = '1';
  if(x==2)
    c = '2';
  if(x==3)
    c = '3';
  if(x==4)
    c = '4';
  if(x==5)
    c = '5';
  if(x==6)
    c = '6';
  if(x==7)
    c = '7';
  if(x==8)
    c = '8';
  if(x==9)
    c = '9';
  if(x==10)
    c = 'A';
  if(x==11)
    c = 'B';
  if(x==12)
    c = 'C';
  if(x==13)
    c = 'D';
  
  return c;
}

void checkButtonStates() {
        for(int j=0 ; j<i; j++)
            {
                    // read the pushbutton input pin:
                    buttonState[j] = digitalRead(buttonPin[j]);
                    // compare the buttonState to its previous state
                    if (buttonState[j] != lastButtonState[j]) {
                        // if the state has changed
                        if (buttonState[j] == HIGH) {
                            // if the current state is HIGH then the button went from off to on:
                            //buttonPushCounter[j]++;
                            //Serial.println("on")
                            //MIDI.sendSongPosition(0);
                            
                            //Serial.print("works");
                            Serial.print("button pushed: ");
                            if(state)
                                {Serial.println(j+i);
                                  setDisplay('B',charIFY(j+i),'O','N');
                                  handleButton(j+i);
                                  //MIDI.sendNoteOn(KICK, 127, 1);
                                  // Send a Note (pitch 42, velo 127 on channel 1)
                                }
                            else
                                {Serial.println(j);
                                  handleButton(j);
                                  //setDisplay('B',charIFY(j),'O','N');
                                  //MIDI.sendNoteOn(SNARE, 127, 1);    // Send a Note (pitch 42, velo 127 on channel 1)
                                }
                        } else {
                        // if the current state is LOW then the button went from on to off
                            if(state)
                            {
                              Serial.print(j+i);
                              Serial.println(" off"); 

                              //setDisplay('B',charIFY(j+i),'O','F');
                              //delay(100);
                              setDisplay(' ',' ',' ',' ');
                              
                              //MIDI.sendNoteOff(KICK, 0, 1);  //stop note
                              }
                            else
                            { Serial.print(j);
                              Serial.println(" off"); 

                              
                              //setDisplay('B',charIFY(j),'O','F');
                              //delay(100);
                              setDisplay(' ',' ',' ',' ');
                              
                              //MIDI.sendNoteOff(SNARE, 0, 1);  //stop note
                              }
      
                        }
                    }
                    lastButtonState[j] = buttonState[j];
            }

}

//void buttonFunctions(int x)
//{
//  if(x==0)
//  {
//    MIDI.sendStart();
//  }
//  if(x==1)
//  {
//    MIDI.sendStop();
//  }
//  else 
//    return;
//}

//void play() {
//  MIDI.sendStart();
//}
//void pause() {
//  MIDI.sendStop();
//}

void handleButton(int b)
{
  if(b==0)
  {
    setDisplay('R','E','C',' ');
    MIDI.sendControlChange(102,0,1);
    
  }
  if(b==1)
  {
    setDisplay('P','L','A','Y');
    MIDI.sendControlChange(103,0,1);
  }
  if(b==2)
  {
    setDisplay('L','O','O','P');
    MIDI.sendControlChange(104,0,1);
  }
  if(b==3)
  {
    setDisplay('K','I','C','K');
    playDrum(KICK, 1);
  }
  if(b==4)
  {
    setDisplay('S','N','R',' ');
    playDrum(SNARE, 1);
  }
  if(b==5)
  {
    setDisplay('H','H','A','T');
    playDrum(HH, 1);
  }
  //fnc pressed
  if(b==6)
  {
    setDisplay('M','E','T',' ');
    MIDI.sendControlChange(105,0,1);
  }
  if(b==7)
  {
    setDisplay('B','E','G',' ');
    MIDI.sendControlChange(106,0,1);
  }
  if(b==8)
  {
    setDisplay('S','O','L','O');
    MIDI.sendControlChange(107,0,1);
  }
  if(b==9)
  {
    setDisplay('C','R','S','H');
    MIDI.sendNoteOn(49, 127, 1);
  }
  if(b==10)
  {
    setDisplay('R','I','M',' ');
    MIDI.sendNoteOn(37, 127, 1);
  }
  if(b==11)
  {
    setDisplay('R','I','D','E');
    MIDI.sendNoteOn(51, 127, 1);
  }
}
void playDrum(int drum, int toggle) {
  if(toggle==1)
  {
    if(drum == SNARE)
      MIDI.sendNoteOn(SNARE, 127, 1);
    if(drum == KICK)
      MIDI.sendNoteOn(KICK, 127, 1);
    if(drum == HH)
      MIDI.sendNoteOn(HH, 127, 1);
  }
  else
  {
    if(drum==SNARE)
      MIDI.sendNoteOff(SNARE, 0, 1);
    if(drum==KICK)
      MIDI.sendNoteOff(KICK, 0, 1);
    if(drum==HH)
      MIDI.sendNoteOff(HH, 0, 1);   
  }
}

void fncButtonStateChange() {
  state = !state;
}
