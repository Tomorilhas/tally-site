#pragma once
void carregarConfig();
extern char wifi_ssid[64], wifi_pass[64], mqtt_server[128], mqtt_user[64], mqtt_pass[64];
extern uint16_t mqtt_port;
extern uint8_t porta_pins[10];
extern char porta_names[10][32];