#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <Wire.h>
#include <RTClib.h>

#define Temp1 A2
#define Temp2 A1
#define Turb A0

float TempIn;
float TempOut;

OneWire oneTemp1(Temp1);
OneWire oneTemp2(Temp2);

DallasTemperature STemp1(&oneTemp1);
DallasTemperature STemp2(&oneTemp2);

SoftwareSerial Rsensor(2, 3); // RX, TX

RTC_DS3231 rtc;

void setup() {
  Serial.begin(9600);
  Rsensor.begin(9600);
  STemp1.begin();
  STemp2.begin();
  
  // Inisialisasi RTC
  if (!rtc.begin()) {
    Serial.println("RTC tidak ditemukan!");
    while (1);
  }

  // Jika RTC kehilangan daya, setel ulang waktu
  if (rtc.lostPower()) {
    Serial.println("RTC kehilangan daya, menyetel ulang waktu!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  pinMode(Temp1, INPUT);
  pinMode(Temp2, INPUT);
  pinMode(Turb, INPUT);
}

void datetime(Stream &output) {
  // Display RTC date and time
  DateTime now = rtc.now();
  output.print(now.year(), DEC);
  output.print('/');
  output.print(now.month(), DEC);
  output.print('/');
  output.print(now.day(), DEC);
}

void clocktime(Stream &output) {
  // Display RTC time only
  DateTime now = rtc.now();
  output.print(now.hour(), DEC);
  output.print(':');
  output.print(now.minute(), DEC);
  output.print(':');
  output.print(now.second(), DEC);
}

void loop() {
  // Baca suhu dari sensor DS18B20
  STemp1.requestTemperatures();
  TempIn = STemp1.getTempCByIndex(0);
  
  STemp2.requestTemperatures();
  TempOut = STemp2.getTempCByIndex(0);

  // Baca nilai turbidity
  int sensorValue = analogRead(Turb);
  int turb = map(sensorValue, 0, 640, 100, 0);
  // Jika nilai turbidity negatif, setel ke 0
  if (turb < 0) {
    turb = 0;
  }

  // Tampilkan waktu dan data sensor ke Serial Monitor
  Serial.print("Date: ");
  datetime(Serial);
  Serial.println();
  Serial.print("Clock: ");
  clocktime(Serial);
  Serial.println();

  Serial.print("TempIn   : ");
  Serial.println(TempIn);
  Serial.print("TempOut  : ");
  Serial.println(TempOut);
  Serial.print("Turb     : ");
  Serial.println(turb);
  Serial.println();

  // Kirim data melalui SoftwareSerial ke ESP32
  datetime(Rsensor);
  Rsensor.print(" ");
  clocktime(Rsensor);
  Rsensor.print(", ");
  Rsensor.print(TempIn);
  Rsensor.print(",");
  Rsensor.print(TempOut);
  Rsensor.print(",");
  Rsensor.println(turb);

  delay(1000); // Tunggu 1 detik sebelum pembacaan berikutnya
}

