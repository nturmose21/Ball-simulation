# ESP32 Ball Simulation with MPU6050 and OLED Display

This project is a **ball simulation system** built on an ESP32 that uses an **MPU6050** motion sensor and displays interactive feedback on a **0.96" OLED screen**. A **push button** allows interaction (e.g. reset or menu control). The simulation visualizes movement like a ball in a sloshing container, responding to tilt detected by the MPU6050.

## 📦 Features
- Reads real-time orientation data from the MPU6050 (accelerometer/gyroscope)
- Simulates ball motion on the OLED screen
- Push button input for user control (e.g., reset or menu navigation)
- Powered by the ESP32 for wireless expandability and fast I²C communication

## 🔌 Wiring Overview

| Component   | ESP32 Pin | Description              |
|------------|-----------|--------------------------|
| MPU6050 VCC| 3.3V      | Power supply             |
| MPU6050 GND| GND       | Ground                   |
| MPU6050 SDA| GPIO21    | I²C data line            |
| MPU6050 SCL| GPIO22    | I²C clock line           |
| OLED VCC   | 3.3V      | Power supply             |
| OLED GND   | GND       | Ground                   |
| OLED SDA   | GPIO21    | I²C data line (shared)   |
| OLED SCL   | GPIO22    | I²C clock line (shared)  |
| Button     | GPIO19    | Digital input            |
| Button GND | GND       | Connected via pull-down  |

> 📝 **Note:** SDA and SCL are shared between the MPU6050 and OLED (I²C bus).

## 🧠 How It Works

- The ESP32 initializes both the MPU6050 and the OLED.
- Motion data is continuously fetched from the MPU6050.
- Using basic physics and smoothing, the sketch simulates how a ball might roll and bounce inside a boundary based on the tilt angles.
- The push button allows the user to reset or interact with the simulation.

## 🛠 Requirements

### Hardware
- ESP32 board
- MPU6050 gyroscope/accelerometer
- 0.96" I²C OLED display (SSD1306)
- Push button
- Breadboard and jumper wires

### Libraries
Install these via the Arduino Library Manager:
- `Adafruit GFX`
- `Adafruit SSD1306`
- `Wire.h`
- `MPU6050` or `Adafruit MPU6050` depending on your sketch

## 📁 Files

- `ball_simulation.ino` – Main Arduino sketch
- `README.md` – Project documentation

## 🖼 Circuit Diagram

![image](https://github.com/user-attachments/assets/794aebd8-3045-4dda-9356-cf2158bdfef3)


## 🚀 Getting Started

1. Connect your components based on the wiring diagram.
2. Open `ball_simulation.ino` in the Arduino IDE.
3. Select your ESP32 board and correct COM port.
4. Upload and monitor the output on the OLED screen.

## 💡 Future Ideas

- Add sound feedback with a buzzer
- Implement Wi-Fi-based data logging or control
- Expand to 3D ball simulation using quaternion math

---

### 📬 License

MIT License

---

### ✍️ Author

Made by Moses – Year 12 High school student - @Hack_updt

