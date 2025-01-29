#ifndef VEMCO_VR2C_LIB_VR2CMESSAGEPARSER_HPP
#define VEMCO_VR2C_LIB_VR2CMESSAGEPARSER_HPP


#include <string>
#include "messages/VR2CStatus.hpp"
#include "messages/VR2CDetection.hpp"

class VR2CMessageParser {
public:
    static constexpr int STATUS_MSG = 1;
    static constexpr int DETECTION_MSG = 2;

    static int identifyMessageType(const std::string &message);
    static bool parseStatusMessage(const std::string &message, VR2CStatus &status);
    static bool parseDetectionMessage(const std::string &message, VR2CDetection &detection);
    static bool validateChecksum(const std::string &message);
};


#endif //VEMCO_VR2C_LIB_VR2CMESSAGEPARSER_HPP
