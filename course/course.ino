//#include <mouvement.h>

// Moteur

const int enablePin = 11; //pwm
const int in1Pin = 13;
const int in2Pin = 12;
bool motorDirection = true;
const int defaultSpeed = 255;

// Servo

int periode=20000;// période entre chaque début d'impulsion en microsecondes
const int pinServo=8; // variable pour le pin connecté à la commande du servo

// Detecteurs

// définition des broches utilisées 
int trig = 8; 
int echo = 6; 
long lecture_echo; 
long cm;

// SETUP =================================

void setup() {
	Serial.begin(115200);
	
	pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(pinServo, OUTPUT);
  pinMode(trig, OUTPUT); 
  pinMode(echo, INPUT); 

  digitalWrite(pinServo,LOW);
  digitalWrite(trig, LOW); 

  forward(defaultSpeed);
  
  Serial.write("OK");
}

// LOOP ==================================

void loop() {
  forward(defaultSpeed);
  delay(1000);
  forward(0);

  digitalWrite(trig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trig, LOW); 
  lecture_echo = pulseIn(echo, HIGH); 
  cm = lecture_echo / 58; 
  Serial.print("Distancem : "); 
  Serial.println(cm); 
  delay(1000); 

  
  if(false) //On détecte un obstacle vers la droite
  {
    setAngle(0);//vers la gauche
  }
  else if(false) // On détecte un obstacle vers la gauche
  {
    setAngle(0); //vers la droite
  }
  if(false) //On détecte un collision imminente à l'avant
  {
    motorDirection = !motorDirection;
    forward(defaultSpeed);
    delay(1000); // TODO ajuster
    motorDirection = !motorDirection;
    forward(defaultSpeed);
  }
}

// MOTEUR / FORWARD =======================

void forward(int motorSpeed){
  analogWrite(enablePin, motorSpeed);
  digitalWrite(in1Pin, !motorDirection);
  digitalWrite(in2Pin, motorDirection);
}

// SETANGLE / SERVO =======================

//fonction setAngle pour envoyer les impulsions
void setAngle(int a){
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
