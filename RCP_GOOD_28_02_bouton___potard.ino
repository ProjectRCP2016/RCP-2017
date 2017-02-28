/*
 * RCP project 2017
 */

#include <Wire.h>
#include <BMDSDIControl.h>



const int                shieldAddress = 0x6E;
BMD_SDICameraControl_I2C sdiCameraControl(shieldAddress);

// Variable encoder
 int val; 
 int encoder0PinA = 3;
 int encoder0PinB = 4;
 int encoder0PinC ;
 int encoder0PinD;
 int encoder0PinE;
 int encoder0PinF;
 int encoder0PinG;
 int encoder0PinH;


 int encoder0Pos1 = 0;
 int encoder0PinALast = LOW;
 int encoder0Pos2 = 0;
 int encoder0PinCLast = LOW;
 int encoder0Pos3 = 0;
 int encoder0PinELast = LOW;
 int encoder0Pos4 = 0;
 int encoder0PinGLast = LOW;
 int n1 = LOW;
 int n2 = LOW;
 int n3 = LOW;
 int n4 = LOW;


// Variable bouton 

const int buttonPin = 2;    
const int ledPin =  13;     
int ledState = LOW;
boolean buttonState = LOW; 
int pressed = 0;


 void setup() {

   //Setup Encoders
   pinMode (encoder0PinA,INPUT);
   pinMode (encoder0PinB,INPUT);
   pinMode (encoder0PinC,INPUT);
   pinMode (encoder0PinD,INPUT);
   pinMode (encoder0PinE,INPUT);
   pinMode (encoder0PinF,INPUT);
   pinMode (encoder0PinG,INPUT);
   pinMode (encoder0PinH,INPUT);

   //Setup Bouton
   pinMode(ledPin, OUTPUT);
   pinMode(buttonPin, INPUT);

   //Monitor 
   Serial.begin(9600);
   Serial.println("Début du programme : ");

   //Initialisation Blackmagic 

   sdiCameraControl.begin();
   Wire.setClock(400000);
   sdiCameraControl.setOverride(true);

 } 


 void loop() { 
  
 float newPosDiv1;
 float newPosDiv2;
 float Gamma;

 int colorCorrection = 0;
 int currentColorMode;
 int boutonState;
 int boutonlastState;
 int lastboutonState;
 int boutonPush;
 int min;
 int max;
 
//Boucle min/max
  switch (pressed) {
    case 0:    
      newPosDiv1=0;
      min= -2;
      max= 2;
      
      break;
    case 1:  
       newPosDiv1=0;
      min= -4;
      max= 4;
      break;
    case 2:   
     newPosDiv1=1;
      min= 0;
      max= 16;
      break;
    case 3: 
    newPosDiv1=0;
      min= -8;
      max= 8;
      break;
}

 //Bouton avec fonction 
 
    if(debounceButton(buttonState) == HIGH && buttonState == LOW)
  {
    pressed++;
    buttonState = HIGH;

    if (pressed > 3)
    {
      pressed = 0;
      }
    
   Serial.println(pressed);
  }
  else if(debounceButton(buttonState) == LOW && buttonState == HIGH)
  {
       buttonState = LOW;
  }

      

 //Récuperation valeur potard : 
 
   n1 = digitalRead(encoder0PinA);
   if ((encoder0PinALast == LOW) && (n1 == HIGH)) {
     if (digitalRead(encoder0PinB) == LOW) {
       encoder0Pos1--;
     } else {
       encoder0Pos1++;
     }
      
      float newPosDiv1 = (encoder0Pos1 / 6.0) * 0.1;
  newPosDiv1 = constrain(newPosDiv1, min, max);

  if (newPosDiv1 == max)
  {
    newPosDiv1 = newPosDiv1 - 0.1; 
    
    } 

    Serial.println(newPosDiv1,2);

//Communication avec caméra Blackmagic
    
    float gammaValues[4] = {
        newPosDiv1,            // Red channel   (-4.0 to 4.0)
        newPosDiv1,            // Green channel (-4.0 to 4.0)
        newPosDiv1,            // Blue channel  (-4.0 to 4.0)
        newPosDiv1            // Luma channel  (-4.0 to 4.0)
      };
      
    sdiCameraControl.writeCommandFixed16(
        1,                   // Destination:    Camera 1
        8,                   // Category:       Color Correction
        2,                   // Param:          Gamma Adjust
        0,                   // Operation:      Assign Value
        gammaValues          // Values
      );
   }


/*
   // Deuxième encoder
     n2 = digitalRead(encoder0PinC);
   if ((encoder0PinCLast == LOW) && (n2 == HIGH)) {
     if (digitalRead(encoder0PinD) == LOW) {
       encoder0Pos2--;
     } else {
       encoder0Pos2++;
     }
      
      float newPosDiv2 = (encoder0Pos2 / 5.0) * 0.1;
  newPosDiv2 = constrain(newPosDiv2, -4.0, 4.0);

  Serial.println (newPosDiv2);
     Serial.println ("/Deuxième");


        float gammaValues[4] = {
        0,            // Red channel   (-4.0 to 4.0)
        newPosDiv2,            // Green channel (-4.0 to 4.0)
        0,            // Blue channel  (-4.0 to 4.0)
        0            // Luma channel  (-4.0 to 4.0)
      };
      
    sdiCameraControl.writeCommandFixed16(
        1,                   // Destination:    Camera 1
        8,                   // Category:       Color Correction
        1,                   // Param:          Gamma Adjust
        0,                   // Operation:      Assign Value
        gammaValues          // Values
      );

     
   }
     

*/


   encoder0PinALast = n1;
   encoder0PinCLast = n2;
   encoder0PinELast = n3;
   encoder0PinGLast = n4;





  


 }

//Fonction bouton 
boolean debounceButton(boolean state)
{
  boolean stateNow = digitalRead(buttonPin);
  if(state!=stateNow)
  {
    delay(10);
    stateNow = digitalRead(buttonPin);
  }
  return stateNow;
  
}

 


 
