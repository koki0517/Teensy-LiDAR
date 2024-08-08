#include <Arduino.h>
#include <LD06.h>
#include <array>
#include <algorithm>

LiDAR::LD06 ld06(Serial1);

#if defined(TEENSYDUINO)
static uint8_t s1bufsize[128];
#endif

void setup() {
  #if defined(TEENSYDUINO) // バッファーの拡張
  Serial1.addMemoryForRead(s1bufsize, sizeof(s1bufsize));
  #endif

  Serial.begin(115200);
  ld06.init();
}

void loop() {
  Serial.println("start");
  for (int i = 0; i < 2; i++){
    std::vector<LiDAR::point> points = ld06.read(true);
    Serial.print(i);
    Serial.print(": ");
    Serial.println(Serial1.available());
    // for (const auto& p : points) {
    //   Serial.print(p.x);
    //   Serial.print(", ");
    //   Serial.print(p.y);
    //   Serial.print(", ");
    //   Serial.print(p.r);
    //   Serial.print(", ");
    //   Serial.print(p.theta);
    //   Serial.print(", ");
    //   Serial.println(p.confidence);
    //   delay(1);
    // }
  }
  delay(5000);
}