/* Connect to GPS receiver and broadcast the position fix down the serial port.


  // On a Pro mini, also need to remember to set the clock speed or your timings and baud rates will be out by a factor of 2 (8 vs 16MHz)
    Radio communicates over SPI (10 -SS ,11 -MISO ,12 -MOSI,13 - SCK)

    Use the Serial port to connect to the GPS
    USe a software Serial for additional comms???
*/

#include <SoftwareSerial.h>
// Include the RFM69 and SPI libraries:
#include <RFM69.h>
#include <SPI.h>

#define NETWORKID 0 //Metwork ID
#define MYNODEID  1  //Me
#define TONODEID  2  //Where we are sending to...

#define FREQUENCY     RF69_868MHZ


#define ENCRYPT       false // Set to "true" to use encryption
#define ENCRYPTKEY    "TOPSECRETPASSWRD" // Use the same 16-byte key on all nodes


SoftwareSerial GPSSerial(8, 9);
RFM69 radio;

String myPosition;


void setup() {
  //Setup Serial ports

  Serial.begin(9600);
  GPSSerial.begin(9600);

  //Setup radio (later)
  Serial.println("Setup");

  // Initialize the RFM69HCW:

  radio.initialize(FREQUENCY, MYNODEID, NETWORKID);
  radio.setHighPower(); // Always use this for RFM69HCW

  // Turn on encryption if desired:

  if (ENCRYPT)
    radio.encrypt(ENCRYPTKEY);
}




void loop() {

  if (GPSSerial.available()) {
    //Clear stuff from the buffer until there's something to read (look for $).
    //    while(!GPSSerial.find('$')){
    //      //Trap the code here
    //    }
    myPosition = GPSSerial.readStringUntil('\n');
    if (myPosition.startsWith("$GPGGA")) {
      //truncate to 62bytes (max length of a datablock)
      myPosition = myPosition.substring(0,60);
      Serial.write(myPosition.c_str(), myPosition.length());

      //send stuff via the radio link
      radio.send(TONODEID, myPosition.c_str(), myPosition.length());

    }


    //Need to decode NEMA sentences now.

  }
  delay(100);
  Serial.println(".");
}
