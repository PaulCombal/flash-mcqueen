/*
 # Product: 6 DOF Sensor-MPU6050 
 # SKU    : SEN0142 
 # Description:     
 # To read  accel/gyro data from 6 DOF Sensor 
*/

#include "Wire.h"                 
#include "I2Cdev.h"
#include "MPU6050.h"
MPU6050 accelgyro;  
int16_t ax, ay, az;  // define accel as ax,ay,az
int16_t gx, gy, gz;  // define gyro as gx,gy,gz

int sax, say, saz, sgx, sgy, sgz;

int nombreEchantillon = 0;
          
#define LED_PIN 13
bool blinkState = false;

void setup() {
  sax = 0;
  say = 0;
  saz = 0;
  sgx = 0;
  sgy = 0;
  sgz = 0;
  Wire.begin();      // join I2C bus   
  Serial.begin(38400);    //  initialize serial communication
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();  

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  pinMode(LED_PIN, OUTPUT);  // configure LED pin
}

void loop() {
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);  // read measurements from device

  sax += ax;
  say += ay;
  saz += az;
  sgx += gx;
  sgy += gy;
  sgz += gz;

  nombreEchantillon++;

  if(nombreEchantillon == 100)
  {
    // display tab-separated accel/gyro x/y/z values
    Serial.print("a/g:\t");
    Serial.print(sax/100); 
    Serial.print("\t");
    Serial.print(say/100); 
    Serial.print("\t");
    Serial.print(saz/100); 
    Serial.print("\t");
    Serial.print(sgx/100); 
    Serial.print("\t");
    Serial.print(sgy/100); 
    Serial.print("\t");
    Serial.println(sgz/100); 

    sax = 0;
    say = 0;
    saz = 0;
    sgx = 0;
    sgy = 0;
    sgz = 0;

    nombreEchantillon = 0;
  }
 
  // blink LED to indicate activity
  blinkState = !blinkState;
  digitalWrite(LED_PIN, blinkState);
}
