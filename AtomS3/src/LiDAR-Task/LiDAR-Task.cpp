#include "LiDAR-Task.h"

void LiDAR_Task(void *pvParameters) {
  while (1) {
    ld06.read(true);
    Serial.println("success");
    // for (const auto& p : ld06.PointCloud) {
    //   Serial.print(p.r);
    //   Serial.print(", ");
    //   Serial.println(p.theta);
    // }
  }
}