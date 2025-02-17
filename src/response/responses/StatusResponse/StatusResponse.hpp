#ifndef VEMCO_VR2C_LIB_STATUSRESPONSE_HPP
#define VEMCO_VR2C_LIB_STATUSRESPONSE_HPP

#include "response/VR2CResponse/VR2CResponse.hpp"
#include <cstdlib>  // for std::strtof and std::strtol
#include <cstring>  // for std::strtok
#include <string>

class StatusResponse : public VR2CResponse {
private:
    std::string timestamp;
    int detectionCount = 0;       // DC
    int pingCount = 0;            // PC
    float lineVoltage = 0.0f;     // LV
    float batteryVoltage = 0.0f;  // BV
    float batteryUsed = 0.0f;     // BU
    float currentConsumption = 0.0f;  // I
    float internalTemperature = 0.0f; // T
    float detectionMemoryUsed = 0.0f; // DU
    float rawMemoryUsed = 0.0f;       // RU
    float tiltX = 0.0f;           // X-axis of XYZ
    float tiltY = 0.0f;           // Y-axis of XYZ
    float tiltZ = 0.0f;           // Z-axis of XYZ
    std::string receiverMode;     // MODE

    // Helper to parse XYZ values
    bool parseXYZ(const std::string &value) {
        size_t firstColon = value.find(':');
        if (firstColon == std::string::npos) return false;

        size_t secondColon = value.find(':', firstColon + 1);
        if (secondColon == std::string::npos) return false;

        // Convert substrings to float using std::strtof
        char *end;

        tiltX = std::strtof(value.c_str(), &end);
        if (*end != ':' || end == value.c_str()) return false;

        tiltY = std::strtof(end + 1, &end);
        if (*end != ':' || end == value.c_str() + firstColon + 1) return false;

        tiltZ = std::strtof(end + 1, &end);
        if (end == value.c_str() + secondColon + 1) return false;

        return true;
    }


    // Helper to parse key-value pairs
    bool parseKeyValuePair(const std::string &key, const std::string &value) {
        if (key == "DC") detectionCount = std::stoi(value);
        else if (key == "PC") pingCount = std::stoi(value);
        else if (key == "LV") lineVoltage = std::stof(value);
        else if (key == "BV") batteryVoltage = std::stof(value);
        else if (key == "BU") batteryUsed = std::stof(value);
        else if (key == "I") currentConsumption = std::stof(value);
        else if (key == "T") internalTemperature = std::stof(value);
        else if (key == "DU") detectionMemoryUsed = std::stof(value);
        else if (key == "RU") rawMemoryUsed = std::stof(value);
        else if (key == "XYZ") return parseXYZ(value);
        else if (key == "MODE") receiverMode = value;
        else return false;  // Unknown key
        return true;
    }


public:
public:
    VR2CResult decode(const std::string &response) override {
        // Decode the basic structure using the parent class
        VR2CResult baseResult = VR2CResponse::decode(response);
        if (!baseResult.isSuccess()) {
            return baseResult;  // Return the error if the base decoding fails
        }

        if (responseData.empty()) {
            return {VR2CResult::ErrorCode::MSG_CONTENT_INVALID, "Response data is empty"};
        }

        // First item is the timestamp
        timestamp = responseData.front();

        // Last item is the mode (RECORDING, STOPPED, STORAGE)
        receiverMode = responseData.back();

        // Process key-value pairs in between
        for (size_t i = 2; i < responseData.size() - 1; ++i) {
            const std::string &item = responseData[i];

            // Split the key and value using '='
            size_t equalPos = item.find('=');
            if (equalPos == std::string::npos) {
                return {VR2CResult::ErrorCode::MSG_CONTENT_INVALID, "Invalid key-value pair: " + item};
            }

            std::string key = item.substr(0, equalPos);
            std::string value = item.substr(equalPos + 1);

            // Parse the key-value pair
            if (!parseKeyValuePair(key, value)) {
                return {VR2CResult::ErrorCode::MSG_CONTENT_INVALID, "Unknown key: " + key};
            }
        }

        return VR2CResult(VR2CResult::SUCCESS);
    }

    [[nodiscard]] std::string toString() const override {
        return "StatusResponse{"
               "timestamp='" + timestamp + '\'' +
               ", detectionCount=" + std::to_string(detectionCount) +
               ", pingCount=" + std::to_string(pingCount) +
               ", lineVoltage=" + std::to_string(lineVoltage) +
               ", batteryVoltage=" + std::to_string(batteryVoltage) +
               ", batteryUsed=" + std::to_string(batteryUsed) +
               ", currentConsumption=" + std::to_string(currentConsumption) +
               ", internalTemperature=" + std::to_string(internalTemperature) +
               ", detectionMemoryUsed=" + std::to_string(detectionMemoryUsed) +
               ", rawMemoryUsed=" + std::to_string(rawMemoryUsed) +
               ", tiltX=" + std::to_string(tiltX) +
               ", tiltY=" + std::to_string(tiltY) +
               ", tiltZ=" + std::to_string(tiltZ) +
               ", receiverMode='" + receiverMode + '\'' +
               '}';
    }

    // Getter methods for the decoded data
    [[nodiscard]] int getDetectionCount() const { return detectionCount; }

    [[nodiscard]] int getPingCount() const { return pingCount; }

    [[nodiscard]] float getLineVoltage() const { return lineVoltage; }

    [[nodiscard]] float getBatteryVoltage() const { return batteryVoltage; }

    [[nodiscard]] float getBatteryUsed() const { return batteryUsed; }

    [[nodiscard]] float getCurrentConsumption() const { return currentConsumption; }

    [[nodiscard]] float getInternalTemperature() const { return internalTemperature; }

    [[nodiscard]] float getDetectionMemoryUsed() const { return detectionMemoryUsed; }

    [[nodiscard]] float getRawMemoryUsed() const { return rawMemoryUsed; }

    [[nodiscard]] float getTiltX() const { return tiltX; }

    [[nodiscard]] float getTiltY() const { return tiltY; }

    [[nodiscard]] float getTiltZ() const { return tiltZ; }

    [[nodiscard]] const std::string &getReceiverMode() const { return receiverMode; }


};

#endif // VEMCO_VR2C_LIB_STATUSRESPONSE_HPP
