#include <HCSR04.h>

//////Initialisation des LEDs/////////
#define led_verte 32
#define led_rouge 33
#define led_blanche 25
//////////////////////////////////////

//////Initialisation ultrasons//////////
#define HC_trig 26                                 
#define HC1 35
#define HC2 34
#define HC3 39
#define HC4 36
HCSR04 hc_front(HC_trig, HC1);                     //(trig, echo)
HCSR04 hc_right(HC_trig, HC2); 
HCSR04 hc_left (HC_trig, HC3); 
HCSR04 hc_back (HC_trig, HC4); 
int erreur_ultrason=0;                             //compteur d'erreur de la lecture ultrason. Incrémente lorsque mesure 0.
int lim_err_HC=5;                                  //limite d'erreur avant reset des capteurs
int front=0; int right=1; int left=2; int back=3; 
int measure_time=60;
int decharge_time=100;
////////////////////////////////////////

/////Initialisation temps de mesure///
double t1=0, t2=0;
double tab[10]={0,0,0,0,0,0,0,0,0,0};
//////////////////////////////////////

//////Setup Ultrasons//////////
void Ultrason_Setup()
{
  if(erreur_ultrason==0){
    digitalWrite(led_blanche, HIGH);
  }
  pinMode(HC_trig, OUTPUT); digitalWrite(HC_trig, LOW);
  pinMode(HC1, OUTPUT);  digitalWrite(HC1, LOW);
  pinMode(HC2, OUTPUT);  digitalWrite(HC2, LOW);
  pinMode(HC3, OUTPUT);  digitalWrite(HC3, LOW);
  pinMode(HC4, OUTPUT);  digitalWrite(HC4, LOW);
  delay(decharge_time); //Temps de décharge des condensateurs des HC, préconisé à 100ms
  pinMode(HC1, INPUT);   pinMode(HC2, INPUT);   pinMode(HC3, INPUT);   pinMode(HC4, INPUT);
  Serial.println("Ultrason Setup Done");
  digitalWrite(led_blanche, LOW);
}
//////////////////////////////////////

void setup()
{ 
  Serial.begin(115200); 
  pinMode(led_verte, OUTPUT);
  pinMode(led_rouge, OUTPUT);
  pinMode(led_blanche, OUTPUT);

  ///////MESURE Ultrason_Setup/////////
  for(int i=0; i<10; i++){
    t1=millis();
    Ultrason_Setup(); 
    t2=millis();
    tab[i]=(double)(t2-t1);
  }
    
  Serial.println("");Serial.println("");Serial.println("");Serial.println("");
  Serial.print("Les 10 temps d'execution de Ultrason_Setup :");
  for(int i=0; i<10; i++){
    Serial.print(tab[i]); Serial.print("   ");
    tab[i]=(double)(t2-t1);
  }
  Serial.println("");Serial.println("");Serial.println("");Serial.println("");
  ///////////////////////////

  ///////MESURE Ultrason_Measure/////////
  for(int j=0; j<4; j++){
    for(int i=0; i<10; i++){
      t1=millis();
      double x = Ultrason_Read(j);
      t2=millis();
      delay(measure_time);
      tab[i]=(double)(t2-t1);
    }
   
    Serial.print("Les 10 temps d'execution de Ultrason_Read capteur numero");Serial.println(j);
    for(int i=0; i<10; i++){
      Serial.print(tab[i]); Serial.print("   ");
      tab[i]=(double)(t2-t1);
    }
    Serial.println("");
  }
  Serial.println("");Serial.println("");Serial.println("");Serial.println("");
  ///////////////////////////

    ///////MESURE Ultrason_indication/////////
  for(int j=0; j<4; j++){
    for(int i=0; i<10; i++){
      t1=millis();
      int y=Ultrason_Indication(j);
      t2=millis();
      delay(measure_time);
      tab[i]=(double)(t2-t1);
    }
   
    Serial.print("Les 10 temps d'execution de Ultrason_Indication capteur numero");Serial.println(j);
    for(int i=0; i<10; i++){
      Serial.print(tab[i]); Serial.print("   ");
      tab[i]=(double)(t2-t1);
    }
    Serial.println("");
  }
  Serial.println("");Serial.println("");Serial.println("");Serial.println("");
  ///////////////////////////


  ///////MESURE Ultrason_All_Read/////////
    for(int i=0; i<10; i++){
      t1=millis();
      double* test3=Ultrason_All_Read();
      t2=millis();
      delay(measure_time);
      delete test3;
      tab[i]=(double)(t2-t1);
    }
    
    Serial.println("Les 10 temps d'execution de Ultrason_All_Read");
    for(int i=0; i<10; i++){
      Serial.print(tab[i]); Serial.print("   ");
      tab[i]=(double)(t2-t1);
    }
    
  Serial.println("");Serial.println("");Serial.println("");Serial.println("");
  ///////////////////////////

  ///////MESURE Ultrason_All_Indication/////////
    for(int i=0; i<10; i++){
      t1=millis();
      int* test4=Ultrason_All_Indication();
      t2=millis();
      delay(measure_time);
      delete test4;
      tab[i]=(double)(t2-t1);
    }
    
    Serial.println("Les 10 temps d'execution de Ultrason_All_Indication");
    for(int i=0; i<10; i++){
      Serial.print(tab[i]); Serial.print("   ");
      tab[i]=(double)(t2-t1);
    }
   
  Serial.println("");Serial.println("");Serial.println("");Serial.println("");
  ///////////////////////////

  ///////MESURE ultrason_ERREUR/////////
    for(int i=0; i<10; i++){
      t1=millis();
      ultrason_ERREUR();
      t2=millis();
      delay(measure_time);
      tab[i]=(double)(t2-t1);
    }
    
    Serial.println("Les 10 temps d'execution de ultrason_ERREUR");
    for(int i=0; i<10; i++){
      Serial.print(tab[i]); Serial.print("   ");
      tab[i]=(double)(t2-t1);
    }
    
  Serial.println("");Serial.println("");Serial.println("");Serial.println("");
  ///////////////////////////

}

void loop()
{
}

/////////////Fonctions 1 capteur/////////////////////////////
double Ultrason_Read(int choix_capteur){ //Lit la distance lu par un des 4 capteurs HC-SR04
  switch (choix_capteur){
    case 0: return hc_front.dist();
    case 1: return hc_right.dist();
    case 2: return hc_left.dist();
    case 3: return hc_back.dist();
  }
}

int Ultrason_Indication(int choix_capteur){  //Indique un degré d'éloignement de l'obstacle entre 0 et 3: 0-erreur 1<20cm 2<50cm 3<100cm 4>100cm
  double distance = Ultrason_Read(choix_capteur);
  if (distance<=0){
    if(++erreur_ultrason>lim_err_HC) { ultrason_ERREUR(); }
    else return 0;
  }
  else if (distance<20)  { return 1; }
  else if (distance<50)  { return 2; }
  else if (distance<100) { return 3; }
  else                   { return 4; }
}
//////////////////////////////////////////////////////////////


/////////////Fonctions 4 capteurs/////////////////////////////
double* Ultrason_All_Read(){ //Ne pas oublier de supprimer l'allocation mémoire distance !
  double* distance= new double[4];
  distance[0]=Ultrason_Read(front);
  delay(measure_time);
  distance[1]=Ultrason_Read(right);
  delay(measure_time);
  distance[2]=Ultrason_Read(left); 
  delay(measure_time);
  distance[3]=Ultrason_Read(back);  
  return distance;
}

int* Ultrason_All_Indication(){ //Ne pas oublier de supprimer l'allocation mémoire indication !
  int* indication=new int[4];
  double* distance=Ultrason_All_Read();
  for (int i=0; i<=3; i++){
    if (distance[i]<=0){
      if(++erreur_ultrason>lim_err_HC) { ultrason_ERREUR(); }
      else indication[i]=0;
    }
    else if (distance[i]<20)  { indication[i]=1; }
    else if (distance[i]<50)  { indication[i]=2; }
    else if (distance[i]<100) { indication[i]=3; }
    else                      { indication[i]=4; }
  }
  delete distance; //Suppression allocation mémoire générée par Ultrason_All_Read();
  return indication;
}
//////////////////////////////////////////////////////////////


/////////////Fonctions erreur/////////////////////////////////
void ultrason_ERREUR(){  //Remise à 0 des capteurs, rouge 100ms, vert 20ms,  rien
  Serial.println("");Serial.println("UNE ERREUR EST SURVENUE SUR LES CAPTEURS ULTRASONS"); Serial.println("");
  digitalWrite(led_rouge, HIGH);
  Ultrason_Setup();
  digitalWrite(led_verte, HIGH); digitalWrite(led_rouge, LOW);  
  delay(20);
  digitalWrite(led_verte, LOW);  
  erreur_ultrason=0;
}
//////////////////////////////////////////////////////////////
