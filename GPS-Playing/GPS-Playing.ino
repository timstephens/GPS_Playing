/* Connect to GPS receiver and broadcast the position fix down the serial port.


// On a Pro mini, also need to remember to set the clock speed or your timings and baud rates will be out by a factor of 2 (8 vs 16MHz)
    Radio communicates over SPI (10 -SS ,11 -MISO ,12 -MOSI,13 - SCK)

    Use the Serial port to connect to the GPS
    USe a software Serial for additional comms???
*/

#include <SoftwareSerial.h>


SoftwareSerial debugSerial(10,11);


String myPosition;
void setup() {
  //Setup Serial ports

  Serial.begin(9600);
  debugSerial.begin(9600);

  //Setup radio (later)
  Serial.println("Setup");

}

void loop() {

  if(debugSerial.available()) {
    //Clear stuff from the buffer until there's something to read (look for $).
//    while(!debugSerial.find('$')){
//      //Trap the code here
//    }
      myPosition = debugSerial.readStringUntil('\n');
      if(myPosition.startsWith("$GPGGA")) {
            Serial.write(myPosition.c_str(), myPosition.length());
      }


    //Need to decode NEMA sentences now. 
    
  }
  delay(100);
  Serial.println(".");
}
