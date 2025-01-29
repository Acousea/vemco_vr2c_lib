#ifndef VEMCO_VR2C_LIB_VR2C_DEVICE_H
#define VEMCO_VR2C_LIB_VR2C_DEVICE_H

#include <string>
#include <vector>
#include <functional>

#include "../VR2CCommunicator/VR2CCommunicator.hpp"
#include "../VR2CMessageParser/VR2CMessageParser.hpp"
#include "../interfaces/VR2CInterfaces.hpp"

class VR2CDevice {
public:
    VR2CDevice(SerialInterface &serial, TimeCallback timeProvider, LogCallback logger = nullptr);

    bool initialize();
    bool setTime();
    int listenForMessages(uint32_t timeout_ms, VR2CStatus &status, VR2CDetection &detection);
    int sendCommand(const std::string &cmd, bool await_ack = true);

private:
    SerialInterface &serial;
    TimeCallback getTimeMs;
    LogCallback log;

    int processStatusMessage(const std::string &message, VR2CStatus &status);
    int processDetectionMessage(const std::string &message, VR2CDetection &detection);
};

#endif // VEMCO_VR2C_LIB_VR2C_DEVICE_H

