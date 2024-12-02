#include <Arduino.h>
#include <M5AtomS3.h>
#include "LD06.h"
#include "./LiDAR-Task/LiDAR-Task.h"

LiDAR::LD06 ld06({0,0}, Serial2, 39);
TaskHandle_t thp[1];
SemaphoreHandle_t mutex = NULL;

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
  
  while (1) {
    // 360度分の点群を取得
    for (int i = 0; i < 3; i++) {
      ld06.readSingle(); // 3回くらい連続で読めばバッファも安定するはず
    }
    BaseType_t result = xSemaphoreTake(mutex,0);
    if (result == pdTRUE){
      Serial.println("success");
    }
    else Serial.println("fail");
    xSemaphoreGive(mutex);
  }
}
