#ifndef VEMCO_VR2C_LIB_VR2CCOMMUNICATOR_HPP
#define VEMCO_VR2C_LIB_VR2CCOMMUNICATOR_HPP

#include <string>
#include <cstdint>
#include <optional>
#include "interfaces/VR2CInterfaces.hpp"
#include "commands/VR2CCommand/Vr2CCommand.hpp"
#include "response/VR2CResponse/VR2CResponse.hpp"

#ifdef ARDUINO
#include <Arduino.h>  // Para delay() en entornos Arduino
#else

#include <thread>
#include <chrono>

#endif

class VR2CCommunicator {
private:
    SerialInterface &serial;
    LogCallback logger;
    TimeCallback timeProvider;
    uint32_t lastTxTimestamp = 0;

    // Utility function to read the full response from the serial interface
    std::string readResponse();

    // Helper to simulate getting the current timestamp
    uint32_t getCurrentTimestamp();

    // Wake up the serial transceivers if necessary
    void wakeUpIfNeeded();

    // Clean serial buffer (simulate cleaning the serial buffer before sending)
    void cleanSerialBuffer();

    // Cross-platform delay function (uses delay() in Arduino, sleep_for in PC)
    static void delay_ms(int milliseconds);

public:
    // Constructor with dependencies injected
    explicit VR2CCommunicator(SerialInterface &serialInterface,
                              LogCallback logCallback,
                              TimeCallback timeCallback)
            : serial(serialInterface), logger(std::move(logCallback)), timeProvider(std::move(timeCallback)) {}

    // Sends a command and waits for the response with retry logic
    template<typename ResponseType>
    std::optional<ResponseType> sendCommand(const VR2CCommand &command) {
        static_assert(std::is_base_of<VR2CResponse, ResponseType>::value,
                      "ResponseType must be derived from VR2CResponse");

        std::string commandString = command.buildCommand();
        int retries = 2;  // Retry once after the initial attempt

        while (retries-- > 0) {
            wakeUpIfNeeded();  // Wake up if needed
            serial.clean();   // Clean the serial buffer

            serial.write(commandString);  // Send the command
            logger("Sending command: " + commandString);

            delay_ms(110);  // Short delay for response

            // Read and decode the response
            std::string responseRawStr = readResponse();
            if (responseRawStr.empty()) {
                logger("No response received.");
            } else {
                logger("Received response: " + responseRawStr);

                ResponseType decodedResponse;
                VR2CResult result = decodedResponse.decode(responseRawStr);

                if (!result.isSuccess()) {
                    logger("Response decoding failed: " + result.getDescription());
                    logger("Retrying command...");
                    continue;
                }

                logger("Command successful: " + decodedResponse.getStatusAsString());
                if (decodedResponse.getStatus() == VR2CResponse::ResponseStatus::OK) {
                    return decodedResponse;
                }
            }
        }

        logger("Failed to get a valid response after retries.");
        return std::nullopt;
    }

};

#endif // VEMCO_VR2C_LIB_VR2CCOMMUNICATOR_HPP