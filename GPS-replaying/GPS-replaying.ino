// THIS END THE BASE STATION, connected to ESP8266

/*
  // On a Pro mini, also need to remember to set the clock speed or your timings and baud rates will be out by a factor of 2 (8 vs 16MHz)
    Radio communicates over SPI (10 -SS ,11 -MISO ,12 -MOSI,13 - SCK)
*/

#include <RFM69.h>
#include <SPI.h>


// Addresses for this node. CHANGE THESE FOR EACH NODE!

#define NETWORKID     0   // Must be the same for all nodes
#define MYNODEID      2   // My node ID
#define TONODEID      1   // Destination node ID

// RFM69 frequency, uncomment the frequency of your module:
#define FREQUENCY     RF69_868MHZ

// AES encryption (or not):

#define ENCRYPT       false // Set to "true" to use encryption
#define ENCRYPTKEY    "TOPSECRETPASSWRD" // Use the same 16-byte key on all nodes

RFM69 radio;

void setup()
{
  // Open a serial port so we can send keystrokes to the module:

  Serial.begin(115200);
  Serial.print("Node ");
  Serial.print(MYNODEID, DEC);
  Serial.println(" ready");


  // Initialize the RFM69HCW:

  radio.initialize(FREQUENCY, MYNODEID, NETWORKID);
  radio.setHighPower(); // Always use this for RFM69HCW

  String myString = "Node 2 radio ready as String";
  radio.send(TONODEID, myString.c_str(), myString.length());
  delay(100);
  char charArray[22] = "Node 2 ready as array";
  radio.send(TONODEID, charArray, 21);
}

void loop()
{

  // RECEIVING

  // In this section, we'll check with the RFM69HCW to see
  // if it has received any packets:

  if (radio.receiveDone()) // Got one!
  {
    // Print out the information:

    //Serial.print("received from node ");
    //Serial.print(radio.SENDERID, DEC);
    Serial.print("gps ");
    //    {"name": "cat", "lat": 51.42, "lon": -1.14}
    // The actual message is contained in the DATA array,
    // and is DATALEN bytes in size:
    //    Serial.print("map {\"name\":");
    //    for (byte i = 34; i < radio.DATALEN && i < 40; i++) //The time is bytes 34-40ish in a $GPGLL string
    //      Serial.print((char)radio.DATA[i]);
    //  Serial.print(",\"lat\":");
    //    for (byte i = 7; i < radio.DATALEN && i < 17; i++) //There are 10 sig figs on the lat and long, and lat starts at #7
    //      Serial.print((char)radio.DATA[i]);
    //    Serial.print(",\"lon\":-");  //Hacky, but we're "always" going to be WEST of Greenwich!!
    //    for (byte i = 22; i < radio.DATALEN && i < 31; i++) //Also hacky: the first digit of the longitude is "always" going to be 0 since we're close to Greenwich. Drop it.
    //      Serial.print((char)radio.DATA[i]);
    //    Serial.println("}");
    //    Serial.write(0);
    // RSSI is the "Receive Signal Strength Indicator",
    // smaller numbers mean higher power.

    for (byte i = 0; i < radio.DATALEN; i++) //The time is bytes 34-40ish in a $GPGLL string
      Serial.print((char)radio.DATA[i]);
    //      Serial.print(",RSSI:");
    //      Serial.println(radio.RSSI);
    Serial.println("");
    Serial.write(0);
    //      Serial.write(0);
  }

  // SENDING
  if (Serial.available()) {
    //    char serialIn[32];
    //    serialIn = "";
    //    int serialInLength = Serial.readBytesUntil(0, serialIn, 32);  //if we readBytes, then we don't need to convert back inot a c_str() when the data is sent to the radio??
    String serialIn = "";
//        serialIn = Serial.readStringUntil(0);

    while (Serial.available()) {
      Serial.println(Serial.read(), DEC);
    }

    Serial.print(serialIn);
    Serial.println();
    radio.send(TONODEID, serialIn.c_str(), serialIn.length()); //, serialInLength);

  }
}
