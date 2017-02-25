//BLYNK_GATE_V0.1



#define BLYNK_PRINT Serial // Comment this out to disable prints and save space
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

#include <Servo.h>
#include <Wire.h>
#include <SimpleTimer.h>
#include <LiquidCrystal_I2C.h>


const int trigPin1 = 2;
const int echoPin1 = 3;
int motor = 6;
int door_led = 10;

Servo myservo; //Heimdal
LiquidCrystal_I2C lcd(0x27,16,2);

long duration1, distance1;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "7230abe6f30147afae8a7fa77a65fb54";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "cchamchi1";
char pass[] = "1234asdf";

// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial1

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(8, 9); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

SimpleTimer timer;





void setup_Heimdal()
{
  myservo.attach(6);
  pinMode(trigPin1,OUTPUT);
  pinMode(echoPin1,INPUT);
  pinMode(door_led,OUTPUT);

}
void gate_open() {
  
  digitalWrite(door_led,HIGH);
  Serial.println("GATE OPEN");
  lcd.setCursor(0,1); //첫줄
  lcd.print("GATE   OPEN");
  delay(1000);

  
  myservo.write(0);
  delay(10);
  myservo.write(80);
  delay(3000);
  digitalWrite(door_led,LOW);
}

  void gate_close()
{
  
  digitalWrite(motor, LOW);
  Serial.println("GATE CLOSED");
  lcd.setCursor(0,1); //첫줄
  lcd.print("GATE CLOSED");
  digitalWrite(door_led,LOW);

}


void Heimdal_run()
  {
    long duration1, distance1;
    digitalWrite(trigPin1,LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);
    duration1 = pulseIn(echoPin1, HIGH);
    distance1 = (duration1/2) / 29.1;
    Serial.println(distance1);
     

  myservo.write(0);
  delay(1000); // moved to main loop
}




BLYNK_WRITE(V2)

{

int i=param.asInt();
if((i=1)&&(distance1 < 5) && (distance1 > 0))
 {
  gate_open();
  
  }else
  
  {
    gate_close();

  } 

}



void setup()
{
  setup_Heimdal();
  
  // Set console baud rate
  Serial.begin(115200);
  delay(10);
  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  
  Blynk.begin(auth, wifi, ssid, pass);
  
  // Setup a function to be called every second
  timer.setInterval(3000L, Heimdal_run);

}

void loop()
{
Blynk.run();
timer.run();
Heimdal_run();
}
