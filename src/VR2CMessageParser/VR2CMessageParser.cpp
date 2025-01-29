#include "VR2CMessageParser.hpp"
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cmath>

int VR2CMessageParser::identifyMessageType(const std::string &message) {
    if (message.find("STS") != std::string::npos) return STATUS_MSG;
    if (message.find("A69") != std::string::npos) return DETECTION_MSG;
    return -1;
}

bool VR2CMessageParser::parseStatusMessage(const std::string &message, VR2CStatus &status) {
    std::istringstream ss(message);
    std::string token;
    int field = 0;

    while (std::getline(ss, token, ',')) {
        switch (field++) {
            case 0: status.receiver_serialno = std::stoul(token); break;
            case 1: status.seqno = static_cast<uint16_t>(std::stoul(token)); break;
            case 2: status.year = static_cast<uint8_t>(std::stoi(token) - 2000); break;
            case 3: status.month = static_cast<uint8_t>(std::stoi(token)); break;
            case 4: status.day = static_cast<uint8_t>(std::stoi(token)); break;
            case 5: status.hour = static_cast<uint8_t>(std::stoi(token)); break;
            case 6: status.minute = static_cast<uint8_t>(std::stoi(token)); break;
            case 7: status.second = static_cast<uint8_t>(std::stoi(token)); break;
            case 8: status.dc = std::stoul(token); break;
            case 9: status.pc = std::stoul(token); break;
            case 10: status.lv_v = std::stof(token); break;
            case 11: status.bv_v = std::stof(token); break;
            case 12: status.bu_percent = std::stof(token); break;
            case 13: status.i_ma = std::stof(token); break;
            case 14: status.temperature_c = std::stof(token); break;
            case 15: status.du_percent = std::stof(token); break;
            case 16: status.ru_percent = std::stof(token); break;
            case 17: status.tilt_g[0] = std::stof(token); break;
            case 18: status.tilt_g[1] = std::stof(token); break;
            case 19: status.tilt_g[2] = std::stof(token); break;
            default: return true;
        }
    }

    return field >= 20;
}

bool VR2CMessageParser::parseDetectionMessage(const std::string &message, VR2CDetection &detection) {
    std::istringstream ss(message);
    std::string token;
    int field = 0;

    while (std::getline(ss, token, ',')) {
        switch (field++) {
            case 0: detection.receiver_serialno = std::stoul(token); break;
            case 1: detection.seqno = static_cast<uint16_t>(std::stoul(token)); break;
            case 2: detection.year = static_cast<uint8_t>(std::stoi(token) - 2000); break;
            case 3: detection.month = static_cast<uint8_t>(std::stoi(token)); break;
            case 4: detection.day = static_cast<uint8_t>(std::stoi(token)); break;
            case 5: detection.hour = static_cast<uint8_t>(std::stoi(token)); break;
            case 6: detection.minute = static_cast<uint8_t>(std::stoi(token)); break;
            case 7: detection.second = static_cast<uint8_t>(std::stoi(token)); break;
            case 8: detection.letter = token[0]; break;
            case 9: detection.freq = static_cast<uint8_t>(std::stoi(token)); break;
            case 10: detection.tagMap = std::stoi(token); break;
            case 11: detection.tagID = std::stoi(token); break;
            case 12: detection.hasSensors = static_cast<uint8_t>(std::stoi(token)); break;
            default: return true;
        }
    }

    return field >= 13;
}

bool VR2CMessageParser::validateChecksum(const std::string &message) {
    size_t hashPos = message.find('#');
    if (hashPos == std::string::npos) return false;

    uint8_t checksum = 0;
    for (size_t i = 0; i < hashPos; i++) {
        checksum += message[i];
    }

    std::string checksumStr = message.substr(hashPos + 1, 2);
    uint8_t expectedChecksum = std::stoi(checksumStr, nullptr, 16);

    return (checksum & 0xFF) == expectedChecksum;
}
