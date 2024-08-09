#include "LD06.h"

namespace LiDAR {

formattedPacket::formattedPacket(const std::array<uint8_t, 47>& packet)
  : rodarSpeed(packet[3] << 8 | packet[2]),
    startAngle((packet[5] << 8 | packet[4]) / 100.0),
    endAngle((packet[43] << 8 | packet[42]) / 100.0),
    timeStamp(packet[45] << 8 | packet[44]) {
  double interval = (endAngle - startAngle) / 11.0;
  if (startAngle > endAngle) { // 360 -> 0 を跨いでいる場合
    interval = (360.0 - startAngle + endAngle) / 11.0;
  }
  for (int i = 0; i < 12; i++) {
    points[i].r = packet[7 + i * 3] << 8 | packet[6 + i * 3];
    points[i].theta = startAngle + interval * i;
    if (points[i].theta >= 360.0) points[i].theta -= 360.0;
    points[i].confidence = packet[8 + i * 3];
    // 極座標から直交座標に変換 "README.md#座標系"を参照
    points[i].x = points[i].r * sin(points[i].theta*M_PI/180.0);
    points[i].y = points[i].r * cos(points[i].theta*M_PI/180.0);
  }
}

#if defined(TEENSYDUINO)
LD06::LD06(HardwareSerial& ser) :serial(ser) {}
#else
LD06::LD06(HardwareSerial& ser, const uint8_t rx) 
  : serial(ser),
    rxPin(rx)
{}
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

}