# 🔫 Laser Tag Project

An awesome C++ + JS project for building a **real-life laser tag system**, combining **Arduino hardware** and a **React-based client app** for tracking, scoring, and managing the game in real time.

---

## 🧠 What’s Inside?

### 1. ⚙️ `arduino_code/`
Arduino C++ code for:
- IR shooting + receiving
- Hit detection & health tracking
- Game logic on the microcontroller

### 2. 💻 `client_app/`
React front-end that:
- Displays live game stats
- Connects to hardware over serial/WiFi/Bluetooth
- Manages players, teams, and results

---

## 🛠️ Technologies Used

| Area          | Tech Stack                  |
|---------------|-----------------------------|
| Hardware      | Arduino UNO, IR modules     |
| Firmware      | C++ (Arduino)               |
| Front-End     | React, Vite                 |
| Communication | Serial/WiFi (custom logic)  |

---

## 🖼️ Screenshots

> Add your own!  
> Put them in `/screenshots/` folder and link them here, like:

![Laser Gun Prototype](screenshots/laser_gun.jpg)
![React UI Preview](screenshots/ui_preview.png)

---

## 🚀 Getting Started

### 🧩 Clone the repo
```bash
git clone https://github.com/yourusername/laser-tag-project.git
cd laser-tag-project
```
### 🧱 Arduino
```
Upload arduino_code/main.ino using the Arduino IDE.
```
### 🖥️ Client App
```
bash
Copy
Edit
cd client_app
npm install
npm run dev
```
