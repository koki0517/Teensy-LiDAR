#include <Arduino.h>
#include <M5AtomS3.h>
#include "LD06.h"
#include "./LiDAR-Task/LiDAR-Task.h"
#include <MadgwickAHRS.h>
Madgwick MadgwickFilter;

LiDAR::LD06 ld06(Serial2, 39);
TaskHandle_t thp[1];
QueueHandle_t xPointsQueue;

void setup()
{
  auto cfg = M5.config();
  AtomS3.begin(cfg);
  Serial.begin(115200);
  ld06.init();
  MadgwickFilter.begin(800);
  disableCore0WDT();
  xPointsQueue = xQueueCreate(10, sizeof(std::array<LiDAR::point, 12>));
  // xTaskCreatePinnedToCore(LiDAR_Task, "LiDAR_Task", 8192, NULL, 1, &thp[0], 0);
}

void loop()
{
  float roll, pitch, yaw;
  float val[6];
  M5.Imu.getGyro(&val[0], &val[1], &val[2]);
  M5.Imu.getAccel(&val[3], &val[4], &val[5]);
  MadgwickFilter.updateIMU(val[0], val[1], val[2], val[3], val[4], val[5]);
  roll = MadgwickFilter.getRoll();
  pitch = MadgwickFilter.getPitch();
  yaw = MadgwickFilter.getYaw();
  delay(1000 / 800);
  Serial.printf("roll: %f, pitch: %f, yaw: %f\r\n", roll, pitch, yaw);
}
