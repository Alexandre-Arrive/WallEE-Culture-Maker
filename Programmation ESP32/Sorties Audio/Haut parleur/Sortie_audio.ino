#ifndef SON_H
    #define SON_H
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void setup() {
  setupModuleSon();
  delay(2000);
}

void loop() {

  // playAudio(x) avec x compris entre 0 et 4
  // pour bien laisser le temps aux sons de se play mettre des delay entre chaque playAudio

}

// Fonction permettant de lire les audios dans la carte selon leur nom.

// /!\ LES SONS MIS DANS LA CARTE DOIVENT ETRE RANGES DANS CET ORDRE : DEMARRAGE, OBSTACLE, DEPLACEMENT MENU, CHUTE, DEBRANCHEMENT /!\

void playAudio(int music){
  switch (music) {
  case 1: //OBSTACLE : quand music = 1, soit quand un obstacle est détecté, alors on joue le son relatif aux obstacles
    myDFPlayer.play(2);  //Play the second mp3 : obstacle
    break;
  case 2: //DEPLACEMENT DANS LE MENU : quand music = 2, soit qu'on cherche à accèder au menu du robot, alors on joue le son relatif au menu du robot
    myDFPlayer.play(3);  //Play the third mp3 : menu
    break;
  case 3: //CHUTE : quand music = 3, soit quand le robot se casse la gueule, alors on joue le son relatif àa la chute du robot
    myDFPlayer.play(4);  //Play the fourth mp3 : chute
    break;
  case 4: //DEBRANCHEMENT DU ROBOT : quand music = 4, soit quand le robot s'apprête à être éteint, alors on joue le son relatif à son débranchement
    myDFPlayer.play(5);  //Play the fifth mp3 : shutdown
    break;
    
  default:
    // if nothing else matches, do the default
    // default is optional
    break;
  }
}

// Fonction permettant de setup le module 
void setupModuleSon(){
  mySoftwareSerial.begin(9600); //Activation de la liaison série
  Serial.begin(115200);         //Activation de la liaison série
  
  Serial.println();                                                       //Texte écrit dans la console au lancement
  Serial.println(F("DFRobot DFPlayer Mini Demo"));                        //Texte écrit dans la console au lancement
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));  //Texte écrit dans la console au lancement

  //Utilisation de la console série softwareSerial pour communiquer avec les fichiers mp3 :
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {                 // Si la connexion ne se fait pas alors
    Serial.println(F("Unable to begin:"));                   //Texte écrit dans la console si la connexion ne se fait pas
    Serial.println(F("1.Please recheck the connection!"));   //Texte écrit dans la console si la connexion ne se fait pas
    Serial.println(F("2.Please insert the SD card!"));       //Texte écrit dans la console si la connexion ne se fait pas
    while(true); //Tant que la connexion ne se fait pas alors on reste bloqué dans ce while()
  }
  Serial.println(F("DFPlayer Mini online."));                //Si la connexion se fait alors on écrit ce texte dans la console

  // Utilisation des fonctions de DFRobotDFPlayer pour faire jouer le premier son :
  
  myDFPlayer.volume(10);  //On set le volume à 1 (valeur comprise entre 0 et 30)
  myDFPlayer.play(1);    //On joue le premier mp3 : DEMARRAGE
  delay(3000);
}

// Fonction affichant les détails de la connexion avec le module DFPlayer permettant de débuguer les erreurs éventuelles
void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
#endif
