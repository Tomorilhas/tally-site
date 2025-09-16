#include <WiFi.h>
#include <PubSubClient.h>
#include "config_manager.h"
#include "port_monitor.h"

WiFiClientSecure espClient;
PubSubClient client(espClient);

void setupWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_pass);
    while (WiFi.status() != WL_CONNECTED) delay(500);
    espClient.setInsecure();
}

void setupMQTT() {
    client.setServer(mqtt_server, mqtt_port);
}

void handleMQTT() {
    if (!client.connected()) {
        while (!client.connected()) {
            client.connect("esp32client", mqtt_user, mqtt_pass);
            delay(2000);
        }
    }
    client.loop();
}

void publicarEstadoPorta(size_t idx, bool estado) {
    int externa_num;
    const char* cor_tally;
    getExternaTally(idx, externa_num, cor_tally);
    char topic[64];
    snprintf(topic, sizeof(topic), "externa/%d/status", externa_num);
    const char *msg = (estado == LOW) ? cor_tally : "BY";
    client.publish(topic, msg, true);
}