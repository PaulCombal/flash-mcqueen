//#include <mouvement.h>

// Moteur

const int enablePin = 11; //pwm
const int in1Pin = 13;
const int in2Pin = 12;
bool motorDirection = true;
const int defaultSpeed = 255;

// Servo

int periode=20000;// période entre chaque début d'impulsion en microsecondes
const int pinServo=4; // variable pour le pin connecté à la commande du servo

// Detecteurs

const int rightSonarTrig = 8; 
const int rightSonarEcho = 6;
long rightSonarRead;
long rightSonarCm;

//const int leftSonarTrig = 8; 
//const int leftSonarEcho = 6;
//long leftSonarRead;
//long leftSonarCm;

//const int frontSonarTrig = 8; 
//const int frontSonarEcho = 6;
//long frontSonarRead;
//long frontSonarCm;

// Algo

const short int neededRecords = 5;
short int savedRecords = 0;
int rightSummedRecords = 0;
int leftSummedRecords = 0;
int frontSummedRecords = 0;
int collisionDistance = 10;
int currentAngle; // Updated after a setAngle call
Position currentPosition;
Position destinationPosition;
bool destinationReached = false;

int defaultSpeed = 200;
int startAngle;
int currentAngle = 90;
//90 -> vers le haut

struct Position
{
  int x;
  int y;
}
typedef struct Position Position;

// SETUP =================================

void setup() {
  Serial.begin(115200);
  
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(pinServo, OUTPUT);
  pinMode(rightSonarTrig, OUTPUT); 
  pinMode(rightSonarEcho, INPUT); 

  digitalWrite(pinServo,LOW);
  digitalWrite(rightSonarTrig, LOW); 
  
  currentPosition.x = 0:
  currentPosition.y = 0:

  destinationPosition.x = 0;
  destinationPosition.y = 0;
  
  Serial.write("OK");

  //TODO Enregistrer les données du gyro pour que actuellement on soit vers notre nord

  //startAngle = /*lecture du gyro*/
  
  setAngle(90);
}

// LOOP ==================================

void loop() {

  /*//Relevé de la distance à droite
  digitalWrite(rightSonarTrig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(rightSonarTrig, LOW); 
  rightSonarRead = pulseIn(rightSonarEcho, HIGH); 
  rightSonarCm = rightSonarRead / 58; 
  rightSummedRecords += rightSonarCm;

  //Relevé de la distance à gauche
  digitalWrite(leftSonarTrig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(leftSonarTrig, LOW); 
  leftSonarRead = pulseIn(leftSonarEcho, HIGH); 
  leftSonarCm = leftSonarRead / 58; 
  leftSummedRecords += leftSonarCm;

  //Relevé de la distance en face
  digitalWrite(frontSonarTrig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(frontSonarTrig, LOW); 
  frontSonarRead = pulseIn(frontSonarEcho, HIGH); 
  frontSonarCm = frontSonarRead / 58; 
  frontSummedRecords += frontSonarCm;*/

  
  savedRecords++;

  if(currentAngle != 80)
    setAngle(80);

  //Si on a fait suffisament de relevés pour être certain que les mesures soient
  //significatives
  if(savedRecords > neededRecords)
  {
    int distanceToRight = rightSummedRecords / (float)neededRecords;
    int distanceToLeft = leftSummedRecords / (float)neededRecords;
    int distanceToFront = frontSummedRecords / (float)neededRecords;

    rightSummedRecords = 0;
    leftSummedRecords = 0;
    frontSummedRecords = 0;

    //Si on détecte un obstacle vers la droite, la gauche, et en face
    if(distanceToRight <= collisionDistance && distanceToLeft <= collisionDistance && distanceToFront <= collisionDistance)
    {
      //Demi tour
      motorDirection = !motorDirection;
      forward(defaultSpeed);
      delay(1000); // TODO ajuster
      motorDirection = !motorDirection;
      forward(defaultSpeed);
    }
    else if(distanceToFront <= collisionDistance)
    {
      if(distanceToLeft <= collisionDistance)
      {
        //On va à droite
        setAngle(180);
      }
      else
      {
        setAngle(0);
      }
    }
    else if(distanceToRight <= collisionDistance)
    {
      // On détecte un obstacle vers la droite
      setAngle(30); //vers la gauche un peu
    }
    else if(distanceToLeft <= collisionDistance)
    {
      //On détecte un collision imminente à la gauche
      setAngle(90); //Vers la droite un chouilla
    }
    else
    {
      //Aucune collision imminente
      forward();
    }

    if(fabs(currentPosition.x - destinationPosition.x) < 10 && fabs(currentPosition.x - destinationPosition.x) < 10)
    {
      //On est arrivé
      Serial.write("On est arrive");
      return;
    }
  }
}

// UPDATEANGLE / GYRO 
//détecte la direction dans laquelle on se dirige

void updateAngle()
{
  //currentAngle = /*lecture du gyro*/;
}

// MOTEUR / FORWARD =======================
//Avance la voiture d'1 cm
void forward(){
  analogWrite(enablePin, defaultSpeed);
  digitalWrite(in1Pin, !motorDirection);
  digitalWrite(in2Pin, motorDirection);

  delay(100);

  analogWrite(enablePin, 0);

  currentPosition.x += cos(currentAngle);
  currentPosition.y += sin(currentAngle);
}

// SETANGLE / SERVO =======================

//fonction setAngle pour envoyer les impulsions
//0 min
//160 max
//80 milieu
void setAngle(int a){
  currentAngle = a;
  int duree=map(a,0,179,1000,2000);// on transforme l'angle en microsecondes et on stocke dans la variable duree
  digitalWrite(pinServo,LOW);//on met le pin à l'état bas
  
  // la boucle qui suit est nécessaire 
  // pour laisser le temps au servo d'aller à sa position
  for (int t=0;t<300;t++){ 
    digitalWrite(pinServo,HIGH);// on envoie l'impulsion
    delayMicroseconds(duree); // pendant la bonne durée
    digitalWrite(pinServo,LOW); // on stoppe l'impulsion
    delayMicroseconds(periode-duree); // on attend le temps restant pour atteindre la période
  }
}
