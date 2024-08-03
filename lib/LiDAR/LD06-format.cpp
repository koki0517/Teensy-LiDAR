#include "LD06.h"

namespace LiDAR {

formattedPacket::formattedPacket(const std::array<uint8_t, 47>& packet)
  : rodarSpeed(packet[3] << 8 | packet[2]),
    startAngle((packet[5] << 8 | packet[4]) / 100.0),
    endAngle((packet[43] << 8 | packet[42]) / 100.0),
    timeStamp(packet[45] << 8 | packet[44]),
    CRC(packet[46]) {
  const double interval = (endAngle - startAngle) / 11.0;
  for (int i = 0; i < 12; i++) {
    polarPoints[i].r = packet[7 + i * 3] << 8 | packet[6 + i * 3];
    polarPoints[i].theta = startAngle + interval * i;
    confidence[i] = packet[8 + i * 3] / 100.0;
    // 極座標から直交座標に変換
    points[i].x = polarPoints[i].r * cos(polarPoints[i].theta);
    points[i].y = polarPoints[i].r * sin(polarPoints[i].theta);
  }
}

}