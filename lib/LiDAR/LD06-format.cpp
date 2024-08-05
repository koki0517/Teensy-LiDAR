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
    points[i].confidence = packet[8 + i * 3] / 100.0;
    // 極座標から直交座標に変換 "README.md#座標系"を参照
    points[i].x = points[i].r * sin(points[i].theta*M_PI/180.0);
    points[i].y = points[i].r * cos(points[i].theta*M_PI/180.0);
  }
}

}