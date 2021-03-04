
  //          EN SIG ZF VR
  // silnik A D6 A6  D4 D5
  // silnik B D7 A2  D8 D9
  //         0/1 an 0/1 PWM
  
byte APWM = 5;   // VR - sterowanie PWM
byte AEN = 6;    // ENABLE - uruchomienie silnika
byte AZF = 4;    // Z/F - sterowanie kierunkiem obrotu

byte BPWM = 9;   // VR - sterowanie PWM
byte BEN = 7;    // ENABLE - uruchomienie silnika
byte BZF = 8;    // Z/F - sterowanie kierunkiem obrotu

/////////////////////////////////////////////////////////

int ASPEED = 0;
int BSPEED = 0;
int acceleration = 0;

volatile bool AKierunek = 0;
volatile bool BKierunek = 1;

boolean k = false;
int minspeed = 0;
int maxspeed = 250;

//////////////////////////////////////////////////////// komunikacja z bluetooth

#include <SoftwareSerial.h> 

SoftwareSerial mySerial(11, 12); // RX, TX

char odebraneDane = "";

////////////////////////////////////////////////////// nadanie prędkości z regulacją przyspieszenia

void predkoscA(int aspeed){
    if(ASPEED > aspeed){
    analogWrite(APWM, ASPEED);
    ASPEED--;
    delay(acceleration);
   }else if(ASPEED < aspeed){
    analogWrite(APWM, ASPEED);
    ASPEED++;
    delay(acceleration);
   }
}

///////////////////////////////////////////////////////// przerwania

 volatile unsigned long AcurrentTime;
 unsigned long AlatestTime;
 unsigned long Ainterval;
 volatile unsigned long A_Pozycja;

 volatile unsigned long BcurrentTime;
 unsigned long BlatestTime;
 unsigned long Binterval;
 volatile unsigned long B_Pozycja;
 
void A_Signal(){
  AcurrentTime = millis();
  Binterval = (BcurrentTime - BlatestTime);
  BlatestTime = BcurrentTime;
  if(AKierunek = 0){
    ++A_Pozycja;
  }else if(AKierunek = 1){
    --A_Pozycja;
  }
}

void B_Signal(){
  BcurrentTime = millis();
  Ainterval = (AcurrentTime - AlatestTime);
  AlatestTime = AcurrentTime;
  if(BKierunek = 1){
    ++B_Pozycja;
  }else if(BKierunek = 0){
    --B_Pozycja;
  }
  
}

/////////////////////////////////////////////////////

void setup() {

pinMode(AEN, OUTPUT); 
digitalWrite(AEN, HIGH);

pinMode(AZF, OUTPUT); 

pinMode(APWM, OUTPUT); 
digitalWrite(APWM, LOW);

pinMode(BEN, OUTPUT); 
digitalWrite(BEN, HIGH);

pinMode(BZF, OUTPUT); 

pinMode(BPWM, OUTPUT); 
digitalWrite(BPWM, LOW);

Serial.begin(9600);
mySerial.begin(9600);

attachInterrupt(digitalPinToInterrupt(2), A_Signal, RISING); //Przerwanie na pinie 2

attachInterrupt(digitalPinToInterrupt(3), B_Signal, RISING); //Przerwanie na pinie 3

}
//////////////////////////////////////////////////

void loop() {

    int speed = 70;
    
  delay(1000);

if(mySerial.available() > 0) {

    odebraneDane = mySerial.read(); 
    String K = String(odebraneDane);
    
    if (K == "F") {
    AKierunek = 1;
    BKierunek = 0;       
    ASPEED = speed;
    BSPEED = speed; 
        
    }else if (K == "B") {
    AKierunek = 0;
    BKierunek = 1; 
    ASPEED = speed;
    BSPEED = speed; 

    }else if (K == "L") {
    AKierunek = 0;
    BKierunek = 0;
    ASPEED = speed;
    BSPEED = speed; 

    }else if (K == "R") {
    AKierunek = 1;
    BKierunek = 1;
    ASPEED = speed;
    BSPEED = speed; 

    }
  }else{
    ASPEED = 0;
    BSPEED = 0; 
}
  
  analogWrite(BPWM,BSPEED);
  analogWrite(APWM,ASPEED);
  digitalWrite(AZF, AKierunek);      //DIRECTION A
  digitalWrite(BZF, BKierunek);      //DIRECTION B  
   



Serial.print("SpeedA:   ");
Serial.println(ASPEED);
Serial.print("PositionA:   ");
Serial.println(A_Pozycja);
Serial.print("IntervalA:   ");
Serial.println(Ainterval);

Serial.print("SpeedB:   ");
Serial.println(BSPEED);
Serial.print("PositionB:   ");
Serial.println(B_Pozycja);
Serial.print("IntervalB:   ");
Serial.println(Binterval);
}
