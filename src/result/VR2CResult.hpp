#ifndef VEMCO_VR2C_LIB_VR2CRESULT_HPP
#define VEMCO_VR2C_LIB_VR2CRESULT_HPP

#include <string>
#include <unordered_map>

class VR2CResult {
public:
    // Enum representing error codes
    enum ErrorCode : int {
        SUCCESS = 0,
        MSG_TOO_SMALL = -1,
        SERIAL_TIMEOUT = -2,
        CHECKSUM_FAILED = -3,
        CMD_STATUS_INVALID = -4,
        CMD_STATUS_FAILURE = -5,
        MSG_CONTENT_INVALID = -6,
        UNKNOWN_ERROR = -100
    };

private:
    ErrorCode code;  // Store the current result code
    std::string additionalInfo;  // Additional information about the error

    // Static mapping of error codes to descriptions
    static const std::unordered_map<ErrorCode, std::string> errorDescriptions;

public:
    // Constructor to initialize with an error code
    explicit VR2CResult(ErrorCode code) : code(code) {}

    // Constructor to initialize with an error code and additional information
    VR2CResult(ErrorCode code, const std::string &additionalErrorInfo) : code(code) {
        additionalInfo = additionalErrorInfo;
    }

    // Default constructor (assume success)
    VR2CResult() : code(SUCCESS) {}

    // Get the current error code
    [[nodiscard]] ErrorCode getCode() const {
        return code;
    }

    // Check if the result is successful
    [[nodiscard]] bool isSuccess() const {
        return code == SUCCESS;
    }

    // Get the description of the current error code
    [[nodiscard]] std::string getDescription() const {
        auto it = errorDescriptions.find(code);
        if (it != errorDescriptions.end()) {
            return it->second + (additionalInfo.empty() ? "" : ": " + additionalInfo);
        }
        return "Unknown error code";
    }

    // Allow static access to description for any code
    static std::string getDescription(ErrorCode code) {
        auto it = errorDescriptions.find(code);
        if (it != errorDescriptions.end()) {
            return it->second;
        }
        return "Unknown error code";
    }
};


#endif // VEMCO_VR2C_LIB_VR2CRESULT_HPP
