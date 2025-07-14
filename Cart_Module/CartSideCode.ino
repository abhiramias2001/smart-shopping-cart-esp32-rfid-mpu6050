#include <esp_now.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include <vector>
#include <ctype.h>

#define WIFI_SSID       "AbhiramiM34"
#define WIFI_PASSWORD   "abhiabhi123"
#define SMTP_HOST       "smtp.gmail.com"
#define SMTP_PORT       465
#define AUTHOR_EMAIL    "helloabhiramias@gmail.com"
#define AUTHOR_PASSWORD "apdf agho jbwu lnrk"
#define RECIPIENT_EMAIL "abhiramias05@gmail.com"

SMTPSession smtp;
ESP_Mail_Session session;

#define irPin 5
int sensorValue;
int m1in1 = 13;
int m1in2 = 12;
int m2in1 = 27;
int m2in2 = 26;
int ena = 14;
int enb = 33;

typedef struct driveStruct {
  float pitch;
  float roll;
} driveStruct;
driveStruct driveVar;

#define SS_PIN 21
#define RST_PIN 22
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte buffer[18];
int total = 0;

struct Entry {
  String uid;
  String key;
  int value;
};

std::vector<Entry> entries;

void movFwd() {
  digitalWrite(m1in1, HIGH);
  digitalWrite(m1in2, LOW);
  digitalWrite(m2in2, HIGH);
  digitalWrite(m2in1, LOW);
  ledcWrite(0, 250);
  ledcWrite(1, 250);
  Serial.println("Cart moving forward");
}

void movBkwd() {
  digitalWrite(m1in1, LOW);
  digitalWrite(m1in2, HIGH);
  digitalWrite(m2in2, LOW);
  digitalWrite(m2in1, HIGH);
  ledcWrite(0, 250);
  ledcWrite(1, 250);
  Serial.println("Cart moving backward");
}

void stopMotion() {
  digitalWrite(m1in1, LOW);
  digitalWrite(m1in2, LOW);
  digitalWrite(m2in2, LOW);
  digitalWrite(m2in1, LOW);
  ledcWrite(0, 0);
  ledcWrite(1, 0);
}

void turnRight() {
  digitalWrite(m1in1, LOW);
  digitalWrite(m1in2, LOW);
  digitalWrite(m2in2, HIGH);
  digitalWrite(m2in1, LOW);
  ledcWrite(0, 0);
  ledcWrite(1, 250);
  Serial.println("Cart turning right");
}

void turnLeft() {
  digitalWrite(m1in1, HIGH);
  digitalWrite(m1in2, LOW);
  digitalWrite(m2in2, LOW);
  digitalWrite(m2in1, LOW);
  ledcWrite(0, 250);
  ledcWrite(1, 0);
  Serial.println("Cart turning left");
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  sensorValue = digitalRead(irPin);
  if (len == sizeof(driveVar)) {
    memcpy(&driveVar, incomingData, sizeof(driveVar));
    if (driveVar.pitch <= -50) {
      if (sensorValue == HIGH) movFwd();
      else stopMotion();
    } else if (driveVar.pitch > -50 && driveVar.pitch <= 50) {
      if (sensorValue == HIGH) {
        if (driveVar.roll <= -50) turnLeft();
        else if (driveVar.roll >= 40) turnRight();
        else stopMotion();
      } else stopMotion();
    } else {
      if (sensorValue == HIGH) movBkwd();
      else stopMotion();
    }
  }
  delay(500);
}

void showData(const String itemName, int price, int total) {
  lcd.setCursor(0, 1);
  lcd.print(itemName + " - Rs." + String(price));
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Total: Rs." + String(total));
}

int addOrDeleteEntry(const String &u, const String &k, int v) {
  for (auto it = entries.begin(); it != entries.end(); ++it) {
    if (it->uid == u) {
      entries.erase(it);
      return 0;  // Removed
    }
  }
  entries.push_back({u, k, v});
  return 1;  // Added
}

const String targetUID = "33 D BF 2C";
String lastUID = "";
unsigned long lastScanTime = 0;
const unsigned long debounceDuration = 3000;

void sendEmail() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
  }

  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sending bill...");

  SMTP_Message msg;
  String body = "Smart Cart generated bill:\n";
  int billTot = 0;
  for (size_t i = 0; i < entries.size(); i++) {
    body += String(i + 1) + ". " + entries[i].key + ": Rs." + entries[i].value + "\n";
    billTot += entries[i].value;
  }
  body += "Total: Rs." + String(billTot);
  msg.text.content = body.c_str();
  msg.sender.name = "Smart Cart";
  msg.sender.email = AUTHOR_EMAIL;
  msg.subject = "Bill";
  msg.addRecipient("Hello, ", RECIPIENT_EMAIL);

  if (!smtp.connect(&session)) return;
  if (!MailClient.sendMail(&smtp, &msg)) Serial.println("Send error: " + smtp.errorReason());
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Bill sent via");
    lcd.setCursor(0, 1);
    lcd.print("mail");
  }
  smtp.closeSession();
}

void setup() {
  Serial.begin(9600);
  pinMode(irPin, INPUT);
  pinMode(m1in1, OUTPUT); pinMode(m1in2, OUTPUT);
  pinMode(m2in1, OUTPUT); pinMode(m2in2, OUTPUT);
  ledcSetup(0, 1000, 8); ledcSetup(1, 1000, 8);
  ledcAttachPin(ena, 0); ledcAttachPin(enb, 1);
  Wire.begin(25, 32);
  lcd.init(); lcd.backlight(); lcd.clear(); lcd.setCursor(0, 0);
  lcd.print("Scan a Tag");
  SPI.begin(); mfrc522.PCD_Init();
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (i) uid += " ";
    uid += String(mfrc522.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();
  if (uid == lastUID && (millis() - lastScanTime < debounceDuration)) return;
  lastUID = uid; lastScanTime = millis();

  if (uid == targetUID) {
    sendEmail();
    delay(20000);
    return;
  }

  byte block = 1, size = sizeof(buffer);
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
  if (mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &mfrc522.uid) != MFRC522::STATUS_OK) return;
  if (mfrc522.MIFARE_Read(block, buffer, &size) != MFRC522::STATUS_OK) return;

  char itemName[15] = "";
  int nameIndex = 0;
  for (int i = 0; i < 8 && nameIndex < 14; i++) {
    if (isalpha(buffer[i])) itemName[nameIndex++] = (char)buffer[i];
    else break;
  }
  itemName[nameIndex] = '\0';

  char c1 = buffer[13], c2 = buffer[14], c3 = buffer[15];
  int price = 0;
  if (isdigit(c1) && isdigit(c2) && isdigit(c3)) price = (c1 - '0') * 100 + (c2 - '0') * 10 + (c3 - '0');
  else if (isdigit(c2) && isdigit(c3)) price = (c2 - '0') * 10 + (c3 - '0');

  int result = addOrDeleteEntry(uid, itemName, price);
  if (result == 1) {
    total += price;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Item added:");
    showData(itemName, price, total);
  } else {
    total -= price;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Item removed:");
    showData(itemName, price, total);
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  delay(1000);
}
