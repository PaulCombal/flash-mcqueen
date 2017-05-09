#include "Arduino.h"

void detectFront(int pin){
  int sensorValue = analogRead(pin);
  //plus l'objet est proche, plus la valeur est grande
  //inversons la valeur afin d'avoir une distance faible si l'objet est proche
  float distanceFromObject = 10000/(float)sensorValue;

  return distanceFromObject;
}

void detectLeft(int pin){
  int sensorValue = analogRead(pin);
  //plus l'objet est proche, plus la valeur est grande
  //inversons la valeur afin d'avoir une distance faible si l'objet est proche
  float distanceFromObject = 10000/(float)sensorValue;

  return distanceFromObject;
}

void detectRight(int pin){
  int sensorValue = analogRead(pin);
  //plus l'objet est proche, plus la valeur est grande
  //inversons la valeur afin d'avoir une distance faible si l'objet est proche
  float distanceFromObject = 10000/(float)sensorValue;

  return distanceFromObject;
}

void detectBack(int pin){
  int sensorValue = analogRead(pin);
  //plus l'objet est proche, plus la valeur est grande
  //inversons la valeur afin d'avoir une distance faible si l'objet est proche
  float distanceFromObject = 10000/(float)sensorValue;

  return distanceFromObject;
}
