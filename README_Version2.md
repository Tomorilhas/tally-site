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
3. O site se inscreve nos tópicos MQTT correspondentes:
   - `externa/1/status`, `externa/2/status`, ... para status de cada externa
   - `externa/1/clients`, `externa/2/clients`, ... para presença de outros usuários
4. O fundo da tela muda de cor conforme o valor recebido:
   - `"PGM"` → vermelho (**ao vivo**)
   - `"PW"` → verde (**pronto para entrar**)
   - `"BY"` → cinza (**fora do ar**)
5. O site mostra o **status da conexão MQTT** e permite **voltar ao menu principal**.
6. Os botões do menu principal exibem a cor do status de cada externa em tempo real.
7. O menu inicial também indica **(em uso)** se outro usuário estiver visualizando aquela externa no momento.
8. Quando um usuário entra em uma externa, a tela do dispositivo permanece sempre ativa (não apaga) enquanto ele estiver naquela externa (em navegadores compatíveis).

---

## 🔐 Configuração MQTT (HiveMQ Cloud)

Este site está configurado para se conectar a um cluster MQTT seguro:

- **Broker**: `wss://...hivemq.cloud:8884/mqtt`
- **Porta**: `8884`
- **Protocolo**: WebSocket TLS
- **Usuário**: (oculto)
- **Senha**: (oculta)

---

## 📱 Como Usar o Site

1. Acesse o site publicado via GitHub Pages:
   - `https://seu-usuario.github.io/tally-site/`
2. Escolha a externa correspondente.
3. O fundo da tela mudará conforme o status recebido via MQTT.
4. Se você e outra pessoa acessarem a mesma externa ao mesmo tempo, aparecerá **(em uso)** no botão correspondente no menu.

---

## 🧪 Testes com MQTT

Você pode testar o funcionamento do site sem o ESP32 usando ferramentas como:

### MQTT Web Client (HiveMQ)
- Acesse: [HiveMQ WebSocket Client](https://www.hivemq.com/demos/websocket-client/)
- Configure:
  - Host: `...hivemq.cloud`
  - Port: `8884`
  - Path: `/mqtt`
  - TLS: ativado
  - Username/Senha: (os mesmos configurados no código, ocultos neste README)

### Tópicos para teste:
- `externa/1/status`
- `externa/2/status`
- `externa/1/clients` (para simular presença de outros usuários)
- ...

### Valores para status:
- `"PGM"` → tela vermelha
- `"PW"` → tela verde
- `"BY"` → tela cinza

---

## 🚀 Publicação com GitHub Pages

1. Crie um repositório público chamado `tally-site`.
2. Faça upload dos arquivos do site.
3. Vá em **Settings > Pages**.
4. Selecione a branch principal e a pasta raiz.
5. O site estará disponível em `https://seu-usuario.github.io/tally-site/`.

---

## 🛠️ Funcionalidades Recentes

- Indicação de presença simultânea de usuários na mesma externa (sinalização “em uso” via MQTT)
- Botões do menu principal sempre refletem o status atual da externa (cor)
- Tela do celular permanece sempre ativa ao entrar em uma externa (Screen Wake Lock API)
- Status aparece imediatamente ao entrar em uma externa (mensagem retained do MQTT)

---

## 📄 Licença

Este projeto é de uso livre para fins jornalísticos e educacionais.