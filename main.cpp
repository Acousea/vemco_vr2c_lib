#include <iostream>
#include <ctime>
#include "src/VR2CDevice/VR2CDevice.hpp"
#include "src/interfaces/MockSerial/MockSerial.hpp"


// Instantiate the mock serial
MockSerial mockSerial;

// Create VR2C device using mock serial, time function, and console logging
VR2CDevice vr2c(
        mockSerial,
        []() -> uint32_t { return static_cast<uint32_t>(std::time(nullptr) * 1000); }, // Get time in ms
        [](const std::string &msg) { std::cout << "LOG: " << msg << std::endl; } // Log messages
);

int main() {
    vr2c.initialize();
    vr2c.setTime();

    // Simulate incoming message
    std::cout << "Enter VR2C message: ";
    VR2CStatus status;
    VR2CDetection detection;
    vr2c.listenForMessages(5000, status, detection);

    return 0;
}
