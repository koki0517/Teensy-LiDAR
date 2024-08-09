#include <Arduino.h>
#include <M5AtomS3.h>
#include "LD06.h"
#include "./LiDAR-Task/LiDAR-Task.h"

LiDAR::LD06 ld06(Serial2, 39);
TaskHandle_t thp[1];
QueueHandle_t xPointsQueue;

void setup() {
  auto cfg = M5.config();
  AtomS3.begin(cfg);
  Serial.begin(115200);
  ld06.init();
  disableCore0WDT();
  xPointsQueue = xQueueCreate(10, sizeof(std::vector<LiDAR::point>));
  xTaskCreatePinnedToCore(LiDAR_Task, "LiDAR_Task", 8192, NULL, 1, &thp[0], 0);
}

void loop(){
  delay(1);
}
