#include <Arduino.h>
#line 1 "/Users/james/Development/Projects/RecordingBuddy/recordingBuddy.ino"



const int buttonPin[] = {2,3,4,5,6,7}; 
int buttonPushCounter[] = {0,0,0,0,0,0};   // counter for the number of button presses
int buttonState[] = {0,0,0,0,0,0};         // current state of the button
int lastButtonState[] = {0,0,0,0,0,0};     // previous state of the button

int i = 6;


#line 12 "/Users/james/Development/Projects/RecordingBuddy/recordingBuddy.ino"
void setup();
#line 19 "/Users/james/Development/Projects/RecordingBuddy/recordingBuddy.ino"
void loop();
#line 12 "/Users/james/Development/Projects/RecordingBuddy/recordingBuddy.ino"
void setup()
{
    Serial.begin(312500);
    for(int j=0 ; j<i; j++)
    {  pinMode(buttonPin[j], INPUT); }
}

void loop()
{
    for(int j=0 ; j<i; j++)
    {
            // read the pushbutton input pin:
            buttonState[j] = digitalRead(buttonPin[j]);
            // compare the buttonState to its previous state
            if (buttonState[j] != lastButtonState[j]) {
                // if the state has changed, increment the counter
                if (buttonState[j] == HIGH) {
                    // if the current state is HIGH then the button went from off to on:
                    buttonPushCounter[j]++;
                    Serial.println("on");
                    Serial.print("button pushed: ");
                    Serial.println(j);
                } else {
                // if the current state is LOW then the button went from on to off:
                    Serial.print(j);
                    Serial.println(" off");
                }
            }
            lastButtonState[j] = buttonState[j];
    }
            
}

