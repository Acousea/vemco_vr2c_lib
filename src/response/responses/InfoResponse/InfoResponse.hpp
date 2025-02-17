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
        // Decode the basic structure using the parent class
        VR2CResult baseResult = VR2CResponse::decode(response);
        if (!baseResult.isSuccess()) {
            return baseResult;  // Return the error if the base decoding fails
        }

        if (responseData.size() < 5) {
            return {VR2CResult::ErrorCode::MSG_CONTENT_INVALID, "Invalid INFO response format"};
        }

        // Assign fields based on responseData order
        frequency = responseData[0];          // e.g., "VR2C-69"
        serialNumber = responseData[1];       // e.g., "450088"
        studyDescription = trimQuotes(responseData[2]); // e.g., "'Possum Lake Fish Study #1'"
        activeCodeMap = responseData[3];      // e.g., "MAP-113"

        // Parse the last two key-value fields (FW and HW)
        for (size_t i = 4; i < responseData.size(); ++i) {
            if (!parseKeyValuePair(responseData[i])) {
                return {VR2CResult::ErrorCode::MSG_CONTENT_INVALID, "Unknown key in INFO response: " + responseData[i]};
            }
        }

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
