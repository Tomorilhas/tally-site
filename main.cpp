#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>

// Caminho do arquivo de configuração
#define CONFIG_FILE "/config.json"

// Máximo de portas monitoradas
#define NUM_PORTAS 10

// Variáveis globais para configuração
char wifi_ssid[64] = "";
char wifi_pass[64] = "";
char mqtt_server[128] = "";
uint16_t mqtt_port = 8883;
char mqtt_user[64] = "";
char mqtt_pass[64] = "";
char porta_names[NUM_PORTAS][32] = {};
uint8_t porta_pins[NUM_PORTAS] = {};

// Estado anterior das portas
bool porta_last_state[NUM_PORTAS];

// MQTT
WiFiClientSecure espClient;
PubSubClient client(espClient);

void carregarConfig() {
    if (!SPIFFS.begin(true)) {
        Serial.println("Falha ao iniciar SPIFFS");
        return;
    }
    File file = SPIFFS.open(CONFIG_FILE, "r");
    if (!file) {
        Serial.println("Arquivo de configuração não encontrado!");
        return;
    }
    size_t size = file.size();
    std::unique_ptr<char[]> buf(new char[size + 1]);
    file.readBytes(buf.get(), size);
    buf.get()[size] = 0;

    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, buf.get());
    if (error) {
        Serial.println("Erro ao ler JSON!");
        return;
    }

    strlcpy(wifi_ssid, doc["wifi"]["ssid"] | "", sizeof(wifi_ssid));
    strlcpy(wifi_pass, doc["wifi"]["pass"] | "", sizeof(wifi_pass));
    strlcpy(mqtt_server, doc["mqtt"]["server"] | "", sizeof(mqtt_server));
    mqtt_port = doc["mqtt"]["port"] | 8883;
    strlcpy(mqtt_user, doc["mqtt"]["user"] | "", sizeof(mqtt_user));
    strlcpy(mqtt_pass, doc["mqtt"]["pass"] | "", sizeof(mqtt_pass));
    JsonArray pins = doc["porta_pins"].as<JsonArray>();
    JsonArray names = doc["porta_names"].as<JsonArray>();
    for (size_t i = 0; i < NUM_PORTAS; i++) {
        porta_pins[i] = pins[i] | 0;
        strlcpy(porta_names[i], names[i] | "", sizeof(porta_names[i]));
    }
    file.close();
}

void setup() {
    Serial.begin(115200);

    carregarConfig();

    // Conectar WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_pass);
    Serial.print("Conectando WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("OK");

    // MQTT SSL
    espClient.setInsecure(); // Para conexões TLS sem root CA

    client.setServer(mqtt_server, mqtt_port);

    // Inicializa portas
    for (size_t i = 0; i < NUM_PORTAS; i++) {
        pinMode(porta_pins[i], INPUT_PULLUP);
        porta_last_state[i] = digitalRead(porta_pins[i]);
    }
}

void mqttReconnect() {
    while (!client.connected()) {
        Serial.print("Conectando MQTT...");
        if (client.connect("esp32client", mqtt_user, mqtt_pass)) {
            Serial.println("OK");
        } else {
            Serial.print("Falha: ");
            Serial.println(client.state());
            delay(2000);
        }
    }
}

void publicarEstadoPorta(size_t idx, bool estado) {
    char topic[64];
    snprintf(topic, sizeof(topic), "externa/%d/status", idx + 1);
    const char *msg = estado ? "FECHADO" : "ABERTO";
    client.publish(topic, msg, true);
}

void loop() {
    if (!client.connected())
        mqttReconnect();
    client.loop();

    for (size_t i = 0; i < NUM_PORTAS; i++) {
        bool estado = digitalRead(porta_pins[i]);
        if (estado != porta_last_state[i]) {
            porta_last_state[i] = estado;
            publicarEstadoPorta(i, estado);
        }
    }
    delay(50);
}
