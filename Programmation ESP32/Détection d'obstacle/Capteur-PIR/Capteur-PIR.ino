//////Définition de la Pin et du délai//////
#ifndef baudrate 
  #define baudrate 9600    //Baudrate du capteur PIR
#endif

#ifndef led_verte
  #define led_verte 32     //LED de retour
#endif
////////////////////////////////////////////

//////Initialisation PIR//////
#ifndef PIR 
  #define PIR 35       //Pin OUT du capteur PIR
#endif

bool sig_PIR = LOW; //Signal de détection de mouvement (format booléen)
//////////////////////////////

//////Fonction PIR_Setup() : initialisation du capteur PIR//////
void PIR_Setup() {
  pinMode(PIR, INPUT); //Mise en sortie de la broche de sortie du capteur
}
////////////////////////////////////////////////////////////////

//////Fonction PIR_Measure() : Détection de mouvement via le capteur PIR//////
void PIR_Measure() {           //Usage cadencé par le programme, mais le capteur va toujours maitenir
  sig_PIR = digitalRead(PIR);  //la sortie à HIGH pendant 5 secondes avant de redescendre en LOW 
  if (sig_PIR) {
    digitalWrite(led_verte, HIGH);
    delay(20);
    digitalWrite(led_verte, LOW);
  }
}
//////////////////////////////////////////////////////////////////////////////

void setup() {
  pinMode(led_verte, OUTPUT); //Mise en sortie de la LED de test
  Serial.begin(baudrate);     //Lancement de la liaison série
  PIR_Setup();                //Initialisation du capteur PIR
}

void loop() {
  PIR_Measure();                     //Relevé d’un mouvement en cours
  Serial.println(sig_PIR ? "Détection : True" : "Détection : False");
  delay(1000);                       //Délai de mesure
} 


