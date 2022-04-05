


const int buttonPin = 2; 
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
 


void setup()
{
    Serial.begin(9600);
    pinMode(buttonPin, INPUT);
}

void loop()
{
	// read the pushbutton input pin:
    buttonState = digitalRead(buttonPin);
    // compare the buttonState to its previous state
    if (buttonState != lastButtonState) {
        // if the state has changed, increment the counter
        if (buttonState == HIGH) {
            // if the current state is HIGH then the button went from off to on:
            buttonPushCounter++;
            Serial.println("on");
            Serial.print("number of button pushes: ");
            Serial.println(buttonPushCounter);
        } else {
          // if the current state is LOW then the button went from on to off:
            Serial.println("off");
        }
    }
    lastButtonState = buttonState;
}
