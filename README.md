# 🛒 Smart Shopping Cart using ESP32, RFID and MPU6050

An IoT-enabled, gesture-controlled smart shopping cart that automates billing using RFID and motion control via MPU6050. Built with ESP32, ESP-NOW, and SMTP protocol for a contactless and efficient shopping experience.

---

## 🚀 Features

- 🔄 **Gesture Control**: Uses MPU6050 to move the cart forward/backward (pith) and right/left (roll) using hand tilt.
- 🧾 **Automatic Billing**: Scans RFID tags for item identification and price.
- 🧠 **ESP-NOW Communication**: Wireless link between user and cart modules (no Wi-Fi needed).
- 💡 **LCD Display**: I2C LCD 16x2 display that shows item name, price, and total price in real-time.
- 📧 **Email Billing**: Sends the bill via Gmail using the SMTP protocol when a special RFID tag is scanned.
- 🚫 **Obstacle Detection**: IR sensor stops the cart when an object is too close.
- ⚙️ **Motor Control**: L298N driver handles movement based on MPU6050 orientation.

---

## 🔧 Hardware Used

| Component            | Quantity | Purpose                              |
|----------------------|----------|--------------------------------------|
| ESP32 Dev Board      | 2        | User and Cart side                   |
| MFRC522 RFID Reader  | 1        | Reads product tags                   |
| MPU6050              | 1        | Detects pitch/roll for motion        |
| L298N Motor Driver   | 1        | Drives motors                        |
| IR Obstacle Sensor   | 1        | Detects objects in path              |
| I2C LCD (16x2)       | 1        | Displays scanned items               |
| DC Motors            | 2        | Move the cart                        |
| Power Supply/Battery | 2        | Power the cart side and user module  |

---

## 🧠 Software & Tools

- **Arduino IDE**
- **SMTP** Protocol for Gmail billing
- **ESP-NOW** Protocol for ESP32 communication
- **Wire.h / SPI.h / MFRC522.h / MPU6050.h / esp_now.h / LiquidCrystal_I2C.h / ESP_Mail_Client.h / WiFi.h**
- **I2C, SPI** Communication Protocols for hardware

---

## 📁 Project Structure
