#include <Wire.h>

const int MPU_ADDR = 0x68;
const int LDR_PIN = A0;

// Red LEDs – earthquake warning
const int redLEDs[] = {10, 12, 9, 8, 7};

// Yellow LEDs – automatic night‑light
const int yellowLEDs[] = {A1, 2, 11, 6, 5, 4, 3};

// Vibration parameters (tune if needed)
const float GRAV_ALPHA = 0.008;
const float VIBE_THRESHOLD = 0.12;
const unsigned long HOLD_TIME = 600;   // red LEDs stay on 600ms after last shake

// Night‑light threshold (LDR reading below this = dark)
const int DARK_THRESHOLD = 400;       // adjust based on your ambient light

float grav_ax = 0, grav_ay = 0, grav_az = 1.0;
unsigned long lastShakeTime = 0;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  
  // Wake MPU6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);
  delay(100);
  
  // Set LED pins as outputs
  for (int i = 0; i < 5; i++) pinMode(redLEDs[i], OUTPUT);
  for (int i = 0; i < 7; i++) pinMode(yellowLEDs[i], OUTPUT);
  
  // Ensure all LEDs are off initially
  for (int i = 0; i < 5; i++) digitalWrite(redLEDs[i], LOW);
  for (int i = 0; i < 7; i++) digitalWrite(yellowLEDs[i], LOW);
  
  Serial.println("System ready (Earthquake + Night‑Light).");
}

void loop() {
  // ---------- EARTHQUAKE DETECTION (MPU6050) ----------
  int16_t ax, ay, az;
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6);
  
  if (Wire.available() == 6) {
    ax = Wire.read() << 8 | Wire.read();
    ay = Wire.read() << 8 | Wire.read();
    az = Wire.read() << 8 | Wire.read();
    
    float ax_g = ax / 16384.0;
    float ay_g = ay / 16384.0;
    float az_g = az / 16384.0;
    
    // Slow gravity estimate (low‑pass filter)
    grav_ax = grav_ax * (1.0 - GRAV_ALPHA) + ax_g * GRAV_ALPHA;
    grav_ay = grav_ay * (1.0 - GRAV_ALPHA) + ay_g * GRAV_ALPHA;
    grav_az = grav_az * (1.0 - GRAV_ALPHA) + az_g * GRAV_ALPHA;
    
    // Dynamic acceleration (shake component)
    float dyn_ax = ax_g - grav_ax;
    float dyn_ay = ay_g - grav_ay;
    float dyn_az = az_g - grav_az;
    float dyn_mag = sqrt(dyn_ax*dyn_ax + dyn_ay*dyn_ay + dyn_az*dyn_az);
    
    if (dyn_mag > VIBE_THRESHOLD) {
      lastShakeTime = millis();
    }
  }
  
  // Turn on red LEDs if a shake was detected recently
  bool earthquakeActive = (millis() - lastShakeTime < HOLD_TIME);
  for (int i = 0; i < 5; i++) {
    digitalWrite(redLEDs[i], earthquakeActive ? HIGH : LOW);
  }
  
  // ---------- NIGHT LIGHT (LDR) ----------
  int ldrValue = analogRead(LDR_PIN);
  bool dark = (ldrValue < DARK_THRESHOLD);
  for (int i = 0; i < 7; i++) {
    digitalWrite(yellowLEDs[i], dark ? HIGH : LOW);
  }
  
  // Debug output (can be commented out later)
  Serial.print("Shake: "); Serial.print(earthquakeActive ? "YES" : "NO");
  Serial.print("  LDR: "); Serial.print(ldrValue);
  Serial.print("  Dark: "); Serial.println(dark ? "YES" : "NO");
  
  delay(20);
}