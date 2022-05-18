/////Définition des pins côté carte/////
#ifndef led_rouge
  #define led_rouge 33 //Pin de la LED rouge
#endif
////////////////////////////////////////

//////Variables temporelles de test//////
#ifndef baudrate
  #define baudrate 115200 //Baudrate de la carte
#endif

double uptime_1 = 0;
double uptime_2 = 0;
double delais[10] = {0,0,0,0,0,0,0,0,0,0};
/////////////////////////////////////////

/////Initialisation Buzzer et mélodie/////
#include "pitches.h"   //Librairie des notes jouables par le buzzer
#include "Tone32.h"    //Librairie des fonctions tone pour l'ESP32

#ifndef buzzer
  #define buzzer 27    //Pin du driver de buzzer
#endif

#ifndef chan_PWM
  #define chan_PWM 9   //Canal PWM de la pin "buzzer"
#endif

#ifndef note
  #define note NOTE_C4 //Note élémentaire du buzzer (plus de valeurs dans “pitches.h”)
#endif

int son[] = { note, 2*note, note, 2*note }; //Mélodie à jouer par le buzzer, 
                                            //basée sur la note originale
int duree[] = { 250, 250, 250, 250 }; //Durée de lecture de chaque note (< ou = à 250)
int nb_notes = 4;                     //Nombre de notes dans la mélodie
//////////////////////////////////////////

/////Fonction Buzzer_Setup() : initialisation du buzzer/////
void Buzzer_Setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(led_rouge, OUTPUT);
}
////////////////////////////////////////////////////////////

/////Fonction Buzzer_Chute() : action du buzzer lors d'une chute///// 
void Buzzer_Chute() {
  digitalWrite(led_rouge, HIGH);
  for (int i = 0; i<nb_notes; i++) { //Lecture de la mélodie pour une chute
    tone(buzzer, son[i], duree[i], chan_PWM);
    delay(250);
    noTone(buzzer);
  }
  digitalWrite(led_rouge, LOW);
  delay(1000);                       //Délai d'activation du buzzer
}
/////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(baudrate);

  //////Chronométrage de la fonction PIR_Setup()//////
  Serial.println("Temps d'exécution de la fonction Buzzer_Setup (10 échantillons) : ");
  for(int i=0; i<10; i++){
    uptime_1 = millis();
    Buzzer_Setup();
    uptime_2 = millis();
    Serial.print((double)(uptime_2 - uptime_1));
    Serial.print(" ; ");
  }
  Serial.println("Terminé.");

  //////Chronométrage de la fonction PIR_Measure()//////
  Serial.println("Temps d'exécution de la fonction Buzzer_Chute (10 échantillons) : ");
  for(int i=0; i<10; i++){
    uptime_1 = millis();
    Buzzer_Chute();
    uptime_2 = millis();
    Serial.print((double)(uptime_2 - uptime_1));
    Serial.print(" ; ");
  }
  Serial.println("Terminé.");
}

void loop() {
  
}
