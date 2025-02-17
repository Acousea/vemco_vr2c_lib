#include "VR2CCommunicator.hpp"

std::string VR2CCommunicator::readResponse() {
    std::string response;
    char c = 0x00;
    const size_t limit_sz = 1024;  // Tamaño máximo de la respuesta permitida
    const uint32_t TIMEOUT_MS = 5000;  // Tiempo de espera en milisegundos

    while (true) {
        // Esperar a que haya datos disponibles
        uint32_t startTime = getCurrentTimestamp();
        while (serial.available() == 0) {
            if ((getCurrentTimestamp() - startTime) > TIMEOUT_MS) {
                logger("VR2CCommunicator: Timeout reading response -> No data available");
                return "";
            }
        }

        // Leer un byte
        int byte = serial.read();
        if (byte == -1) {
            break;  // Error al leer
        }

        c = static_cast<char>(byte);
        response += c;

        // Verificar si alcanzamos el límite del buffer
        if (response.size() >= limit_sz) {
            logger("VR2CCommunicator: Buffer overflow -> Response too large");
            return "";
        }

        // Finaliza si se encuentra el carácter de nueva línea
        if (response.size() >= 2 && response.substr(response.size() - 2) == "\\n") {
            break;
        }
    }

    // Eliminar espacios en blanco adicionales
    response.erase(response.find_last_not_of(" \r\n") + 1);

    return response;
}


uint32_t VR2CCommunicator::getCurrentTimestamp() {
#ifdef ARDUINO
    return millis();
#else
    return static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count());
#endif
}

void VR2CCommunicator::wakeUpIfNeeded() {
    uint32_t currentTimestamp = getCurrentTimestamp();
    if ((currentTimestamp - lastTxTimestamp) >= 100) {
        logger("VR2CCommunicator -> Waking up the device...");
        serial.write("\r");  // Send carriage return to wake up the device
        delay_ms(120);       // Delay for 120 ms (platform-specific)
        lastTxTimestamp = currentTimestamp;
    }
}

void VR2CCommunicator::cleanSerialBuffer() {
    while (serial.available() > 0) {
        serial.read();  // Consume and discard data
    }
}

void VR2CCommunicator::delay_ms(int milliseconds) {
#ifdef ARDUINO
    delay(milliseconds);
#else
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
#endif
}

