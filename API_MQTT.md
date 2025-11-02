# 📡 API MQTT - REFERÊNCIA RÁPIDA

## 🌐 Configuração do Broker

```javascript
// Conexão Web (WebSocket)
const client = mqtt.connect('wss://broker.hivemq.com:8884/mqtt');

// Conexão ESP32 (TCP)
const char* MQTT_HOST = "broker.hivemq.com";
const uint16_t MQTT_PORT = 1883;
```

---

## 📤 Tópicos de Publicação (ESP32 → Web)

### Botões

```cpp
// Quando qualquer botão é pressionado
Tópico: IFCE_caua/botoes/BOTAO_[1-6]
Payload: "PRESSIONADO"
```

**Exemplo:**
```cpp
mqtt.publish("IFCE_caua/botoes/BOTAO_4", "PRESSIONADO");
```

### Status do LED

```cpp
// Após mudar cor do LED
Tópico: IFCE_caua/led_status
Payload: "RGB(255,0,128)"
```

### Status do Buzzer

```cpp
// Quando buzzer toca
Tópico: IFCE_caua/buzzer_status
Payload: "Tocando 1000Hz"
```

### Créditos

```cpp
// Atualização dos créditos do jogador
Tópico: IFCE_caua/credits
Payload: "1000"
```

**Exemplo:**
```cpp
mqtt.publish("IFCE_caua/credits", String(playerCredits).c_str());
```

### Estatísticas

```cpp
// Estatísticas do casino
Tópico: IFCE_caua/stats
Payload: "jogos,vitorias,apostas_totais,maior_premio"
```

**Exemplo:**
```cpp
String stats = String(gamesPlayed) + "," + String(totalWins) + "," + 
               String(totalBets) + "," + String(biggestWin);
mqtt.publish("IFCE_caua/stats", stats.c_str());
```

### Conquistas

```cpp
// Quando uma conquista é desbloqueada
Tópico: IFCE_caua/achievements
Payload: "nome_da_conquista"
```

**Exemplo:**
```cpp
mqtt.publish("IFCE_caua/achievements", "Primeiro Passo");
```

### Status do Casino

```cpp
// Estado de bloqueio/desbloqueio
Tópico: IFCE_caua/casino_status
Payload: "UNLOCKED" | "LOCKED" | "INSUFFICIENT_CREDITS"
```

### Sensores

```cpp
// Temperatura (DHT11)
Tópico: IFCE_caua/temperatura
Payload: "25.5"

// Umidade (DHT11)
Tópico: IFCE_caua/umidade
Payload: "60.0"

// Luminosidade (LDR)
Tópico: IFCE_caua/luminosidade
Payload: "75"  // Porcentagem (0-100)
```

---

## 📥 Tópicos de Subscrição (Web → ESP32)

### Controle do LED RGB

```javascript
// Formato: "R,G,B" (valores 0-255)
client.publish('IFCE_caua/led_control', '255,0,0');  // Vermelho
client.publish('IFCE_caua/led_control', '0,255,0');  // Verde
client.publish('IFCE_caua/led_control', '0,0,255');  // Azul
client.publish('IFCE_caua/led_control', '255,215,0'); // Dourado
```

**Cores Predefinidas:**
| Cor | RGB | Uso |
|-----|-----|-----|
| Vermelho | `255,0,0` | Derrota/Erro |
| Verde | `0,255,0` | Vitória |
| Azul | `0,0,255` | Navegação |
| Dourado | `255,215,0` | Jackpot/Conquista |
| Magenta | `255,0,255` | Aviso |
| Ciano | `0,255,255` | Informação |
| Branco | `255,255,255` | Neutro |

### Controle do Buzzer

```javascript
// Frequência em Hz (50-5000)
client.publish('IFCE_caua/buzzer_control', '1000');  // Beep 1kHz
client.publish('IFCE_caua/buzzer_control', '440');   // Lá (A4)
client.publish('IFCE_caua/buzzer_control', '2000');  // Agudo
```

**Frequências Musicais Comuns:**
| Nota | Frequência (Hz) |
|------|-----------------|
| Dó (C4) | 262 |
| Ré (D4) | 294 |
| Mi (E4) | 330 |
| Fá (F4) | 349 |
| Sol (G4) | 392 |
| Lá (A4) | 440 |
| Si (B4) | 494 |
| Dó (C5) | 523 |

### Mensagem no Display

```javascript
// Mostra mensagem rolando no OLED
client.publish('IFCE_caua/display_msg', 'Bem-vindo ao Casino!');
client.publish('IFCE_caua/display_msg', 'Voce ganhou 500 creditos!');
```

### Apostas e Ganhos

```javascript
// Fazer uma aposta
client.publish('IFCE_caua/bet', '50');  // Aposta 50 créditos

// Registrar vitória
client.publish('IFCE_caua/win', '100');  // Ganhou 100 créditos
```

### Roleta

```javascript
// Iniciar giro da roleta
client.publish('IFCE_caua/roulette_spin', '1');

// Enviar resultado
client.publish('IFCE_caua/roulette_result', 'VERMELHO');
client.publish('IFCE_caua/roulette_result', 'VERDE');
client.publish('IFCE_caua/roulette_result', 'AZUL');
```

---

## 💻 Exemplos de Código

### JavaScript (Web) - Conexão Completa

```javascript
const BROKER = 'wss://broker.hivemq.com:8884/mqtt';
const client = mqtt.connect(BROKER);

let credits = 1000;

client.on('connect', () => {
  console.log('MQTT Conectado!');
  
  // Subscreve aos tópicos relevantes
  client.subscribe('IFCE_caua/botoes/#');
  client.subscribe('IFCE_caua/credits');
  client.subscribe('IFCE_caua/casino_status');
  client.subscribe('IFCE_caua/achievements');
});

client.on('message', (topic, message) => {
  const msg = message.toString();
  console.log(`${topic}: ${msg}`);
  
  // Processa mensagens
  if (topic.startsWith('IFCE_caua/botoes/')) {
    const btnNum = topic.split('_')[2];
    handleButton(parseInt(btnNum));
  }
  
  if (topic === 'IFCE_caua/credits') {
    credits = parseInt(msg);
    updateUI();
  }
  
  if (topic === 'IFCE_caua/casino_status') {
    if (msg === 'LOCKED') {
      alert('🔒 Casino bloqueado!');
    } else if (msg === 'UNLOCKED') {
      console.log('✅ Casino desbloqueado!');
    }
  }
  
  if (topic === 'IFCE_caua/achievements') {
    showAchievement(msg);
  }
});

// Função para fazer aposta
function placeBet(amount) {
  if (credits >= amount) {
    client.publish('IFCE_caua/bet', amount.toString());
    credits -= amount;
  }
}

// Função para registrar vitória
function registerWin(amount) {
  client.publish('IFCE_caua/win', amount.toString());
  credits += amount;
  
  // LED verde para vitória
  client.publish('IFCE_caua/led_control', '0,255,0');
  
  // Som de vitória
  client.publish('IFCE_caua/buzzer_control', '1200');
}

// Função para controlar LED
function setLED(r, g, b) {
  client.publish('IFCE_caua/led_control', `${r},${g},${b}`);
}

// Função para tocar som
function playSound(frequency, duration = 500) {
  client.publish('IFCE_caua/buzzer_control', frequency.toString());
}
```

### Arduino (ESP32) - Subscrição

```cpp
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  msg.trim();
  
  Serial.printf("[MQTT] %s => %s\n", topic, msg.c_str());
  
  // Controle do LED
  if (String(topic) == TOPIC_LED_CONTROL) {
    int r, g, b;
    sscanf(msg.c_str(), "%d,%d,%d", &r, &g, &b);
    setLEDColor(r, g, b);
  }
  
  // Controle do Buzzer
  else if (String(topic) == TOPIC_BUZZER_CONTROL) {
    int freq = msg.toInt();
    if (freq >= 50 && freq <= 5000) {
      tone(BUZZER_PIN, freq, 500);
    }
  }
  
  // Mensagem no Display
  else if (String(topic) == TOPIC_DISPLAY_MSG) {
    showScrollingMessage(msg);
  }
  
  // Aposta
  else if (String(topic) == TOPIC_BET) {
    unsigned long betAmount = msg.toInt();
    if (betAmount <= playerCredits) {
      playerCredits -= betAmount;
      totalBets += betAmount;
      gamesPlayed++;
      publishCredits();
      saveData();
    }
  }
  
  // Vitória
  else if (String(topic) == TOPIC_WIN) {
    unsigned long winAmount = msg.toInt();
    playerCredits += winAmount;
    totalWins++;
    if (winAmount > biggestWin) {
      biggestWin = winAmount;
    }
    publishCredits();
    saveData();
  }
}

void setup() {
  // ... inicialização ...
  
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(mqttCallback);
  
  // Subscreve aos tópicos
  if (mqtt.connect(clientId.c_str())) {
    mqtt.subscribe(TOPIC_LED_CONTROL);
    mqtt.subscribe(TOPIC_BUZZER_CONTROL);
    mqtt.subscribe(TOPIC_DISPLAY_MSG);
    mqtt.subscribe(TOPIC_BET);
    mqtt.subscribe(TOPIC_WIN);
  }
}
```

---

## 🎮 Fluxo de um Jogo Típico

### 1. Jogador Faz Aposta

```javascript
// Web → ESP32
client.publish('IFCE_caua/bet', '50');
```

```cpp
// ESP32 processa
playerCredits -= 50;
totalBets += 50;
gamesPlayed++;
```

### 2. Jogo Executa

```javascript
// Animações, cálculos, etc.
// LED animado
client.publish('IFCE_caua/led_control', '255,0,0');
setTimeout(() => {
  client.publish('IFCE_caua/led_control', '0,255,0');
}, 500);
```

### 3. Resultado

```javascript
// Se ganhou
if (won) {
  const winAmount = betAmount * multiplier;
  client.publish('IFCE_caua/win', winAmount.toString());
  
  // Feedback
  client.publish('IFCE_caua/led_control', '0,255,0');  // Verde
  client.publish('IFCE_caua/buzzer_control', '1200');  // Beep agudo
  client.publish('IFCE_caua/display_msg', `GANHOU ${winAmount}!`);
}
```

```cpp
// ESP32 atualiza
playerCredits += winAmount;
totalWins++;
publishCredits();  // Envia créditos atualizados
checkAchievements();  // Verifica conquistas
saveData();  // Salva na EEPROM
```

### 4. ESP32 Publica Status

```cpp
// ESP32 → Web
mqtt.publish(TOPIC_CREDITS, String(playerCredits).c_str());
mqtt.publish(TOPIC_STATS, statsString.c_str());

// Se desbloqueou conquista
if (newAchievement) {
  mqtt.publish(TOPIC_ACHIEVEMENTS, achievementName);
}
```

---

## 🔍 Debugging

### Monitorar Tópicos (MQTT Explorer)

Use MQTT Explorer para visualizar todos os tópicos:
- Download: http://mqtt-explorer.com/

**Configuração:**
- Host: `broker.hivemq.com`
- Port: `1883`
- Protocol: `mqtt://`

### Console do Navegador

```javascript
// Habilite logging
client.on('message', (topic, message) => {
  console.log(`📨 ${topic}: ${message.toString()}`);
});

client.on('error', (err) => {
  console.error('❌ MQTT Error:', err);
});
```

### Serial Monitor do Arduino

```cpp
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) msg += (char)payload[i];
  
  // Log detalhado
  Serial.println("=== MQTT MESSAGE ===");
  Serial.printf("Topic: %s\n", topic);
  Serial.printf("Payload: %s\n", msg.c_str());
  Serial.printf("Length: %d\n", length);
  Serial.println("====================");
}
```

---

## 💡 Melhores Práticas

### 1. QoS (Quality of Service)

```cpp
// QoS 0: Fire and forget (padrão, mais rápido)
mqtt.publish(topic, msg);

// QoS 1: At least once (garantido, mais lento)
mqtt.publish(topic, msg, false, 1);
```

### 2. Retained Messages

```cpp
// Mensagem retida (último valor fica salvo no broker)
mqtt.publish(topic, msg, true);  // retained = true
```

### 3. Wildcards na Subscrição

```javascript
// Subscreve a todos os botões
client.subscribe('IFCE_caua/botoes/#');

// Subscreve a todos os tópicos
client.subscribe('IFCE_caua/#');

// Multi-level wildcard
client.subscribe('+/botoes/+');
```

### 4. Reconexão Automática

```javascript
client.on('close', () => {
  console.log('Desconectado. Reconectando...');
  setTimeout(() => {
    client.reconnect();
  }, 3000);
});
```

```cpp
void loop() {
  if (!mqtt.connected()) {
    ensureMqtt();  // Reconecta automaticamente
  }
  mqtt.loop();
}
```

---

## 📊 Resumo dos Tópicos

| Tópico | Direção | Formato | Descrição |
|--------|---------|---------|-----------|
| `botoes/BOTAO_X` | ESP32 → Web | String | Botão pressionado |
| `led_control` | Web → ESP32 | "R,G,B" | Controlar LED RGB |
| `led_status` | ESP32 → Web | "RGB(...)" | Status do LED |
| `buzzer_control` | Web → ESP32 | "freq_Hz" | Tocar buzzer |
| `buzzer_status` | ESP32 → Web | String | Status do buzzer |
| `display_msg` | Web → ESP32 | String | Mensagem no OLED |
| `credits` | ESP32 → Web | "valor" | Créditos atuais |
| `bet` | Web → ESP32 | "valor" | Fazer aposta |
| `win` | Web → ESP32 | "valor" | Registrar vitória |
| `casino_status` | Bidirecional | String | Status do casino |
| `stats` | ESP32 → Web | CSV | Estatísticas |
| `achievements` | ESP32 → Web | String | Conquista desbloqueada |
| `temperatura` | ESP32 → Web | "valor" | Sensor DHT11 |
| `umidade` | ESP32 → Web | "valor" | Sensor DHT11 |
| `luminosidade` | ESP32 → Web | "0-100" | Sensor LDR |
| `roulette_spin` | Web → ESP32 | "1" | Girar roleta |
| `roulette_result` | Web → ESP32 | String | Resultado da roleta |

---

**Desenvolvido com ❤️ por IkkiKuuro e Cauã Alessandro**

🎰 **BOA SORTE NO CASINO!** 🎰
