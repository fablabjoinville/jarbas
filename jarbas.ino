#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>

#define RELAY D1

const int   HTTPS_PORT = 443;
const char* WIFI = "WIFI";
const char* PASSWORD = "PASSWORD";

const char* HOST = "hooks.slack.com";
const char* URL  = "URL";

ESP8266WiFiMulti wifi;

bool turnOff = false;
bool coffeeIsReady = false;

void prepareCoffee() {
  delay(9 * 60 * 1000); // Wait 9 minutes.
  coffeeIsReady = true;
}

void notifySlack() {
  HTTPClient client;

  client.begin(HOST, HTTPS_PORT, String("AC:95:5A:58:B8:4E:0B:CD:B3:97:D2:88:68:F5:CA:C1:0A:81:E3:6E"));
  client.addHeader("Content-Type", "application/x-www-form-urlencoded");
  client.GET();
  client.end();
}

void waitAndTurnOffCoffeeMachine() {
  delay(10 * 60 * 1000); // Wait 10 minutes.
  digitalWrite(RELAY, LOW); // Turn off.
  turnOff = true;
}

void setup() {
  wifi.addAP(WIFI, PASSWORD);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, HIGH);
}

void loop() {
  if (!turnOff && !coffeeIsReady) prepareCoffee();

  if (!turnOff && coffeeIsReady && wifi.run() == WL_CONNECTED) {
    notifySlack();
    waitAndTurnOffCoffeeMachine();
  }

  delay(1000);
}
