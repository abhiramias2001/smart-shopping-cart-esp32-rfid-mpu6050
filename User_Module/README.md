# 👋 Smart Shopping Cart – User Module

This branch contains the **User-side code** for the Smart Shopping Cart project. The user module acts as a **gesture controller**, detecting hand tilt via an MPU6050 sensor and transmitting motion commands to the cart via **ESP-NOW** protocol.

---

## 🔧 Responsibilities of User Module

- 🎮 Read tilt angles (pitch and roll) from **MPU6050 sensor**
- 🧠 Convert hand gestures into motion commands (forward, backward, turn)
- 📡 Transmit pitch/roll values wirelessly to the cart using **ESP-NOW**
- 🔌 Powered via USB or battery, lightweight and portable

---

## 🔌 Hardware Connections (ESP32 – User Side)

| Module              | ESP32 Pin (Typical)  | Notes                         |
|---------------------|----------------------|-------------------------------|
| MPU6050             | SDA: 21, SCL: 22     | Communicates via I2C          |
| ESP-NOW Transmitter | Built-in Wi-Fi radio | Sends pitch/roll data to cart |
| Power               | USB or Battery       | 3.7V or 5V                    |

---

## 📂 File Description

| File Name         | Purpose                                                                 |
|-------------------|-------------------------------------------------------------------------|
| `UserModule.ino`  | Main program that:<br>• Initializes MPU6050<br>• Reads pitch/roll values<br>• Maps gesture to motion<br>• Sends data via ESP-NOW |

---

## 📡 ESP-NOW Communication

- The user module uses **ESP-NOW** to send tilt information to the cart ESP32.
- Data structure:
```cpp
typedef struct driveStruct {
  float pitch;
  float roll;
} driveStruct;
```
