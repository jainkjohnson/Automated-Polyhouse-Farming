#include <dht.h>
#include <SoftwareSerial.h>
SoftwareSerial SIM900(8, 9);
#define dht_dpin 7 //no ; here. Set equal to channel sensor is on
#define relay1 10
#define soli 11
#define solinoid2 12
#define fan 14
#define tank 15
dht DHT;
int flag = 1;
int sms = 1;
int sms1 = 1;
void setup(){
  Serial.begin(19200);
  SIM900.begin(19200);
  //Let system settle
  delay(700);//Wait rest of 1000ms recommended delay before
  //accessing sensor
  pinMode(soli, OUTPUT);
}//end "setup()"

void loop(){
  //This is the "heart" of the program.
  DHT.read11(dht_dpin);

    Serial.print("humidity = ");
    Serial.println(DHT.humidity);
    
    Serial.print("temperature = ");
    Serial.println(DHT.temperature); 
    
    int sensorValue = analogRead(A0);
    Serial.print("Moisture = "); 
    Serial.println(sensorValue);
     
    int tankvalue = analogRead(A7);
    
   
    
    if(tankvalue == 0){
      
      digitalWrite(tank ,HIGH);
      
    }else{
        digitalWrite(tank ,LOW);
    }
    
    if(DHT.temperature < 25 & flag == 1){
      if(sms == 1){
      SIM900.print("AT+CMGF=1\r");                                                        // AT command to send SMS message
      delay(100);
      SIM900.println("AT + CMGS = \"+919633623332\"");                                     // recipient's mobile number, in international format
      delay(100);
      
      SIM900.println("BULB = ON ------ FOG = OFF ------- FAN = OFF");        // message to send
      delay(100);
      SIM900.println((char)26);                       // End AT command with a ^Z, ASCII code 26
      delay(100); 
      SIM900.println();
      delay(5000); 
      sms = 0;    
    
  }
       digitalWrite(relay1, HIGH);
       digitalWrite(soli, LOW);
       digitalWrite(fan, LOW );
       
       Serial.println("bulb = ON");
       Serial.println("fog = OFF");
       Serial.println("fan = OFF");
       delay(1000);
    
    }else if(DHT.temperature > 25 & flag == 1){
      
      if(sms1 == 1){
      SIM900.print("AT+CMGF=1\r");                                                        // AT command to send SMS message
      delay(100);
      SIM900.println("AT + CMGS = \"+919633623332\"");                                     // recipient's mobile number, in international format
      delay(100);
      
      SIM900.println("BULB = OFF, FOG = ON,  FAN = ON");        // message to send
      delay(100);
      SIM900.println((char)26);                       // End AT command with a ^Z, ASCII code 26
      delay(100); 
      SIM900.println();
      delay(5000); 
      sms1 = 0;    
    
  }
        digitalWrite(relay1, LOW);
        digitalWrite(soli, HIGH);
        digitalWrite(fan, HIGH);
        Serial.println("bulb = OFF");
        Serial.println("fog = ON");
        Serial.println("fan = ON");
        delay(1000);
    }
    
    if(sensorValue > 1000 ){
      digitalWrite(solinoid2,HIGH);
      Serial.println("dip = ON");
      delay(1000);
    }else{
       Serial.println("dip = OFF");
    }
    
    char c = Serial.read();
    if (c == 'H') {
    digitalWrite(relay1, HIGH);    
    flag = 0;
    delay(1000);
    }
    
    else if (c == 'L') {
    digitalWrite(relay1, LOW);
    flag = 0;
    delay(1000);
    }
    
    else if (c == 'R') {
    digitalWrite(soli, LOW);
    flag = 0;
    delay(1000);
    }
    
    else if (c == 'S') {
    digitalWrite(soli, HIGH);
    flag = 0;
    delay(1000);
    }
    
    else if (c == 'A') {
    digitalWrite(solinoid2, HIGH);
    flag = 0;
    delay(1000);
    }
    
    else if (c == 'B') {
    digitalWrite(solinoid2, LOW);
    flag = 0;
    delay(1000);
    }
    
    else if (c == 'X') {
    digitalWrite(fan, HIGH);
    flag = 0;
    delay(1000);
    }
    
    else if (c == 'Y') {
    digitalWrite(fan, LOW);
    flag = 0;
    delay(1000);
    }
    
    else if (c == 'R'){
      flag = 1;
      delay(1000);
    }
    delay(2000);//Don't try to access too frequently... in theory
  
  
  //should be once per two seconds, fastest,
  //but seems to work after 0.8 second.
}// end loop()


