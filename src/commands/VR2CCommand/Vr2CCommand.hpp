#ifndef VEMCO_VR2C_LIB_VR2CCOMMAND_HPP
#define VEMCO_VR2C_LIB_VR2CCOMMAND_HPP

#include <cstdio>  // For snprintf
#include <string>
#include <utility>
#include <cctype>
#include <iomanip>

// Clase base para representar cualquier comando genérico
class VR2CCommand {
public:
    // Categorías de comandos y sus correspondientes enumeraciones
    struct CommandCategory {
        // Serial Communication Commands
        enum class SerialCommunication : int {
            BAUDRATE, QUIT
        };

        // Basic Receiver Control/Status Commands
        enum class ReceiverControl : int {
            INFO, STATUS, START, STOP, TIME, ERASE, STORAGE, RESETBATTERY
        };

        // Real-Time Mode Control Commands
        enum class RealTimeMode : int {
            RTMPROFILE, RTM232, RTM485, RTMNOW, RTMINFO, RTMOFF, RTMAUTOERASE
        };

        // Detection Log Manual Retrieval Commands
        enum class DetectionLog : int {
            READBEG, READEND, READREC, READACK
        };

        // Función para obtener el nombre del comando directamente
        static const std::string &getCommandName(SerialCommunication command) {
            static const std::string names[] = {"BAUDRATE", "QUIT"};
            return names[static_cast<int>(command)];
        }

        static const std::string &getCommandName(ReceiverControl command) {
            static const std::string names[] = {
                    "INFO", "STATUS", "START", "STOP", "TIME", "ERASE", "STORAGE", "RESETBATTERY"
            };
            return names[static_cast<int>(command)];
        }

        static const std::string &getCommandName(RealTimeMode command) {
            static const std::string names[] = {
                    "RTMPROFILE", "RTM232", "RTM485", "RTMNOW", "RTMINFO", "RTMOFF", "RTMAUTOERASE"
            };
            return names[static_cast<int>(command)];
        }

        static const std::string &getCommandName(DetectionLog command) {
            static const std::string names[] = {
                    "READBEG", "READEND", "READREC", "READACK"
            };
            return names[static_cast<int>(command)];
        }
    };

    // Inner Factory class to generate specific commands
    class Factory {
    public:
        // Factory method to create a BAUDRATE command
        static VR2CCommand createBaudRateCommand(const std::string& serialNumber, int baudRate) {
            // Validate the baud rate
            if (baudRate != 9600 && baudRate != 19200 && baudRate != 38400 && baudRate != 57600 && baudRate != 115200) {
                throw std::invalid_argument("Invalid baud rate. Supported rates: 9600, 19200, 38400, 57600, 115200.");
            }
            return VR2CCommand(serialNumber, "BAUDRATE", std::to_string(baudRate));
        }

        // Factory method to create a QUIT command
        static VR2CCommand createQuitCommand(const std::string& serialNumber) {
            return VR2CCommand(serialNumber, "QUIT");
        }

        static VR2CCommand createInfoCommand(const std::string& serialNumber) {
            return VR2CCommand(serialNumber, "INFO");
        }

        static VR2CCommand createStatusCommand(const std::string& serialNumber) {
            return VR2CCommand(serialNumber, "STATUS");
        }
    };
protected:
    std::string serialNumber;
    std::string port;
    std::string commandName;
    std::string parameters;

    [[nodiscard]] int calculateChecksum() const {
        int sum = 0;
        for (char c: serialNumber + port) {
            if (isdigit(c)) {
                sum += (c - '0');
            }
        }
        return sum;
    }

public:
    VR2CCommand(std::string serial, std::string cmdName, std::string params = "", std::string port="0")
            : serialNumber(std::move(serial)), port(std::move(port)), commandName(std::move(cmdName)),
              parameters(std::move(params)) {}

    virtual ~VR2CCommand() = default;

    // Construcción del comando en formato ASCII usando snprintf
    [[nodiscard]] std::string buildCommand() const {
        char buffer[256];  // Use a fixed-size buffer (safe for embedded systems)
        int checksum = calculateChecksum();

        if (parameters.empty()) {
            snprintf(buffer, sizeof(buffer), "*%s.%s#%02d,%s\r",
                     serialNumber.c_str(), port.c_str(), checksum, commandName.c_str());
        } else {
            snprintf(buffer, sizeof(buffer), "*%s.%s#%02d,%s=%s\r",
                     serialNumber.c_str(), port.c_str(), checksum, commandName.c_str(), parameters.c_str());
        }

        return buffer;
    }
};


#endif //VEMCO_VR2C_LIB_VR2CCOMMAND_HPP