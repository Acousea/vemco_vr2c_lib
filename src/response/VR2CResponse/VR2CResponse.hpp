#ifndef VEMCO_VR2C_LIB_VR2CRESPONSE_HPP
#define VEMCO_VR2C_LIB_VR2CRESPONSE_HPP


#include "result/VR2CResult.hpp"
#include <cstdint>
#include <vector>
#include <string>
#include <charconv>
#include <unordered_map>
#include <algorithm>

class VR2CResponse {
public:
    enum class ResponseStatus {
        OK,
        FAILURE,
        INVALID,
        UNKNOWN // Unknown status
    };

    static uint8_t calculate8BitChecksum(const std::string &data) {
        uint8_t checksum = 0;
        for (char c: data) {
            checksum += static_cast<uint8_t>(c);
        }
        return checksum;
    }

private:
    std::string serialNumber;
    int port;
    int checksum;
    int length;
    ResponseStatus status;
    uint8_t receivedChecksum;

protected:

    std::vector<std::string> responseData;
public:
    VR2CResponse() = default;

    // Pure virtual method toString
    virtual std::string toString() const = 0;

    [[nodiscard]] const std::string &getSerialNumber() const { return serialNumber; }

    [[nodiscard]] int getPort() const { return port; }

    [[nodiscard]] int getChecksum() const { return checksum; }

    [[nodiscard]] int getLength() const { return length; }

    [[nodiscard]] const std::vector<std::string> &getResponseData() const { return responseData; }

    [[nodiscard]] ResponseStatus getStatus() const { return status; }

    [[nodiscard]] std::string getStatusAsString() const {
        switch (status) {
            case ResponseStatus::OK:
                return "OK";
            case ResponseStatus::FAILURE:
                return "FAILURE";
            case ResponseStatus::INVALID:
                return "INVALID";
            case ResponseStatus::UNKNOWN:
                return "UNKNOWN";
        }
        return "UNKNOWN";
    }

    [[nodiscard]] uint8_t getReceivedChecksum() const { return receivedChecksum; }

    virtual VR2CResult decode(const std::string &response) {
        if (response.empty() || response[0] != '*') {
            return {VR2CResult::MSG_TOO_SMALL, "Invalid response format: " + response};
        }

        // Separar la respuesta en dos partes
        size_t headerEnd = response.find(']');
        if (headerEnd == std::string::npos) {
            return {VR2CResult::MSG_TOO_SMALL, "Invalid response format: missing ']'"};
        }

        std::string header = response.substr(0, headerEnd + 1);
        std::string dataAndStatus = response.substr(headerEnd + 1);

        size_t checksumPos = dataAndStatus.rfind(",#");
        if (checksumPos == std::string::npos || checksumPos + 3 > dataAndStatus.size()) {
            return {VR2CResult::CHECKSUM_FAILED, "Checksum not found or incorrectly formatted"};
        }
        std::string checksumHex = dataAndStatus.substr(checksumPos + 2, 2);

        unsigned int received;
        if (std::from_chars(checksumHex.data(), checksumHex.data() + checksumHex.size(), received, 16).ec !=
            std::errc()) {
            return {VR2CResult::CHECKSUM_FAILED, "Invalid received checksum: " + checksumHex};
        }
        receivedChecksum = static_cast<uint8_t>(received);

        // Parsear el estado y el checksum recibido
        size_t lastCommaPos = dataAndStatus.rfind(',');
        if (lastCommaPos == std::string::npos) {
            return {VR2CResult::MSG_TOO_SMALL, "Invalid data and status format: " + dataAndStatus};
        }
        // Verificar el checksum
        std::string dataForChecksum = dataAndStatus.substr(1, lastCommaPos - 1);
        if (calculate8BitChecksum(dataForChecksum) != receivedChecksum) {
            return {VR2CResult::CHECKSUM_FAILED,
                    "Checksum mismatch => calculated: " + std::to_string(calculate8BitChecksum(dataForChecksum)) +
                    ", received: " + std::to_string(receivedChecksum)};
        }

        // Parsear el encabezado
        size_t dotPos = header.find('.');
        if (dotPos == std::string::npos) {
            return {VR2CResult::MSG_TOO_SMALL, "Missing dot in serial/port section: " + header};
        }

        serialNumber = header.substr(1, dotPos - 1);
        if (std::from_chars(header.data() + dotPos + 1, header.data() + header.size(), port).ec != std::errc()) {
            return {VR2CResult::MSG_TOO_SMALL, "Invalid port value: " + header};
        }

        size_t hashPos = header.find('#');
        if (hashPos == std::string::npos) {
            return {VR2CResult::MSG_TOO_SMALL, "Missing checksum delimiter: " + header};
        }

        if (std::from_chars(header.data() + hashPos + 1, header.data() + header.size(), checksum).ec != std::errc()) {
            return {VR2CResult::MSG_TOO_SMALL, "Invalid checksum value: " + header};
        }

        size_t bracketPos = header.find('[');
        if (bracketPos == std::string::npos) {
            return {VR2CResult::MSG_TOO_SMALL, "Missing length delimiter: " + header};
        }

        if (std::from_chars(header.data() + bracketPos + 1, header.data() + headerEnd, length).ec != std::errc()) {
            return {VR2CResult::MSG_TOO_SMALL, "Invalid length value: " + header};
        }

        // Penultimate comma position (last comma from dataForChecksum)
        auto penultimateCommaPos = dataForChecksum.rfind(',');
        if (penultimateCommaPos == std::string::npos) {
            return {VR2CResult::MSG_TOO_SMALL, "Invalid data format: " + dataForChecksum};
        }
        // The contents till the lastCommaPos starting from the ned
        std::string statusStr = dataForChecksum.substr(penultimateCommaPos + 1);
        if (statusStr == "OK") {
            status = ResponseStatus::OK;
        } else if (statusStr == "FAILURE") {
            status = ResponseStatus::FAILURE;
        } else if (statusStr == "INVALID") {
            status = ResponseStatus::INVALID;
        } else {
            status = ResponseStatus::UNKNOWN;
        }


        // Parsear la respuesta
        std::string responseStr = dataForChecksum.substr(0, penultimateCommaPos);
        if (responseStr.empty()) {
            return VR2CResult(VR2CResult::SUCCESS);
        }

        size_t start = 0;
        size_t end = responseStr.find(',');
        while (end != std::string::npos) {
            responseData.push_back(responseStr.substr(start, end - start));
            start = end + 1;
            end = responseStr.find(',', start);
        }
        responseData.push_back(responseStr.substr(start));

        return VR2CResult(VR2CResult::SUCCESS);
    }
};


#endif //VEMCO_VR2C_LIB_VR2CRESPONSE_HPP
