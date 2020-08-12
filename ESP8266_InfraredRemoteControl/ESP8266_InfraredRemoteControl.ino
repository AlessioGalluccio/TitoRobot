/*
 * ****************************************************************************
 *  University of Milano - Bicocca
 *  DISCo - Department of Informatics, Systems and Communication
 *  Viale Sarca 336, 20126 Milano, Italy
 *
 *  Copyright © 2019 by:
 *    Davide Marelli   - davide.marelli@unimib.it
 *    Paolo Napoletano - paolo.napoletano@unimib.it
 * ****************************************************************************
 * 
 * Demo code, interface an IR receiver and decode remote's keypresses.
 * 
 * Components:
 *   1x NodeMCU 1.0 (ESP8266)
 *   1x VS1838B IR receiver
 *   1x IR Remote (NEC protocol)
 *   
 * NB: requires the 'IRremoteESP8266' library, install it from the Arduino IDE library manager
 * 
 * Notes:
 *   - VS1838B pins:
 *       1. Out
 *       2. GND
 *       3. Vcc (2v7 - 5v5)
 *   - As per the VS1838B datasheet:
 *       - a 100ohm resistor on the Vcc protects the component from damages, 200ohm is ok as it drops 0.3V when powered at 3v3
 *       - capacitors for power filtering are required
 *       - a >20Kohm pull-up resistor is required on the Out pin, ESP's internal pull-ups are guaranteed between 30K and 100K
 *       NB: always use the pull-up, add 100ohm resistor and capacitors only if you have all the components!
 *   - ESP's Pull-ups ratings and pin availability -> https://bbs.espressif.com/viewtopic.php?t=1079#p4097
 *       pin mapping -> https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h
 *       keep in mind that D0 and D4 are already used by board LEDs
 *   - Remote codes (HEX):  0xFFFFFF -> repetition of previous key (key hold)
 *       CH- -> 0xFFA25D       CH  -> 0xFF629D       CH+ -> 0xFFE21D
 *       |<< -> 0xFF22DD       >>| -> 0xFF02FD       >|| -> 0xFFC23D
 *        -  -> 0xFFE01F        +  -> 0xFFA857       EQ  -> 0xFF906F
 *        0  -> 0xFF6897      100+ -> 0xFF9867      200+ -> 0xFFB04F
 *        1  -> 0xFF30CF        2  -> 0xFF18E7        3  -> 0xFF7A85
 *        4  -> 0xFF10EF        5  -> 0xFF38C7        6  -> 0xFF5AA5
 *        7  -> 0xFF42BD        8  -> 0xFF4AB5        9  -> 0xFF52AD
 * 
 * Schematic:
 *   - VS1838B pin 1 -> ESP digital I/O pin, use one with internal pull-up
 *             pin 2 -> GND (0V)
 *             pin 3 -> to Vcc (3v3)
 *
 */

// Include IR libraries
#include <IRremoteESP8266.h>   // library core
#include <IRrecv.h>            // receiver data types
#include <IRutils.h>           // print function

// IR Receiver
#define RECV_PIN D1        // receiver input pin, eg. D1 (D0 and D4 are already used by board LEDs)

// Initialize sensor
IRrecv irrecv(RECV_PIN);

void setup() {
  irrecv.enableIRIn(true);   // enable the receiver (true -> enable internal pull-up)

  Serial.begin(115200);
  Serial.println("\n\nSetup completed.\n\n");
}

void loop() {
  static decode_results results;

  if (irrecv.decode(&results)) {  // if new value received, value is stored in 'results'
    Serial.print(F("Decoded value: 0x"));
    serialPrintUint64(results.value, HEX);  // print the value. print() & println() can't handle printing long longs. (uint64_t)
    Serial.print(F(", "));
    printKey(results.value);
    //Serial.print(resultToHumanReadableBasic(&results));
    irrecv.resume();   // resume ir reception
  }

}

void printKey(unsigned long value) {
  Serial.print(F("Key: "));
  switch (value & 0xFFFFFF) {
    case 0xFFA25D:
      Serial.println(F("CH-"));
      break;
    case 0xFF629D:
      Serial.println(F("CH"));
      break;
    case 0xFFE21D:
      Serial.println(F("CH+"));
      break;
    case 0xFF22DD:
      Serial.println(F("|<<"));
      break;
    case 0xFF02FD:
      Serial.println(F(">>|"));
      break;
    case 0xFFC23D:
      Serial.println(F(">||"));
      break;
    case 0xFFE01F:
      Serial.println(F("-"));
      break;
    case 0xFFA857:
      Serial.println(F("+"));
      break;
    case 0xFF906F:
      Serial.println(F("EQ"));
      break;
    case 0xFF6897:
      Serial.println(F("0"));
      break;
    case 0xFF9867:
      Serial.println(F("100+"));
      break;
    case 0xFFB04F:
      Serial.println(F("200+"));
      break;
    case 0xFF30CF:
      Serial.println(F("1"));
      break;
    case 0xFF18E7:
      Serial.println(F("2"));
      break;
    case 0xFF7A85:
      Serial.println(F("3"));
      break;
    case 0xFF10EF:
      Serial.println(F("4"));
      break;
    case 0xFF38C7:
      Serial.println(F("5"));
      break;
    case 0xFF5AA5:
      Serial.println(F("6"));
      break;
    case 0xFF42BD:
      Serial.println(F("7"));
      break;
    case 0xFF4AB5:
      Serial.println(F("8"));
      break;
    case 0xFF52AD:
      Serial.println(F("9"));
      break;
    case 0xFFFFFF:   // key hold
      Serial.println(F("HOLD"));
      break;
    default:
      Serial.println(F("UNKNOWN"));
  }
}
