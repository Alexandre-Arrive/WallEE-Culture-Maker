////PIN ENCODEUR////////////
#define pin_encodeur_1 13 
#define pin_encodeur_2 14
#define pin_encodeur_key 16


/////////PIN LED///////////
#define led_blanche 25

/////Initialisation temps de mesure///
double t1=0, t2=0;
double tab[10]={0,0,0,0,0,0,0,0,0,0};
//////////////////////////////////////

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

void setup() {
  Serial.begin (115200);
   ///////MESURE Setup_encodeur/////////
    for(int i=0; i<10; i++){
      t1=millis();
      Setup_Encodeur();
      t2=millis();
      tab[i]=(double)(t2-t1);
      delay(100);
    }
    
    Serial.println("");Serial.println("");Serial.println("");Serial.println("");
    Serial.print("Les 10 temps d'execution de Setup_encodeur :");
    for(int i=0; i<10; i++){
      Serial.print(tab[i]); Serial.print("   ");
      tab[i]=(double)(t2-t1);
    }
    Serial.println("");Serial.println("");Serial.println("");Serial.println("");
    ///////////////////////////

    ///////MESURE rotation_encodeur/////////
    for(int i=0; i<10; i++){
      t1=millis();
      rotation_encodeur();
      t2=millis();
      tab[i]=(double)(t2-t1);
      delay(100);
    }
    
    Serial.println("");Serial.println("");Serial.println("");Serial.println("");
    Serial.print("Les 10 temps d'execution de rotation_encodeur :");
    for(int i=0; i<10; i++){
      Serial.print(tab[i]); Serial.print("   ");
      tab[i]=(double)(t2-t1);
    }
    Serial.println("");Serial.println("");Serial.println("");Serial.println("");
    ///////////////////////////

    ///////MESURE bouton_on_off/////////
    for(int i=0; i<10; i++){
      t1=millis();
      bouton_on_off();
      t2=millis();
      tab[i]=(double)(t2-t1);
      delay(100);
    }
    
    Serial.println("");Serial.println("");Serial.println("");Serial.println("");
    Serial.print("Les 10 temps d'execution de bouton_on_off :");
    for(int i=0; i<10; i++){
      Serial.print(tab[i]); Serial.print("   ");
      tab[i]=(double)(t2-t1);
    }
    Serial.println("");Serial.println("");Serial.println("");Serial.println("");
    ///////////////////////////

    ///////MESURE encod.Setup/////////
    for(int i=0; i<10; i++){
      t1=millis();
      encod.Setup();
      t2=millis();
      tab[i]=(double)(t2-t1);
      delay(100);
    }
    
    Serial.println("");Serial.println("");Serial.println("");Serial.println("");
    Serial.print("Les 10 temps d'execution de encod.Setup :");
    for(int i=0; i<10; i++){
      Serial.print(tab[i]); Serial.print("   ");
      tab[i]=(double)(t2-t1);
    }
    Serial.println("");Serial.println("");Serial.println("");Serial.println("");
    ///////////////////////////
}

void loop(){
  Serial.println("");
  Serial.print("Compteur : ");Serial.println(encod.compteur);
  Serial.print("Sens_rot : ");Serial.println(encod.sens_rot);
  Serial.print("Bouton   : ");Serial.println(encod.bouton);
  delay(2500);
}

/////////////////////////////////FONCTIONS ENCODEUR//////////////////////////////////
void Setup_Encodeur(){
  encod.Setup();
  attachInterrupt(digitalPinToInterrupt(pin_encodeur_1), rotation_encodeur, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pin_encodeur_key), bouton_on_off, CHANGE);
}

void rotation_encodeur(){
  encod.etat_s1=digitalRead(pin_encodeur_1);
  if( abs(millis() - encod.temps) > 50 ){
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
