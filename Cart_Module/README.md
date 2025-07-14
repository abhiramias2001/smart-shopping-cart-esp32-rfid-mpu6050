# 🛒 Smart Shopping Cart – Cart Module

This branch contains the **Cart-side code** for the Smart Shopping Cart. It handles RFID-based item scanning, LCD display updates, obstacle detection using an IR sensor, billing logic, and email bill generation via the ESP Mail Client.

---

## 🔧 Responsibilities of Cart Module

- 📚 Scan items using the **RC522 RFID reader**
- 💡 Display item name, price, and total on **I2C LCD**
- 🧠 Receive motion data (pitch/roll) from **User module via ESP-NOW**
- 🧾 Detect when a **special RFID tag** is scanned to trigger billing
- 🚫 Stop the cart if an obstacle is detected using **IR sensor**
- 📧 Send the final bill to a registered email via **Gmail SMTP**

---

## 🔌 Hardware Connections (ESP32 – Cart Side)

| Module              | ESP32 Pin (Typical)                                                      | Notes                           |
|---------------------|---------------------------------------------------------------------------|---------------------------------|
| RC522 RFID Reader   | SPI (MISO: 19, MOSI: 23, SCK: 18, SDA: 21)                                | Connect via SPI                 |
| I2C LCD Display     | SDA: 25, SCL: 32                                                          | Use `LiquidCrystal_I2C` library |
| IR Sensor           | GPIO 5                                                                    | Digital input pin               |
| L298N Motor Driver  | IN1–IN4: GPIO 13, 12, 27, 26<br>ENA & ENB: GPIO 14, 33                    | ENA/ENB controlled via PWM      |

---

## 📂 File Description

| File Name         | Purpose                                                                                                                   |
|------------------|---------------------------------------------------------------------------------------------------------------------------|
| `CartSideCode.ino` | Main cart logic including:<br>• RFID tag reading and item mapping<br>• LCD display updates<br>• IR obstacle detection<br>• ESP-NOW-based motion control<br>• Email bill sending via SMTP |

---

## 📬 Email Billing (ESP Mail Client)

- Sends a bill when a **predefined RFID UID** is scanned (acts as the "checkout tag").
- Uses **ESP Mail Client** library to send the bill via Gmail using **SMTP over SSL**.
- Requires a **Gmail App Password**.
