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
  client.get('https://fablabjoinville-bot.herokuapp.com/coffee_gifs')
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
