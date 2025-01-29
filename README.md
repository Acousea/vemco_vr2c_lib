# 📡 VR2C Library – Generic & Portable Communication Interface
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Platform](https://img.shields.io/badge/platform-Arduino%20|%20Linux%20|%20Windows-blue)
![Language](https://img.shields.io/badge/language-C++-blue.svg)
![Status](https://img.shields.io/badge/status-Active-brightgreen.svg)

## 🌟 Overview
**VR2C** is a **fully portable** C++ library for communicating with VR2C acoustic receivers.  
It abstracts **serial communication, time handling, and logging** via **callbacks**, making it usable on:
- ✅ **Arduino** (HardwareSerial, SoftwareSerial)
- ✅ **Embedded Linux** (e.g., Raspberry Pi)
- ✅ **PC** (for debugging/simulation)

---

## 🎯 **Features**
✔ **Hardware-agnostic** – Works with Arduino, Linux, and Windows  
✔ **Serial communication abstraction** – Uses a `SerialInterface` for flexibility  
✔ **Pluggable logging & time** – Integrates with any logging system  
✔ **Message parsing** – Supports VR2C detections & status messages  
✔ **Unit-testable** – Easily test on a PC with mock serial  

---

## 🚀 **Installation**
### **1️⃣ Clone the Repository**
```sh
git clone https://github.com/antonioap101/VR2C-Library.git
cd VR2C-Library
```

### **2️⃣ Add to Your Project**
For **Arduino**, place it in the `libraries` folder:
```sh
mv VR2C-Library ~/Arduino/libraries/VR2C
```

For **PC/Linux Development**:
```sh
mkdir build && cd build
cmake ..
make
```

---

## 📌 **How to Use**
### **1️⃣ Setup a Serial Interface**
#### **For Arduino**
```cpp
#include <Arduino.h>
#include "VR2CDevice.h"
#include "VR2CInterfaces.h"

ArduinoSerial serial(Serial);
VR2CDevice vr2c(
    serial, 
    []() { return millis(); },  // Time callback
    [](const std::string &msg) { Serial.println(msg.c_str()); } // Logger callback
);

void setup() {
    Serial.begin(9600);
    vr2c.initialize();
    vr2c.setTime();
}

void loop() {
    VR2CStatus status;
    VR2CDetection detection;
    vr2c.listenForMessages(1000, status, detection);
}
```

#### **For PC/Linux (Testing)**
```cpp
#include <iostream>
#include "VR2CDevice.h"

MockSerial mockSerial;
VR2CDevice vr2c(
    mockSerial,
    []() -> uint32_t { return static_cast<uint32_t>(std::time(nullptr) * 1000); },
    [](const std::string &msg) { std::cout << "[LOG] " << msg << std::endl; }
);

int main() {
    vr2c.initialize();
    vr2c.setTime();
    
    mockSerial.write("450088,010,2024-01-01 18:37:03,A69-1601,179,#77\r\n");
    
    VR2CStatus status;
    VR2CDetection detection;
    vr2c.listenForMessages(1000, status, detection);

    return 0;
}
```

---

## 📝 **License**
This project is licensed under the **MIT License** – free to use and modify.  
See [`LICENSE`](./LICENSE) for details.

---

## 💡 **Contributing**
Want to improve this library? Feel free to **fork** and submit a **pull request**! 🚀
- Report issues [here](https://github.com/antonioap101/VR2C-Library/issues).
- Discuss features and enhancements.
