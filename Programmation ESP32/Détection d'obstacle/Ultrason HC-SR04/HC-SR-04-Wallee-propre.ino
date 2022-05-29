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
int front=0; int right=1; int left=2; int back=3;  //on affecte un nombre à front, right, left, back pour du code plus explicite
int measure_time=60;                               //temps entre 2 mesure sur un même capteur
int decharge_time=100;                             //temps de décharge des condo lors du Setup (ou trop d'erreurs)
////////////////////////////////////////


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
  Ultrason_Setup(); //Initialize HCSR04s
}

void loop()
{
   double distance_avant=Ultrason_Read(front);
   double distance_droite=Ultrason_Read(right);
   double distance_gauche=Ultrason_Read(left);
   double distance_back=Ultrason_Read(back);

   int indication_distance_front=Ultrason_Indication(front);
   int indication_distance_droite=Ultrason_Indication(right);
   int indication_distance_left=Ultrason_Indication(left);
   int indication_distance_back=Ultrason_Indication(back);

   double* distance = Ultrason_All_Read();
   int* indication = Ultrason_All_Indication();

   ultrason_ERREUR(); //A ne pas utiliser en général
   
   Serial.println("Boucle réalisée");
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
  delete distance; //Suppression allocation mémoire
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
