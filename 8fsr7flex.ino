// ====================================================================
// NEXUS ANANYA V15: 15-CHANNEL BIOMECHANICAL MASTER CONTROL (LINEARIZED)
// ====================================================================

// --- 8 FSR Pressure Sensors ---
const int Fsr1 = 36; // ADC1
const int Fsr2 = 39; // ADC1
const int Fsr3 = 34; // ADC1
const int Fsr4 = 35; // ADC1
const int Fsr5 = 16; // ADC2
const int Fsr6 = 17; // ADC2
const int Fsr7 = 5;  // ADC2
const int Fsr8 = 18; // ADC2

// --- 7 Flex Bend Sensors ---
const int Flex1 = 32; // ADC1
const int Flex2 = 33; // ADC1
const int Flex3 = 25; // ADC1
const int Flex4 = 26; // ADC1
const int Flex5 = 21; // ADC2
const int Flex6 = 22; // ADC2 (Upload ke waqt unplug karein)
const int Flex7 = 23; // ADC2 (Upload ke waqt unplug karein)

void setup() {
  Serial.begin(115200);
  
  // Explicit Input Declaration for Noise Filtering
  pinMode(Fsr1, INPUT); pinMode(Fsr2, INPUT); pinMode(Fsr3, INPUT); pinMode(Fsr4, INPUT);
  pinMode(Fsr5, INPUT); pinMode(Fsr6, INPUT); pinMode(Fsr7, INPUT); pinMode(Fsr8, INPUT);
  
  pinMode(Flex1, INPUT); pinMode(Flex2, INPUT); pinMode(Flex3, INPUT); pinMode(Flex4, INPUT);
  pinMode(Flex5, INPUT); pinMode(Flex6, INPUT); pinMode(Flex7, INPUT);
}

// 4th-Degree Polynomial Curve Fitting Function to Linearize ESP32 ADC
// (Updated with your EXACT 33-point Laboratory Readings)
float calibrateADC(int raw) {
  // Dead zone fixing based on your lowest reading (Reading #1 was 6)
  if (raw < 6) return 0.0;
  // Saturation zone fixing based on max scale
  if (raw > 4090) return 4095.0;

  // Exact 4th-degree polynomial coefficients derived from your 33-point data
  // Equation: Corrected = a*x^4 + b*x^3 + c*x^2 + d*x + e
  double x = (double)raw;
  
  double a = -1.187e-11;
  double b =  1.172e-7;
  double c = -4.184e-4;
  double d =  1.385;
  double e =  -5.42;

  double corrected = (a * x * x * x * x) + (b * x * x * x) + (c * x * x) + (d * x) + e;

  // Constraints boundary check
  if (corrected < 0) corrected = 0;
  if (corrected > 4095) corrected = 4095;

  return (float)corrected;
}

void loop() {
  // Reading Raw 8 FSRs
  int raw_f1 = analogRead(Fsr1);  int raw_f2 = analogRead(Fsr2);
  int raw_f3 = analogRead(Fsr3);  int raw_f4 = analogRead(Fsr4);
  int raw_f5 = analogRead(Fsr5);  int raw_f6 = analogRead(Fsr6);
  int raw_f7 = analogRead(Fsr7);  int raw_f8 = analogRead(Fsr8);

  // Reading Raw 7 Flex Sensors
  int raw_fl1 = analogRead(Flex1); int raw_fl2 = analogRead(Flex2);
  int raw_fl3 = analogRead(Flex3); int raw_fl4 = analogRead(Flex4);
  int raw_fl5 = analogRead(Flex5); int raw_fl6 = analogRead(Flex6); int raw_fl7 = analogRead(Flex7);

  // Applying Curve Fitting to get Linearized Values for all 15 Channels
  float f1 = calibrateADC(raw_f1);  float f2 = calibrateADC(raw_f2);
  float f3 = calibrateADC(raw_f3);  float f4 = calibrateADC(raw_f4);
  float f5 = calibrateADC(raw_f5);  float f6 = calibrateADC(raw_f6);
  float f7 = calibrateADC(raw_f7);  float f8 = calibrateADC(raw_f8);

  float fl1 = calibrateADC(raw_fl1); float fl2 = calibrateADC(raw_fl2);
  float fl3 = calibrateADC(raw_fl3); float fl4 = calibrateADC(raw_fl4);
  float fl5 = calibrateADC(raw_fl5); float fl6 = calibrateADC(raw_fl6); float fl7 = calibrateADC(raw_fl7);

  // --- STREAMING LINEARIZED 15-DATA POINTS MATRIX (with decimal precision) ---
  Serial.print(f1, 1);  Serial.print(","); Serial.print(f2, 1);  Serial.print(",");
  Serial.print(f3, 1);  Serial.print(","); Serial.print(f4, 1);  Serial.print(",");
  Serial.print(f5, 1);  Serial.print(","); Serial.print(f6, 1);  Serial.print(",");
  Serial.print(f7, 1);  Serial.print(","); Serial.print(f8, 1);  Serial.print(",");
  
  Serial.print(fl1, 1); Serial.print(","); Serial.print(fl2, 1); Serial.print(",");
  Serial.print(fl3, 1); Serial.print(","); Serial.print(fl4, 1); Serial.print(",");
  Serial.print(fl5, 1); Serial.print(","); Serial.print(fl6, 1); Serial.print(",");
  Serial.println(fl7, 1); // Packet Terminator Line

  delay(40); // High-speed telemetry for smooth real-time tracking
}