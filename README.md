# SR04_projet

## Explication code CA-FONCTIONNE.ino


## Client (Navigateur Web) demande la page:
Lorsque vous ouvrez la page web sur votre navigateur, une requête HTTP est envoyée à l'adresse IP du module ESP8266 sur le port 80 (car le serveur Web est configuré pour écouter sur ce port).

## ESP8266 (Serveur Web) reçoit la demande:
Le serveur Web ESP8266 (qui est configuré pour répondre aux requêtes sur les chemins '/' et '/getPIRState') détecte la requête HTTP. En fonction du chemin de la requête, il appelle la fonction correspondante.

## Traitement de la demande:
Si la requête est pour la racine ('/'), la fonction `handleRoot` est appelée. Cette fonction génère une réponse HTML en remplaçant `%ETAT%` par l'état actuel du capteur PIR (1 pour mouvement détecté, 0 sinon). Si la requête est pour '/getPIRState', la fonction `getPIRState` est appelée. Cette fonction renvoie simplement l'état actuel du capteur PIR en texte brut (1 ou 0).

## Réponse au client:
Le serveur Web renvoie la réponse générée au client. Dans le cas de la page principale ('/'), il envoie la page HTML mise à jour avec l'état actuel du capteur PIR. Dans le cas de '/getPIRState', il envoie simplement l'état en texte brut.

## JavaScript sur la page HTML côté client:
Le code JavaScript inclus dans la page HTML s'exécute côté client. Il utilise une requête asynchrone (AJAX) pour interroger périodiquement le serveur (/getPIRState) et mettre à jour dynamiquement le contenu de la page avec l'état actuel du capteur PIR.

## Mise à jour dynamique de la page:
Le JavaScript met à jour la partie de la page où se trouve l'élément avec l'ID "etatPIR" en fonction de la réponse du serveur, indiquant s'il y a un mouvement détecté ou non.

En résumé, lorsque vous ouvrez la page web, le navigateur envoie des requêtes au serveur ESP8266, le serveur répond avec l'état actuel du capteur PIR, et le JavaScript sur la page met à jour dynamiquement le contenu pour refléter cet état. Ce processus se répète périodiquement en raison de l'intervalle défini par `setInterval(miseAJour, 1000);`.
