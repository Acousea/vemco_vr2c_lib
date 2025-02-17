#ifndef VEMCO_VR2C_LIB_BOOSTSERIAL_HPP
#define VEMCO_VR2C_LIB_BOOSTSERIAL_HPP


#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include "interfaces/VR2CInterfaces.hpp"

class BoostSerial : public SerialInterface {
private:
    boost::asio::io_service io;
    boost::asio::serial_port serialPort;

public:
    // Constructor que abre el puerto serie
    explicit BoostSerial(const std::string &port, unsigned int baudRate)
            : serialPort(io, port) {
        // Configuración básica del puerto serie
        serialPort.set_option(boost::asio::serial_port_base::baud_rate(baudRate));
        serialPort.set_option(boost::asio::serial_port_base::character_size(8));
        serialPort.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
        serialPort.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
        serialPort.set_option(
                boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
    }

    // Verifica si hay datos disponibles para leer

    size_t available() override {
#if defined(_WIN32)
        COMSTAT status;
        DWORD errors;
        if (!ClearCommError(serialPort.native_handle(), &errors, &status)) {
            std::cerr << "Error obteniendo bytes disponibles: " << GetLastError() << std::endl;
            return 0;
        }
        return status.cbInQue;  // Bytes disponibles en la cola de entrada
#else
        int bytesAvailable = 0;
        if (ioctl(serialPort.native_handle(), FIONREAD, &bytesAvailable) == -1) {
            std::cerr << "Error obteniendo bytes disponibles: " << strerror(errno) << std::endl;
            return 0;
        }
        return static_cast<size_t>(bytesAvailable);
#endif
    }

    // Leer un byte del puerto serie
    int read() override {
        char c;
        boost::system::error_code ec;
        boost::asio::read(serialPort, boost::asio::buffer(&c, 1), ec);

        if (ec) {
            std::cerr << "Error al leer del puerto serie: " << ec.message() << std::endl;
            return -1;  // Devuelve -1 en caso de error
        }
        return static_cast<int>(c);
    }

    void clean() override {
        boost::system::error_code ec;

#if defined(_WIN32)
        if (!PurgeComm(serialPort.native_handle(), PURGE_RXCLEAR | PURGE_TXCLEAR)) {
            std::cerr << "Error al purgar el puerto serie: " << GetLastError() << std::endl;
        }
#else
        if (tcflush(serialPort.native_handle(), TCIOFLUSH) == -1) {
        std::cerr << "Error al purgar el puerto serie: " << strerror(errno) << std::endl;
    }
#endif
    }


    // Escribir datos al puerto serie
    void write(const std::string &data) override {
        boost::system::error_code ec;
        boost::asio::write(serialPort, boost::asio::buffer(data), ec);

        if (ec) {
            std::cerr << "Error al escribir en el puerto serie: " << ec.message() << std::endl;
        }
    }

    // Cerrar el puerto
    void close() {
        if (serialPort.is_open()) {
            serialPort.close();
        }
    }

    ~BoostSerial() override {
        close();
    }
};


#endif //VEMCO_VR2C_LIB_BOOSTSERIAL_HPP
