#pragma once
void setupWiFi();
void setupMQTT();
void handleMQTT();
void publicarEstadoPorta(size_t idx, bool estado);