#include "VR2CDevice.hpp"

VR2CDevice::VR2CDevice(SerialInterface &serial, TimeCallback timeProvider, LogCallback logger)
        : serial(serial), getTimeMs(timeProvider), log(logger) {}

bool VR2CDevice::initialize() {
    if (log) log("Initializing VR2C Device...");
    return sendCommand("INIT", true) == 0;
}

bool VR2CDevice::setTime() {
    std::string cmd = "TIME=2024-01-01 12:00:00\r\f";
    return sendCommand(cmd, true) == 0;
}

int VR2CDevice::listenForMessages(uint32_t timeout_ms, VR2CStatus &status, VR2CDetection &detection) {
    std::string message;
    uint32_t startTime = getTimeMs();

    while ((getTimeMs() - startTime) < timeout_ms) {
        if (serial.available() > 0) {
            char c = serial.read();
            message += c;
            if (c == '\n') break;
        }
    }

    if (message.empty()) return 0;

    int msgType = VR2CMessageParser::identifyMessageType(message);
    switch (msgType) {
        case VR2CMessageParser::STATUS_MSG:
            return processStatusMessage(message, status);
        case VR2CMessageParser::DETECTION_MSG:
            return processDetectionMessage(message, detection);
        default:
            return -2;
    }
}

int VR2CDevice::sendCommand(const std::string &cmd, bool await_ack) {
    serial.write(cmd);

    if (await_ack) {
        std::string response;
        return serial.available() ? 0 : -1;
    }

    return 0;
}

int VR2CDevice::processStatusMessage(const std::string &message, VR2CStatus &status) {
    return VR2CMessageParser::parseStatusMessage(message, status) ? 1 : -3;
}

int VR2CDevice::processDetectionMessage(const std::string &message, VR2CDetection &detection) {
    return VR2CMessageParser::parseDetectionMessage(message, detection) ? 1 : -3;
}
