#include "LD06.h"

namespace LiDAR {

#if defined(TEENSYDUINO)
LD06::LD06(HardwareSerial& ser) :serial(ser) {}
#else
LD06::LD06(const uint8_t rx, HardwareSerial& ser) 
  : serial(ser),
    rxPin(rx)
{}
#endif

void LD06::init() {
  #if defined(TEENSYDUINO)
  serial.begin(BAUD_RATE);
  #else
  serial.begin(BAUD_RATE, SERIAL_8N1, rx_pin);
  #endif

  update360();
}

void LD06::update(bool waitToRead, bool readAll) {
  if (waitToRead) {
    while (!updateSingle()); // 1パケは強制で読む、2パケ以降は任意
    if (readAll) {
      while (updateSingle());
    }
  }
  else {
    if (readAll){
      while (updateSingle()); // 多分1番使う
    }
    else updateSingle();
  }
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
  lastStartAngle = fPacket.startAngle;
  lastEndAngle = fPacket.endAngle;
  latestfPacket = fPacket;

  Serial.print("time stamp: ");
  Serial.println(fPacket.timeStamp);
  return true;
}

void LD06::update360() {
  update(true);
  double startAngle = lastStartAngle;
  double accumulatedAngle = 0.0;

  while (accumulatedAngle < 360.0) {
    update(true);
    double currentEndAngle = lastEndAngle;

    double angleDifference = currentEndAngle - startAngle;
    if (angleDifference < 0) {
      angleDifference += 360.0;
    }

    accumulatedAngle += angleDifference;
    startAngle = currentEndAngle;
  }
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
  std::copy(fPacket.points.begin(), fPacket.points.end(), std::back_inserter(points));
}

std::vector<point>& LD06::read360(bool ifUpdate) {
  if (ifUpdate) update();
  return pointCloud;
}

}