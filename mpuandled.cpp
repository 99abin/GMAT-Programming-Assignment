#include <Wire.h> // Include the Wire library for I2C communication
#include <MPU6050.h> // Include the MPU6050 library for the accelerometer and gyroscope

MPU6050 mpu; // Create an instance of the MPU6050 class
const int depan = 26, belakang = 14, kanan = 4, kiri = 12, buzz = 7; // Define pin numbers for motors and buzzer

void setup() {
  Serial.begin(115200); // Initialize serial communication at 115200 baud rate
  Wire.begin(22, 21); // Initialize I2C communication with specified SDA and SCL pins
  mpu.initialize(); // Initialize the MPU6050 sensor
  pinMode(depan, OUTPUT); // Set the front motor pin as output
  pinMode(belakang, OUTPUT); // Set the back motor pin as output
  pinMode(kanan, OUTPUT); // Set the right motor pin as output
  pinMode(kiri, OUTPUT); // Set the left motor pin as output
  pinMode(buzz, OUTPUT); // Set the buzzer pin as output
  
  // Test the connection to the MPU6050 sensor
  if (mpu.testConnection()) {
    Serial.println("MPU6050 terhubung"); // Print message if connected
  } else {
    Serial.println("MPU6050 tidak terhubung"); // Print message if not connected
  }
}

void loop() {
  int16_t ax, ay, az; // Variables to hold accelerometer data
  int16_t gx, gy, gz; // Variables to hold gyroscope data

  // Get motion data from the MPU6050
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Convert raw accelerometer data to g's
  float ax_g = ax / 16384.0;
  float ay_g = ay / 16384.0;
  float az_g = az / 16384.0;

  // Calculate pitch and roll angles in degrees
  float pitch = atan2(ay_g, az_g) * 180 / PI; 
  float roll = atan2(-ax_g, sqrt(ay_g * ay_g + az_g * az_g)) * 180 / PI;  

  static float yaw = 0; // Initialize yaw angle
  float gz_dps = gz / 131.0; // Convert gyroscope data to degrees per second
  yaw += gz_dps * 0.01; // Update yaw angle based on gyroscope data

  // Print pitch, roll, and yaw values to the serial monitor
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

  // Control motors based on roll angle
  if (roll > 0) {
    digitalWrite(kanan, HIGH); // Move right if roll is positive
    digitalWrite(kiri, LOW);
    Serial.print("10");
  }
  else if (roll < 0) {
    digitalWrite(kanan, LOW);
    digitalWrite(kiri, HIGH); // Move left if roll is negative
    Serial.print("01");
  }

  // Control motors based on pitch angle
  if (pitch > 0) {
    digitalWrite(belakang, HIGH); // Move backward if pitch is positive
    digitalWrite(depan, LOW);
    Serial.print("10");
  }
  else if (pitch < 0) {
    digitalWrite(belakang, LOW);
    digitalWrite(depan, HIGH); // Move forward if pitch is negative
    Serial.print("01");
  }

  // Stop all motors if both roll and pitch are zero
  if (roll == 0 && pitch == 0) {
    digitalWrite(depan, LOW);
    digitalWrite(belakang, LOW);
    digitalWrite(kanan, LOW);
    digitalWrite(kiri, LOW);
    Serial.print("0000");
  }

  // Control buzzer based on yaw angle
  if (yaw > 0) {
    tone(25, 262, 500); // Play a tone if yaw is positive
  }
  else if (yaw < 0) {
    tone(25, 125, 500); // Play a different tone if yaw is negative
  }

  delay(1000); // Wait for 1 second before the next loop iteration
}
