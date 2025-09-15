# 📡 Tally Site para Repórteres de Externa

Este projeto é um site simples e responsivo que atua como um **indicador de status (tally)** para repórteres que participam de transmissões ao vivo remotamente. Ele se conecta a um **broker MQTT** (HiveMQ Cloud) e muda de cor conforme o status da externa (preview, program ou idle).

---

## 🎯 Objetivo

Permitir que repórteres em campo saibam, em tempo real, se estão:
- 🟢 Em **preview** (prontos para entrar no ar)
- 🔴 Em **program** (ao vivo)
- ⚪️ Em **idle** (fora do ar)

---

## 🧠 Como Funciona

1. O site é acessado por celular, sem necessidade de login.
2. O repórter escolhe qual **Externa** está usando (Externa 1 a 5).
3. O site se inscreve no tópico MQTT correspondente:
   - `externa/1/status`, `externa/2/status`, etc.
4. O fundo da tela muda de cor conforme o valor recebido:
   - `"program"` → vermelho
   - `"preview"` → verde
   - `"idle"` → cinza
5. O site mostra o **status da conexão MQTT** e permite **voltar ao menu principal**.
6. O menu inicial exibe quais externas estão **em uso**.

---

## 🔐 Configuração MQTT (HiveMQ Cloud)

Este site está configurado para se conectar ao seu cluster MQTT seguro:

- **Broker**: `wss://2ca26723bea1429ab8136fa9d24ea28d.s1.eu.hivemq.cloud:8884/mqtt`
- **Porta**: `8884`
- **Protocolo**: WebSocket TLS


---

## 📱 Como Usar o Site

1. Acesse o site publicado via GitHub Pages:
   - `https://seu-usuario.github.io/tally-site/`
2. Escolha a externa correspondente.
3. O fundo da tela mudará conforme o status recebido via MQTT.

---

## 🧪 Testes com MQTT

Você pode testar o funcionamento do site sem o ESP32 usando ferramentas como:

### MQTT Web Client (HiveMQ)
- Acesse: [HiveMQ WebSocket Client](https://www.hivemq.com/demos/websocket-client/)
- Configure:
  - Host: `2ca26723bea1429ab8136fa9d24ea28d.s1.eu.hivemq.cloud`
  - Port: `8884`
  - Path: `/mqtt`
  - TLS: ativado
  - Username: `morilhas`
  - Password: `Pi3,1415`

### Tópicos para teste:
- `externa/1/status`
- `externa/2/status`
- ...

### Valores:
- `"program"` → tela vermelha
- `"preview"` → tela verde
- `"idle"` → tela cinza

---

## 🚀 Publicação com GitHub Pages

1. Crie um repositório público chamado `tally-site`.
2. Faça upload do arquivo `tally_site.html`.
3. Vá em **Settings > Pages**.
4. Selecione a branch principal e a pasta raiz.
5. O site estará disponível em `https://seu-usuario.github.io/tally-site/`.

---

## 🛠️ Futuras Melhorias

- Hospedagem local via ESP32
- Suporte a múltiplos usuários simultâneos
- Histórico de status por externa

---

## 📄 Licença

Este projeto é de uso livre para fins jornalísticos e educacionais.

