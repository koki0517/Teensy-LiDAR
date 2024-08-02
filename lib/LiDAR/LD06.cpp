#include "LD06.h"

namespace LiDAR {

#if defined(TEENSYDUINO)
LD06::LD06(HardwareSerial& ser) :serial(ser) {}
#else
LD06::LD06(const uint8_t rx, HardwareSerial& ser) 
  :serial(ser),
  rxPin(rx)
{}
#endif

void LD06::init() {
  #if defined(TEENSYDUINO)
  serial.begin(BAUD_RATE);
  #else
  serial.begin(BAUD_RATE, SERIAL_8N1, rx_pin);
  #endif
}

formattedPacket LD06::formatPacket(const std::array<uint8_t,47>& packet){
  formattedPacket fPacket;

  return fPacket;
}

void LD06::update(){
  while (1){
    if (serial.available() < PACKET_LENGTH) return;

    if (serial.peek() == HEADER){
      break;
    } else {
      serial.read();
    }
  }

  std::array<uint8_t,47> packet{};
  for (auto& d : packet){
    d = serial.read();
  }
  if (!checkCRC(packet)) return; // CRCが合わない場合は無視
  
  formattedPacket fPacket = formatPacket(packet);
}

}