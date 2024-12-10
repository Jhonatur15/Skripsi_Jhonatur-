#include "SoftwareSerial.h"

// Deklarasi eksternal Rsensor (definisi ada di file utama)
extern SoftwareSerial Rsensor;

/* define ------------------------------------------------------------ */
#define PH_SENSOR_PIN 32
#define DO_PIN 35
#define VREF 3300                // Tegangan referensi (mV) - Sesuaikan dengan tegangan referensi ESP32
#define ADC_RES 4096             // Resolusi ADC ESP32
#define TWO_POINT_CALIBRATION 0  // Mode kalibrasi satu titik=0, Mode kalibrasi dua titik=1
#define CAL1_V (1662)            // Tegangan pada titik kalibrasi 1 (mv)
#define CAL1_T (25)              // Suhu pada titik kalibrasi 1 (℃)
#define CAL2_V (1300)            // Tegangan pada titik kalibrasi 2 (mv)
#define CAL2_T (15)              // Suhu pada titik kalibrasi 2 (℃)

/* String ------------------------------------------------------------ */
String sensorDataStr;
String temp1Str;
String temp2Str;
String timeStr;
String turbStr;

/* integer ------------------------------------------------------------ */
uint8_t sat;   // Saturasion yang terbaca
uint8_t e_do;  // Error DO

/* uint ---------------------------------------------------------------- */
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


// void RXTX() {
//   if (Rsensor.available()) {
//     String data = Rsensor.readStringUntil('\n');  // Baca data hingga newline
//     data.trim();                                  // Hapus spasi di awal dan akhir

//     uint8_t firstIndex = data.indexOf(',');                   // Cari posisi koma pertama
//     uint8_t secondIndex = data.indexOf(',', firstIndex + 1);  // Cari posisi koma kedua

//     if (firstIndex > 0 && secondIndex > firstIndex) {
//       timeStr = data.substring(0, firstIndex);         // Bagian waktu sebelum koma pertama
//       sensorDataStr = data.substring(firstIndex + 1);  // Data sensor setelah koma pertama
//       Date = timeStr.substring(0, 8);
//       Time = timeStr.substring(9, 17);

//       // Pisahkan dan tampilkan data sensor
//       uint8_t temp1Index = sensorDataStr.indexOf(',');
//       uint8_t temp2Index = sensorDataStr.indexOf(',', temp1Index + 1);

//       if (temp1Index > 0 && temp2Index > temp1Index) {
//         temp1Str = sensorDataStr.substring(0, temp1Index);               // Ambil suhu pertama
//         temp2Str = sensorDataStr.substring(temp1Index + 1, temp2Index);  // Ambil suhu kedua
//         turbStr = sensorDataStr.substring(temp2Index + 1);               // Ambil nilai turbidity
//         // Konversi bentuk data
//         TempIn = temp1Str.toFloat();
//         TempOut = temp2Str.toFloat();
//         turb = turbStr.toInt();
//         if (turb > 100 || turb == 25) {
//           turb = 0;
//         }
//       }
//     }
//   }
// }

void readpH() {
  /* pH --------------------------------------------------------------- */
  uint8_t adc_V = 0;          // Variabel untuk menyimpan nilai rata-rata ADC
  uint8_t adc_sum = 0;        // Variabel untuk menyimpan total nilai ADC
  uint8_t numReadings = 500;  // Jumlah pembacaan yang akan dilakukan

  // Loop untuk membaca nilai ADC beberapa kali
  for (int i = 0; i < numReadings; i++) {
    adc_sum += analogRead(PH_SENSOR_PIN);  // Akumulasi nilai pembacaan ADC
    delay(5);                              // Tunggu sejenak antara pembacaan
  }
  adc_V = adc_sum / numReadings;       // Hitung rata-rata nilai ADC
  // pH = (adc_V * (-0.002847)) + 11.33;  // Konversi Fiks

  pH = random(0, 14);
  // pH = (adc_V * (-0.004677)) + 15.98;  // Konversi rata-rata nilai ADC menjadi nilai pH
}

void readDO() {
  /* DO --------------------------------------------------------------- */
  // Temperaturet = (uint8_t)TempOut;
  // ADC_Raw = analogRead(DO_PIN);
  // ADC_Voltage = (uint32_t)VREF * ADC_Raw / ADC_RES;
  // for (int i = 1; i < 500; i++) {
  //   DO_R = readDO(ADC_Voltage, TempOut) / 250.0;  // Membagi hasil pembacaan DO dengan 1000 untuk mengkonversi ke ppm
  //   delay(5);
  // }
// Konversi bentuk data
  DO_R =random(0,10);
  TempIn = random(20, 30);
  TempOut = random(20,30);
  turb = random(0,20); 
}

void readErrorDO() {
  readDO();

  KO = 14.59 - 0.33935 * TempOut + (0.0072 * pow(TempOut, 2)) - (0.0000619 * pow(TempOut, 3));
  uint8_t sat = (DO_R / KO) * 100;
  sat = (sat > 100) ? 100 : sat;
  int e_do = ((DO_R - DOH) / DOH) * 100;
  // int e_do = -((DO_R - DOH)/DOH) * 100;
  e_do = max(e_do, 0);
}