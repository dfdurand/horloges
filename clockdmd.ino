#include <SPI.h>        //SPI.h must be included as DMD is written by SPI (the IDE complains otherwise)
#include <DMD.h>        //
#include <TimerOne.h>   //
#include <Wire.h>
#include <DS3231.h>
#include "Arial_black_16.h"
#include "SystemFont5x7.h"
#include "Arial_black_16.h"
#include "Arial_Black_16_ISO_8859_1.h"



DS3231 clock;
RTCDateTime dt;


DMD dmd(1,1);

/*--------------------------------------------------------------------------------------
Gestionnaire d'interruption pour l'analyse d'actualisation DMD pilotée par Timer1 (TimerOne), q
il est appelé à la période définie dans Timer1.initialize();
--------------------------------------------------------------------------------------*/
void ScanDMD()
{ 
  dmd.scanDisplayBySPI();
}

/*--------------------------------------------------------------------------------------
  Afficher les chiffres de l'horloge sur l'écran à partir d'une valeur de temps à 4 chiffres 
  et sélectionner si les deux-points clignotants sont activés ou désactivés
--------------------------------------------------------------------------------------*/
void ShowClockNumbers( unsigned int uiTime, byte bColonOn )
{
   dmd.clearScreen(true);
   dmd.drawChar(  0,  1,'0'+((uiTime%10000)/1000), GRAPHICS_NORMAL );   // thousands
   dmd.drawChar(  7,  1, '0'+((uiTime%1000) /100),  GRAPHICS_NORMAL );   // hundreds
   dmd.drawChar( 16,  1, '0'+((uiTime%100)  /10),   GRAPHICS_NORMAL );   // tens
   dmd.drawChar( 24,  1, '0'+ (uiTime%10),          GRAPHICS_NORMAL );   // units
   if( bColonOn )
      dmd.drawChar( 14,  1, ':', GRAPHICS_OR     );   // clock colon overlay on
   else
      dmd.drawChar( 14,  1, ':', GRAPHICS_NOR    );   // clock colon overlay off
}


void setup(void)
{
   
   //initialiser l'utilisation de l'interruption/CPU de TimerOne utilisée pour scanner et rafraîchir l'affichage
   Timer1.initialize( 5000 );           //période en microsecondes pour appeler ScanDMD. Tout ce qui dépasse 5000 (5 ms) et vous pouvez voir le scintillement.
   Timer1.attachInterrupt( ScanDMD );   //attachez l'interruption Timer1 à ScanDMD qui va à dmd.scanDisplayBySPI()

   //effacer/initialiser les pixels DMD contenus dans la RAM
   dmd.clearScreen( true );   //"true" est normal (tous les pixels désactivés), "false" est négatif (tous les pixels activés)
   dmd.selectFont(Arial_Black_16);
   //dmd.selectFont(SystemFont5x7);
   
  Serial.begin(9600);

   //clock component 

  clock.begin();
  clock.setDateTime(__DATE__, __TIME__);  

}

void loop(void)
{
  
   dt = clock.getDateTime(); //récupérer l'heure du module DS3231
   int heure = dt.hour ;  //récupérer l'heure
   int min = dt.minute ;  // récupérer les minutes
  

   unsigned int ui;
   
   ui = 100*heure + min ; //méthode pour concantener 
    
   // 10 x 14 font clock,y compris la démonstration des modes OR et NOR pour les pixels afin que les deux-points clignotants puissent être superposés
   ShowClockNumbers( ui, true );
   delay( 1000 );
   ShowClockNumbers( ui, false );
   delay( 1000 );
   ShowClockNumbers( ui, true );
   delay( 1000 );
   ShowClockNumbers( ui, false );
   delay( 1000 );
}