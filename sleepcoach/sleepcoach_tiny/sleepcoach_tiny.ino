#include <math.h>


// Sleep coach using ATTiny 85


char* mode = "time_choose"; // Modes are time_choose, sleep_coach, and off
// Time choose mode is where the user choses between 7, 14, 21, and 28 minutes of sleep coaching
// sleep coach mode is the mode with pulsating light
// off is when the sleep coaching is complete. A button press will bring it into time choose mode

int time_choice = 7;

const int LED1Pin = 5;
const int LED2Pin = 9;
const int LED3Pin = 6;
const int LED4Pin = 10;

const int ButtonPin = 11;
int button_state = 0;
int button_pushed = 0; // This is the indicator that the button was pushed and released
int button_counter = 0; // This is used to detect how long the button is held for

#define potPin 5 // Pin for potentiometer
int pot_val; // Value of potentiometer reading

int blink_pattern[4] = {1,0,0,0}; // This is the indicator for the current time setting. It will blink 1 of 4 LEDs
int blink = 1; // This is used for blinking the LEDs


unsigned long currentTime;
double milis_timer[1] = {0}; // This is used to keep track of the timer used to tick for each milisecond
double second_timer[1] = {0}; // This is used to keep track of the timer used to tick for each second
double blink_timer[1] = {0};  // This is used to keep track of each half second for blinking
int ticked = 0;

int delay_int = 1;
int brightness = 0;
int max_brightness = 244;
double brightincrease = 1;
double k = 0.00108;
double k_initial = 0.00108;
double k_final = 0.00065;
double x = 3*3.14159/2/k; // This starts it at 0 brightness

double total_time = 420; // seconds for entire breathing coaching
double current_time = 0;

int button_press_initiate[1];     // storage for button press function
int button_press_completed[1];    // storage for button press function


void setup() {                
  // initialize the digital pin as an output.
  pinMode(LED1Pin, OUTPUT);
  pinMode(LED2Pin, OUTPUT);
  pinMode(LED3Pin, OUTPUT);
  pinMode(LED4Pin, OUTPUT);
  pinMode(ButtonPin, INPUT);
//  Serial.begin(9600);  
}


void loop() {
  
pot_val = analogRead(potPin);

max_brightness = pot_val/(1024/254);

button_state = digitalRead(ButtonPin);
button_pushed = button_press (button_state, button_press_initiate, button_press_completed);
  
if (mode == "time_choose"){
  
delay(10);

if (button_pushed == 1){
time_choice += 7;
button_pushed = 0;
}
if (time_choice > 28){time_choice = 7;}
  
  if (time_choice == 7){
    blink_pattern[0] = max_brightness;
    blink_pattern[1] = 0;
    blink_pattern[2] = 0;
    blink_pattern[3] = 0;
    k_final = 0.00065; // Equates to 10 second breath cycle
    total_time = 420;
  }
  if (time_choice == 14){
    blink_pattern[0] = 0;
    blink_pattern[1] = max_brightness;
    blink_pattern[2] = 0;
    blink_pattern[3] = 0;
    k_final = 0.00056; // Equates to 12 second breath cycle
    total_time = 840;
  }
  if (time_choice == 21){
    blink_pattern[0] = 0;
    blink_pattern[1] = 0;
    blink_pattern[2] = max_brightness;
    blink_pattern[3] = 0;
    k_final = 0.00046; // Equates to 14 second breath cycle
    total_time = 1260;
  }
  if (time_choice == 28){
    blink_pattern[0] = 0;
    blink_pattern[1] = 0;
    blink_pattern[2] = 0;
    blink_pattern[3] = max_brightness;
    k_final = 0.000406; // Equates to 16 second breath cycle
    total_time = 1680;
  }
 
if (tick(1000,second_timer) == 1){
if (button_state == 1){button_counter += 1;} 
}

if (button_state == 0){button_counter = 0;}

if (tick(500,blink_timer) == 1){
 if (blink == 1){blink = 0;}
 else if (blink == 0){blink = 1;}
}

if (button_state == 1){
blink = 1;
}

if (blink == 0){  
  blink_pattern[0] = 0;
  blink_pattern[1] = 0;
  blink_pattern[2] = 0;
  blink_pattern[3] = 0;
}

analogWrite(LED1Pin,  blink_pattern[0]);
analogWrite(LED2Pin,  blink_pattern[1]);
analogWrite(LED3Pin,  blink_pattern[2]);
analogWrite(LED4Pin,  blink_pattern[3]);

if (button_counter >= 3){ // If the user holds the button for 3 seconds, start the sleep coach
button_pushed = 0;
mode = "sleep_coach";
button_counter = 0;
}
 
}
  
if (mode == "sleep_coach"){
  
if (current_time >= total_time){
current_time = 0;
mode = "off";
x = 0;
}
  
brightness = max_brightness/2*(1 + sin(k*x));
  
if (tick(delay_int,milis_timer) == 1){
  x += brightincrease;
}
if (tick(1000,second_timer) == 1){
  current_time += 1;
  k = k_initial + current_time*(k_final-k_initial)/total_time;
  if (button_state == 1){button_counter += 1;} 
}
if (x*k >= 2*3.14159){x=0;}
//else if (brightness <= 0){brightincrease = 1;}
  analogWrite(LED1Pin,brightness);
  analogWrite(LED2Pin,brightness);
  analogWrite(LED3Pin,brightness);
  analogWrite(LED4Pin,brightness);

if (button_state == 0){button_counter = 0;}

if (button_pushed == 1 && current_time > 5){ // Turn the device off by pushing the button, but do not create false press after starting
mode = "off";
button_pushed = 0;
button_counter = 0;
current_time = 0;
x = 3*3.14159/2/k; // Start it back at 0 brightness
}

}

if (mode == "off"){
  
analogWrite(LED1Pin,  0);
analogWrite(LED2Pin,  0);
analogWrite(LED3Pin,  0);
analogWrite(LED4Pin,  0);
  

delay(1);

if (button_pushed == 1){ // Turn the device on by pushing the button
mode = "time_choose";
button_pushed = 0;
button_counter = 0;
}

current_time = 0;
  
}

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

void tick_reset(double timekeeper[1]){
currentTime = millis();
timekeeper[0] = currentTime;
}
