#ifndef VEMCO_VR2C_LIB_VR2CDETECTION_HPP
#define VEMCO_VR2C_LIB_VR2CDETECTION_HPP

#include <cstdint>

struct VR2CDetection {
    uint32_t receiver_serialno;
    uint16_t seqno;
    uint8_t year, month, day, hour, minute, second;
    char letter;
    uint8_t freq;
    uint16_t tagMap;
    uint16_t tagID;
    uint8_t hasSensors;
    uint8_t sensorADC[2];
    double latitude, longitude;
    uint32_t timestamp_ms;
};
#endif //VEMCO_VR2C_LIB_VR2CDETECTION_HPP
