#include <Arduino.h>
#include "config_manager.h"
#include "mqtt_manager.h"
#include "port_monitor.h"
#include "config_portal.h"

#define CONFIG_BUTTON_PIN 0

TaskHandle_t taskMonitorHandle = NULL;

void setup() {
    Serial.begin(115200);
    carregarConfig();
    setupWiFi();
    setupMQTT();

    pinMode(CONFIG_BUTTON_PIN, INPUT_PULLUP);

    // Inicializa portas
    setupPortMonitor();

    // Cria task para monitorar as portas no core 0
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

void loop() {
    handleMQTT();

    // Rotina de configuração
    if (checkConfigButton()) {
        Serial.println("Entrando em modo configuração...");
        startConfigPortal();
        while (true) {
            handleConfigPortalLoop();
            delay(10);
        }
    }

    delay(10);
}