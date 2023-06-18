
#include "TinyGPS++.h"
#include "SoftwareSerial.h"

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
SoftwareSerial serial_connection(2, 3);  //tx,rx
TinyGPSPlus gps;                         // GPS object to process the NMEA data
RF24 radio(9, 8);
#define DEGTORAD 0.0174532925199432957f

double Lat = 22.8030048;
double Lon = 89.5351920;

float msg[5];


int pinA = 4;

double text;
double rext;

double distance;

const byte address[6] = "zulas";
String Lat_val;
String Long_val;

void setup() {
  // put your setup code here, to run once:
  radio.begin();
  Serial.begin(9600);
  pinMode(pinA, INPUT);

  serial_connection.begin(9600);
  Serial.println("GPS Start");
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {


  while (serial_connection.available()) {
    gps.encode(serial_connection.read());
  }
  if (gps.location.isUpdated()) {

    if (digitalRead(pinA) == HIGH) {
      Serial.println("Key Pressed");
      Lat = (gps.location.lat());
      msg[1] = Lat;
      Lon = (gps.location.lng());
      msg[2] = Lon;

    }

    else {
      msg[3] = (gps.location.lat());
      msg[4] = gps.location.lng();

      Serial.print("Satellite Count:");
      Serial.println(gps.satellites.value());
      //    double lat=gps.location.lat();
      //    Lat_val=String(lat, 6);
      Serial.print("Latitude:");
      Serial.println(gps.location.lat(), 6);
      Serial.print("Longitude:");
      Serial.println(gps.location.lng(), 6);

      Serial.print("Altitude Feet:");
      Serial.println(gps.altitude.feet());
      Serial.println("");


      Serial.println("Fixed Co-ordinates: ");
      Serial.print("Latitude :");
      Serial.println(Lat, 6);
      //  radio.write(&text, sizeof(text));
      //
      Serial.print("Longitude ");
      Serial.println(Lon, 6);
      //  radio.write(&rext, sizeof(rext));



      // Distance

      const float R = 6371000;  // km
      float p1 = gps.location.lat() * DEGTORAD;
      float p2 = Lat * DEGTORAD;
      float dp = (Lat - gps.location.lat()) * DEGTORAD;
      float dl = (Lon - gps.location.lng()) * DEGTORAD;

      float x = sin(dp / 2) * sin(dp / 2) + cos(p1) * cos(p2) * sin(dl / 2) * sin(dl / 2);
      float y = 2 * atan2(sqrt(x), sqrt(1 - x));

      distance = R * y;
      msg[0] = distance;
      radio.write(&msg, sizeof(msg));
      Serial.print("Distance : ----->");
      Serial.print(distance, 6);
      Serial.println(" meter.");
      //22.803386
      //89.538856


      Serial.print("Date: ");
      if (gps.date.isValid()) {
        Serial.print(gps.date.month());
        Serial.print("/");
        Serial.print(gps.date.day());
        Serial.print("/");
        Serial.println(gps.date.year());
      }

      Serial.print("Time: ");
      if (gps.time.isValid()) {
        if (gps.time.hour() < 10) Serial.print(F("0"));
        Serial.print(gps.time.hour());
        Serial.print(":");
        if (gps.time.minute() < 10) Serial.print(F("0"));
        Serial.print(gps.time.minute());
        Serial.print(":");
        if (gps.time.second() < 10) Serial.print(F("0"));
        Serial.print(gps.time.second());
        Serial.print(".GMT");
      }
      Serial.println("");
      Serial.println("_____________________________________________________");
      delay(1000);
    }
  }
}
