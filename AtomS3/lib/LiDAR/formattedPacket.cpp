#include "LD06.h"

namespace LiDAR {

point formattedPacket::ROBOT_POINT_DIFF = {0, 0};

formattedPacket::formattedPacket(const std::array<uint8_t, 47>& packet)
  : rodarSpeed(packet[3] << 8 | packet[2]),
    startAngle((packet[5] << 8 | packet[4]) / 100.0),
    endAngle((packet[43] << 8 | packet[42]) / 100.0),
    timeStamp(packet[45] << 8 | packet[44]) 
{
  double interval = (endAngle - startAngle) / 11.0;
  if (startAngle > endAngle) { // 360 -> 0 を跨いでいる場合
    interval = (360.0 - startAngle + endAngle) / 11.0;
  }

  for (int i = 0; i < 12; i++) {
    uint16_t r = packet[7 + i * 3] << 8 | packet[6 + i * 3];
    double theta = startAngle + interval * i;
    if (theta >= 360.0) theta -= 360.0;
    points[i].theta = theta;
    points[i].confidence = packet[8 + i * 3];

    // 極座標から直交座標に変換 "README.md#座標系"を参照
    points[i].x = r * sin(theta*M_PI/180.0);
    points[i].y = r * cos(theta*M_PI/180.0);

    // ロボット座標系に変換
    points[i] += ROBOT_POINT_DIFF; 
  }
}

}