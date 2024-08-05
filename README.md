# Teensy-LiDAR
TeensyやESP32でLD06 LiDARを動かします。  
他のマイコンで動かす場合はコンストラクタのSerialをうまいこと設定してください。

~~車輪の再発明~~
# 開発環境
* PlatformIO
* Teensy4.1
* M5Stack Atom S3

# 使い方
* `LiDAR::point`   
    * `double x` / mm  
    * `double y` / mm  
    直交座標
* `LiDAR::polarPoint`  
    * `double r`: 半径 / mm  
    * `double theta`: LiDARの印を原点とし、反時計回り正。0°≦θ≦360°  
    極座標

* `LiDAR::LD06::LD06(HardwareSerial& ser = Serial1)`  
    Teensyduinoの場合はこれが適用される
* `LiDAR::LD06::LD06(const uint8_t rx, HardwareSerial& ser = Serial1)`  
    Teensyduino以外、ESP32を想定
* `LiDAR::LD06::init()`  
    シリアルの初期化
* `LiDAR::LD06::update(bool waitToRead = false , bool readAll = true)`  
    `bool waitToRead` パケットが来ていないときは待つ  
    `bool readAll` 複数パケットが来ているときは全て読む
* `LiDAR::LD06::update()`   
    360度分のデータを更新
* `LiDAR::LD06::read(bool waitToRead = false , bool readAll = true)`   
    1パケ分読む
* `LiDAR::LD06::read360()`   
    360度分読む

# 参考
1. [LD06 Development manual](https://storage.googleapis.com/mauser-public-images/prod_description_document/2021/315/8fcea7f5d479f4f4b71316d80b77ff45_096-6212_a.pdf)
2. [TanoRobo](https://github.com/TanoRoboRCJ/Software/blob/feature/%2316_RAICHO/Firmware/F446-LiDAR/)
3. [Lidar_LD06_for_Arduino](https://github.com/henjin0/Lidar_LD06_for_Arduino)
