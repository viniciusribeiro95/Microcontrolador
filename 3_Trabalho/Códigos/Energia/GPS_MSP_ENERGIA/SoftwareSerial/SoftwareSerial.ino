#include <SoftwareSerial.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;      

volatile float minutes, seconds;
volatile int degree, secs, mins;

SoftwareSerial mySerial(P1_2, P1_1); // RX, TX

void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println("Aguardando pelo sinal dos satelites...");
  mySerial.begin(9600);
}

void loop() // run over and over
{
 smartDelay(1000); /* Generate precise delay of 1ms */
        if (!gps.location.isValid())
        {          
          Serial.print("Latitude : ");
          Serial.println("************");
          Serial.print("Longitude : ");
          Serial.println("************");
         
          
        }
        else
        {
          //DegMinSec(gps.location.lat());
          Serial.print("Link para Google Maps:   https://maps.google.com/maps/?&z=10&q=");
          Serial.print(gps.location.lat(), 8);
          Serial.print(",");
          Serial.println(gps.location.lng(), 8);

        }
        if (!gps.altitude.isValid())
        {
          Serial.print("Altitude : ");
          Serial.println(gps.altitude.meters(), 6);
        }
        else
        {
          Serial.print("Altitude : ");
          Serial.println(gps.altitude.meters(), 6);    
        }
        if (!gps.time.isValid())
        {
          Serial.print("Time : ");
          Serial.println("*****");
        }
        else
        {
          char time_string[32];
          sprintf(time_string, "Time : %02d:%02d:%02d \n",gps.time.hour(), gps.time.minute(), gps.time.second());
          Serial.print(time_string);    
        }
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


