#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>

#define RELAY D1

const int   HTTPS_PORT = 443;
const char* WIFI = "WIFI";
const char* PASSWORD = "PASSWORD";

const char* HOST = "hooks.slack.com";
const char* URL  = "URL";
String PAYLOAD = String("{\"text\": \"@here Café quentinho na cafeteira!\", \"link_names\": 1, \"attachments\":[{\"text\":\" \", \"image_url\":\"https://media2.giphy.com/media/687qS11pXwjCM/giphy.gif\"}]}");

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////AINDA PRECISA ADICIONAR UMA FUNCAO QUE ESCOLHE, ALEATORIAMENTE, UM DOS DEZ GIFS SELECIONADOS COM TAG "coffee",////
////                              PORQUE EU NÃO SEI FAZER ISSOOOOOOOOOOO                                          //// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/* const char *gifs[] = {
        "https://media2.giphy.com/media/687qS11pXwjCM/giphy.gif",
        "https://media.giphy.com/media/zJ8ldRaGLnHTa/giphy.gif",
        "https://media.giphy.com/media/3o6Zt7efI3ruag4zEA/giphy.gif",
        "https://media.giphy.com/media/3oFyDpRagf96Uz9rzO/giphy.gif",      
        "https://media.giphy.com/media/3oKIPx16LFvftHPLiM/giphy.gif",
        "https://media.giphy.com/media/zJ8ldRaGLnHTa/giphy.gif",
        "https://media.giphy.com/media/Z6vszQ8Mweukw/giphy.gif",
        "https://media.giphy.com/media/Q6joirtIBHUsw/giphy.gif",
        "https://media.giphy.com/media/5Ztn33chuvutW/giphy.gif",
        "https://media.giphy.com/media/3o6ZsYZtTSdFOdxPEY/giphy.gif"        
        };

*/

ESP8266WiFiMulti wifi;

bool turnOff = false;
bool coffeeIsReady = false;

void prepareCoffee() {
  delay(9 * 60 * 1000); // Wait 9 minutes.
  coffeeIsReady = true;
}

void notifySlack() {
  HTTPClient client;
  
  client.begin(HOST, HTTPS_PORT, URL, String("AC:95:5A:58:B8:4E:0B:CD:B3:97:D2:88:68:F5:CA:C1:0A:81:E3:6E"));
  client.addHeader("Content-Type", "application/x-www-form-urlencoded");
  client.POST(PAYLOAD);
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

