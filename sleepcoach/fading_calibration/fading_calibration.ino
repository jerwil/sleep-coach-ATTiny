#include <math.h>

// Sleep coach using ATTiny 85


const int LEDPin = 0;                  // IC leg 6 (PB1), output to red channel

unsigned long currentTime;
double second_timer[1] = {0}; // This is use dto keep track of the timer used to tick for each second
int ticked = 0;

int delay_int = 1;
int brightness = 0;
double brightincrease = 1;
double k = 0.00108*5;
double x = 3*3.14159/2/k; // This starts it at 0 brightness

int button_state = 0;
int button_pushed = 0; // This is the indicator that the button was pushed and released
int button_press_initiate[1];     // storage for button press function
int button_press_completed[1];    // storage for button press function


void setup() {                
  // initialize the digital pin as an output.
  pinMode(LEDPin, OUTPUT);
}


void loop() {
  
brightness = 127*(1 + sin(k*x));
  
if (tick(delay_int,second_timer) == 1){
  x += brightincrease;
}
//if (brightness >= 255){brightincrease = -1;}
//else if (brightness <= 0){brightincrease = 1;}
  analogWrite(LEDPin,brightness);
}

int button_press (int button_indicator, int button_press_initiated[1], int button_press_complete[1]){
	if (button_indicator == 0 && button_press_initiated[0] == 1) {
	button_press_complete[0] = 1;
	button_press_initiated[0] = 0;
	}
	else if (button_indicator == 1){
	button_press_initiated[0] = 1;
	button_press_complete[0] = 0;
	}
	else {button_press_complete[0] = 0;}
return button_press_complete[0];
}

int tick(int delay, double timekeeper[1]){
currentTime = millis();
if(currentTime >= (timekeeper[0] + delay)){
	timekeeper[0] = currentTime;
	return 1;
  }
else {return 0;}
}
