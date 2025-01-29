#ifndef VEMCO_VR2C_LIB_VR2CINTERFACES_HPP
#define VEMCO_VR2C_LIB_VR2CINTERFACES_HPP


#include <string>
#include <functional>
#include <cstdint>

// Abstract Serial Interface
class SerialInterface {
public:
    virtual ~SerialInterface() = default;
    virtual size_t available() = 0;
    virtual int read() = 0;
    virtual void write(const std::string &data) = 0;
};

// Time Provider Interface
using TimeCallback = std::function<uint32_t()>;

// Logging Callback Interface
using LogCallback = std::function<void(const std::string &)>;



#endif //VEMCO_VR2C_LIB_VR2CINTERFACES_HPP
