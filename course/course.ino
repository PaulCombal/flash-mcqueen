//#include <mouvement.h>

int motorPin = 13;

void setup() {
	Serial.begin(9600);
	
	pinMode(motorPin, OUTPUT);

  Serial.write("OK");
}

void loop() {
	// put your main code here, to run repeatedly:
	//forward(10, motorPin);
  digitalWrite(motorPin, HIGH);
  delay(1000);
  digitalWrite(motorPin, LOW);
  delay(1000);
}

