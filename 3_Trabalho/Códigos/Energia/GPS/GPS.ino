#include <TinyGPS++.h>

TinyGPSPlus gps;  

double latitude;
double longitude;

void setup() {
  Serial.begin(9600);
}

void loop() {
       smartDelay(1000); /* Generate precise delay of 1ms */
  if (gps.location.isValid())
        {          
          latitude = gps.location.lat(),8;
          longitude = gps.location.lng(),8;
          Serial.println(latitude);
          Serial.println(longitude);
          sendsms();
        }
        else{
          latitude = 1;
          longitude = 1;
          sendsms();
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

void sendsms()
{
    Serial.print(" AT+CMGF=1\r");
    delay(500);
    Serial.print("AT+CMGS =\"061995523211\"\r");
    delay(500); 
    Serial.print(latitude);
   Serial.print(longitude);
  
   Serial.print("\r");
   delay(500);
    Serial.print(0x1A);
}
