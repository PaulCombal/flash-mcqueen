#define LEFT 180
#define RIGHT 0
#define STRAIGHT 90
#define FRONT true
#define SIDES false

struct Position
{
  float x;
  float y;
};
typedef struct Position Position;


// Moteur

const int enablePin = 11; //pwm
const int in1Pin = 13;
const int in2Pin = 12;
const int reservedLowPin = 9;

bool motorDirection = false;
const int defaultSpeed = 175;

// Servo

int periode=20000;// période entre chaque début d'impulsion en microsecondes
const int pinServo=2; // variable pour le pin connecté à la commande du servo

// Detecteurs  

const int rightSonarTrig = 8;
const int leftSonarTrig = 7;
const int frontSonarTrig = 4;


const int rightSonarEcho = 6;
long rightSonarRead;
long rightSonarCm;

const int leftSonarEcho = 5;
long leftSonarRead;
long leftSonarCm;

const int frontSonarEcho = 3;
long frontSonarRead;
long frontSonarCm;

// Algo

const short int neededRecords = 5;
short int savedRecords = 0;
int rightSummedRecords = 0;
int leftSummedRecords = 0;
int frontSummedRecords = 0;
int collisionDistance = 10;
int collisionDistanceFront = 30;
int currentWheelAngle = 90; // Updated after a setAngle call
float currentVehicleAngle = 90; //Updated after currentWheelAngle used
Position currentPosition;
Position destinationPosition;
bool destinationReached = false;


// SETUP =================================

void setup() {
  Serial.begin(115200);

  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  
  pinMode(pinServo, OUTPUT);
  
  pinMode(rightSonarTrig, OUTPUT);
  pinMode(leftSonarTrig, OUTPUT);
  pinMode(frontSonarTrig, OUTPUT);
  
  pinMode(rightSonarEcho, INPUT);
  pinMode(leftSonarEcho, INPUT);
  pinMode(frontSonarEcho, INPUT);

  //digitalWrite(pinServo,LOW);
  digitalWrite(rightSonarTrig, LOW);
  digitalWrite(leftSonarTrig, LOW);
  digitalWrite(frontSonarTrig, LOW);

  currentPosition.x = 0;
  currentPosition.y = 0;

  destinationPosition.x = 300;
  destinationPosition.y = 100;

  Serial.write("OK");

  digitalWrite(reservedLowPin, LOW);

  setAngle(STRAIGHT);
}

// LOOP ==================================

void loop() {

  return;

  digitalWrite(rightSonarTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(rightSonarTrig, LOW);

  rightSonarRead = pulseIn(rightSonarEcho, HIGH);
  rightSonarCm = rightSonarRead / 58;
  rightSummedRecords += rightSonarCm;

  delay(50);

  digitalWrite(leftSonarTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(leftSonarTrig, LOW);

  leftSonarRead = pulseIn(leftSonarEcho, HIGH);
  leftSonarCm = leftSonarRead / 58;
  leftSummedRecords += leftSonarCm;

  delay(50);

  digitalWrite(frontSonarTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(frontSonarTrig, LOW);

  //Relevé de la distance en face
  frontSonarRead = pulseIn(frontSonarEcho, HIGH);
  frontSonarCm = frontSonarRead / 58;
  frontSummedRecords += frontSonarCm;


  savedRecords++;

  //if(currentWheelAngle != 80)
  //  setAngle(80);

  //Si on a fait suffisament de relevés pour être certain que les mesures soient
  //significatives
  if(savedRecords > neededRecords)
  {
    Serial.print("Distance to right is ");
    Serial.print(rightSummedRecords);
    Serial.print("\n");
    
    Serial.print("Distance to left is ");
    Serial.print(leftSummedRecords);
    Serial.print("\n");

    Serial.print("Distance to front is ");
    Serial.print(frontSummedRecords);
    Serial.print("\n");

    Serial.print("Ma position est: ");
    Serial.print(currentPosition.x);
    Serial.print("\t - \t");
    Serial.print(currentPosition.y);
    Serial.print("\n");

    Serial.print("L'angle du véhicule est: ");
    Serial.print(currentVehicleAngle);
    Serial.print("\n");

    //Si on détecte un obstacle vers la droite, la gauche, et en face
    if(collide(rightSummedRecords, SIDES) && collide(leftSummedRecords, SIDES) && collide(frontSummedRecords, FRONT))
    {
      //Demi tour
      Serial.println("Il faut faire demi-tour");
      motorDirection = !motorDirection;
      forward();
      forward();
      motorDirection = !motorDirection;
    }
    else if(collide(frontSummedRecords, FRONT))
    {
      //Si on détecte un obstacle en face

      //Si on détecte un obstacle à droite
      if(collide(rightSummedRecords, SIDES))
      {
        //On va à gauche
        Serial.println("Un obstacle est proche en face et a droite");
        setAngle(LEFT);
        forward();
      }
      /*else if(distanceToLeft <= collisionDistance)
      {
        Serial.println("Un obstacle est proche en face et à droite");
        setAngle(LEFT);
        forward();
        setAngle(STRAIGHT);
      }*/
      else
      {
        //dans tous les autres cas on va à droite
        Serial.println("Il y a un obstacle juste en face de moi et éventuellement à gauche");
        setAngle(RIGHT);
        forward();
      }
    }
    else if(collide(rightSummedRecords, SIDES))
    {
      // On détecte un obstacle vers la droite
      Serial.println("Un obstacle est proche juste sur ma droite");
      setAngle(110); //vers la gauche un peu
      forward();
    }
    else if(collide(leftSummedRecords, SIDES))
    {
      //On détecte un collision imminente à la gauche
      Serial.println("Un obstacle est proche juste sur ma gauche");
      setAngle(70); //Vers la droite un chouilla
      forward();
    }
    else if(currentPosition.x < 10 && (currentVehicleAngle > 90 && currentVehicleAngle < 270))
    {
      //On sort de la zone par la gauche
      Serial.println("Je ne détecte rien, mais longe le bord gauche de la zone");
      setAngle(RIGHT);
      forward();
    }
    else if(currentPosition.x > 290 && (currentVehicleAngle < 90 && currentVehicleAngle > 270))
    {
      //On sort de la zone par la droite
      Serial.println("Je ne détecte rien, mais longe le bord droit de la zone");
      setAngle(RIGHT);
      forward();
    }
    else if(currentPosition.y < 10 && (currentVehicleAngle > 180 && currentVehicleAngle < 360))
    {
      //On sort de la zone par en bas
      Serial.println("Je ne détecte rien, mais longe le bord inferieur de la zone");
      setAngle(RIGHT);
      forward();
    }
    else if(currentPosition.y > 190 && currentVehicleAngle < 180 && currentVehicleAngle > 0)
    {
      //On sort de la zone par le haut
      Serial.println("Je ne détecte rien, mais longe le bord superieur de la zone");
      setAngle(RIGHT);
      forward();
    }
    else
    {
      //Aucune collision imminente
      Serial.println("Rien ne se passe");
      setAngle(LEFT);
      forward();
    }

    if(frontSummedRecords > 0 && frontSummedRecords < 7)
    {
      Serial.println("Oups, on s'est mangé un mur");
      setAngle(STRAIGHT);
      motorDirection = !motorDirection;
      forward();
      forward();
      motorDirection = !motorDirection;
      
    }

    if(fabs(currentPosition.x - destinationPosition.x) < 10 && fabs(currentPosition.x - destinationPosition.x) < 10)
    {
      //On est arrivé
      Serial.write("On est arrive");
      return;
    }

    rightSummedRecords = 0;
    leftSummedRecords = 0;
    frontSummedRecords = 0;
  } // => fin de "Si on a assez de relevés"
} // => fin de loop

// CAPTEURS ==============================
bool collide(int summedDistance, bool isFront)
{
  if(isFront)
    return summedDistance > 0 && summedDistance < collisionDistanceFront;
  else
    return summedDistance > 0 && summedDistance < collisionDistance;
}

// MOTEUR / FORWARD =======================
//Avance la voiture d'1 cm
void forward(){
  analogWrite(enablePin, defaultSpeed);
  digitalWrite(in1Pin, motorDirection);
  digitalWrite(in2Pin, !motorDirection);

  delay(100);

  digitalWrite(enablePin, LOW);

    //Si on va tout droit
  if(motorDirection == false)
  {
    if(currentWheelAngle > 90) {
      currentVehicleAngle += 1.34;
    }
    else if(currentWheelAngle < 90) {
      currentVehicleAngle -= 1.34;
    }
    
    currentPosition.x += cos(currentVehicleAngle * 3.14 / 180);
    currentPosition.y += sin(currentVehicleAngle * 3.14 / 180);
  }
  else
  {
    //Marche arrière
    if(currentWheelAngle > 90) {
      currentVehicleAngle -= 1.34;
    }
    else if(currentWheelAngle < 90) {
      currentVehicleAngle += 1.34;
    }
    
    currentPosition.x -= cos(currentVehicleAngle * 3.14 / 180);
    currentPosition.y -= sin(currentVehicleAngle * 3.14 / 180);
  }

  if(currentVehicleAngle > 360)
    currentVehicleAngle -= 360;
  if(currentVehicleAngle < -360)
    currentVehicleAngle += 360;

}

// SETANGLE / SERVO =======================

//fonction setAngle pour envoyer les impulsions
void setAngle(int a){
  currentWheelAngle = a;
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
