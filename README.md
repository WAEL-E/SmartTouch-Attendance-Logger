# SmartTouch Attendance Logger (ESP32 + Firebase)

A practical IoT-based attendance system designed for small classrooms to help lecturers take manual attendance more efficiently — without calling names or relying on easily-shared QR codes.

## 🔧 What It Does

- Students enter their student ID on a keypad
- They confirm physical presence via a touch sensor (prototype version)
- The system logs their name, date, and time to Firebase in real-time
- LCD gives instant feedback and confirmation

> ⚠️ In real deployment, the touch sensor would be replaced with **fingerprint** or **facial recognition** — similar to how exam halls use face ID with RFID fallback. But this prototype uses touch input to keep it simple and cost-effective.

## 🎯 Why This Matters

- QR codes can be shared — this system requires physical presence
- Faster than calling names, ideal for small classrooms or lab sessions
- Real-time cloud logging with timestamps

## 📸 Screenshots

| LCD Output         | System Build        | Firebase Realtime Log |
|--------------------|---------------------|------------------------|
| ![LCD](![alt text](<WhatsApp Image 2025-05-15 at 08.10.27_5fa01ad5.jpg>)) | ![Build](![alt text](<WhatsApp Image 2025-05-15 at 08.06.32_03423ea1.jpg>)) | ![Log](![alt text](<Screenshot 2025-05-15 081139.png>)) |

## 🛠️ Built With

- **ESP32** microcontroller
- **4x4 Keypad**
- **TTP223 Touch Sensor**
- **I2C LCD 16x2**
- **RTC DS3231**
- **Firebase Realtime Database**
- C++ (Arduino)

## 🗂️ Future Ideas

- Fingerprint sensor (R307) or Face Recognition
- RFID card scan fallback
- Daily reports or CSV export
- Admin mode with summary display

## 📁 File Info

- `SmartTouch_Attendance.ino` — full working source code
- `images/` — pictures and screenshots for documentation

## 📜 License

This project is open-sourced under the MIT License — feel free to use, build on, or contribute.
