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

  destinationPosition.x = 300;
  destinationPosition.y = 150;
  
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
      Serial.println("Il faut faire demi-tour");
      motorDirection = !motorDirection;
      forward();
      forward();
      motorDirection = !motorDirection;
    }
    else if(distanceToFront <= collisionDistance)
    {
      //Si on détecte un obstacle en face

      //Si on détecte un obstacle à gauche ou si on longne la limite  gauche
      if(distanceToLeft <= collisionDistance || currentPosition.x < 2)
      {
        //On va à droite
        Serial.println("Un obstacle est proche en face et a gauche");
        setAngle(180);
        forward();
        setAngle(90);
      }
      else
      {
        Serial.println("Un obstacle est proche en face et à droite");
        setAngle(0);
        forward();
        setAngle(90);
      }
    }
    else if(distanceToRight <= collisionDistance)
    {
      // On détecte un obstacle vers la droite
      Serial.println("Un obstacle est proche juste sur ma droite");
      setAngle(80); //vers la gauche un peu
      forward();
      setAngle(90);
    }
    else if(distanceToLeft <= collisionDistance)
    {
      //On détecte un collision imminente à la gauche
      Serial.println("Un obstacle est proche juste sur ma gauche");
      setAngle(100); //Vers la droite un chouilla
      forward();
      setAngle(90);
    }
    else if(currentPosition.x < 2)
    {
      Serial.print("Je ne détecte rien, mais longe le bord gauche de la zone");
    }
    else if(currentPosition.x > 298)
    {
      Serial.print("Je ne détecte rien, mais longe le bord droit de la zone");
    }
    else if(currentPosition.y < 2)
    {
      Serial.print("Je ne détecte rien, mais longe le bord inferieur de la zone");
    }
    else if(currentPosition.y > 198)
    {
      Serial.print("Je ne détecte rien, mais longe le bord superieur de la zone");
    }
    else
    {
      //Aucune collision imminente
      Serial.println("Aucun obstacle détecté, ma position est");
      Serial.print(currentPosition.x);
      Serial.print("\t");
      Serial.print(currentPosition.y);
      Serial.print("\n");
      
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
