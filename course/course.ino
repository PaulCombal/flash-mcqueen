//#include <mouvement.h>

const int enablePin = 11; //pwm
const int in1Pin = 13;
const int in2Pin = 12;

const bool motorDirection = true;

void setup() {
	Serial.begin(9600);
	
	pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);

  Serial.write("OK");
}

void loop() {
  forward(200);
}

void forward(int motorSpeed){
  analogWrite(enablePin, motorSpeed);
  digitalWrite(in1Pin, !motorDirection);
  digitalWrite(in2Pin, motorDirection);
}

