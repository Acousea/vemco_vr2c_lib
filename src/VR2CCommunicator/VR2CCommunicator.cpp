#include "VR2CCommunicator.hpp"

VR2CCommunicator::VR2CCommunicator(SerialInterface &serial, TimeCallback timeProvider)
        : serial(serial), getTimeMs(timeProvider) {}

void VR2CCommunicator::flushSerialBuffer() {
    while (serial.available()) {
        serial.read();
    }
}

bool VR2CCommunicator::readMessage(std::string &message, uint32_t timeout_ms) {
    message.clear();
    uint32_t start_time = getTimeMs();

    while ((getTimeMs() - start_time) < timeout_ms) {
        if (serial.available() > 0) {
            char c = serial.read();
            message += c;
            if (c == '\n') break;  // Stop reading when newline is received
        }
    }

    return !message.empty();
}

bool VR2CCommunicator::sendMessage(const std::string &cmd, std::string &response, bool await_ack) {
    flushSerialBuffer();
    serial.write(cmd);

    if (await_ack) {
        return readMessage(response, 1000); // 1 second timeout
    }

    return true;
}
