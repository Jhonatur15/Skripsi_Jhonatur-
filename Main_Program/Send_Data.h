// Variabel global
unsigned long time_ms;
unsigned long lastUpdateFirebase = 0;
unsigned long lastUpdateSpreadsheet = 0;
const unsigned long INTERVAL_FIREBASE = 60;      // Interval Firebase (ms)
const unsigned long INTERVAL_SPREADSHEET = 60;  // Interval Spreadsheet (ms)

/* Firebase ACCESS ===================================================== */
#define API_KEY "AIzaSyBCFik5bSRN6HuZcdP42tkj2qQlUxmuNUw"
#define DATABASE_URL "https://smarcos2-9de35-default-rtdb.firebaseio.com/"

// Fungsi untuk mengecek dan menjalankan tugas berbasis millis
void updateIfNeeded(unsigned long &lastUpdateTime, unsigned long interval, void (*callback)()) {
  time_ms = millis();  // Ambil waktu saat ini
  if (time_ms - lastUpdateTime >= interval) {
    lastUpdateTime = time_ms;  // Perbarui waktu terakhir
    callback();  // Panggil fungsi pengiriman data
  }
}

void sendDataToFirebase(String path, String jsonPayload) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = DATABASE_URL + path + ".json?auth=" + API_KEY;

    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.PUT(jsonPayload);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.print("Response payload: ");
      Serial.println(http.getString());
    } else {
      Serial.print("Error sending data: ");
      Serial.println(httpResponseCode);
    }

    http.end();  // Close connection
  } else {
    Serial.println("WiFi disconnected");
  }
}


void sendDataToSpreadsheet() {
  HTTPClient http;
  // Seand Data Sensor --------------------------------------------------- */
  // Buat string untuk menyimpan semua rule yang aktif
  String activeRules = "";
  for (int i = 1; i <= 32; i++) {
    if (fuzzy->isFiredRule(i)) {
      if (activeRules.length() > 0) {
        activeRules += ",";
      }
      activeRules += String(i);
    }
  }
  String url = "/macros/s/" + GAS_ID + "/exec?tempOut=";
  url += String(TempOut);
  url += "&tempIn=";
  url += String(TempIn);
  url += "&turb=";
  url += String(turb);
  url += "&pH=";
  url += String(pH);
  url += "&DO=";
  url += String(DO_R);
  // Seand Data Tranduser------------------------------------------------- */
  url += "&heater=";
  url += String(StrHeater);
  url += "&aerator=";
  url += String(StrAerator);
  url += "&sirku=";
  url += String(StrSirkulasi);
  url += "&rule=";
  url += activeRules;
  Serial.print("Requesting URL: ");
  Serial.println(url);
  http.begin("https://" + String(host) + url);  // HTTPS!
  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
  } else {
    Serial.println("Error on HTTP request");
  }
  http.end();
}

// Fungsi pengiriman data ke Firebase
void sendDataToFirebaseTask() {
  // Payload 1: Aktuator
  String aktuatorPayload = "{";
  aktuatorPayload += "\"Heater\":\"" + StrHeater + "\",";
  aktuatorPayload += "\"Aerator\":\"" + StrAerator + "\",";
  aktuatorPayload += "\"Valve\":\"" + StrSirkulasi + "\"";
  aktuatorPayload += "}";  // Pastikan tidak ada koma setelah elemen terakhir
  sendDataToFirebase("/Aktuator", aktuatorPayload);

  // Payload 2: Read Sensor
  String readSensorPayload = "{";
  readSensorPayload += "\"TempOut\":" + String(TempOut) + ",";
  readSensorPayload += "\"TempIn\":" + String(TempIn) + ",";
  readSensorPayload += "\"Turbidity\":" + String(turb) + ",";
  readSensorPayload += "\"pH\":" + String(pH) + ",";
  readSensorPayload += "\"DO\":" + String(DO_R);
  readSensorPayload += "}";  // Pastikan tidak ada koma setelah elemen terakhir
  sendDataToFirebase("/ReadSensor", readSensorPayload);
}

// Fungsi pengiriman data ke Spreadsheet
void sendDataToSpreadsheetTask() {
  sendDataToSpreadsheet();
}
