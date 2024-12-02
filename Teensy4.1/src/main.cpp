#include <Arduino.h>
#include <LD06.h>
#include <array>
#include <algorithm>

LiDAR::LD06 ld06({500,500}, Serial1);

#if defined(TEENSYDUINO)
static uint8_t s1bufsize[128];
#endif

void setup() {
  #if defined(TEENSYDUINO) // バッファーの拡張
  Serial1.addMemoryForRead(s1bufsize, sizeof(s1bufsize));
  analogWriteFrequency (23, 30000);
  analogWrite(23, 256*0.50);
  delay(110);
  analogWrite(23, 256*0.3);
  #endif

  Serial.begin(115200);
  ld06.init();
}

void loop() {
  std::vector<LiDAR::point> points = ld06.read(true);
  for (const auto& p : points) {
    Serial.print(p.x);
    Serial.print(", ");
    Serial.print(p.y);
    Serial.print(", ");
    Serial.println(p.theta);
  }
}