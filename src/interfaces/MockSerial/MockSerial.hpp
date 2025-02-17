#ifndef VEMCO_VR2C_LIB_MOCKSERIAL_HPP
#define VEMCO_VR2C_LIB_MOCKSERIAL_HPP

#include <iostream>
#include <string>
#include <limits>
#include "interfaces/VR2CInterfaces.hpp"

class MockSerial : public SerialInterface {
private:
    std::string inputBuffer;  // Almacena el texto a simular
    size_t currentIndex = 0;  // Puntero al carácter actual en el buffer

public:
    size_t available() override {
        if (!inputBuffer.empty() && currentIndex < inputBuffer.size()) {
            return inputBuffer.size() - currentIndex;
        }

        std::cout << "The buffer is empty. If you want, enter the text to simulate between double quotes \"\": ";
        std::string userInput;
        std::getline(std::cin, userInput);

        // Verificar si el input está correctamente entre comillas
        if (userInput.size() >= 2 && userInput.front() == '"' && userInput.back() == '"') {
            // Extraer el contenido dentro de las comillas y asignarlo directamente al inputBuffer
            inputBuffer = userInput.substr(1, userInput.size() - 2);
        } else {
            inputBuffer.clear();  // Si no hay input válido, limpiar el buffer
        }
        currentIndex = 0;  // Reiniciar el índice al inicio del nuevo buffer
        return inputBuffer.size() - currentIndex;  // Retorna la cantidad de datos disponibles
    }

    void clean() override {
        inputBuffer.clear();  // Limpiar el buffer de entrada
        currentIndex = 0;
    }

    int read() override {
        if (available() > 0) {
            char c = inputBuffer[currentIndex++];
            return static_cast<int>(c);
        } else {
            std::cout << "No hay datos disponibles para leer." << std::endl;
            return -1;  // Código de error cuando no hay más datos
        }
    }

    void write(const std::string &data) override {
        std::cout << "MockSerial TX: " << data << std::endl;  // Muestra los datos enviados
    }
};

#endif // VEMCO_VR2C_LIB_MOCKSERIAL_HPP
