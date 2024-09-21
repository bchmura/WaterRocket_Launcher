#ifndef COMMUNICATIONS_HPP
#define COMMUNICATIONS_HPP

#include <cstdlib>

enum PacketType {
    StatusType = 0x01,
    PingType = 0x02,
    PingAckType = 0x03,
    CmdToggleSolenoidType = 0x04,
    CmdToggleActuatorType = 0x05
  };

typedef struct {
    uint8_t type = PacketType::PingType;
    unsigned int pingId;
} Ping;

typedef struct {
    uint8_t type = PacketType::PingAckType;
    unsigned int pingId;
} PingAck;

typedef struct {
    uint8_t type = PacketType::CmdToggleSolenoidType;
    bool newStateRequested;
} CmdToggleSolenoid;

typedef struct {
    uint8_t type = PacketType::CmdToggleActuatorType;
    bool newStateRequested;
} CmdToggleActuator;












#endif //COMMUNICATIONS_HPP
