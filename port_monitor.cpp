#include <Arduino.h>
#include "config_manager.h"
#include "mqtt_manager.h"
#include "port_monitor.h"

bool porta_last_state[10] = {};

void setupPortMonitor() {
    for (size_t i = 0; i < 10; i++) {
        pinMode(porta_pins[i], INPUT_PULLUP);
        porta_last_state[i] = digitalRead(porta_pins[i]);
    }
}

void getExternaTally(size_t porta_idx, int &externa_num, const char* &cor_tally) {
    externa_num = (porta_idx / 2) + 1;
    cor_tally = ((porta_idx % 2) == 0) ? "PGM" : "PW";
}

void monitorPortasTask(void *pvParameters) {
    while (true) {
        for (size_t i = 0; i < 10; i++) {
            bool estado = digitalRead(porta_pins[i]);
            if (estado != porta_last_state[i]) {
                porta_last_state[i] = estado;
                publicarEstadoPorta(i, estado);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}