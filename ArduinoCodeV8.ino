
#include <SPI.h>
#include <WiFiNINA.h>
#include <avr/dtostrf.h>
#include <LiquidCrystal.h>

char const *CAR = "Car";
char const *SERVER="ev-scheduler.appspot.com";
const int HTTPPORT= 443;
char const * SSID_NAME = "Secret"; 
char const * SSID_PASS = "Secret"; 

int val,duration;
int prevstate = LOW;
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
int status = WL_IDLE_STATUS;
WiFiSSLClient client;

void getResponseServer() {
  Serial.println(F("\nGoogle Cloud' Server response:\n"));
  while (client.available()) {
    char c = client.read();
    Serial.print(c); 
  }
}

void HTTPrequest(char* payload) {
  int contentLength = strlen(payload);
  int timeout = 0;


  if (client.connect(SERVER, HTTPPORT)) {
    Serial.println("connection to server successful");
   
    client.print(F("PUT "));
    client.print("/api/location/test-location/charger/test-charger");    
    client.print(" HTTP/1.1\r\n");
    client.print(F("Host: "));
    client.print("ev-scheduler.appspot.com");
    client.print(F("\r\n"));  
    client.print(F("User-Agent: "));
    client.print("Arduino Nano 33 IoT");
    client.print(F("\r\n"));
    client.print(F("Authorization:"));
    client.print(F("Bearer OOgUCgdUAtJaMFIVXmbLDxmASiQHdc"));
    client.print(F("\r\n"));
    client.print(F("Connection: close\r\n"));
    client.print(F("Content-Type: application/json\r\n"));
    client.print(F("Content-Length: "));
    client.print(contentLength);
    client.print(F("\r\n\r\n"));
    client.print(payload);
    client.print(F("\r\n"));
   
    Serial.print(F("PUT "));
    Serial.print("/api/location/test-location/charger/test-charger");    
    Serial.print(" HTTP/1.1\r\n");
    Serial.print(F("Host: "));
    Serial.print("ev-scheduler.appspot.com");
    Serial.print(F("\r\n"));  
    Serial.print(F("User-Agent: "));
    Serial.print("Arduino Nano 33 IoT");
    Serial.print(F("\r\n"));
    Serial.print(F("Authorization:"));
    Serial.print(F("Bearer OOgUCgdUAtJaMFIVXmbLDxmASiQHdc"));
    Serial.print(F("\r\n"));
    Serial.print(F("Connection: close\r\n"));
    Serial.print(F("Content-Type: application/json\r\n"));
    Serial.print(F("Content-Length: "));
    Serial.print(contentLength);
    Serial.print(F("\r\n\r\n"));
    Serial.print(payload);
    Serial.print(F("\r\n"));
   
  while (!client.available() && timeout < 4000) {
    timeout++;
    delay(1);
    if (timeout >= 4000) {
      Serial.println(F("MAX time duration met"));
      break;
    }
  }
    getResponseServer();
  }

  client.stop();
}

void setup() {

   // declare pin 9 to be an output:
  pinMode(9, OUTPUT);
  Serial.begin(9600);
  pinMode(2, INPUT);  //PWM Pin
  pinMode(3, INPUT);  //IO Pin  
  analogWrite(9, 5);
  //PWM the LCD contrast pin a low voltage   
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Welcome to the EVSE!");
  //Initialize serial and wait for port to open:
 
  while (!Serial) {
    ; 
  }
 
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Failed connection with Wi-fi module");
    while (true);
  }
 
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SSID_NAME);
    status = WiFi.begin(SSID_NAME, SSID_PASS);
    delay(10000);
  }
  Serial.println("Connected to wifi network");
}

void loop(){

  char payload[200];

  duration = pulseIn(2, HIGH);
  if(duration == 0) {
    lcd.clear();
    lcd.print("No vehicle connected");
    payload = "Not Vehicle Connected";
  }
  val = digitalRead(3);
  if(val == HIGH) {
    //9V
    lcd.clear();
    lcd.print("Connected");
    payload = "Connected";
    lcd.setCursor(0,1);
    if(prevstate = HIGH)
    {
      lcd.print("Charge Complete");
      payload = "Charge Complete";
      prevstate = LOW;
    }
    if(prevstate = LOW)
      lcd.print("EVSE in Standby");
      payload = "EVSE Standby";
  }
  //Duration range not filled in, need to test onsite
  if(val == LOW)  {
    //6V
    if()  {//check duration range for 12A
      lcd.clear();
      lcd.print("Connected");
      payload = "Connected";
      lcd.setCursor(0,1);
      lcd.print("Charging at 12A");
    }
    if()  {//check duration range for 16A
      lcd.clear();
      lcd.print("Connected");
      payload = "Connected";
      lcd.setCursor(0,1);
      lcd.print("Charging at 16A");
    }
    if()  {//check duration range for 30A
      lcd.clear();
      lcd.print("Connected");
      payload = "Connected";
      lcd.setCursor(0,1);
      lcd.print("Charging at 30A");
    }
    prevstate = HIGH;
    }
  }
 
  sprintf(payload, "%s","");
  sprintf(payload, "{\"");
  sprintf(payload, "%s%s\":%s", payload, CAR);
  sprintf(payload, "%s}", payload);
  HTTPrequest(payload);
  delay(5000);
} 
