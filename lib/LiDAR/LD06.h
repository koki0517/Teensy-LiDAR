#pragma once
#include <Arduino.h>
#include <vector>
#include <array>
#include <algorithm>
#include <cfloat>

namespace LiDAR {

struct point{ // 点の直交座標
  double x;
  double y;

  point operator+(const point& p)  const { return {x + p.x, y + p.y}; }
  point operator-(const point& p)  const { return {x - p.x, y - p.y}; }
  point operator*(const double& d) const { return {x * d, y * d}; }
  point operator/(const double& d) const { return {x / d, y / d}; }
};

struct polarPoint{ // 点の極座標
  uint16_t r;
  double theta;
};

struct formattedPacket{ // 1パケの情報 181バイト
  /* 0x54 ヘッダー
   * 0x2C データ長
   * 以上固定
   */
  uint16_t rodarSpeed; // deg/s
  double startAngle; // deg
  std::array<polarPoint, 12> polarPoints; // 極座標
  std::array<point, 12> points; // 直交座標
  std::array<uint8_t, 12> confidence;
  double endAngle; // deg
  uint16_t timeStamp; // ms 30,000を超えたらリカウント
  uint8_t CRC;
  formattedPacket(const std::array<uint8_t,47>& packet);
};

class LD06 { // ちな5Hzで回ってた
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
  void update360(); // 一回転分のデータを取得
protected:
  #if not defined(TEENSYDUINO)
  uint8_t rxPin;
  #endif

  HardwareSerial& serial;
  const uint32_t BAUD_RATE = 230400; // 通信速度 / Hz
  const int16_t PACKET_LENGTH = 47; // 1パケの長さ
  const uint8_t HEADER = 0x54; // パケットの先頭
  const uint8_t DATA_LENGTH = 0x2C; // 2byte目にくる 1パケの中の点の数 12個で固定

  double lastStartAngle = 0;
  double lastEndAngle = 0;

  bool updateSingle();
  bool checkCRC(const std::array<uint8_t,47>& packet) const;
};

}