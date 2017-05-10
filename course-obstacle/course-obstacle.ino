//#include <mouvement.h>
/*include of gyro libraries*/

#include <Wire.h>
#include "../I2Cdev.h"
#include "../MPU6050.h"

MPU6050 accelgyro;
int16_t ax, ay, az;  // define accel as ax,ay,az
int16_t gx, gy, gz;  // define gyro as gx,gy,gz

int motorPin = 13;

void setup() {
	Serial.begin(9600);

	pinMode(motorPin, OUTPUT);

  Serial.write("OK");

  Wire.begin();      // join I2C bus
  Serial.begin(38400);    //  initialize serial communication
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void loop() {
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
}
