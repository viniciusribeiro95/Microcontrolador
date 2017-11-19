#if defined(__MSP430G2452__) || defined(__MSP430G2553__) || defined(__MSP430G2231__) // LaunchPad specific
#include "Energia.h"
#else
#error Board not supported
#endif

// Include application, user and local libraries
#include "LCD_5110.h"
char c = 0;
int i = 0;
String s = "";
String nome = "";
String idade = "";
String resp = "";
String cont = "";
String readString;

LCD_5110 myScreen = 
  LCD_5110(
    P1_4,    // Chip Select 
    P2_4,    // Serial Clock  
    P2_1,    // Serial Data   
    P2_0,    // Data/Command  
    P1_5,    // Reset         
    P2_3,    // Backlight      
    PUSH2    // Push Button 2  
  );


// Add setup code 
void setup() {   
    myScreen.begin();
    myScreen.setFont(0);
    myScreen.text(0, 0, "Iniciando     Sistema");
    myScreen.text(0, 3, "PULSEIRA      LOCALIZADORA");
    
    delay(3000);
    myScreen.clear();
    
    Serial.begin(9600);

}

// Display mask supports 3 digit + one decimal place
char display[8] = {' ', ' ', ' ', '.', ' ', 0x7f, 'C', 0x00};
char displayCalibrationPoint[8] = {' ', ' ', ' ', '.', ' ', 0x7f, 'C', 0x00};


// Add loop code 
void loop() {
  
 while(Serial.available() && i <= 3){
      delay(3);
      char c = Serial.read();
      s += c;
      readString += c;
      s = readString;
 }    

    if(s != ""){  
      if(i==0){
       nome = s;
      }
      if(i==1){
        idade = s;
      }
      if(i==2){
        resp = s;
      }
      if(i==3){
        cont = s;
      }
     readString = "";
     s = "";
     i++;
 }
     
  
  myScreen.clear();
  myScreen.text(0,0,"Nome:");
  myScreen.text(0,1,nome);
  myScreen.text(0,3,"Idade: ");
  myScreen.text(0,4,idade);
  delay (2000);
  
  myScreen.clear();
  myScreen.text(0,0,"Responsavel:");
  myScreen.text(0,1,resp);
  myScreen.text(0,3,"Contato: ");
  myScreen.text(0,4,cont);
  delay (2000);
//}
//else{
// 
//  myScreen.clear();
//  myScreen.text(0,0,"Nome:");
//  myScreen.text(0,1,s);
//  myScreen.text(0,3,"Idade: ");
//  myScreen.text(0,4,"21 Anos");
//  delay (2000);
//  
//  myScreen.clear();
//  myScreen.text(0,0,"Responsavel:");
//  myScreen.text(0,1,"Eu mesmo");
//  myScreen.text(0,3,"Contato: ");
//  myScreen.text(0,4,"(61)99884-1011");
//  delay (2000);
//}
}


