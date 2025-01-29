#ifndef VEMCO_VR2C_LIB_VR2CSTATUS_HPP
#define VEMCO_VR2C_LIB_VR2CSTATUS_HPP

#include <cstdint>

struct VR2CStatus {
    uint32_t receiver_serialno;
    uint16_t seqno;
    uint8_t year, month, day, hour, minute, second;
    uint32_t dc, pc;
    float lv_v, bv_v, bu_percent, i_ma, temperature_c, du_percent, ru_percent;
    float tilt_g[3];
};


#endif //VEMCO_VR2C_LIB_VR2CSTATUS_HPP
