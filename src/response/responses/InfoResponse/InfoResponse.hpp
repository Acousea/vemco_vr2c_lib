#ifndef VEMCO_VR2C_LIB_INFORESPONSE_HPP
#define VEMCO_VR2C_LIB_INFORESPONSE_HPP


#include <string>
#include <cstring>  // for std::strtok
#include "response/VR2CResponse/VR2CResponse.hpp"
#include "result/VR2CResult.hpp"

class InfoResponse : public VR2CResponse {
private:
    std::string frequency;          // VR2C Frequency (e.g., VR2C-69)
    std::string serialNumber;       // Device serial number
    std::string studyDescription;   // Description of the active study
    std::string activeCodeMap;      // Active code map (e.g., MAP-113)
    std::string firmwareVersion;    // Firmware version (e.g., FW=1.0.2)
    std::string hardwareVersion;    // Hardware version (e.g., HW=3)

    // Helper to trim leading and trailing quotes from a string
    static std::string trimQuotes(const std::string &str) {
        if (str.size() >= 2 && str.front() == '\'' && str.back() == '\'') {
            return str.substr(1, str.size() - 2);
        }
        return str;
    }

    // Helper to parse key-value pairs
    bool parseKeyValuePair(const std::string &token) {
        size_t eqPos = token.find('=');
        if (eqPos != std::string::npos) {
            std::string key = token.substr(0, eqPos);
            std::string value = token.substr(eqPos + 1);

            if (key == "FW") firmwareVersion = value;
            else if (key == "HW") hardwareVersion = value;
            else return false;  // Unknown key

            return true;
        }
        return false;
    }

public:
    VR2CResult decode(const std::string &response) override {
//        // Decode the basic structure using the parent class
//        VR2CResult baseResult = VR2CResponse::decode(response);
//        if (!baseResult.isSuccess()) {
//            return baseResult;  // Return the error if the base decoding fails
//        }
//
//        // Tokenize the responseData using ',' as the delimiter
//        char buffer[responseData.size() + 1];
//        std::strcpy(buffer, responseData.c_str());
//
//        char *token = std::strtok(buffer, ",");
//        int tokenIndex = 0;
//
//        while (token != nullptr) {
//            switch (tokenIndex) {
//                case 0:  // VR2C Frequency and serial number (e.g., VR2C-69:450088)
//                {
//                    char *colonPos = std::strchr(token, ':');
//                    if (colonPos == nullptr) {
//                        return {VR2CResult::MSG_TOO_SMALL, "Missing ':' in VR2C frequency/serial pair"};
//                    }
//                    *colonPos = '\0';
//                    frequency = token;
//                    serialNumber = colonPos + 1;
//                    break;
//                }
//                case 1:  // Study description (trim quotes if present)
//                    studyDescription = trimQuotes(token);
//                    break;
//                case 2:  // Active code map (e.g., MAP-113)
//                    activeCodeMap = token;
//                    break;
//                default:  // Firmware and hardware versions (e.g., FW=1.0.2, HW=3)
//                    if (!parseKeyValuePair(token)) {
//                        return {VR2CResult::MSG_TOO_SMALL, "Failed to parse key-value pair: " + std::string(token)};
//                    }
//                    break;
//            }
//            token = std::strtok(nullptr, ",");
//            ++tokenIndex;
//        }

        return VR2CResult(VR2CResult::SUCCESS);
    }

    [[nodiscard]] std::string toString() const override {
        return "Frequency: " + frequency + ", Serial: " + serialNumber + ", Study: " + studyDescription +
               ", Active Code Map: " + activeCodeMap + ", Firmware: " + firmwareVersion + ", Hardware: " + hardwareVersion;

    }

    // Getter methods for the decoded data
    [[nodiscard]] const std::string &getFrequency() const { return frequency; }

    [[nodiscard]] const std::string &getSerialNumber() const { return serialNumber; }

    [[nodiscard]] const std::string &getStudyDescription() const { return studyDescription; }

    [[nodiscard]] const std::string &getActiveCodeMap() const { return activeCodeMap; }

    [[nodiscard]] const std::string &getFirmwareVersion() const { return firmwareVersion; }

    [[nodiscard]] const std::string &getHardwareVersion() const { return hardwareVersion; }
};


#endif //VEMCO_VR2C_LIB_INFORESPONSE_HPP
