void setup()  
{  
  Serial.begin(9600);  
  //Abaixo, as portas quer serão ativadas pelo celular, via Bluetooth  
  pinMode(5,OUTPUT);  
  pinMode(6,OUTPUT);  
  pinMode(7,OUTPUT);  
}  
    
void loop()  
{  
  if(Serial.available()>=2)  
  {  
    int key=Serial.read();  
    int val=Serial.read();  
    digitalWrite(key,val);  
  }  

