# ESP32 Tally Modular System

Este projeto implementa um sistema Tally para produção audiovisual utilizando ESP32, MQTT e configuração via portal web. O código é modularizado para facilitar manutenção e expansão.

## Funcionalidades

- **Dual Core:** Utiliza os dois núcleos do ESP32 via FreeRTOS (monitoramento das portas em um core independente).
- **Configuração fácil:** Portal web via Access Point para inserção dos dados de WiFi, MQTT e portas, usando página HTML hospedada no próprio ESP.
- **Modularização:** Código dividido em arquivos separados:
  - `main.cpp`: inicialização, tasks e loop principal
  - `config_manager.h/cpp`: leitura da configuração (`config.json`)
  - `mqtt_manager.h/cpp`: conexão MQTT e publicação dos estados das portas
  - `port_monitor.h/cpp`: lógica de tally e monitoramento das portas
  - `config_portal.h/cpp`: portal de configuração via AP
- **Monitoramento de portas:** Cada dupla de portas corresponde a uma externa, ímpar (PGM/vermelho), par (PW/verde).

## Como funciona

1. **Configuração inicial**
   - Pressione o botão de configuração por 3 segundos.
   - O ESP32 entra em modo Access Point (`TallyConfigESP`, senha: `esp32config`).
   - Conecte-se pelo celular e preencha os dados no portal web.

2. **Operação**
   - O ESP32 conecta ao WiFi e MQTT.
   - Monitoramento das portas em tempo real (core 0).
   - Publicação do estado das portas via MQTT.
   - Se pressionar o botão de configuração novamente, retorna ao modo de configuração.

## Como pedir este código para uma IA

Veja o arquivo `prompt-tally-esp32.txt` para um prompt detalhado para IA.

## Organização dos arquivos

- `main.cpp`
- `config_manager.h` / `config_manager.cpp`
- `mqtt_manager.h` / `mqtt_manager.cpp`
- `port_monitor.h` / `port_monitor.cpp`
- `config_portal.h` / `config_portal.cpp`
- `config.json` (arquivo de configuração)
- `README.md`
- `prompt-tally-esp32.txt`

## Exemplo de arquivo de configuração (`config.json`)

```json
{
  "wifi": {
    "ssid": "MeuWiFi",
    "pass": "MinhaSenha"
  },
  "mqtt": {
    "server": "broker.mqtt.com",
    "port": 1883,
    "user": "usuario",
    "pass": "senha"
  },
  "porta_pins": [12,13,14,15,16,17,18,19,21,22],
  "porta_names": [
    "Porta 1","Porta 2","Porta 3","Porta 4","Porta 5",
    "Porta 6","Porta 7","Porta 8","Porta 9","Porta 10"
  ]
}