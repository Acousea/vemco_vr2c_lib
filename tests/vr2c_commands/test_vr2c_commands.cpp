#include <gtest/gtest.h>
#include "commands/VR2CCommand/Vr2CCommand.hpp"


// Prueba para comprobar la construcción de un comando básico
TEST(VR2CCommandTest, BuildSimpleCommand) {
    VR2CCommand command("450088", "0", VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::ReceiverControl::INFO));

    std::string expectedCommand = "*450088.0#25,INFO\r";  // El checksum correcto es 25 para este ejemplo
    ASSERT_EQ(command.buildCommand(), expectedCommand);
}

// Prueba para comandos con parámetros
TEST(VR2CCommandTest, CommandWithParameters) {
    VR2CCommand command("450088", "0", VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::SerialCommunication::BAUDRATE), "9600");

    std::string expectedCommand = "*450088.0#25,BAUDRATE=9600\r";  // Checksum para este ejemplo sigue siendo 25
    ASSERT_EQ(command.buildCommand(), expectedCommand);
}

/// ============================ Tests for CommandCategory ============================
// ---------------------------- Receiver Control ----------------------------
TEST(VR2CCommandTest, GetCommandNameStart) {
    ASSERT_EQ(VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::ReceiverControl::START), "START");
}

TEST(VR2CCommandTest, GetCommandNameStop) {
    ASSERT_EQ(VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::ReceiverControl::STOP), "STOP");
}

TEST(VR2CCommandTest, GetCommandNameErase) {
    ASSERT_EQ(VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::ReceiverControl::ERASE), "ERASE");
}

TEST(VR2CCommandTest, GetCommandNameInfo) {
    ASSERT_EQ(VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::ReceiverControl::INFO), "INFO");
}

TEST(VR2CCommandTest, GetCommandNameResetBattery) {
    ASSERT_EQ(VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::ReceiverControl::RESETBATTERY), "RESETBATTERY");
}

TEST(VR2CCommandTest, GetCommandNameStorage) {
    ASSERT_EQ(VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::ReceiverControl::STORAGE), "STORAGE");
}

TEST(VR2CCommandTest, GetCommandNameStatus) {
    ASSERT_EQ(VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::ReceiverControl::STATUS), "STATUS");
}

TEST(VR2CCommandTest, GetCommandNameTime) {
    ASSERT_EQ(VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::ReceiverControl::TIME), "TIME");
}
// ---------------------------- Serial Communication ----------------------------
TEST(VR2CCommandTest, GetCommandNameBaudrate) {
    ASSERT_EQ(VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::SerialCommunication::BAUDRATE), "BAUDRATE");
}

TEST(VR2CCommandTest, GetCommandNameQuit) {
    ASSERT_EQ(VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::SerialCommunication::QUIT), "QUIT");
}


// ---------------------------- Port Control ----------------------------

TEST(VR2CCommandTest, GetCommandNameDetectionLogReadAck) {
    ASSERT_EQ(VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::DetectionLog::READACK), "READACK");
}

TEST(VR2CCommandTest, GetCommandNameDetectionLogReadBeg) {
    ASSERT_EQ(VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::DetectionLog::READBEG), "READBEG");
}

TEST(VR2CCommandTest, GetCommandNameDetectionLogReadEnd) {
    ASSERT_EQ(VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::DetectionLog::READEND), "READEND");
}

TEST(VR2CCommandTest, GetCommandNameDetectionLogReadRec) {
    ASSERT_EQ(VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::DetectionLog::READREC), "READREC");
}

// ---------------------------- Sensor Control ----------------------------

TEST(VR2CCommandTest, GetCommandNameRealTimeModeRTM232) {
    ASSERT_EQ(VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::RealTimeMode::RTM232), "RTM232");
}

TEST(VR2CCommandTest, GetCommandNameRealTimeModeRTM485) {
    ASSERT_EQ(VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::RealTimeMode::RTM485), "RTM485");
}

TEST(VR2CCommandTest, GetCommandNameRealTimeModeRTMAUTOERASE) {
    ASSERT_EQ(VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::RealTimeMode::RTMAUTOERASE), "RTMAUTOERASE");
}

TEST(VR2CCommandTest, GetCommandNameRealTimeModeRTMINFO) {
    ASSERT_EQ(VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::RealTimeMode::RTMINFO), "RTMINFO");
}

TEST(VR2CCommandTest, GetCommandNameRealTimeModeRTMNOW) {
    ASSERT_EQ(VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::RealTimeMode::RTMNOW), "RTMNOW");
}
TEST(VR2CCommandTest, GetCommandNameRealTimeModeRTMOFF) {
    ASSERT_EQ(VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::RealTimeMode::RTMOFF), "RTMOFF");
}

TEST(VR2CCommandTest, GetCommandNameRealTimeModeRTMPROFILE) {
    ASSERT_EQ(VR2CCommand::CommandCategory::getCommandName(
            VR2CCommand::CommandCategory::RealTimeMode::RTMPROFILE), "RTMPROFILE");
}

