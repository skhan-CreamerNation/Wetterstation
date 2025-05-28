#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>
#include <WiFiManager.h>    // Für die einfache WLAN-Konfiguration

// === RGB-LED-Konfiguration ===
#define RGB_LED_PIN 8 // <<< ACHTUNG: Dieser Pin ist für ESP32-C3 oft falsch! Bitte prüfen! <<<
#define NUM_PIXELS 1
Adafruit_NeoPixel pixel(NUM_PIXELS, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);

// === Sensor-Definitionen ===
#define DHTPIN 4
#define DHTTYPE DHT11
#define TILT_PIN 10

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// === Datenstrukturen für Messwerte und historische Daten ===
struct SensorData {
  float temp;
  float hum;
  bool tilted;
  String timestamp;
};
SensorData currentData;

// Für stündliche Durchschnittswerte (letzte 24 Stunden)
struct HourlyAvgData {
  float avgTemp;
  float avgHum;
  String timestamp; // Zeitstempel der Stunde
  int count; // Anzahl der Messungen für den Durchschnitt
};
HourlyAvgData hourlyAverages[24];
int hourlyAvgIndex = 0; // Aktueller Index für den Ringpuffer

bool ledEnabled = true;

// WiFiManager Instanz
WiFiManager wifiManager;

// === LED-Funktion ===
void setLedColor(uint8_t r, uint8_t g, uint8_t b) {
  if (!ledEnabled) {
    pixel.clear();
    pixel.show();
    return;
  }
  pixel.setPixelColor(0, pixel.Color(r, g, b));
  pixel.show();
}

String getTimestamp() {
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime (&epochTime);
  char buffer[80];
  strftime(buffer, 80, "%H:%M", ptm);
  return String(buffer);
}

void readAndProcessSensors() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (!isnan(t) && !isnan(h) && t >= -20 && t <= 60 && h >= 0 && h <= 100) {
    currentData.temp = t;
    currentData.hum = h;
    Serial.print("Temperatur: "); Serial.print(currentData.temp); Serial.print(" °C, ");
    Serial.print("Luftfeuchtigkeit: "); Serial.print(currentData.hum); Serial.println(" %");
  } else {
    Serial.println("Ungültige DHT-Werte gelesen oder Sensorfehler.");
  }

  currentData.tilted = (digitalRead(TILT_PIN) == LOW);
  Serial.print("Tilt-Sensor: "); Serial.println(currentData.tilted ? "ALARM!" : "Normal");

  currentData.timestamp = getTimestamp();
}

String getHTML() {
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>ESP32 Wetterstation</title>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <style>
      body { font-family: Arial, sans-serif; margin: 20px; background-color: #f4f4f4; color: #333; }
      h1 { color: #0056b3; }
      .sensor-box {
        display: flex;
        flex-wrap: wrap;
        justify-content: space-around;
        background-color: #fff;
        margin: 10px 0;
        padding: 20px;
        border: 1px solid #ddd;
        border-radius: 8px;
        box-shadow: 0 2px 4px rgba(0,0,0,0.1);
      }
      .sensor-value {
        text-align: center;
        margin: 10px;
        padding: 10px;
        border-right: 1px solid #eee;
      }
      .sensor-value:last-child {
        border-right: none;
      }
      .sensor-value h2 { margin: 0 0 5px 0; font-size: 1.5em; color: #555; }
      .sensor-value span { font-size: 1.8em; font-weight: bold; color: #007bff; }
      .alert { color: #dc3545; font-weight: bold; }
      .timestamp { font-size: 0.9em; color: #777; margin-top: 15px; }
      .chart-container {
        background-color: #fff;
        padding: 20px;
        border: 1px solid #ddd;
        border-radius: 8px;
        box-shadow: 0 2px 4px rgba(0,0,0,0.1);
        margin-top: 20px;
      }
      button {
        background-color: #007bff;
        color: white;
        padding: 10px 15px;
        border: none;
        border-radius: 5px;
        cursor: pointer;
        font-size: 1em;
        margin-top: 20px;
      }
      button:hover {
        background-color: #0056b3;
      }
    </style>
  </head>
  <body>
    <h1>Wetterstation</h1>
    <div class="sensor-box">
      <div class="sensor-value">
        <h2>Temperatur:</h2>
        <span id="temp">)rawliteral" + String(currentData.temp) + R"rawliteral(&deg;C</span>
      </div>
      <div class="sensor-value">
        <h2>Luftfeuchtigkeit:</h2>
        <span id="hum">)rawliteral" + String(currentData.hum) + R"rawliteral( %</span>
      </div>
      <div class="sensor-value">
        <h2>Tilt-Sensor:</h2>
        <span id="tilt" class=")rawliteral" + (currentData.tilted ? "alert" : "") + R"rawliteral(">)rawliteral" + (currentData.tilted ? "ALARM!" : "Normal") + R"rawliteral(</span>
      </div>
      <p class="timestamp">Letzte Messung: <span id="time">)rawliteral" + currentData.timestamp + R"rawliteral(</span></p>
    </div>
    <div class="chart-container">
      <canvas id="chart" width="800" height="300"></canvas>
    </div>
    <div>
      <button onclick="toggleLed()">LED )rawliteral" + (ledEnabled ? "AUS" : "EIN") + R"rawliteral(</button>
    </div>
    <script>
      const ctx = document.getElementById('chart').getContext('2d');
      const chart = new Chart(ctx, {
        type: 'line',
        data: {
          labels: [],
          datasets: [
            { label: 'Temperatur (°C)', data: [], borderColor: 'red', backgroundColor: 'rgba(255, 0, 0, 0.1)', fill: false, tension: 0.3 },
            { label: 'Luftfeuchtigkeit (%)', data: [], borderColor: 'blue', backgroundColor: 'rgba(0, 0, 255, 0.1)', fill: false, tension: 0.3 }
          ]
        },
        options: {
          responsive: true,
          maintainAspectRatio: false,
          scales: {
            y: {
              beginAtZero: false,
              title: {
                display: true,
                text: 'Wert'
              },
              suggestedMin: 0,
              suggestedMax: 50
            },
            x: {
                title: {
                    display: true,
                    text: 'Uhrzeit'
                }
            }
          },
          plugins: {
              tooltip: {
                  mode: 'index',
                  intersect: false,
              },
              legend: {
                  display: true,
                  position: 'top'
              }
          }
        }
      });

      function updateData() {
        fetch('/data')
          .then(r => r.json())
          .then(data => {
            document.getElementById('temp').textContent = data.temp + ' °C';
            document.getElementById('hum').textContent = data.hum + ' %';
            document.getElementById('tilt').textContent = data.tilted ? 'ALARM!' : 'Normal';
            document.getElementById('tilt').className = data.tilted ? 'alert' : '';
            document.getElementById('time').textContent = data.timestamp;

            chart.data.labels = data.hourlyLabels;
            chart.data.datasets[0].data = data.hourlyTemp;
            chart.data.datasets[1].data = data.hourlyHum;
            chart.update();
          })
          .catch(error => console.error('Fehler beim Abrufen der Daten:', error));
      }

      function toggleLed() {
        fetch('/led?toggle=1')
          .then(response => {
            if (response.ok) {
              location.reload();
            }
          })
          .catch(error => console.error('Fehler beim Umschalten der LED:', error));
      }

      updateData();
      setInterval(updateData, 5000);
    </script>
  </body>
  </html>
  )rawliteral";
  return html;
}

void handleData() {
  StaticJsonDocument<1200> doc;
  doc["temp"] = currentData.temp;
  doc["hum"] = currentData.hum;
  doc["tilted"] = currentData.tilted;
  doc["timestamp"] = currentData.timestamp;

  JsonArray labels = doc.createNestedArray("hourlyLabels");
  JsonArray temp = doc.createNestedArray("hourlyTemp");
  JsonArray hum = doc.createNestedArray("hourlyHum");

  for (int i = 0; i < 24; i++) {
    int index = (hourlyAvgIndex + i) % 24;
    // NUR DATEN HINZUFÜGEN, WENN count > 0 (bedeutet: es ist ein gültiger Wert)
    if (hourlyAverages[index].count > 0) {
      labels.add(hourlyAverages[index].timestamp);
      temp.add(hourlyAverages[index].avgTemp);
      hum.add(hourlyAverages[index].avgHum);
    }
  }

  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json);
}

void handleLed() {
  if (server.hasArg("toggle")) {
    ledEnabled = !ledEnabled;
  }
  server.send(200, "text/plain", "OK");
}

// === WLAN-Verbindungsfunktion nur mit WiFiManager ===
void configureNetwork() {
  Serial.println("Versuche Verbindung zum WLAN über WiFiManager...");
  setLedColor(255, 0, 0); // Rot = Verbindung wird versucht

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true); // Alte Verbindungen trennen
  delay(100);

  // WiFiManager versucht sich mit gespeicherten Daten zu verbinden.
  // Wenn keine Daten gespeichert sind oder die Verbindung fehlschlägt,
  // wird ein Access Point "WetterstationAP" (ohne Passwort) gestartet.
  // Verbinde dich damit und gib deine WLAN-Daten ein.
  if (!wifiManager.autoConnect("WetterstationAP")) {
    Serial.println("WiFiManager Verbindung fehlgeschlagen und Timeout.");
    setLedColor(255, 0, 0); // Rot = Finaler Fehler, WiFiManager scheitert
    Serial.println("Neustart in 5 Sekunden...");
    delay(5000); // Gib Zeit, die LED zu sehen
    ESP.restart(); // Neustart, wenn auch WiFiManager scheitert
  } else {
    Serial.println("Erfolgreich mit WLAN über WiFiManager verbunden!");
    Serial.print("IP-Adresse: ");
    Serial.println(WiFi.localIP());
    setLedColor(0, 255, 0); // Grün = verbunden
  }
}

// === SETUP FUNKTION ===
void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(TILT_PIN, INPUT_PULLUP);
  dht.begin();

  pixel.begin();
  pixel.setBrightness(50);
  pixel.clear();
  pixel.show();

  // Initialisiere den hourlyAverages Puffer
  for (int i = 0; i < 24; i++) {
    hourlyAverages[i].avgTemp = NAN;
    hourlyAverages[i].avgHum = NAN;
    hourlyAverages[i].count = 0; // Sicherstellen, dass alles auf 0 ist
    hourlyAverages[i].timestamp = "";
  }

  configureNetwork(); // WLAN-Verbindung herstellen (nur über WiFiManager)

  timeClient.begin();
  timeClient.setTimeOffset(7200); // 7200 Sekunden = 2 Stunden (GMT+2 für Sommerzeit in Österreich/Deutschland)
  // Stelle sicher, dass die Zeit mindestens einmal synchronisiert wird
  while(!timeClient.update()) {
    Serial.println("NTP-Synchronisierung läuft...");
    delay(1000);
  }
  Serial.print("NTP Zeit synchronisiert: ");
  Serial.println(timeClient.getFormattedTime());


  server.on("/", HTTP_GET, []() { server.send(200, "text/html; charset=UTF-8", getHTML()); }); // <<< HIER GEÄNDERT: charset=UTF-8 hinzugefügt <<<
  server.on("/data", HTTP_GET, handleData);
  server.on("/led", HTTP_GET, handleLed);
  server.begin();

  Serial.println("Webserver gestartet.");
  setLedColor(0, 255, 0); // Grün = bereit
}

void loop() {
  static unsigned long lastMeasureMillis = 0;
  static unsigned long lastHourlyUpdateMillis = 0;
  static unsigned long lastNTPUpdateMillis = 0;

  server.handleClient();
  if (millis() - lastNTPUpdateMillis >= 3600000) { // NTP-Update jede Stunde
    timeClient.update();
    Serial.println("NTP Zeit aktualisiert.");
    lastNTPUpdateMillis = millis();
  }

  if (millis() - lastMeasureMillis >= 5000) { // Messungen alle 5 Sekunden
    setLedColor(0, 0, 255); // Kurz blau beim Messen
    readAndProcessSensors();
    lastMeasureMillis = millis();

    // LED-Farben basierend auf Status
    if (currentData.tilted) {
      setLedColor(255, 0, 255); // Magenta bei Tilt-Alarm
    } else if (currentData.temp > 30) {
      setLedColor(255, 165, 0); // Orange bei hoher Temperatur
    } else if (WiFi.status() == WL_CONNECTED) {
      setLedColor(0, 255, 0); // Grün, wenn alles OK
    } else {
      setLedColor(255, 0, 0); // Rot, wenn kein WLAN
    }
  }

  // Dieser Block aktualisiert die Daten für den Graphen jede Minute
  if (millis() - lastHourlyUpdateMillis >= 60000) {
    hourlyAverages[hourlyAvgIndex].avgTemp = currentData.temp;
    hourlyAverages[hourlyAvgIndex].avgHum = currentData.hum;
    hourlyAverages[hourlyAvgIndex].timestamp = currentData.timestamp.substring(0, 5); // Nur HH:MM

    hourlyAverages[hourlyAvgIndex].count = 1; // <<< WICHTIGE NEUE ZEILE: Datenpunkt ist jetzt gültig! <<<

    hourlyAvgIndex = (hourlyAvgIndex + 1) % 24; // Nächster Speicherplatz im Ringpuffer
    lastHourlyUpdateMillis = millis();
  }
}