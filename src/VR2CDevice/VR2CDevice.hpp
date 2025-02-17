#ifndef VEMCO_VR2C_LIB_VR2C_DEVICE_H
#define VEMCO_VR2C_LIB_VR2C_DEVICE_H

#include <string>
#include <utility>
#include <vector>
#include <functional>

#include "VR2CCommunicator/VR2CCommunicator.hpp"
#include "interfaces/VR2CInterfaces.hpp"
#include "response/responses/StatusResponse/StatusResponse.hpp"
#include <iostream>

class VR2CDevice {
private:
    std::string serialNumber;
    VR2CCommunicator communicator;
    LogCallback logger;
    TimeCallback timeProvider;

public:
    // Constructor to initialize the device with the serial interface and callbacks
    VR2CDevice(std::string serialNumber, SerialInterface &serialInterface,
               const LogCallback &logCallback, const TimeCallback &timeCallback)
            : serialNumber(std::move(serialNumber)),
              communicator(serialInterface, logCallback, timeCallback),
              logger(logCallback),
              timeProvider(timeCallback) {}

    // Set the baud rate of the device
    bool setBaudRate(int baudRate) {
        VR2CCommand command = VR2CCommand::Factory::createBaudRateCommand(serialNumber, baudRate);
//        return communicator.sendCommand(command).has_value();
        return true;
    }

    // Send the QUIT command to end the current session
    bool quit() {
        VR2CCommand command = VR2CCommand::Factory::createQuitCommand(serialNumber);
//        return communicator.sendCommand(command).has_value();
        return true;
    }

    // Send the STATUS command to get the status of the device
    bool getStatus() {
        VR2CCommand command = VR2CCommand::Factory::createStatusCommand(serialNumber);
        auto result = communicator.sendCommand<StatusResponse>(command);
        if (!result.has_value()) {
            return false;
        }
        auto &response = result.value();
        logger("Received status response: " + response.toString());
        // Do something with the response
        return true;
    }

};

#endif // VEMCO_VR2C_LIB_VR2C_DEVICE_H

