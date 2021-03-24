//          EN SIG ZF VR
// silnik A D6 A6  D4 D5
// silnik B D7 A2  D8 D9
//         0/1 an 0/1 PWM
  
byte APWM = 5;   // VR - PWM speed control
byte AEN = 6;    // ENABLE - engine enable
byte AZF = 4;    // Z/F - engine rotation direction

byte BPWM = 9;   // VR - PWM speed control
byte BEN = 7;    // ENABLE - engine enable
byte BZF = 8;    // Z/F - engine rotation direction

int ASPEED = 0;
int BSPEED = 0;
int speed = 70;

volatile bool ADIR;
volatile bool BDIR;
volatile bool ADIR_F = 1;
volatile bool BDIR_F = 0;
volatile bool ADIR_B = !ADIR_F;
volatile bool BDIR_B = !BDIR_F;

// Bluetooth configuration

#include <SoftwareSerial.h> 
SoftwareSerial mySerial(11, 12); // RX, TX
String recieved = "";

// interrupts
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
  if(ADIR = 1) ++A_Pozycja;
  else --A_Pozycja;
}
void B_Signal(){
  BcurrentTime = millis();
  Ainterval = (AcurrentTime - AlatestTime);
  AlatestTime = AcurrentTime;
  if(BDIR = 0) ++B_Pozycja;
  else --B_Pozycja;
}
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
  
  mySerial.begin(9600);
  
  attachInterrupt(digitalPinToInterrupt(2), A_Signal, RISING); //Przerwanie na pinie 2
  attachInterrupt(digitalPinToInterrupt(3), B_Signal, RISING); //Przerwanie na pinie 3
}
void loop() {
  if(mySerial.available() > 0) {
    ASPEED = speed;
    BSPEED = speed;
    recieved = mySerial.read();
    if (recieved == "F") {
      ADIR = ADIR_F;
      BDIR = BDIR_F;
    }else if (recieved == "B") {
      ADIR = ADIR_B;
      BDIR = BDIR_B;
    }else if (recieved == "L") {
      ADIR = ADIR_B;
      BDIR = BDIR_F;
    }else if (recieved == "R") {
      ADIR = ADIR_F;
      BDIR = BDIR_B;
    }
  }else{
    ASPEED = 0;
    BSPEED = 0; 
  }
  analogWrite(APWM,ASPEED);
  analogWrite(BPWM,BSPEED);
  digitalWrite(AZF, ADIR);
  digitalWrite(BZF, BDIR);
}
