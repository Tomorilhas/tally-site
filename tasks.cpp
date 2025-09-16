#include <Arduino.h>
#include "port_monitor.h"

TaskHandle_t taskMonitorHandle = NULL;

void startMonitorTask() {
    xTaskCreatePinnedToCore(
        monitorPortasTask,
        "MonitorPortas",
        4096,
        NULL,
        1,
        &taskMonitorHandle,
        0 // core 0
    );
}