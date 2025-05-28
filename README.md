# ITP Projekt Wetterstation - Variante 1

## Eine einfache Wetterstation mit Temperatur- & Luftfeuchtigkeitssensor

**Verfasser:** Simon Khan, Josef Ibrahim

**Datum:** 28.05.2025

---

## 1. Einführung

Dieses Projekt wurde gewählt, um die **Grundlagen der Sensorik und Mikrocontroller-Programmierung** anhand einer praktischen Anwendung zu erlernen. Eine Wetterstation bietet hierfür eine ideale Plattform, da sie verschiedene Sensoren und die Verarbeitung von Daten beinhaltet. Die Dokumentation ist im Passiv verfasst, um den Fokus auf die durchgeführten Arbeiten und nicht auf die handelnden Personen zu legen.

---

## 2. Projektbeschreibung

Es wurde eine einfache Wetterstation auf einem **Breadboard** aufgebaut, die **Temperatur und Luftfeuchtigkeit** erfassen kann. Die gesammelten Daten werden mithilfe eines **ESP32 C3 DevKitM1 Mikrocontrollers** verarbeitet. Der zugehörige Code wird in diesem Repository zur Verfügung gestellt.

---

## 3. Theorie

Für die Realisierung dieses Projekts sind Kenntnisse in folgenden Bereichen notwendig:

* **ESP32 C3 DevKitM1:** Dies ist ein kostengünstiger und stromsparender Mikrocontroller mit integriertem Wi-Fi und Bluetooth. Er eignet sich hervorragend für IoT-Anwendungen und ist die zentrale Steuerungseinheit unserer Wetterstation. Die Programmierung erfolgt in der Arduino IDE mit der ESP-IDF (Espressif IoT Development Framework).
* **DHT11 Sensor:** Dieser Sensor misst digitale Temperatur- und Luftfeuchtigkeitswerte. Er ist einfach zu verwenden und liefert ausreichend genaue Daten für grundlegende Wetteranwendungen. Die Kommunikation mit dem DHT11 erfolgt über ein Ein-Draht-Protokoll, das eine präzise Zeitsteuerung erfordert.
* **ALLNET B15 Tilt Sensor:** Ein einfacher Neigungsschalter, der erkennt, ob der Sensor geneigt ist oder nicht. In diesem Projekt wird er verwendet, um eine zusätzliche Zustandsänderung zu erfassen, die über die reinen Wetterdaten hinausgeht.
* **RGB-LED (NeoPixel):** Eine programmierbare LED, die durch Farbwechsel verschiedene Betriebszustände der Wetterstation anzeigt (z.B. WLAN-Status, Messung, Alarme). Sie ermöglicht eine visuelle Rückmeldung über den Systemstatus, ohne auf die serielle Konsole angewiesen zu sein.
* **NTPClient (Network Time Protocol):** Ermöglicht dem ESP32, die aktuelle Uhrzeit automatisch über das Internet abzurufen. Dies ist wichtig, um den Messungen korrekte Zeitstempel zuzuweisen und Daten chronologisch zu speichern oder anzuzeigen.
* **Webserver auf ESP32:** Der ESP32 stellt eine Webseite bereit, über die die Sensordaten im Browser angezeigt werden können. Er empfängt HTTP-Anfragen vom Browser, verarbeitet diese und sendet HTML-Seiten sowie Daten (z.B. im JSON-Format) zurück an den Client.
* **WiFiManager Bibliothek:** Diese Bibliothek vereinfacht die WLAN-Einrichtung erheblich. Anstatt WLAN-Zugangsdaten fest im Code zu hinterlegen, stellt sie einen temporären Access Point bereit, über den Nutzer die Zugangsdaten für ihr Heim-WLAN einfach über ein Webinterface eingeben können.
* **JSON (JavaScript Object Notation):** Ein leichtgewichtiges, menschenlesbares Textformat für den Austausch von Daten. Es wird im Projekt verwendet, um Sensordaten effizient zwischen dem ESP32-Webserver und dem Webbrowser zu übertragen, da es von JavaScript einfach geparst und verarbeitet werden kann.
* **Chart.js (JavaScript Bibliothek):** Eine flexible und leistungsstarke JavaScript-Bibliothek, die im Webbrowser verwendet wird. Sie ermöglicht die einfache Darstellung von Daten in Form von interaktiven Grafiken (z.B. Liniendiagrammen für Temperatur- und Feuchtigkeitsverläufe), was die Visualisierung der historischen Sensordaten verbessert.
* **`millis()`-Funktion:** Eine grundlegende Funktion im Arduino-Framework, die die Anzahl der seit dem Start des Mikrocontrollers verstrichenen Millisekunden zurückgibt. Sie wird zur zeitgesteuerten Ausführung von Aufgaben (z.B. das regelmäßige Auslesen von Sensoren) genutzt, ohne den Hauptprogrammfluss zu blockieren. Dies ist entscheidend für die Implementierung nicht-blockierender Operationen auf dem ESP32, während gleichzeitig der Webserver und andere Dienste laufen.
* **Breadboard:** Ein Steckbrett ermöglicht den schnellen und lötfreien Aufbau elektronischer Schaltungen. Es ist ideal für Prototypen und Experimente.
* **Jumperkabel:** Diese Kabel werden verwendet, um die einzelnen Komponenten auf dem Breadboard miteinander zu verbinden.

Fragestellungen, die während des Projekts behandelt wurden:
* Wie werden verschiedene Sensoren an einen Mikrocontroller angeschlossen und ausgelesen?
* Wie können die gesammelten Sensordaten verarbeitet und ausgegeben werden?
* Welche Bibliotheken und Funktionen sind für die Kommunikation mit den verwendeten Sensoren erforderlich?
* Wie wird der Mikrocontroller für die jeweilige Hardware konfiguriert?
* Wie kann der ESP32 dynamisch mit dem Internet verbunden und die Zeit synchronisiert werden?
* Wie können Sensordaten über eine Webseite visualisiert und interaktiv dargestellt werden?
* Wie werden kritische Systemzustände visuell signalisiert?

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

![image](https://github.com/user-attachments/assets/32523bee-99ec-4c4d-9679-cdb1468639a7)

Dieser Code ist kommentiert, um seine Funktionalität und die einzelnen Schritte der Datenverarbeitung zu erklären. Er initialisiert den DHT11-Sensor, liest in regelmäßigen Abständen Temperatur- und Luftfeuchtigkeitswerte aus und gibt diese auf der seriellen Konsole aus. (Der Code für den Tilt Sensor wird noch ergänzt, sobald er vorhanden ist.)

### 4.3 Bilder und Schaltungen

![image](https://github.com/user-attachments/assets/67db4e9c-3a27-4f3e-a2b8-0500db868cb3)
