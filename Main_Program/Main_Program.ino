/* Includes ============================================================= */
#include <LiquidCrystal_I2C.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <WiFi.h>
#include "SoftwareSerial.h"

/* Internet ACCESS ===================================================== */
#define WIFI_SSID "YOLO"
#define WIFI_PASSWORD "********"

// # SMARCOS
String GAS_ID = "AKfycbxv3grJ8_4gpOjfdsJVO_7iT0um3vEalIusHJb9DJySSviVv_SSkeMqETRHhZ5fLuw-7A";
// # SKRIPSI
String GAS_ID = "AKfycbwgkzXCWOftcTPrJ-hlIwDXjBtfvDdUxBqTvHoflACNFYc4kTurhAjS99lLb02aVak-"; //--> spreadsheet script ID
const char *host = "script.google.com";  // only google.com not https://google.com


/* Private define ====================================================== */
#define ONE_WIRE_BUS 33
#define UP_Sec (300)
// #define UP_Sec   (3)
#define UP_MSec (UP_Sec * 1000)


/* Tipe Data =========================================================== */
/* unsigned ---------------------------------------------------------------- */
const uint16_t DO_Table[41] = {
  14460, 14220, 13820, 13440, 13090, 12740, 12420, 12110, 11810, 11530,
  11260, 11010, 10770, 10530, 10300, 10080, 9860, 9660, 9460, 9270,
  9080, 8900, 8730, 8570, 8410, 8250, 8110, 7960, 7820, 7690,
  7560, 7430, 7300, 7180, 7070, 6950, 6840, 6730, 6630, 6530, 6410
};

/* Float --------------------------------------------------------------- */
float mean_pH;                    // Rata-rata pH
float TempOut;                    // Temperature Input(air di kolam)
float TempIn;                     // Temperature Input(air di filter)
float DOH = 5;                    // DO yang diharapkan
float DO_R;                       // DO yang terbaca
float pH;                         // pH yang terbaca
float KO;                         // Kosentrasi Oksigen
const float DESIRED_TEMP = 35.0;  // Desired temperature

/* Integer -------------------------------------------------------------- */
uint8_t turb;  // turbidity yang terbaca
uint8_t rule;

/* Inisialisasi object ================================================= */
/* LCD I2C ------------------------------------------------------------- */
LiquidCrystal_I2C lcd(0x27, 20, 4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
SoftwareSerial Rsensor(18, 19);  // RX, TX

/* RELAY POSITION ===================================================== */
/* AERATOR ------------------------------------------------------------ */
uint8_t Aerator0 = 27;
uint8_t Aerator1 = 27;
uint8_t Aerator2 = 14;
uint8_t Aerator3 = 26;
float aerat;
/* Sirkulasi ---------------------------------------------------------- */
uint8_t POMPA = 5;
uint8_t VALVE = 17;
float sirku;
/* Heataer ------------------------------------------------------------ */
uint8_t Heater = 25;
float heat;
/* String ------------------------------------------------------------ */
String Date;
String Time;
String StrSirkulasi;
String StrAerator;
String StrHeater;

/* FUZZY DAN SET NILAI TIAP VARIABLE ================================= */
/* INPUT ------------------------------------------------------------- */
#include <Fuzzy.h>
Fuzzy *fuzzy = new Fuzzy();
// FuzzyInput pH
FuzzySet *AC          = new FuzzySet(4, 4, 6, 7);
FuzzySet *NT          = new FuzzySet(6.5, 7.5, 7.5, 8.5);
FuzzySet *AL          = new FuzzySet(8, 9, 11, 11);
// FuzzyInput Temperature
FuzzySet *CD          = new FuzzySet(0, 0, 21, 25);
FuzzySet *NM          = new FuzzySet(25, 28, 28, 30);
FuzzySet *HO          = new FuzzySet(29, 33, 35, 35);
// FuzzyInput Turbidity
FuzzySet *LT          = new FuzzySet(0, 0, 10, 15);
FuzzySet *MT          = new FuzzySet(10, 15, 21, 26);
FuzzySet *HT          = new FuzzySet(25, 30, 100, 100);
// FuzzyInput Error DO
FuzzySet *ZD          = new FuzzySet(0, 0, 0, 25);
FuzzySet *LD          = new FuzzySet(0, 30, 30, 60);
FuzzySet *MD          = new FuzzySet(40, 70, 70, 100);
FuzzySet *HD          = new FuzzySet(75, 100, 100, 100);
// FuzzyInput Saturation
FuzzySet *LS          = new FuzzySet(0, 0, 10, 25);
FuzzySet *MS          = new FuzzySet(10, 20, 45, 55);
FuzzySet *HS          = new FuzzySet(45, 55, 100, 100);
/* OUTPUT ------------------------------------------------------------ */
// FuzzyOutput Valve
FuzzySet *OFF_V             = new FuzzySet(0, 0, 0.4, 0.6);
FuzzySet *ON_V             = new FuzzySet(0.4, 0.6, 1, 1);
// FuzzyOutput Aerator
FuzzySet  *AER_0          = new FuzzySet(0, 0, 0, 1);
FuzzySet  *AER_1          = new FuzzySet(0, 1, 1, 2);
FuzzySet  *AER_2          = new FuzzySet(1, 2, 2, 3);
FuzzySet  *AER_3          = new FuzzySet(2, 3, 3, 3);
// FuzzyOutput Heater
FuzzySet *OFF_H           = new FuzzySet(0, 0, 0.4, 0.6);
FuzzySet *ON_H            = new FuzzySet(0.4, 0.6, 1, 1);

/* PANGGIL TAB LAIN ------------------------------------------------- */
#include "Fuzzy_Set.h"
#include "Fuzzy_Rule.h"
#include "Send_Data.h"
#include "Read_sensor.h"


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
  wificonnect();


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

void wificonnect(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void printActiveRule(int ruleIndex) {
  rule = ruleIndex;
  Serial.print("Active Rule: ");
  Serial.println(rule);
}

void loop() {

  // RXTX();
  readpH();
  readDO();
  readErrorDO();

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
  heat = fuzzy->defuzzify(3);

  // Sirkulasi
  if (sirku >= 0.5) {
    digitalWrite(POMPA, HIGH);
    digitalWrite(VALVE, HIGH);
    StrSirkulasi = "ON";
  } else {
    digitalWrite(POMPA, HIGH);
    digitalWrite(VALVE, LOW);
    StrSirkulasi = "OFF";
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

  // Periksa apakah perlu mengirim data ke Firebase
  updateIfNeeded(lastUpdateFirebase, INTERVAL_FIREBASE, sendDataToFirebaseTask);

  // Periksa apakah perlu mengirim data ke Spreadsheet
  updateIfNeeded(lastUpdateSpreadsheet, INTERVAL_SPREADSHEET, sendDataToSpreadsheetTask);

  Serial.print("Time : ");
  Serial.println(Time);  // Tampilkan waktu (hh:mm:ss)

  // Print the active rule
  for (int i = 1; i <= 33; i++) {
    if (fuzzy->isFiredRule(i)) {
      printActiveRule(i);
    }
  }

  Serial.print("TempOut: ");Serial.print(TempOut);  Serial.println(" *C");
  Serial.print("TempIn: "); Serial.print(TempIn);   Serial.println(" *C");
  Serial.print("Turb: ");   Serial.print(turb);     Serial.println(" NTU");
  Serial.print("pH :");     Serial.print(pH);       Serial.println(" pH");
  Serial.print("DO_R :");   Serial.print(DO_R);     Serial.println(" mg/l");
  Serial.print("Sat :");    Serial.print(sat);      Serial.println(" %");

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

  lcd.setCursor(0, 0);  // Atur kursor ke baris 1, kolom 1
  lcd.print("Date: ");
  lcd.print(Date);      // Tampilkan waktu di LCD
  lcd.setCursor(0, 1);  // Atur kursor ke baris 1, kolom 1
  lcd.print("Time: ");
  lcd.print(Time);  // Tampilkan waktu di LCD

  // Desplay LCD I2C Data Sensor
  lcd.setCursor(0, 2);
  lcd.print("O/I:");
  lcd.print((int)TempOut);
  lcd.print("/");
  lcd.print((int)TempIn);
  lcd.print("  ");
  lcd.setCursor(10, 2);
  lcd.print(" TU/ : ");
  lcd.print(turb);
  lcd.print("%");
  lcd.setCursor(0, 3);
  lcd.print("pH :");
  lcd.print((float)pH);
  lcd.setCursor(8, 3);
  lcd.print(" DO : ");
  lcd.print((int)DO_R);
  lcd.print("mgl");
  delay(1000);
}