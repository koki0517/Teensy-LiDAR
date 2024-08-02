#pragma once
#include <Arduino.h>
#include <vector>
#include <array>
#include <algorithm>

namespace LiDAR {

struct point{ // 点の座標
  double x;
  double y;
};

struct polarPoint{ // 点の極座標
  double r;
  double theta;
};

struct formattedPacket{ // 1パケの情報
  /* 0x54 ヘッダー
   * 0x2C データ長
   * 以上固定
   */
  uint16_t rodarSpeed; // 回転速度 / deg/s
  double startAngle; // 開始角度 / deg
  std::array<polarPoint, 12> points; // 点の情報
  double endAngle; // 終了角度 / deg
  uint16_t timeStamp; // タイムスタンプ / ms 3000を超えたらリカウント
  uint8_t CRC; // チェックサム
};

class LD06 {
public:
  // 通信用のシリアルの参照を受け取り、初期化する
  #if defined(TEENSYDUINO)
  // Teensyでの使用を想定
  LD06(HardwareSerial& ser = Serial1);
  #else
  // ESP32での使用を想定
  LD06(const uint8_t rx, HardwareSerial& ser = Serial1);
  #endif

  void init();
  bool checkCRC(const std::array<uint8_t,47>& packet); // チェックサムの検証

  // パケットを受信し、メンバの配列を更新する
  void update();
protected:
  #if not defined(TEENSYDUINO)
  uint8_t rxPin;
  #endif

  HardwareSerial& serial; // 通信用のシリアル
  const uint32_t BAUD_RATE = 230400; // 通信速度 / Hz
  const int16_t PACKET_LENGTH = 47; // 1パケの長さ

  const uint8_t HEADER = 0x54; // パケットの先頭
  const uint8_t DATA_LENGTH = 0x2C; // 2byte目にくる 1パケの中の点の数 12個で固定

  formattedPacket formatPacket(const std::array<uint8_t,47>& packet);
};

}