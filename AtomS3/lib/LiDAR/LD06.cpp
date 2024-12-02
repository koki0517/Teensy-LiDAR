#include "LD06.h"

namespace LiDAR {

#if defined(TEENSYDUINO)
LD06::LD06(point robotPoint, HardwareSerial& ser) 
  : serial(ser) { formattedPacket::ROBOT_POINT_DIFF = robotPoint; }
#else
LD06::LD06(point robotPoint, HardwareSerial& ser, const uint8_t rx) 
  : serial(ser), rxPin(rx) {  formattedPacket::ROBOT_POINT_DIFF = robotPoint;  }
#endif

void LD06::init() const {
  #if defined(TEENSYDUINO)
  serial.begin(BAUD_RATE);
  #else
  serial.begin(BAUD_RATE, SERIAL_8N1, rxPin);
  #endif
}

bool LD06::updateSingle(){
  while (1){
    if (serial.available() < PACKET_LENGTH) return false;
    if (serial.read() == HEADER){
      if (serial.read() == DATA_LENGTH) break;
    }
  }

  std::array<uint8_t,47> packet{};
  packet[0] = HEADER;
  packet[1] = DATA_LENGTH;
  for (auto it = packet.begin() + 2; it != packet.end(); it++){ // 先頭を2バイトを避けて読む
    *it = serial.read();
  }
  if (!checkCRC(packet)) return false; // CRCが合わない場合は無視
  
  formattedPacket fPacket(packet);
  latestfPacket = fPacket;

  return true;
}

bool LD06::checkCRC(const std::array<uint8_t,47>& packet) const {
  uint8_t crc = 0;
  for (auto it = packet.begin(); it != packet.end() - 1; it++) { // 最後のCRC自身は除く
    crc = CrcTable[(crc ^ *it) & 0xff];
  }
  return (crc == packet.back());
}

std::vector<point> LD06::read(bool waitToRead, bool readAll) {
  std::vector<point> packets;
  if (waitToRead) {
    while (!updateSingle()); // 1パケは強制で読む、2パケ以降は任意
    mergePoints(latestfPacket, packets);
    if (readAll) {
      while (updateSingle()) mergePoints(latestfPacket, packets);
    }
  }
  else {
    if (readAll){
      while (updateSingle()) mergePoints(latestfPacket, packets); // 多分1番使う
    }
    else {
      if (updateSingle()) mergePoints(latestfPacket, packets);
    }
  }
  return packets;
}

void LD06::mergePoints(const formattedPacket& fPacket, std::vector<point>& points) {
  points.reserve(12);
  for (auto& p : fPacket.points) {
    points.push_back(p);
  }
}

std::array<point, 12> LD06::readSingle(){
  while (!updateSingle());
  return latestfPacket.points;
}

}