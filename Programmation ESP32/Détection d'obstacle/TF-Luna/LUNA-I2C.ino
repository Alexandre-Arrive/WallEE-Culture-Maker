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
    Luna_Setup(); //Initialize TF-Luna

}

void loop()
{
  Luna_Measure();
  Serial.println(luna_dist);
  Serial.println(luna_flux);
  Serial.println(luna_temp);
  Serial.println("");
  delay(25);
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
