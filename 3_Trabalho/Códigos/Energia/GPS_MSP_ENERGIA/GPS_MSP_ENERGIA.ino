#include <TinyGPS++.h>

TinyGPSPlus gps;  

char server[] = "https://www.google.com/maps/?q=";
String data;
float dado_lat; // variáveis de armazenamento
char lat_aux[10]; // variáveis de armazenamento
String latParaString; // variáveis de armazenamento
float dado_lng; // variáveis de armazenamento
char lng_aux[10]; // variáveis de armazenamento
String lngParaString; // variáveis de armazenamento


void setup() {
  Serial.begin(9600);/* Define baud rate for serial communication */
}

void loop() {
       smartDelay(1000); /* Generate precise delay of 1ms */
  if (gps.location.isValid())
        {          
    dado_lat=gps.location.lat();
    dado_lng=gps.location.lng();
   
    dtostrf(dado_lat,6,6,lat_aux);
    latParaString = String(lat_aux);
   
    dtostrf(dado_lng,6,6,lng_aux);
    lngParaString = String(lng_aux);
   
    data+=server;
    data+=latParaString;
    data+=",";
    data+=lngParaString;

    Serial.println(data);
   
  }
//  else{
//    Serial.println("S");
//  }

}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (Serial.available())  /* Encode data read from GPS while data is available on serial port */
      gps.encode(Serial.read());
    /* Encode basically is used to parse the string received by the GPS and to store it in a buffer so that information can be extracted from it */
  } while (millis() - start < ms);
}

