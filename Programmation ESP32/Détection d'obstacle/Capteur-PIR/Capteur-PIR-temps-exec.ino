//////Définition de la Pin//////
#ifndef baudrate 
  #define baudrate 115200    //Baudrate du microcontrôleur utilisé
#endif

#ifndef led_verte
  #define led_verte 32       //LED de retour
#endif
////////////////////////////////////////////

//////Variables temporelles de test//////
double uptime_1 = 0;
double uptime_2 = 0;
double delais[10] = {0,0,0,0,0,0,0,0,0,0};
/////////////////////////////////////////

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
void PIR_Measure() {               //Usage cadencé par le programme, mais le capteur va toujours maitenir
  sig_PIR = digitalRead(PIR);  //la sortie à HIGH pendant 5 secondes avant de redescendre en LOW 
  if (sig_PIR == HIGH) {
    digitalWrite(led_verte, HIGH);
    delay(20);
    digitalWrite(led_verte, LOW);
  }
}
//////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(baudrate);

  //////Chronométrage de la fonction PIR_Setup()//////
  Serial.println("Temps d'exécution de la fonction PIR_Setup (10 échantillons) : ");
  for(int i=0; i<10; i++){
    uptime_1 = millis();
    PIR_Setup();
    uptime_2 = millis();
    Serial.print((double)(uptime_2 - uptime_1));
    Serial.print(" ; ");
  }
  Serial.println("Terminé.");

  //////Chronométrage de la fonction PIR_Measure()//////
  Serial.println("Temps d'exécution de la fonction PIR_Measure (10 échantillons) : ");
  for(int i=0; i<10; i++){
    uptime_1 = millis();
    PIR_Measure();
    uptime_2 = millis();
    Serial.print((double)(uptime_2 - uptime_1));
    Serial.print(" ; ");
  }
  Serial.println("Terminé.");
}

void loop(){

}
