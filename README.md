# ITP Projekt Wetterstation - Variante 1

## Eine einfache Wetterstation mit Temperatur- & Luftfeuchtigkeitssensor

Verfasser: **Simon Khan, Josef Ibrahim**

Datum: **28.05.2025**

---
## 1. Einführung

Dieses Projekt wurde gewählt, um die Grundlagen der Sensorik und Mikrocontroller-Programmierung anhand einer praktischen Anwendung zu erlernen. Eine Wetterstation bietet hierfür eine ideale Plattform, da sie verschiedene Sensoren und die Verarbeitung von Daten beinhaltet. Die Dokumentation ist im Passiv verfasst, um den Fokus auf die durchgeführten Arbeiten und nicht auf die handelnden Personen zu legen.

---
## 2. Projektbeschreibung

Es wurde eine einfache Wetterstation auf einem Breadboard aufgebaut, die Temperatur und Luftfeuchtigkeit erfassen kann. Die gesammelten Daten werden mithilfe eines ESP32 C3 DevKitM1 Mikrocontrollers verarbeitet. Der zugehörige Code wird in diesem Repository zur Verfügung gestellt.

---
## 3. Theorie

Für die Realisierung dieses Projekts sind Kenntnisse in folgenden Bereichen notwendig:

* **ESP32 C3 DevKitM1:** Dies ist ein kostengünstiger und stromsparender Mikrocontroller mit integriertem Wi-Fi und Bluetooth. Er eignet sich hervorragend für IoT-Anwendungen und ist die zentrale Steuerungseinheit unserer Wetterstation. Die Programmierung erfolgt in der Arduino IDE mit der ESP-IDF (Espressif IoT Development Framework).
* **DHT11 Sensor:** Dieser Sensor misst digitale Temperatur- und Luftfeuchtigkeitswerte. Er ist einfach zu verwenden und liefert ausreichend genaue Daten für grundlegende Wetteranwendungen. Die Kommunikation mit dem DHT11 erfolgt über ein Ein-Draht-Protokoll, das eine präzise Zeitsteuerung erfordert.
* **ALLNET B15 Tilt Sensor:** Ein einfacher Neigungsschalter, der erkennt, ob der Sensor geneigt ist oder nicht. In diesem Projekt wird er verwendet, um eine zusätzliche Zustandsänderung zu erfassen, die über die reinen Wetterdaten hinausgeht.
* **Breadboard:** Ein Steckbrett ermöglicht den schnellen und lötfreien Aufbau elektronischer Schaltungen. Es ist ideal für Prototypen und Experimente.
* **Jumperkabel:** Diese Kabel werden verwendet, um die einzelnen Komponenten auf dem Breadboard miteinander zu verbinden.

Fragestellungen, die während des Projekts behandelt wurden:
* Wie werden verschiedene Sensoren an einen Mikrocontroller angeschlossen und ausgelesen?
* Wie können die gesammelten Sensordaten verarbeitet und ausgegeben werden?
* Welche Bibliotheken und Funktionen sind für die Kommunikation mit den verwendeten Sensoren erforderlich?
* Wie wird der Mikrocontroller für die jeweilige Hardware konfiguriert?

---
## 4. Arbeitsschritte

Die Umsetzung der Wetterstation erfolgte in zwei Hauptschritten: dem Hardware-Aufbau auf dem Breadboard und der Entwicklung des zugehörigen Codes.

### 4.1 Hardware-Aufbau

Der Aufbau der Schaltung erfolgte auf einem Breadboard unter Verwendung der folgenden Komponenten:

* **Breadboard:** Als Basis für die gesamte Schaltung.
* **Jumperkabel:** Zur Verbindung der einzelnen Komponenten.
* **ESP32 C3 DevKitM1:** Der Mikrocontroller, der die Sensoren steuert und die Daten verarbeitet.
* **DHT11 Sensor:** Angeschlossen an einen digitalen GPIO-Pin des ESP32, um Temperatur und Luftfeuchtigkeit zu messen.
* **ALLNET B15 Tilt Sensor:** Angeschlossen an einen weiteren digitalen GPIO-Pin, um Neigungsereignisse zu erkennen.



### 4.2 Code

Der Code für die Wetterstation wird hier zur Verfügung gestellt. Er beinhaltet die Initialisierung der Sensoren, das Auslesen der Daten und die Ausgabe über die serielle Schnittstelle.

// === Funktion zum Auslesen der Sensorwerte ===
void readSensorData() {
  // Temperatur vom DHT11-Sensor auslesen (in °C)
  float temperature = dht.readTemperature();
  
  // Luftfeuchtigkeit vom DHT11-Sensor auslesen (in %)
  float humidity = dht.readHumidity();
  
  // Neigungssensor (Tilt) abfragen – liefert true, wenn geneigt
  bool tilt = digitalRead(TILT_PIN);

  // Prüfen, ob die Sensorwerte gültig sind (nicht "NaN")
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Fehler beim Auslesen des DHT11-Sensors!");
    return; // Keine Speicherung bei Fehler
  }
  // Sensorwerte in der globalen Struktur 'currentData' speichern
  currentData.temp = temperature;
  currentData.hum = humidity;
  currentData.tilted = tilt;
  currentData.timestamp = timeClient.getFormattedTime(); // Aktuelle Uhrzeit dazuspeichern
  Serial.println("Sensorwerte aktualisiert");
}
// === HTTP-Handler-Funktion zur Ausgabe der aktuellen Sensordaten als JSON ===
void handleCurrentData() {
  StaticJsonDocument<256> doc; // JSON-Dokument mit max. 256 Byte

  // Sensorwerte in das JSON-Dokument eintragen
  doc["temp"] = currentData.temp;       // Temperatur in °C
  doc["hum"] = currentData.hum;         // Luftfeuchtigkeit in %
  doc["tilt"] = currentData.tilted;     // Neigungsstatus (true/false)
  doc["time"] = currentData.timestamp;  // Zeitstempel im Format HH:MM:SS

  String response;
  serializeJson(doc, response);         // JSON-Dokument in String umwandeln
  // JSON-Antwort mit HTTP-Code 200 (OK) an den Browser senden
  server.send(200, "application/json", response);
}

Dieser Code ist kommentiert, um seine Funktionalität und die einzelnen Schritte der Datenverarbeitung zu erklären. Er initialisiert den DHT11-Sensor, liest in regelmäßigen Abständen Temperatur- und Luftfeuchtigkeitswerte aus und gibt diese auf der seriellen Konsole aus. (Der Code für den Tilt Sensor wird noch ergänzt, sobald er vorhanden ist.)

### 4.3 Bilder und Schaltungen

Hier werden Bilder und Schemata der aufgebauten Schaltung eingefügt.

