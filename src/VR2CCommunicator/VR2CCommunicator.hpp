#ifndef VEMCO_VR2C_LIB_VR2CCOMMUNICATOR_HPP
#define VEMCO_VR2C_LIB_VR2CCOMMUNICATOR_HPP

#include <string>
#include <cstdint>
#include "../interfaces/VR2CInterfaces.hpp"


class VR2CCommunicator {
public:
    VR2CCommunicator(SerialInterface &serial, TimeCallback timeProvider);

    bool readMessage(std::string &message, uint32_t timeout_ms);
    bool sendMessage(const std::string &cmd, std::string &response, bool await_ack);
    void flushSerialBuffer();

private:
    SerialInterface &serial;
    TimeCallback getTimeMs;
};


#endif // VR2C_COMMUNICATOR_H
