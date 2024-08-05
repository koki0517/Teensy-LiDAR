#pragma once
#include <Arduino.h>
#include <vector>
#include <array>
#include <utility>
#include <algorithm>

namespace LiDAR {

struct point{
  double x;
  double y;

  uint16_t r;
  double theta;

  uint8_t confidence;

  point operator+(const point& p)  const { return {x + p.x, y + p.y}; }
  point operator-(const point& p)  const { return {x - p.x, y - p.y}; }
  point operator*(const double& d) const { return {x * d, y * d}; }
  point operator/(const double& d) const { return {x / d, y / d}; }
};

struct formattedPacket{ // 1パケの情報
  uint16_t rodarSpeed; // deg/s
  double startAngle; // deg
  std::array<point, 12> points;
  double endAngle; // deg
  uint16_t timeStamp; // ms 30,000を超えたらリカウント
  formattedPacket(const std::array<uint8_t,47>& packet);
  formattedPacket() = default;
};

class LD06 {
public:
  #if defined(TEENSYDUINO)
  // Teensyでの使用を想定
  LD06(HardwareSerial& ser = Serial1);
  #else
  // ESP32での使用を想定
  LD06(const uint8_t rx, HardwareSerial& ser = Serial1);
  #endif

  void init();
  // メンバの配列を1パケ分更新
  void update(bool waitToRead = false /* バッファ内にない場合に待機して読むか */, bool readAll = true /*2パケ以降を読むか*/);
  // 一回転分のデータを取得
  void update360(); 

  std::vector<point> read(bool waitToRead = false , bool readAll = true);
  std::vector<point>& read360(bool ifUpdate = true);
protected:
  #if not defined(TEENSYDUINO)
  uint8_t rxPin;
  #endif

  HardwareSerial& serial;
  const uint32_t BAUD_RATE = 230400; // 通信速度 / Hz
  const int16_t PACKET_LENGTH = 47; // 1パケの長さ
  const uint8_t HEADER = 0x54; // パケットの先頭
  const uint8_t DATA_LENGTH = 0x2C; // 2byte目にくる 1パケの中の点の数 12個で固定

  std::vector<point> pointCloud = std::vector<point>(360);
  double lastStartAngle = 0;
  double lastEndAngle = 0;
  formattedPacket latestfPacket;

  bool updateSingle();
  bool checkCRC(const std::array<uint8_t,47>& packet) const;
  void updatePointCloud(const formattedPacket& fPacket);
  void mergePoints(const formattedPacket& fPacket, std::vector<point>& points);
};

}