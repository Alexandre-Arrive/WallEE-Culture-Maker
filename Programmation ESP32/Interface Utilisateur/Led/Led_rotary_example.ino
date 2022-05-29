////PIN ENCODEUR////////////
#define pin_encodeur_1 25//13 
#define pin_encodeur_2 26//14
#define pin_encodeur_key 27//16
int delay_rotation=100;


/////////PIN LED///////////
#define led_blanche 13//2
#define led_rouge 12//5
#define led_verte 14//12

#define buzzer 33
#define buz_pwm_channel 5 //Choisit le canal 0

///////////////////////CLASSE///////////////////////
class encodeur{
public:
  encodeur();           //Constructeur
  void Setup();         //Setup
  long unsigned temps;  //NE PAS UTILISER
  bool old_etat_s1;     //NE PAS UTILISER
  bool old_etat_bouton; //NE PAS UTILISER 
  bool etat_s1;         //NE PAS UTILISER
  bool etat_bouton;     //NE PAS UTILISER
  int compteur;         //peut être négatif et positif
  bool sens_rot;        //0=gauche 1=droite
  bool bouton;          //0=non   1=oui
};
  
encodeur::encodeur(){  //Constructeur
}

void encodeur::Setup(){ //Setup
  pinMode(pin_encodeur_1,INPUT); 
  pinMode(pin_encodeur_2,INPUT);
  pinMode(pin_encodeur_key,INPUT); //à 0 lorsque bouton enclenché
  temps=millis();
  compteur=0;
  old_etat_s1=digitalRead(pin_encodeur_1);
}
///////////////////////FIN CLASSE/////////////////////

encodeur encod; //déclare un object encod global

///POUR LE TEST////
int old_compteur=encod.compteur;
int led_3bit=0;
//////////////////

void setup() {
  Serial.begin (115200);
  pinMode(led_blanche, OUTPUT);
  pinMode(led_verte, OUTPUT);
  pinMode(led_rouge, OUTPUT);
  pinMode(buzzer, OUTPUT); 

 ledcSetup(buz_pwm_channel, 500, 8); // Configuration du canal 0 avec la fréquence et la résolution choisie
 ledcAttachPin(buzzer, buz_pwm_channel); // Assigne le canal PWM au pin 23
 ledcWrite(buz_pwm_channel, 255); // Créer la tension en sortie choisie 3.3 V -> 255
 Setup_Encodeur();
}

void loop(){
  Serial.println("");
  Serial.print("Compteur : ");Serial.println(encod.compteur);
  Serial.print("Sens_rot : ");Serial.println(encod.sens_rot);
  Serial.print("Bouton   : ");Serial.println(encod.bouton);
  programme_led_test();
  delay(25);
}

/////////////////////////////////FONCTIONS ENCODEUR//////////////////////////////////
void Setup_Encodeur(){
  encod.Setup();
  attachInterrupt(digitalPinToInterrupt(pin_encodeur_1), rotation_encodeur, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pin_encodeur_key), bouton_on_off, CHANGE);
}

void rotation_encodeur(){
  encod.etat_s1=digitalRead(pin_encodeur_1);
  if( abs(millis() - encod.temps) > delay_rotation ){
    if(digitalRead(pin_encodeur_2) != encod.old_etat_s1){ // Si 2 different de l'ancien état 1
      encod.compteur--;
      encod.sens_rot=0;
    }
    else{
      encod.compteur++; 
      encod.sens_rot=1;
    }
    encod.temps = millis();
  } 
  encod.old_etat_s1 = encod.etat_s1 ;
}

void bouton_on_off(){
  bool read_bouton=digitalRead(pin_encodeur_key);
  encod.bouton=not(read_bouton);
}
/////////////////////////////////////////////////////////////////////////////////////

//////////Programme de TEST///////////////
void programme_led_test(){
  if (old_compteur<encod.compteur){
    if(led_3bit<=0){ led_3bit=7; }
    else { led_3bit--; };  
  }
  else if (old_compteur>encod.compteur){
    if(led_3bit>=7){ led_3bit=0; }
    else { led_3bit++; };    
  }
  if (encod.bouton==1){
    digitalWrite(buzzer, HIGH);
    led_3bit=random(0, 8);
  }
  else{
    digitalWrite(buzzer,LOW);
  }
  int tampon=led_3bit;
  Serial.println(led_3bit);
  if(tampon-4>=0){ tampon=tampon-4; digitalWrite(led_verte, HIGH); }
  else { digitalWrite(led_verte, LOW);}
  if(tampon-2>=0){ tampon=tampon-2; digitalWrite(led_rouge, HIGH); }
  else { digitalWrite(led_rouge, LOW);}
  if(tampon-1>=0){ tampon=tampon-1; digitalWrite(led_blanche, HIGH); }
  else { digitalWrite(led_blanche, LOW);} 
  old_compteur=encod.compteur;
}
