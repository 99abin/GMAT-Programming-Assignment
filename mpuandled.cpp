#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;
const int depan = 26, belakang = 14, kanan = 4, kiri = 12, buzz = 7;

void setup() {
  Serial.begin(115200);
  Wire.begin(22, 21);  
  mpu.initialize();
  pinMode(depan, OUTPUT);
  pinMode(belakang, OUTPUT);
  pinMode(kanan, OUTPUT);
  pinMode(kiri, OUTPUT);
  pinMode(buzz, OUTPUT);
  
  if (mpu.testConnection()) {
    Serial.println("MPU6050 terhubung");
  } else {
    Serial.println("MPU6050 tidak terhubung");
  }
}

void loop() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  float ax_g = ax / 16384.0;
  float ay_g = ay / 16384.0;
  float az_g = az / 16384.0;


  float pitch = atan2(ay_g, az_g) * 180 / PI; 
  float roll = atan2(-ax_g, sqrt(ay_g * ay_g + az_g * az_g)) * 180 / PI;  

  static float yaw = 0;
  float gz_dps = gz / 131.0; 
  yaw += gz_dps * 0.01;  

  Serial.println("========================");
  Serial.print("Pitch : ");
  Serial.print(pitch);
  Serial.println(" degree");

  Serial.print("Roll  : ");
  Serial.print(roll);
  Serial.println(" degree");

  Serial.print("Yaw   : ");
  Serial.print(yaw);
  Serial.println(" degree");
  Serial.println("========================");

  if (roll>0) {
    digitalWrite(kanan, HIGH);
    digitalWrite(kiri, LOW);
    Serial.print("10");
  }
  else if (roll<0) {
    digitalWrite(kanan, LOW);
    digitalWrite(kiri, HIGH);
    Serial.print("01");
  }

  if (pitch>0) {
    digitalWrite(belakang, HIGH);
    digitalWrite(depan, LOW);
    Serial.print("10");
  }
  else if (pitch<0) {
    digitalWrite(belakang, LOW);
    digitalWrite(depan, HIGH);
    Serial.print("01");
  }

  if (roll == 0 && pitch == 0) {
    digitalWrite(depan, LOW);
    digitalWrite(belakang, LOW);
    digitalWrite(kanan, LOW);
    digitalWrite(kiri, LOW);
    Serial.print("0000");
  }

  if (yaw>0) {
    tone(25,262, 500);
  }
  else if (yaw<0) {
    tone(25, 125, 500);
  }

  delay(1000);
}