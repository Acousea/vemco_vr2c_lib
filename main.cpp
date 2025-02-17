#include <iostream>
#include <ctime>
#include "src/VR2CDevice/VR2CDevice.hpp"
#include "src/interfaces/MockSerial/MockSerial.hpp"
#include "interfaces/BoostSerial/BoostSerial.hpp"


void test_checksum() {
    auto dataForChecksum = "2012-03-20 13:25:28,STS,DC=0,PC=0,LV=0.0,BV=2.9,BU=0.1,I=6.2,T=23.0,DU=0.0,RU=0.0,XYZ=0.97:0.00:0.13,RECORDING,OK"; // Should be 0x9C
    auto dataForChecksum2 = "2025-02-03 18:41:14,STS,DC=175,PC=2045,LV=14.8,BV=0.3,BU=0.0,I=6.5,T=22.7,DU=0.1,RU=0.1,XYZ=-0.53:-0.78:0.34,RECORDING,OK"; // Should be 0x49
    auto dataForChecksum3 = "VR2C-69:450088,'Possum Lake Fish Study #1',MAP-113,FW=1.0.2,HW=3,OK"; // Should be #2D (45)
    auto checksum = VR2CResponse::calculate8BitChecksum(dataForChecksum);
    auto checksum2 = VR2CResponse::calculate8BitChecksum(dataForChecksum2);
    auto checksum3 = VR2CResponse::calculate8BitChecksum(dataForChecksum3);
    std::cout << "Checksum: " << static_cast<int>(checksum) << std::endl;
    std::cout << "Checksum2: " << static_cast<int>(checksum2) << std::endl;
    std::cout << "Checksum3: " << static_cast<int>(checksum3) << std::endl;

}

// Instantiate the mock serial
MockSerial mockSerial;
//BoostSerial boostSerial("COM3", 9600);

// Create VR2C device using mock serial, time function, and console logging
VR2CDevice vr2c(
        "450088", // Serial number "450315"
        mockSerial, // Serial interface
        [](const std::string &msg) {
            std::cout << "LOG: " << msg << std::endl;
        }, // Log messages
        []() -> uint32_t { return static_cast<uint32_t>(std::time(nullptr) * 1000); } // Get time in ms
);

int main() {
    // Simulate incoming message
    std::cout << "Testing... ";
    test_checksum();
}

int main2() {
    // Simulate incoming message
    std::cout << "Starting VR2C communication... ";
//    vr2c.getStatus();
    auto response = vr2c.getInfo();

    if (!response.has_value()) {
        std::cout << "Failed to get response" << std::endl;
    }

    const auto &info = response.value();
    std::cout << "Info: " << info.toString() << std::endl;

    return 0;
}
