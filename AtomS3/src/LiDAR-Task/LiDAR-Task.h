#pragma once
#include <Arduino.h>
#include <M5AtomS3.h>
#include "LD06.h"
#include "esp_task_wdt.h"

extern LiDAR::LD06 ld06;

void LiDAR_Task(void *pvParameters);