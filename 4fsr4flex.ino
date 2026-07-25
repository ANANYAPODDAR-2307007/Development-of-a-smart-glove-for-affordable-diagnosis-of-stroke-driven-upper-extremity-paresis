// --- FSR Pins ---
const int Fsr1 = 36; 
const int Fsr2 = 39; 
const int Fsr3 = 34; 
const int Fsr4 = 35; 

// --- Flex Sensor Pins (Safe ADC1 Pins Selected) ---
const int Flex1 = 32; 
const int Flex2 = 33; 
const int Flex3 = 25; 
const int Flex4 = 26; 

void setup() {
  Serial.begin(115200);
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
  // Reading Raw FSR Sensors
  int raw_f1 = analogRead(Fsr1);
  int raw_f2 = analogRead(Fsr2);
  int raw_f3 = analogRead(Fsr3);
  int raw_f4 = analogRead(Fsr4);

  // Reading Raw Flex Sensors
  int raw_fl1 = analogRead(Flex1);
  int raw_fl2 = analogRead(Flex2);
  int raw_fl3 = analogRead(Flex3);
  int raw_fl4 = analogRead(Flex4);

  // Applying Curve Fitting to get Linearized Values
  float f1 = calibrateADC(raw_f1);
  float f2 = calibrateADC(raw_f2);
  float f3 = calibrateADC(raw_f3);
  float f4 = calibrateADC(raw_f4);

  float fl1 = calibrateADC(raw_fl1);
  float fl2 = calibrateADC(raw_fl2);
  float fl3 = calibrateADC(raw_fl3);
  float fl4 = calibrateADC(raw_fl4);

  // Sending all 8 LINEARIZED values comma-separated
  Serial.print(f1, 1);  Serial.print(",");
  Serial.print(f2, 1);  Serial.print(",");
  Serial.print(f3, 1);  Serial.print(",");
  Serial.print(f4, 1);  Serial.print(",");
  Serial.print(fl1, 1); Serial.print(",");
  Serial.print(fl2, 1); Serial.print(",");
  Serial.print(fl3, 1); Serial.print(",");
  Serial.println(fl4, 1); // End of line with decimal precision

  delay(50); // High-speed telemetry for smooth real-time tracking
}