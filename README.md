# Teensy-LiDAR
TeensyやESP32でLD06 LiDARを動かします。  
他のマイコンで動かす場合はコンストラクタのSerialをうまいこと設定してください。

~~車輪の再発明~~
# 開発環境
* PlatformIO
* Teensy4.1
* M5Stack Atom S3

# 使い方
## 構造体
* `LiDAR::point`   
    * `double x` / mm  
    * `double y` / mm  
    直交座標
* `LiDAR::polarPoint`  
    * `double r`: 半径 / mm  
    * `double theta`: LiDARの印を原点とし、反時計回り正。0°≦θ≦360°  
    極座標

## Public
* `LD06(HardwareSerial& ser = Serial1)`  
    Teensyduinoの場合はこれが適用される
* `LD06(const uint8_t rx, HardwareSerial& ser = Serial1)`  
    Teensyduino以外、ESP32を想定
* `init()`  
    シリアルの初期化
* `update()`  
    バッファーにパケットが届いていればそれを読み取る  
    ない場合は何もしない

## Protected
* ` bool checkCRC(const std::array<uint8_t,47>& packet)`  
    CRCの確認を行う

# 参考
1. [LD06 Development manual](https://storage.googleapis.com/mauser-public-images/prod_description_document/2021/315/8fcea7f5d479f4f4b71316d80b77ff45_096-6212_a.pdf)
2. [TanoRobo](https://github.com/TanoRoboRCJ/Software/blob/feature/%2316_RAICHO/Firmware/F446-LiDAR/)
3. [Lidar_LD06_for_Arduino](https://github.com/henjin0/Lidar_LD06_for_Arduino)
