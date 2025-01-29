#ifndef VEMCO_VR2C_LIB_MOCKSERIAL_HPP
#define VEMCO_VR2C_LIB_MOCKSERIAL_HPP


#include <iostream>
#include "../VR2CInterfaces.hpp"

class MockSerial : public SerialInterface {
public:
    size_t available() override { return 1; } // Simulate always having data available

    int read() override {
        char c;
        std::cin.get(c); // Read from standard input
        return static_cast<int>(c);
    }

    void write(const std::string &data) override {
        std::cout << "MockSerial TX: " << data << std::endl; // Print data to standard output
    }
};

#endif //VEMCO_VR2C_LIB_MOCKSERIAL_HPP
