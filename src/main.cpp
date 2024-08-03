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
  ld06.update(true);
  delay(1);
}