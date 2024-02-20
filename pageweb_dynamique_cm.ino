#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"

// Définition des entrées/sorties
#define PIN_LED_ROUGE 5
#define PIN_LED_VERTE 4
#define TRIGGER_PIN 12
#define ECHO_PIN 14

// Page html
const char index_html[] PROGMEM = R"=====(
<!doctype html>
<html lang="fr">
    <head>
        <meta charset="utf-8">
        <title>Place de parking</title>
        <script>
            function verifierPlace() {
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function() {
                    if (this.readyState == 4 && this.status == 200) {
                        document.getElementById("etatPlace").innerHTML = "Distance : " + this.responseText + " cm";
                    }
                };
                xhttp.open("GET", "/etatPlace", true);
                xhttp.send();
            }
        </script>
    </head>
    <body>
        <h1>Place de parking</h1>
        <button onclick="verifierPlace()">La place est-elle libre ?</button>
        <h2 id="etatPlace">%ETAT_PLACE%</h2>

        <script>
            // Appeler la fonction au chargement de la page pour afficher l'état initial de la place
            verifierPlace();
        </script>
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
  String temp(reinterpret_cast<const __FlashStringHelper *> (index_html));

  // Mesurer la distance avec le capteur HC-SR04
  long duration, distance;
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2) / 29.1;

  // Mettre à jour l'état de la place en fonction de la distance mesurée
  temp.replace("%ETAT_PLACE%", String(distance));
  serverWeb.send(200, "text/html", temp);
}

void getEtatPlace() {
  // Mesurer la distance avec le capteur HC-SR04
  long duration, distance;
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2) / 29.1;

  // Renvoyer la distance mesurée
  serverWeb.send(200, "text/plain", String(distance));
}

void setup() {
  // Mise en place d'une liaison série
  Serial.begin(9600L);
  delay(100);

  // Configuration des entrées/sorties
  pinMode(PIN_LED_ROUGE, OUTPUT);
  pinMode(PIN_LED_VERTE, OUTPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Mode de connexion
  WiFi.mode(WIFI_STA);
 
  // Démarrage de la connexion
  WiFi.begin(SSID, PASSWORD);

  static WiFiEventHandler onConnectedHandler = WiFi.onStationModeConnected(onConnected);
  static WiFiEventHandler onGotIPHandler = WiFi.onStationModeGotIP(onGotIP);

  // Mise en place du serveur WebServer
  serverWeb.on("/etatPlace", HTTP_GET, getEtatPlace);
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
