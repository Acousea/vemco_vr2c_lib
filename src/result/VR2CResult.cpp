#include "VR2CResult.hpp"

// Define error descriptions
const std::unordered_map<VR2CResult::ErrorCode, std::string> VR2CResult::errorDescriptions = {
        {SUCCESS, "Command executed successfully"},
        {MSG_TOO_SMALL, "Message buffer too small to contain the full message"},
        {SERIAL_TIMEOUT, "Timeout while waiting for a response on the serial interface"},
        {CHECKSUM_FAILED, "Checksum validation failed"},
        {CMD_STATUS_INVALID, "Command status returned INVALID"},
        {CMD_STATUS_FAILURE, "Command status returned FAILURE"},
        {UNKNOWN_ERROR, "An unknown error occurred"}
};
