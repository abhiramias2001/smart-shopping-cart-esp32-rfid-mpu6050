#include <WiFi.h>
#include <esp_now.h>
#include "Wire.h"

const int MPU_ADDR = 0x68; // Default I2C address for MPU6050

uint8_t broadcastAddress[] = {0x78, 0x42, 0x1C, 0x6C, 0x56, 0xB4};  // Receiver MAC

int16_t accX, accY, accZ;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

typedef struct driveStruct {
  float pitch;
  float roll;
} driveStruct;
driveStruct driveVar;


void setup() {
  Wire.begin();
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);

  // Wake up MPU6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // Power management register
  Wire.write(0);    // Wake up MPU6050
  Wire.endTransmission(true);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Request accelerometer data
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // Starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true); // Read 6 bytes (ACC X, Y, Z)

  accX = Wire.read() << 8 | Wire.read();
  accY = Wire.read() << 8 | Wire.read();
  accZ = Wire.read() << 8 | Wire.read();

  // Convert to 'g'
  float ax = accX / 16384.0;
  float ay = accY / 16384.0;
  float az = accZ / 16384.0;

  // Calculate pitch and roll
  driveVar.pitch = atan2(ay, sqrt(ax * ax + az * az)) * 180.0 / PI;
  driveVar.roll  = atan2(-ax, az) * 180.0 / PI;

  // Print results
  Serial.print("Pitch: ");
  Serial.print(driveVar.pitch);
  Serial.print("°, Roll: ");
  Serial.print(driveVar.roll);
  Serial.println("°");

  if(driveVar.pitch<=-50){
    Serial.println("fwd");
  }
  else if(driveVar.pitch>-50 && driveVar.pitch<=50){
    Serial.println("stop");
  }
  else{
    Serial.println("bkwd");
  }

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&driveVar, sizeof(driveVar));

  if (result == ESP_OK) {
    Serial.println("Sent: " + String(driveVar.pitch));
  } else {
    Serial.println("Send failed");
  }

  delay(2000);
}
