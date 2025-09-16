#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>
#include "config_manager.h"

char wifi_ssid[64] = "", wifi_pass[64] = "", mqtt_server[128] = "", mqtt_user[64] = "", mqtt_pass[64] = "";
uint16_t mqtt_port = 8883;
uint8_t porta_pins[10] = {};
char porta_names[10][32] = {};

void carregarConfig() {
    if (!SPIFFS.begin(true)) return;
    File file = SPIFFS.open("/config.json", "r");
    if (!file) return;
    size_t size = file.size();
    std::unique_ptr<char[]> buf(new char[size + 1]);
    file.readBytes(buf.get(), size);
    buf.get()[size] = 0;
    DynamicJsonDocument doc(2048);
    if (deserializeJson(doc, buf.get())) return;
    strlcpy(wifi_ssid, doc["wifi"]["ssid"] | "", sizeof(wifi_ssid));
    strlcpy(wifi_pass, doc["wifi"]["pass"] | "", sizeof(wifi_pass));
    strlcpy(mqtt_server, doc["mqtt"]["server"] | "", sizeof(mqtt_server));
    mqtt_port = doc["mqtt"]["port"] | 8883;
    strlcpy(mqtt_user, doc["mqtt"]["user"] | "", sizeof(mqtt_user));
    strlcpy(mqtt_pass, doc["mqtt"]["pass"] | "", sizeof(mqtt_pass));
    JsonArray pins = doc["porta_pins"].as<JsonArray>();
    JsonArray names = doc["porta_names"].as<JsonArray>();
    for (size_t i = 0; i < 10; i++) {
        porta_pins[i] = pins[i] | 0;
        strlcpy(porta_names[i], names[i] | "", sizeof(porta_names[i]));
    }
    file.close();
}