#include <Arduino.h>
#include <Wire.h>
#include <TFLI2C.h>  // TFLuna-I2C 

//////Initialisation des LEDs/////////
#define led_verte 32
#define led_rouge 33
bool vert = HIGH;
//////////////////////////////////////

//////Initialisation TF-Luna//////////
TFLI2C luna;
int16_t  luna_addr = TFL_DEF_ADR; //adresse I2C (0x10 default)
uint16_t luna_frame = FPS_35;     // frame rate de 35

int16_t  luna_dist = 0 ;   // distance in centimeters
int16_t  luna_flux = 0 ;   // signal quality in arbitrary units
int16_t  luna_temp = 0 ;   // temperature in 0.01 degree Celsius
//////////////////////////////////////

/////Initialisation temps de mesure///
double t1=0, t2=0;
double tab[10]={0,0,0,0,0,0,0,0,0,0};
//////////////////////////////////////

//////Setup TF-Luna//////////
void Luna_Setup()
{
    if( luna.Soft_Reset(luna_addr))
    {
        Luna_OK();
    }
    else Luna_ERREUR();  
    luna.Save_Settings(luna_addr);

    if( luna.Set_Frame_Rate(luna_frame, luna_addr))
    {
      Luna_OK();
    }
    else Luna_ERREUR();
    luna.Save_Settings(luna_addr);

    if(luna.Get_Frame_Rate(luna_frame, luna_addr)==luna_frame){
      Luna_OK();
    }
    else Luna_ERREUR();
    luna.Save_Settings(luna_addr);
}
//////////////////////////////////////

void setup()
{
    Serial.begin(115200);   // Initialize Serial port
    Wire.begin();           // Initialize Wire library
    pinMode(led_verte, OUTPUT);
    pinMode(led_rouge, OUTPUT);

    ///////MESURE Luna_Setup/////////
    for(int i=0; i<10; i++){
      t1=millis();
      Luna_Setup(); 
      t2=millis();
      tab[i]=(double)(t2-t1);
    }
    
    Serial.println("");Serial.println("");Serial.println("");Serial.println("");
    Serial.print("Les 10 temps d'execution de Luna_Setup :");
    for(int i=0; i<10; i++){
      Serial.print(tab[i]); Serial.print("   ");
      tab[i]=(double)(t2-t1);
    }
    Serial.println("");Serial.println("");Serial.println("");Serial.println("");
    ///////////////////////////

    ///////MESURE Luna_Measure/////////
    for(int i=0; i<10; i++){
      t1=millis();
      Luna_Measure();
      t2=millis();
      tab[i]=(double)(t2-t1);
    }
    
    Serial.println("");Serial.println("");Serial.println("");Serial.println("");
    Serial.print("Les 10 temps d'execution de Luna_Measure :");
    for(int i=0; i<10; i++){
      Serial.print(tab[i]); Serial.print("   ");
      tab[i]=(double)(t2-t1);
    }
    Serial.println("");Serial.println("");Serial.println("");Serial.println("");
    ///////////////////////////

    ///////MESURE Luna_OK/////////
    for(int i=0; i<10; i++){
      t1=millis();
      Luna_OK(); 
      t2=millis();
      tab[i]=(double)(t2-t1);
    }
    
    Serial.println("");Serial.println("");Serial.println("");Serial.println("");
    Serial.print("Les 10 temps d'execution de Luna_OK :");
    for(int i=0; i<10; i++){
      Serial.print(tab[i]); Serial.print("   ");
      tab[i]=(double)(t2-t1);
    }
    Serial.println("");Serial.println("");Serial.println("");Serial.println("");
    ///////////////////////////

    ///////MESURE Luna_ERREUR/////////
    for(int i=0; i<10; i++){
      t1=millis();
      Luna_ERREUR(); 
      t2=millis();
      tab[i]=(double)(t2-t1);
    }
    
    Serial.println("");Serial.println("");Serial.println("");Serial.println("");
    Serial.print("Les 10 temps d'execution de Luna_ERREUR :");
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

//////Fonctions TF-Luna//////////

//Fonction de mesure de la distance, qualité de la mesure (flux) et de la température du Luna
void Luna_Measure(){
  if(luna.getData(luna_dist, luna_flux, luna_temp, luna_addr)){
    Luna_OK();
    luna_temp=int16_t(luna_temp/100);
  }
  else Luna_ERREUR();
  
}

//Fonction lorsqu'une action du Luna s'est bien déroulée
void Luna_OK(){
  if (vert==LOW){vert=HIGH; }
  else vert=LOW;
  digitalWrite(led_verte, vert);
}

//Fonction lorsqu'une action du Luna s'est mal déroulée
void Luna_ERREUR(){
  luna.printStatus(); //Affiche les erreurs sur le terminal
  digitalWrite(led_rouge, HIGH);
  digitalWrite(led_verte, LOW);
  delay(20);
  digitalWrite(led_rouge, LOW);
}
//////////////////////////////////////
