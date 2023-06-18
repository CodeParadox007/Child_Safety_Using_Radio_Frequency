
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 8);
const int buzzer = 6;
#include <SoftwareSerial.h>  //bluetooth library
#include <string.h>
SoftwareSerial bt(0, 1);  // RX, TX
int p = 5;
double dis = 0;
int range = 500;
double Lat;
double Lon;
float msg[5];

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const byte address[6] = "zulas";
void setup() {

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.startListening();
  pinMode(p, OUTPUT);
}

void loop() {

  digitalWrite(p, HIGH);

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);


  if (Serial.available()) {
    String BT_input = Serial.readString();  // read input string from bluetooth
    range = BT_input.toInt();

    Serial.print("Safe Distance Selected as ");
    Serial.print(range);
    Serial.println(" Meter");


    if (radio.available()) {



      radio.read(&msg, sizeof(msg));



      // Sending Data
      Lat = msg[3];
      Lon = msg[4];

      if (msg[0] < 2000) {
        dis = msg[0];
      }

      Serial.print("dis: ");
      Serial.println(dis);
      Serial.print("Lat: ");
      Serial.println(Lat, 6);
      Serial.print("Lon: ");
      Serial.println(Lon, 6);

      if (int(msg[0]) > range && (500 - (int(msg[0]))) > 0) {
        tone(buzzer, 1000);
      } else {
        noTone(buzzer);
      }

      msg[0] = 0.00;
    }





    display.setTextSize(2);
    display.setCursor(5, 0);
    display.print("*Distance*");
    // display.setCursor(0,4);
    // display.print("__________");
    display.setCursor(20, 20);
    display.print(dis);

    display.setTextSize(1);
    display.print(" meter");
    display.setCursor(0, 40);
    display.print("Latitude: ");
    display.print(Lat, 6);
    display.setCursor(0, 50);
    display.print("Longitude: ");
    display.print(Lon, 6);


    // if(int(msg[0])>range && (500-(int(msg[0]))) >0) {
    // display.clearDisplay()  ;
    // // display.setTextColor(SSD1306_WHITE);
    // display.setTextSize(2);
    // display.setCursor(5,0);
    // display.print("*Alert !!*");
    // display.setCursor(30, 30) ;
    // display.print("Child is in Danger.");

    //   }
    display.display();


    delay(500);
  }
