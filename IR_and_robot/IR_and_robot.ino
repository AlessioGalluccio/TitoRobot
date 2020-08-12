#include <IRremote.h>
#include "ArduinoMotorShieldR3.h"

int RECV_PIN = 7;
ArduinoMotorShieldR3 md;
IRrecv irrecv(RECV_PIN);
decode_results results;

#define CH1 0xFFA25D
#define CH 0xFF629D
#define CH2 0xFFE21D
#define PREV 0xFF22DD
#define NEXT 0xFF02FD
#define PLAYPAUSE 0xFFC23D
#define VOL1 0xFFE01F
#define VOL2 0xFFA857
#define EQ 0xFF906F
#define BUTON0 0xFF6897
#define BUTON100 0xFF9867
#define BUTON200 0xFFB04F
#define BUTON1 0xFF30CF
#define BUTON2 0xFF18E7
#define BUTON3 0xFF7A85
#define BUTON4 0xFF10EF
#define BUTON5 0xFF38C7
#define BUTON6 0xFF5AA5
#define BUTON7 0xFF42BD
#define BUTON8 0xFF4AB5
#define BUTON9 0xFF52AD

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  md.init();
  irrecv.enableIRIn();
}

void loop() {
  
  if(irrecv.decode(&results)){
    Serial.println("Received");
    switch(results.value){
      case BUTON2:
        Serial.println("M1 Speed 100% Forward");
        md.setM1Speed(400);
        Serial.println("M2 Speed 100% Forward");
        md.setM2Speed(400);
        break;
      case BUTON8:
        Serial.println("M1 Speed 100% Backward");
        md.setM1Speed(-400);
        Serial.println("M2 Speed 100% Backward");
        md.setM2Speed(-400);
        break;
      case BUTON4:
        Serial.println("M1 Speed 100% Backward");
        md.setM1Speed(-400);
        Serial.println("M2 Speed 100% Forward");
        md.setM2Speed(400);
        break;
      case BUTON6:
        Serial.println("M1 Speed 100% Forward");
        md.setM1Speed(400);
        Serial.println("M2 Speed 100% Backward");
        md.setM2Speed(-400);
        break;
      default:
        md.setM1Speed(0);
        md.setM2Speed(0);
        break;
    }
    irrecv.resume();
  }
  //delay(100);
}
