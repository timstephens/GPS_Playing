/* Connect to GPS receiver and broadcast the position fix down the serial port.


  // On a Pro mini, also need to remember to set the clock speed or your timings and baud rates will be out by a factor of 2 (8 vs 16MHz)
    Radio communicates over SPI (10 -SS ,11 -MISO ,12 -MOSI,13 - SCK)

    Use the Serial port to connect to the GPS
    USe a software Serial for additional comms???
*/


// THIS END THE GPS
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

  Serial.begin(57600);
  GPSSerial.begin(9600);

  //Setup radio (later)
  Serial.println("Setup");

  // Initialize the RFM69HCW:

  radio.initialize(FREQUENCY, MYNODEID, NETWORKID);
  radio.setHighPower(); // Always use this for RFM69HCW

  // Turn on encryption if desired:

  if (ENCRYPT)
    radio.encrypt(ENCRYPTKEY);


  // Setup the GPS to not return NMEA data.
  //  GPSSerial.println(F("$PUBX,40,RMC,0,0,0,0*47")); //RMC OFF
  //  delay(100);
  //  GPSSerial.println(F("$PUBX,40,VTG,0,0,0,0*5E")); //VTG OFF
  //  delay(100);
  //  GPSSerial.println(F("$PUBX,40,GGA,0,0,0,0*5A")); //CGA OFF
  //  delay(100);
  //  GPSSerial.println(F("$PUBX,40,GSA,0,0,0,0*4E")); //GSA OFF
  //  delay(100);
  //  GPSSerial.println(F("$PUBX,40,GSV,0,0,0,0*59")); //GSV OFF
  //  delay(100);
  //  GPSSerial.println(F("$PUBX,40,GLL,0,0,0,0*5C")); //GLL OFF
  //  delay(1000);
  // psMode();
  String myString = "Node 1 ready";
  radio.send(TONODEID, myString.c_str(), myString.length());
  Serial.println("ready");

}


int loopNo = 0;

void loop() {


  //ECHO FROM GPS TO RADIO

  if (GPSSerial.available()) {
    //Clear stuff from the buffer until there's something to read (look for $).
    //    while(!GPSSerial.find('$')){
    //      //Trap the code here
    //    }
    myPosition = GPSSerial.readStringUntil('\n');
    Serial.write(myPosition.c_str(), myPosition.length());
    Serial.println();

    if (myPosition.startsWith("$GPGLL")) {
      //truncate to 62bytes (max length of a datablock)
      myPosition = myPosition.substring(0, 60);
      //      Serial.write(myPosition.c_str(), myPosition.length());
      //    Serial.println();
      //send stuff via the radio link
      radio.send(TONODEID, myPosition.c_str(), myPosition.length());

    }
  }
  //  loopNo ++ ;
  //  if (loopNo > 2000) { //Only do this every 100 loops for convenience.
  //    Serial.println("resetting");
  //    psMode();
  //    loopNo = 0;
  //  }

  //RECEIVE FROM RADIO TO GPS (SEND TO SERIAL FOR DEBUG)
  if (radio.receiveDone()) // Got one!
  {

    for (byte i = 0; i < radio.DATALEN; i++) //The time is bytes 34-40ish in a $GPGLL string
      Serial.print((char)radio.DATA[i]);
    //      Serial.print(",RSSI:");
    //      Serial.println(radio.RSSI);
    Serial.println("");
  }

  //  delay(100);
  //  Serial.println(loopNo);
}
void psMode() {
  //  Write something to poll current power mode
  // byte buf[] = {0xb5, 0x62, 0x06, 0x11, 0x00, 0x17, 0x34, };
  //  byte buf [] = {0xb5, 0x62, 0x06, 0x04, 0x04, 0xff, 0xff, 0x04, 0x00, 0x10, 0x57}; //hardware reset GPS.
  //  byte buf[] = {0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x03, 0xE8, 0x03, 0xfa, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, 0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf9,0x24};
  byte buf[] {0xB5, 0x62, 0x06, 0x3B, 0x2C, 0x00, 0x01, 0x06, 0x00, 0x00, 0x0E, 0x90, 0x42, 0x01, 0x88, 0x13, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x4F, 0xC1, 0x03, 0x00, 0x87, 0x02, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x64, 0x40, 0x01, 0x00, 0x96, 0x5E};

  GPSSerial.write(buf, sizeof(buf)) ;

  GPSSerial.println();
  //GPSSerial.println("$PUBX,03*30");  //Finish the message
  delay(100);
  while (GPSSerial.available()) {
    Serial.write(GPSSerial.read());  //echo through to the Serial port.
  }

}

