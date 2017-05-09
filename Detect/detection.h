#include "Arduino.h"

void forward(int distance, int pin){
	for (int i = 0; i < distance; ++i)
	{
		digitalWrite(pin, HIGH);
	}
