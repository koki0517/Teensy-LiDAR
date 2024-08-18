#include <Arduino.h>
#include <M5AtomS3.h>
#include "LD06.h"
#include "./LiDAR-Task/LiDAR-Task.h"

LiDAR::LD06 ld06(Serial2, 39);
TaskHandle_t thp[1];
SemaphoreHandle_t mutex = NULL;
std::array<LiDAR::point,456> points;

void setup()
{
  auto cfg = M5.config();
  AtomS3.begin(cfg);
  Serial.begin(115200);
  ld06.init();
  disableCore0WDT();
  mutex = xSemaphoreCreateMutex();
  //xTaskCreatePinnedToCore(LiDAR_Task, "LiDAR_Task", 16384, NULL, 1, &thp[0], 0);
}

void loop(){
  for (int i = 0; i < 3; i++) {
    ld06.readSingle(); // 3回くらい連続で読めばバッファも安定するはず
  }
  while (1) {
    // 360度分の点群を取得
    std::array<LiDAR::point,456> _points;
    for (int i = 0; i < 38; i++) {
      std::array<LiDAR::point,12> p = ld06.readSingle();
      for (int j = 0; j < 12; j++){
        _points[i*12+j] = p[j];
      }
    }
    BaseType_t result = xSemaphoreTake(mutex,0);
    if (result == pdTRUE){
      Serial.println("success");
    }
    else Serial.println("fail");
    for (int i = 0; i < 456; i++){
      points[i] = _points[i];
    }
    xSemaphoreGive(mutex);
    // for (const auto& p : ld06.PointCloud) {
    //   Serial.print(p.r);
    //   Serial.print(", ");
    //   Serial.println(p.theta);
    // }
  }
}
