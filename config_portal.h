#include <WiFi.h>
#include <WebServer.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

// Defina o pino do botão de configuração
#define CONFIG_BUTTON_PIN 0 // Troque para o pino desejado

// Credenciais do Access Point
const char* ap_ssid = "TallyConfigESP";
const char* ap_pass = "esp32config";

// Porta do servidor web
WebServer server(80);

// HTML simples para configuração
const char* config_html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Configuração ESP32</title>
  <style>
    body { font-family: Arial; margin:40px; }
    input, select { margin-bottom: 12px; width: 300px; padding: 5px; }
    label { display: block; margin-top: 18px; font-weight: bold; }
    button { padding: 8px 20px; }
  </style>
</head>
<body>
  <h2>Configuração do ESP32 Tally</h2>
  <form method="POST" action="/save">
    <label>SSID WiFi:</label>
    <input name="ssid" required>
    <label>Senha WiFi:</label>
    <input name="wifipass" required type="password">
    <label>Servidor MQTT:</label>
    <input name="mqtt_server" required>
    <label>Porta MQTT:</label>
    <input name="mqtt_port" required type="number" value="8883">
    <label>Usuário MQTT:</label>
    <input name="mqtt_user" required>
    <label>Senha MQTT:</label>
    <input name="mqtt_pass" required type="password">
    <label>Pinos das portas (separados por vírgula):</label>
    <input name="porta_pins" required value="12,13,14,15,16,17,18,19,21,22">
    <label>Nomes das portas (separados por vírgula):</label>
    <input name="porta_names" required value="Porta 1,Porta 2,Porta 3,Porta 4,Porta 5,Porta 6,Porta 7,Porta 8,Porta 9,Porta 10">
    <button type="submit">Salvar Configuração</button>
  </form>
  <br>
  <a href="https://viewfinder.tv.br" target="_blank">Ir para ViewFinder</a>
</body>
</html>
)rawliteral";

// Função para salvar config.json
void handleSave() {
  DynamicJsonDocument doc(2048);

  doc["wifi"]["ssid"]     = server.arg("ssid");
  doc["wifi"]["pass"]     = server.arg("wifipass");
  doc["mqtt"]["server"]   = server.arg("mqtt_server");
  doc["mqtt"]["port"]     = server.arg("mqtt_port").toInt();
  doc["mqtt"]["user"]     = server.arg("mqtt_user");
  doc["mqtt"]["pass"]     = server.arg("mqtt_pass");

  // Processa pinos e nomes
  JsonArray pins = doc.createNestedArray("porta_pins");
  String pins_str = server.arg("porta_pins");
  int last = 0;
  for (int i = 0; i < pins_str.length(); i++) {
    if (pins_str[i] == ',' || i == pins_str.length()-1) {
      String val = pins_str.substring(last, (i == pins_str.length()-1) ? i+1 : i);
      pins.add(val.toInt());
      last = i+1;
    }
  }
  JsonArray names = doc.createNestedArray("porta_names");
  String names_str = server.arg("porta_names");
  last = 0;
  for (int i = 0; i < names_str.length(); i++) {
    if (names_str[i] == ',' || i == names_str.length()-1) {
      String val = names_str.substring(last, (i == names_str.length()-1) ? i+1 : i);
      names.add(val);
      last = i+1;
    }
  }

  File file = SPIFFS.open("/config.json", "w");
  if (!file) {
    server.send(500, "text/plain", "Falha ao salvar arquivo!");
    return;
  }
  serializeJson(doc, file);
  file.close();
  server.send(200, "text/html", "<h3>Configuração salva!<br>Reinicie o ESP.</h3>");
}

// Função para servir o formulário
void handleRoot() {
  server.send(200, "text/html", config_html);
}

// Inicializa Access Point e servidor web
void startConfigPortal() {
  WiFi.disconnect(true);
  delay(500);
  WiFi.softAP(ap_ssid, ap_pass);

  if (!SPIFFS.begin(true)) {
    Serial.println("Falha ao iniciar SPIFFS");
  }

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.on("/", handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.begin();
  Serial.println("Servidor de configuração iniciado.");
}

// Chama esta função no loop principal enquanto estiver no modo config
void handleConfigPortalLoop() {
  server.handleClient();
}

// Detecta botão de configuração pressionado por 3 segundos
bool checkConfigButton() {
  static unsigned long pressedTime = 0;
  static bool lastState = HIGH;
  bool state = digitalRead(CONFIG_BUTTON_PIN);

  if (state == LOW && lastState == HIGH) {
    pressedTime = millis();
  }
  if (state == LOW && (millis() - pressedTime > 3000)) {
    lastState = state;
    return true;
  }
  lastState = state;
  return false;
}