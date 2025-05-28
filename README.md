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

Eine detaillierte Skizze der Schaltung wird in Kürze hier eingefügt.

### 4.2 Code

Der Code für die Wetterstation wird hier zur Verfügung gestellt. Er beinhaltet die Initialisierung der Sensoren, das Auslesen der Daten und die Ausgabe über die serielle Schnittstelle.

```c++
// Hier wird der Code für die Wetterstation eingefügt
// Der Code wird später hochgeladen und hier eingefügt.
// Beispiel:
/*
#include <DHT.h>

#define DHTPIN 4      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHT11 Test!"));
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
}
*/
```
Dieser Code ist kommentiert, um seine Funktionalität und die einzelnen Schritte der Datenverarbeitung zu erklären. Er initialisiert den DHT11-Sensor, liest in regelmäßigen Abständen Temperatur- und Luftfeuchtigkeitswerte aus und gibt diese auf der seriellen Konsole aus. (Der Code für den Tilt Sensor wird noch ergänzt, sobald er vorhanden ist.)

### 4.3 Bilder und Schaltungen

Hier werden Bilder und Schemata der aufgebauten Schaltung eingefügt.

---
Feel free to upload your actual code and add images of your project setup. Let me know if you need any adjustments or further sections!
