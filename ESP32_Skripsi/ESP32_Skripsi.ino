/* Includes ============================================================= */
#include <Firebase_ESP_Client.h>
#include <LiquidCrystal_I2C.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <WiFi.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include "SoftwareSerial.h"


/* Internet ACCESS ===================================================== */
#define WIFI_SSID "BBI_Mijen"
#define WIFI_PASSWORD "bbimijen"
// #define WIFI_SSID "YOLO"
// #define WIFI_PASSWORD "********"
// #define WIFI_SSID "Kontrakan_IoT"
// #define WIFI_PASSWORD "jhoniot1945"

#define API_KEY "AIzaSyBCFik5bSRN6HuZcdP42tkj2qQlUxmuNUw"
#define DATABASE_URL "https://smarcos2-9de35-default-rtdb.firebaseio.com/"
// // # SKRIPSI_1
// String GAS_ID = "AKfycbxv3grJ8_4gpOjfdsJVO_7iT0um3vEalIusHJb9DJySSviVv_SSkeMqETRHhZ5fLuw-7A"; //--> spreadsheet script ID
// # SKRIPSI_2
String GAS_ID = "AKfycbwgkzXCWOftcTPrJ-hlIwDXjBtfvDdUxBqTvHoflACNFYc4kTurhAjS99lLb02aVak-"; //--> spreadsheet script ID
const char* host = "script.google.com"; // only google.com not https://google.com


/* Private define ====================================================== */
#define ONE_WIRE_BUS 33
#define PH_SENSOR_PIN 32
#define DO_PIN 35
#define VREF 3300      // Tegangan referensi (mV) - Sesuaikan dengan tegangan referensi ESP32
#define ADC_RES 4096   // Resolusi ADC ESP32
#define TWO_POINT_CALIBRATION 0 // Mode kalibrasi satu titik=0, Mode kalibrasi dua titik=1
#define CAL1_V (1662) // Tegangan pada titik kalibrasi 1 (mv)
#define CAL1_T (25)   // Suhu pada titik kalibrasi 1 (℃)
#define CAL2_V (1300) // Tegangan pada titik kalibrasi 2 (mv)
#define CAL2_T (15)   // Suhu pada titik kalibrasi 2 (℃)
#define UP_Sec   (300)
// #define UP_Sec   (3)
#define UP_MSec    ( UP_Sec * 1000 )


/* Tipe Data =========================================================== */
/* unsigned ---------------------------------------------------------------- */
unsigned long time_ms;
unsigned long time_1000_ms_buf;
unsigned long time_sheet_update_buf;
unsigned long time_dif;
/* uint ---------------------------------------------------------------- */
const uint16_t DO_Table[41] = {
    14460, 14220, 13820, 13440, 13090, 12740, 12420, 12110, 11810, 11530,
    11260, 11010, 10770, 10530, 10300, 10080, 9860, 9660, 9460, 9270,
    9080, 8900, 8730, 8570, 8410, 8250, 8110, 7960, 7820, 7690,
    7560, 7430, 7300, 7180, 7070, 6950, 6840, 6730, 6630, 6530, 6410};
uint16_t ADC_Voltage;
uint16_t ADC_Raw;

int16_t readDO(uint32_t volt_mv, uint8_t Temp_c) {
#if TWO_POINT_CALIBRATION == 0
  uint16_t V_saturation = (uint32_t)CAL1_V + (uint32_t)35 * Temp_c - (uint32_t)CAL1_T * 35;
  return (volt_mv * DO_Table[Temp_c] / V_saturation);
#else
  uint16_t V_saturation = (int16_t)((int8_t)Temp_c - CAL2_T) * ((uint16_t)CAL1_V - CAL2_V) / ((uint8_t)CAL1_T - CAL2_T) + CAL2_V;
  return (volt_mv * DO_Table[Temp_c] / V_saturation);
#endif
}

/* Float --------------------------------------------------------------- */
float mean_pH;          // Rata-rata pH
float TempOut;          // Temperature Input(air di kolam)
float TempIn;           // Temperature Input(air di filter)
float DOH = 5;          // DO yang diharapkan
float DO_R;             // DO yang terbaca
float pH;               // pH yang terbaca
float KO;               // Kosentrasi Oksigen
const float DESIRED_TEMP = 35.0; // Desired temperature

/* Integer -------------------------------------------------------------- */
int turb;               // turbidity yang terbaca
int sat;                // Saturasion yang terbaca
int e_do;               // Error DO
int rule;

/* Inisialisasi object ================================================= */
/* LCD I2C ------------------------------------------------------------- */
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
SoftwareSerial Rsensor(18, 19); // RX, TX
/* Firebase Autotification --------------------------------------------- */
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;
bool signupOK = false;


/* RELAY POSITION ===================================================== */
/* AERATOR ------------------------------------------------------------ */
int Aerator0 = 27;
int Aerator1 = 27;
int Aerator2 = 14;
int Aerator3 = 26;
float aerat;
/* Sirkulasi ---------------------------------------------------------- */
int POMPA = 5;
int VALVE = 17;
float sirku;
/* Heataer ------------------------------------------------------------ */
int Heater = 25;
float heat;
/* String ------------------------------------------------------------ */
String sensorDataStr;
String temp1Str;
String temp2Str;
String timeStr;
String turbStr;
String Date;
String Time;
String StrSirkulasi;
String StrAerator;
String StrHeater;


/* FUZZY DAN SET NILAI TIAP VARIABLE ================================= */
/* INPUT ------------------------------------------------------------- */
#include <Fuzzy.h>
Fuzzy *fuzzy              = new Fuzzy();
// FuzzyInput pH
FuzzySet *AC              = new FuzzySet(4, 4, 6, 7);
FuzzySet *NT              = new FuzzySet(6.5, 7.5, 7.5, 8.5);
FuzzySet *AL              = new FuzzySet(8, 9, 11, 11);
// FuzzyInput Temperature
FuzzySet *CD              = new FuzzySet(0, 0, 21, 25);
FuzzySet *NM              = new FuzzySet(24, 28, 28, 30);
FuzzySet *HO              = new FuzzySet(29, 33, 35, 35);
// FuzzyInput Turbidity
FuzzySet *LT              = new FuzzySet(0, 0, 10, 25);
FuzzySet *MT              = new FuzzySet(25, 40, 50, 65);
FuzzySet *HT              = new FuzzySet(65, 80, 100, 100);
// FuzzyInput Error DO
FuzzySet *ZD              = new FuzzySet(0, 0, 0, 25);
FuzzySet *LD              = new FuzzySet(0, 30, 30, 60);
FuzzySet *MD              = new FuzzySet(40, 70, 70, 100);
FuzzySet *HD              = new FuzzySet(75, 100, 100, 100);
// FuzzyInput Saturation
FuzzySet *LS              = new FuzzySet(0, 0, 10, 20);
FuzzySet *MS              = new FuzzySet(10, 20, 45, 55);
FuzzySet *HS              = new FuzzySet(45, 55, 100, 100);
/* OUTPUT ------------------------------------------------------------ */
// FuzzyOutput Sirkulasi
FuzzySet *POM             = new FuzzySet(0, 0, 0.4, 0.6);
FuzzySet *VAL             = new FuzzySet(0.4, 0.6, 1, 1);
// FuzzyOutput Aerator
FuzzySet  *AER_0          = new FuzzySet(0, 0, 0, 1);
FuzzySet  *AER_1          = new FuzzySet(0, 1, 1, 2);
FuzzySet  *AER_2          = new FuzzySet(1, 2, 2, 3);
FuzzySet  *AER_3          = new FuzzySet(2, 3, 3, 3);
// FuzzyOutput Heater
FuzzySet *OFF             = new FuzzySet(0, 0, 0.4, 0.6);
FuzzySet *ON              = new FuzzySet(0.4, 0.6, 1, 1);

/* PANGGIL TAB LAIN ------------------------------------------------- */
#include "Fuzzy_Set.h"
#include "Fuzzy_Rule.h"


void setup() {
  /* Setup ========================================================== */
  /* Begin Setup ---------------------------------------------------- */
  Serial.begin(115200);
  Rsensor.begin(9600);
  /* LCD I2C Setup -------------------------------------------------- */
  lcd.backlight();  // Activate backlight
  lcd.init();       // Init the LCD
  lcd.home();
  /* Connect WiFi ----------------------------------------------------- */
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
   while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);}
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  /* Config Firebase --------------------------------------------------- */
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("signUp OK");
    signupOK = true;} 
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());}
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println();
  
  /* Status Tranduser ================================================= */
  pinMode(POMPA, OUTPUT);
  pinMode(VALVE, OUTPUT);
  pinMode(Heater, OUTPUT);
  pinMode(Aerator1, OUTPUT);
  pinMode(Aerator2, OUTPUT);
  pinMode(Aerator3, OUTPUT);
  fuzzySet();
  fuzzyRule();
}

void printActiveRule(int ruleIndex) {
  rule = ruleIndex;
  Serial.print("Active Rule: ");
  Serial.println(rule);
}

void update_google_sheet()
{
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

    String url = "/macros/s/" + GAS_ID + 
    "/exec?tempOut=";url += String(TempOut);         url += "&tempIn=";url += String(TempIn);        url += "&turb=";url += String(turb);  
    url += "&pH=";url += String(pH);              url += "&DO=";url += String(DO_R);
    // Seand Data Tranduser------------------------------------------------- */
    url += "&heater=";url += String(StrHeater);      url += "&aerator=";url += String(StrAerator);  
    url += "&sirku=";url += String(StrSirkulasi);   url += "&rule=";url += activeRules;
    Serial.print("Requesting URL: ");
    Serial.println(url);
    http.begin("https://" + String(host) + url); // HTTPS!
    int httpCode = http.GET();   
    if(httpCode > 0) {
        String payload = http.getString();
    } else {
        Serial.println("Error on HTTP request");
    }
    http.end();
}


void loop() 
{
  if (Rsensor.available()) {
    String data = Rsensor.readStringUntil('\n'); // Baca data hingga newline
    data.trim(); // Hapus spasi di awal dan akhir

    int firstIndex = data.indexOf(','); // Cari posisi koma pertama
    int secondIndex = data.indexOf(',', firstIndex + 1); // Cari posisi koma kedua

    if (firstIndex > 0 && secondIndex > firstIndex) {
      timeStr = data.substring(0, firstIndex); // Bagian waktu sebelum koma pertama
      sensorDataStr = data.substring(firstIndex + 1); // Data sensor setelah koma pertama
      Date = timeStr.substring(0, 8);      
      Time = timeStr.substring(9, 17);

      // Pisahkan dan tampilkan data sensor
      int temp1Index = sensorDataStr.indexOf(',');
      int temp2Index = sensorDataStr.indexOf(',', temp1Index + 1);

      if (temp1Index > 0 && temp2Index > temp1Index) {
        temp1Str = sensorDataStr.substring(0, temp1Index); // Ambil suhu pertama
        temp2Str = sensorDataStr.substring(temp1Index + 1, temp2Index); // Ambil suhu kedua
        turbStr = sensorDataStr.substring(temp2Index + 1); // Ambil nilai turbidity
        // Konversi bentuk data
        TempIn = temp1Str.toFloat(); 
        TempOut = temp2Str.toFloat(); 
        turb = turbStr.toInt();
        if (turb > 100 || turb == 25) {
            turb = 0;
        }
      }
    }
  }  
  /* pH --------------------------------------------------------------- */
  int adc_V = 0; // Variabel untuk menyimpan nilai rata-rata ADC
  int adc_sum = 0; // Variabel untuk menyimpan total nilai ADC
  int numReadings = 500; // Jumlah pembacaan yang akan dilakukan

  // Loop untuk membaca nilai ADC beberapa kali
  for (int i = 0; i < numReadings; i++) {
    adc_sum += analogRead(PH_SENSOR_PIN); // Akumulasi nilai pembacaan ADC
    delay(5); // Tunggu sejenak antara pembacaan
  }
  adc_V = adc_sum / numReadings;    // Hitung rata-rata nilai ADC
  pH = (adc_V * (-0.002847)) + 11.33;  // Konversi Fiks
  // pH = (adc_V * (-0.004677)) + 15.98;  // Konversi rata-rata nilai ADC menjadi nilai pH
  
  /* DO --------------------------------------------------------------- */
  // Temperaturet = (uint8_t)TempOut;
  ADC_Raw = analogRead(DO_PIN);
  ADC_Voltage = (uint32_t)VREF * ADC_Raw / ADC_RES;
  for (int i=1; i<500; i++){
      DO_R = readDO(ADC_Voltage, TempOut)/250.0; // Membagi hasil pembacaan DO dengan 1000 untuk mengkonversi ke ppm
      delay(5);
  }

  // TempOut = 26;
  // turb = 0 ;
  // pH = 11;
  // DO_R = 4;

  KO = 14.59 - 0.33935 * TempOut + (0.0072 * pow(TempOut, 2)) - (0.0000619 * pow(TempOut, 3));
  int sat = (DO_R/KO) * 100;
  sat = (sat > 100) ? 100 : sat;
  int e_do = -((DO_R - DOH)/DOH) * 100;
  e_do = max(e_do, 0);
  
  Serial.println(" ");
  Serial.println(" ");
  fuzzy->setInput(1, pH);
  fuzzy->setInput(2, TempOut);
  fuzzy->setInput(3, turb);
  fuzzy->setInput(4, sat);
  fuzzy->setInput(5, e_do);


  fuzzy->fuzzify();
  sirku = fuzzy->defuzzify(1);
  aerat = fuzzy->defuzzify(2);
  heat  = fuzzy->defuzzify(3);

  // Sirkulasi
  if (sirku >= 0.5) {
    digitalWrite(POMPA, HIGH);
    digitalWrite(VALVE, HIGH);
    StrSirkulasi = "VALVE";
  } else {
    digitalWrite(POMPA, HIGH);
    digitalWrite(VALVE, LOW);
    StrSirkulasi = "POMPA";
  }

  // Heater
  if (heat >= 0.5) {
    digitalWrite(Heater, LOW);
    StrHeater = "ON";
  } else {
    digitalWrite(Heater, HIGH);
    StrHeater = "OFF";
  }

  // Aerator
  if (aerat <= 0.5) {
    digitalWrite(Aerator1, LOW);
    digitalWrite(Aerator2, HIGH);
    digitalWrite(Aerator3, HIGH);
    StrAerator = "Tidak Ada=Satu";
  } else if (aerat > 0.5 && aerat <= 1.5) {
    digitalWrite(Aerator1, LOW);
    digitalWrite(Aerator2, HIGH);
    digitalWrite(Aerator3, HIGH);
    StrAerator = "Satu";
  } else if (aerat > 1.5 && aerat <= 2.5) {
    digitalWrite(Aerator1, LOW);
    digitalWrite(Aerator2, LOW);
    digitalWrite(Aerator3, HIGH);
    StrAerator = "Dua";
  } else if (aerat > 2.5 && aerat <= 3.5) {
    digitalWrite(Aerator1, LOW);
    digitalWrite(Aerator2, LOW);
    digitalWrite(Aerator3, LOW);
    StrAerator = "Tiga";
  }
  
  time_ms = millis();
  time_dif = time_ms - time_1000_ms_buf;
  time_ms = millis();
  time_dif = time_ms - time_sheet_update_buf;    
  if ( time_dif >= UP_MSec ) // Specific period
  {time_sheet_update_buf = time_ms;
  update_google_sheet();}

  Serial.print("Time : ");
  Serial.println(Time); // Tampilkan waktu (hh:mm:ss)

  /* Send Sensor Read ====================================================== */
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    /* Temperature --------------------------------------------------------- */
    if (Firebase.RTDB.setFloat(&fbdo, "Read Sensor/*C", TempOut)) {Serial.print("Data TempOut: ");
      Serial.print(TempOut);  Serial.println(" *C");
      Serial.print("Data TempIn: ");
      Serial.print(TempIn);  Serial.println(" *C");
    } else {Serial.print("Error mengirim data TempOut sekitar ke Firebase: ");
      Serial.println("FAILED: " + fbdo.errorReason());}
    /* Turbidity ----------------------------------------------------------- */
    if (Firebase.RTDB.setFloat(&fbdo, "Read Sensor/NTU", turb)) {Serial.print("Data Turb: ");
      Serial.print(turb); Serial.println(" NTU");
    } else {Serial.print("Error mengirim data DO sekitar ke Firebase: ");
      Serial.println("FAILED: " + fbdo.errorReason());}
    /* pH ------------------------------------------------------------------ */
    if (Firebase.RTDB.setFloat(&fbdo, "Read Sensor/pH", pH)) {Serial.print("Data pH: "); 
      Serial.print(pH); Serial.println(" pH");
    } else {Serial.print("Error mengirim data pH sekitar ke Firebase: ");
      Serial.println("FAILED: " + fbdo.errorReason());}
    /* DO ------------------------------------------------------------------ */
    if (Firebase.RTDB.setFloat(&fbdo, "Read Sensor/mgl", DO_R)) {Serial.print("Data DO: ");
      Serial.print(DO_R); Serial.println(" mg/l");
    } else {Serial.print("Error mengirim data DO sekitar ke Firebase: ");
      Serial.println("FAILED: " + fbdo.errorReason());} 
    /* Saturation ---------------------------------------------------------- */
    if (Firebase.RTDB.setFloat(&fbdo, "DO Air/sat", sat)) {Serial.print("Data Sat: ");
      Serial.print(sat);  Serial.println(" %");
    } else {Serial.print("Error mengirim data DO sekitar ke Firebase: ");
      Serial.println("FAILED: " + fbdo.errorReason());}
    /* Error DO ------------------------------------------------------------ */
    if (Firebase.RTDB.setFloat(&fbdo, "DO Air/Err_DO", e_do)) {Serial.print("Data E_DO: ");
      Serial.print(e_do);  Serial.println(" %");} 
    else {Serial.print("Error mengirim data DO sekitar ke Firebase: ");
      Serial.println("FAILED: " + fbdo.errorReason());}
    /* Sirkulasi ----------------------------------------------------------- */
    if (Firebase.RTDB.setString(&fbdo, "Aktuator/Sirkulasi", StrSirkulasi)) {} 
    else {Serial.print("Error mengirim data DO sekitar ke Firebase: ");
      Serial.println("FAILED: " + fbdo.errorReason());}     
    /* Aerator ------------------------------------------------------------- */
    if (Firebase.RTDB.setString(&fbdo, "Aktuator/Aerator", StrAerator)) {}
    else {Serial.print("Error mengirim data DO sekitar ke Firebase: ");
      Serial.println("FAILED: " + fbdo.errorReason());}
    /* Heater -------------------------------------------------------------- */
    if (Firebase.RTDB.setString(&fbdo, "Aktuator/Heater", StrHeater)) {}
    else {      Serial.print("Error mengirim data DO sekitar ke Firebase: ");
      Serial.println("FAILED: " + fbdo.errorReason());}
  }
  // Print the active rule
  for (int i = 1; i <= 33; i++) {
    if (fuzzy->isFiredRule(i)) {
      printActiveRule(i);
    }
  }


  /* Display data on Serial --------------------------------------------------- */
  Serial.print("Sirku : ");
  Serial.print(sirku);
  Serial.print("  ||   Str : ");
  Serial.print(StrSirkulasi);
  Serial.println("");

  Serial.print("Aerat : ");
  Serial.print(aerat);
  Serial.print("  ||   Str : ");
  Serial.print(StrAerator);
  Serial.println("");

  Serial.print("Heat  : ");
  Serial.print(heat);
  Serial.print("  ||   Str : ");
  Serial.print(StrHeater);
  Serial.println("");
  Serial.println("");
  Serial.println("");
  
  lcd.setCursor(0, 0);      // Atur kursor ke baris 1, kolom 1
  lcd.print("Date: ");
  lcd.print(Date);          // Tampilkan waktu di LCD  
  lcd.setCursor(0, 1);      // Atur kursor ke baris 1, kolom 1
  lcd.print("Time: ");
  lcd.print(Time);          // Tampilkan waktu di LCD     

  // Desplay LCD I2C Data Sensor
  lcd.setCursor(0, 2);
  lcd.print("O/I:"); lcd.print((int)TempOut); lcd.print("/");lcd.print((int)TempIn); lcd.print("  ");
  lcd.setCursor(10, 2);
  lcd.print(" TU/ : "); lcd.print(turb); lcd.print("%");
  lcd.setCursor(0, 3);
  lcd.print("pH :"); lcd.print((float)pH);
  lcd.setCursor(8, 3);
  lcd.print(" DO : "); lcd.print((int)DO_R); lcd.print("mgl");
  delay(1000);
}


