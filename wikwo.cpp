/* 
  MAGANG GMRT - STUDI KASUS 3
  Nama : Fadhil Dzikri Supriyadi
  NIM  : 558754  */

#include <Wire.h>
#include <Servo.h>
#include <MPU6050.h>

MPU6050 mpu;

// Servo declaration
Servo servo1, servo2, servo3, servo4, servo5;

// Pin mapping
#define SERVO1_PIN 13
#define SERVO2_PIN 12
#define SERVO3_PIN 14
#define SERVO4_PIN 27
#define SERVO5_PIN 26
#define PIR_PIN    25

// Variables
int posInitial = 90; // initial (0° means servo center)
int pirState = LOW;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);
  servo4.attach(SERVO4_PIN);
  servo5.attach(SERVO5_PIN);

  pinMode(PIR_PIN, INPUT);

  // set all servos to initial (90°)
  resetServos();

  Serial.println("Sistem Siap!");
}

void loop() {
  pirState = digitalRead(PIR_PIN);

  if (pirState == HIGH) {
    Serial.println("Gerakan terdeteksi! Semua servo ke posisi acak...");
    randomMovement();
    delay(1000);
    resetServos();
  } else {
    float roll, pitch, yaw;
    bacaMPU(roll, pitch, yaw);

    // Roll kontrol servo 1 dan 2
    if (roll > 10) {
      servo1.write(60);
      servo2.write(60);
    } else if (roll < -10) {
      servo1.write(120);
      servo2.write(120);
    } else {
      servo1.write(posInitial);
      servo2.write(posInitial);
    }

    // Pitch kontrol servo 3 dan 4
    if (pitch > 10) {
      servo3.write(120);
      servo4.write(120);
    } else if (pitch < -10) {
      servo3.write(60);
      servo4.write(60);
    } else {
      servo3.write(posInitial);
      servo4.write(posInitial);
    }

    // Yaw kontrol servo 5
    if (abs(yaw) > 10) {
      servo5.write(yaw > 0 ? 120 : 60);
      delay(1000);
      servo5.write(posInitial);
    }
  }

  delay(100);
}

// ----------------------------
// Fungsi Tambahan
// ----------------------------
void bacaMPU(float &roll, float &pitch, float &yaw) {
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Konversi sederhana (bisa disesuaikan dengan kalibrasi)
  roll  = atan2(ay, az) * 180 / PI;
  pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180 / PI;
  yaw   = gz / 131.0;  // kira-kira dari data gyro
}

void randomMovement() {
  servo1.write(random(60, 120));
  servo2.write(random(60, 120));
  servo3.write(random(60, 120));
  servo4.write(random(60, 120));
  servo5.write(random(60, 120));
}

void resetServos() {
  servo1.write(posInitial);
  servo2.write(posInitial);
  servo3.write(posInitial);
  servo4.write(posInitial);
  servo5.write(posInitial);
}
