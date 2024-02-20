#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"

// Définition des entrées/sorties
#define PIN_LED_ROUGE 5
#define PIN_LED_VERTE 4

// Page html
const char index_html[] PROGMEM = R"=====(
  <!doctype html>
<html lang="fr">
    <head>
        <meta charset="utf-8">
        <title>Commande LED</title>
    </head>
    <body>
        <a href="/switchLedOn">
            <button>Allumer la LED</button>
        </a>
        <a href="/switchLedOff">
            <button>Eteindre la LED</button>
        </a>
    </body>
</html>
)=====";

// Informations de connexion : cachées dans fichier InfoConnexion.h
// Vous pouvez décommenter ici ou créer comme moi un fichier InfoConnexion.h
const char * SSID = "Redmi Note 11";
const char * PASSWORD = "esther2001";

// Gestion des événements du WiFi
// Lorsque la connexion vient d'aboutir
void onConnected(const WiFiEventStationModeConnected& event);
// Lorsque l'adresse IP est attribuée
void onGotIP(const WiFiEventStationModeGotIP& event);

// Objet WebServer
ESP8266WebServer serverWeb(80);

// Fonctions du serveur Web
void handleRoot() {
  serverWeb.send(200, "text/html", index_html);
}

void switchLedOn() {
  digitalWrite(PIN_LED_VERTE, HIGH);
  handleRoot();
}

void switchLedOff() {
  digitalWrite(PIN_LED_VERTE, LOW);
  handleRoot();
}

void setup() {
  // Mise en place d'une liaison série
  Serial.begin(9600L);
  delay(100);

  // Configuration des entrées/sorties
  pinMode(PIN_LED_ROUGE, OUTPUT);
  pinMode(PIN_LED_VERTE, OUTPUT);

  // Mode de connexion
  WiFi.mode(WIFI_STA);
 
  // Démarrage de la connexion
  WiFi.begin(SSID, PASSWORD);

  static WiFiEventHandler onConnectedHandler = WiFi.onStationModeConnected(onConnected);
  static WiFiEventHandler onGotIPHandler = WiFi.onStationModeGotIP(onGotIP);

  // Mise en place du serveur WebServer
  serverWeb.on("/switchLedOn", switchLedOn);
  serverWeb.on("/switchLedOff", switchLedOff);
  serverWeb.on("/", handleRoot);
  serverWeb.on("/index.html", handleRoot);
  serverWeb.begin();
}

void loop() {
  // Si l'objet est connecté au réseau, on effectue les tâches qui doivent l'être dans ce cas
  if (WiFi.isConnected()) {
    digitalWrite(PIN_LED_ROUGE, HIGH);
    serverWeb.handleClient();
  }
  else {
    digitalWrite(PIN_LED_ROUGE, LOW);
  }

}

void onConnected(const WiFiEventStationModeConnected& event) {
  Serial.println("WiFi connecté");
  Serial.println("Adresse IP : " + WiFi.localIP().toString());
}

void onGotIP(const WiFiEventStationModeGotIP& event) {
  Serial.println("Adresse IP : " + WiFi.localIP().toString());
  Serial.println("Passerelle IP : " + WiFi.gatewayIP().toString());
  Serial.println("DNS IP : " + WiFi.dnsIP().toString());
  Serial.print("Puissance de réception : ");
  Serial.println(WiFi.RSSI());
}
