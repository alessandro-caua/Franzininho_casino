# 🎰 FRANZININHO CASINO - Documentação Completa

![Casino Status](https://img.shields.io/badge/Status-Active-success)
![Version](https://img.shields.io/badge/Version-2.0-blue)
![License](https://img.shields.io/badge/License-MIT-green)

## 📋 Índice

- [Sobre o Projeto](#sobre-o-projeto)
- [Características](#características)
- [Hardware Necessário](#hardware-necessário)
- [Instalação e Setup](#instalação-e-setup)
- [Sistema de Segurança](#sistema-de-segurança)
- [Jogos Disponíveis](#jogos-disponíveis)
- [API MQTT](#api-mqtt)
- [Sistema de Economia](#sistema-de-economia)
- [Conquistas](#conquistas)
- [Guia de Uso](#guia-de-uso)
- [Troubleshooting](#troubleshooting)

---

## 🎮 Sobre o Projeto

O **Franzininho Casino** é um projeto IoT interativo que combina hardware (ESP32/Franzininho WiFi) com interfaces web modernas para criar uma experiência de cassino completa. O projeto utiliza protocolo MQTT para comunicação em tempo real entre a placa e as aplicações web.

### Autores
- **IkkiKuuro**
- **Cauã Alessandro**

### Tecnologias Utilizadas
- **Hardware:** ESP32/Franzininho WiFi
- **Comunicação:** MQTT (HiveMQ Broker)
- **Frontend:** HTML5, CSS3, JavaScript
- **Bibliotecas:** PubSubClient, Adafruit_GFX, Chart.js
- **Sensores:** DHT11, LDR, Display OLED SSD1306

---

## ✨ Características

### 🔐 Sistema de Segurança (3 Camadas)
- **Camada 1:** MQTT funciona normalmente (casino completamente oculto)
- **Camada 2:** Ativação via mensagem "casino hub" + LED vermelho
- **Camada 3:** Konami Code nos botões físicos (↑↑↓↓←→←→)
- Música do baú de Zelda ao desbloquear
- Animação LED RGB durante desbloqueio
- **📖 [Guia completo de ativação](ATIVACAO_CASINO.md)**

### 🎰 Jogos Disponíveis
1. **Slot Machine (Caça-níqueis)** 🎰
   - 6 símbolos diferentes
   - Multiplicadores até x100
   - Animações de vitória com partículas

2. **Blackjack 21** 🃏
   - Jogo clássico contra o dealer
   - Sistema de apostas
   - Contagem de cartas visual

3. **Roleta RGB** 🎡
   - Apostas em cores (Vermelho, Verde, Azul, Branco)
   - Animação LED sincronizada
   - Multiplicadores atrativos

4. **Roleta Russa** 💀
   - Escolha seu alvo
   - Tensão crescente
   - Efeitos sonoros

5. **Dados/Craps** 🎲
   - 6 tipos de apostas diferentes
   - Multiplicadores até x30
   - Estatísticas em tempo real

### 💰 Sistema de Economia
- **Créditos iniciais:** 1000
- **Persistência de dados** via Preferences (EEPROM)
- **Histórico de apostas** completo
- **Estatísticas detalhadas**

### 🏆 Sistema de Conquistas
- 6 conquistas diferentes
- Feedback sonoro e visual
- Salvas permanentemente
- Notificações em tempo real

### 📊 Análise de Dados
- **Gráficos Chart.js** em todas as páginas
- Histórico de apostas
- Taxa de vitória
- Distribuição de resultados

### 🎨 Interface Moderna
- **Design Cyberpunk/Neon**
- Modo claro/escuro (theme toggle)
- Animações suaves
- Efeitos de partículas
- Responsivo

---

## 🛠️ Hardware Necessário

### Componentes
| Item | Descrição | Quantidade |
|------|-----------|------------|
| **Franzininho WiFi** ou **ESP32** | Microcontrolador principal | 1 |
| **Display OLED SSD1306** | 128x64 pixels, I2C | 1 |
| **LED RGB** | Comum ou Anodo/Cátodo | 1 |
| **Buzzer Passivo** | Para efeitos sonoros | 1 |
| **Botões** | Push buttons | 6 |
| **Sensor DHT11** | Temperatura e umidade | 1 |
| **LDR** | Sensor de luminosidade | 1 |
| **Resistores** | 10kΩ (pull-up) e 330Ω (LED) | Vários |

### Pinagem

```cpp
// LED RGB
LED_R_PIN = 14
LED_G_PIN = 13
LED_B_PIN = 12

// Buzzer
BUZZER_PIN = 17

// Botões (com pull-up interno)
BTN1_PIN = 2   // CIMA
BTN2_PIN = 3   // BAIXO
BTN3_PIN = 4   // VOLTAR
BTN4_PIN = 5   // CONFIRMAR
BTN5_PIN = 6   // EXTRA1
BTN6_PIN = 7   // EXTRA2

// Display OLED (I2C)
OLED_SDA_PIN = 8
OLED_SCL_PIN = 9

// Sensores
DHTPIN = 15      // DHT11
LDR_PIN = 1      // LDR (ADC)
```

### Esquema de Conexão

```
ESP32/Franzininho WiFi
├── GPIO 14 ──► LED RGB (Red)
├── GPIO 13 ──► LED RGB (Green)
├── GPIO 12 ──► LED RGB (Blue)
├── GPIO 17 ──► Buzzer (+)
├── GPIO 2-7 ──► Botões (6x) com pull-up
├── GPIO 8 (SDA) ──► OLED SDA
├── GPIO 9 (SCL) ──► OLED SCL
├── GPIO 15 ──► DHT11 Data
└── GPIO 1 (ADC) ──► LDR
```

---

## 📦 Instalação e Setup

### 1. Preparação do Ambiente Arduino IDE

```bash
# Instale a Arduino IDE (versão 2.0+ recomendada)
# Adicione o suporte para ESP32:
# File > Preferences > Additional Board Manager URLs:
https://dl.espressif.com/dl/package_esp32_index.json
```

### 2. Bibliotecas Necessárias

Instale via **Library Manager** (Sketch > Include Library > Manage Libraries):

```
- PubSubClient (v2.8+)
- DHT sensor library (v1.4+)
- Adafruit GFX Library (v1.11+)
- Adafruit SSD1306 (v2.5+)
- WiFi (incluída no ESP32 core)
- Preferences (incluída no ESP32 core)
```

### 3. Configuração do Código

Edite o arquivo `codigo_esp32_atualizado.ino`:

```cpp
// Configure suas credenciais Wi-Fi
const char* WIFI_SSID = "SEU_WIFI";
const char* WIFI_PASS = "SUA_SENHA";

// Opcionalmente, mude o broker MQTT
const char* MQTT_HOST = "broker.hivemq.com";
const uint16_t MQTT_PORT = 1883;
```

### 4. Upload do Código

1. Conecte o ESP32 via USB
2. Selecione a placa: **Tools > Board > ESP32 Dev Module**
3. Selecione a porta: **Tools > Port > COMx** (Windows) ou **/dev/ttyUSBx** (Linux)
4. Clique em **Upload** (→)

### 5. Configuração das Páginas Web

1. Abra qualquer página HTML em um navegador moderno
2. As páginas se conectarão automaticamente ao broker MQTT
3. **Importante:** Use o mesmo tópico MQTT configurado no ESP32

### 6. Teste de Funcionamento

1. Abra o **Serial Monitor** (115200 baud)
2. Verifique a conexão Wi-Fi
3. Verifique a conexão MQTT
4. Teste a sequência secreta nos botões
5. Abra `casino_menu.html` no navegador

---

## 🔐 Sistema de Segurança

### Sequência Secreta (Konami Code)

Para desbloquear o cassino, pressione os botões na seguinte ordem:

```
1. BTN1 (CIMA)
2. BTN1 (CIMA)
3. BTN2 (BAIXO)
4. BTN2 (BAIXO)
5. BTN3 (VOLTAR)
6. BTN4 (CONFIRMAR)
7. BTN3 (VOLTAR)
8. BTN4 (CONFIRMAR)
```

### O que acontece ao desbloquear?

1. 🎵 **Música do baú de Zelda** toca no buzzer
2. 🌈 **LED RGB** faz animação arco-íris
3. 📺 **Display OLED** mostra "CASINO UNLOCKED!"
4. 💰 **Créditos atuais** são exibidos
5. 📡 **MQTT** publica status `UNLOCKED`

### Timeout

- A sequência deve ser completada em **3 segundos**
- Se errar, a sequência é resetada
- Sem limite de tentativas

---

## 🎮 Jogos Disponíveis

### 1. 🎰 Slot Machine (Caça-níqueis)

**Arquivo:** `slot_machine.html`

#### Símbolos e Pagamentos
| Combinação | Multiplicador |
|------------|---------------|
| 💎💎💎 | x100 (JACKPOT!) |
| ⭐⭐⭐ | x50 |
| 🍀🍀🍀 | x25 |
| 🍒🍒🍒 | x10 |
| 🍋🍋🍋 | x5 |
| 🍊🍊🍊 | x3 |
| 2 iguais | x1 |

#### Controles
- **BTN1:** Aumentar aposta (+10)
- **BTN2:** Diminuir aposta (-10)
- **BTN4:** GIRAR
- **BTN3:** Voltar ao menu

---

### 2. 🎲 Dados/Craps

**Arquivo:** `dados_craps.html`

#### Tipos de Apostas
| Aposta | Condição | Pagamento |
|--------|----------|-----------|
| Lucky 7 | Total = 7 | x4 |
| Eleven | Total = 11 | x15 |
| Pares | Ambos dados iguais | x30 |
| Menor que 7 | Total < 7 | x2 |
| Maior que 7 | Total > 7 | x2 |
| Snake Eyes | Total = 2 | x30 |

#### Controles
- **BTN1:** Aumentar aposta
- **BTN2:** Diminuir aposta
- **BTN4:** ROLAR DADOS
- **BTN3:** Voltar ao menu

---

### 3. 🃏 Blackjack 21

**Arquivo:** `blackjack_21.html`

#### Regras
- Objetivo: Chegar mais perto de 21 que o dealer sem estourar
- Ás vale 1 ou 11
- Figuras valem 10
- Blackjack natural paga x2.5

#### Controles
- **BTN1/BTN2:** Navegar opções
- **BTN4:** HIT (pedir carta) / STAND (parar) / Confirmar
- **BTN3:** Voltar ao menu

---

### 4. 🎡 Roleta RGB

**Arquivo:** `roleta_apostas.html`

#### Cores e Pagamentos
- **Vermelho:** x2
- **Verde:** x5
- **Azul:** x3
- **Branco:** x10

#### Controles
- **BTN1:** Aumentar aposta
- **BTN2:** Diminuir aposta
- **BTN4:** GIRAR ROLETA
- **BTN3:** Voltar ao menu

---

### 5. 💀 Roleta Russa

**Arquivo:** `russian_roulette.html`

#### Como Jogar
1. Escolha um alvo (você ou oponente)
2. Confirme para atirar
3. 1/6 de chance de "tiro"
4. Ganhe ou perca seus créditos

---

## 📡 API MQTT

### Broker
```
Host: broker.hivemq.com
Port: 1883 (TCP) / 8884 (WebSocket SSL)
Protocol: MQTT v3.1.1
```

### Tópicos (Topics)

#### 🔘 Botões
```
IFCE_caua/botoes/BOTAO_1  → "PRESSIONADO"
IFCE_caua/botoes/BOTAO_2  → "PRESSIONADO"
IFCE_caua/botoes/BOTAO_3  → "PRESSIONADO"
IFCE_caua/botoes/BOTAO_4  → "PRESSIONADO"
IFCE_caua/botoes/BOTAO_5  → "PRESSIONADO"
IFCE_caua/botoes/BOTAO_6  → "PRESSIONADO"
```

#### 💡 LED RGB
```
IFCE_caua/led_control  ← "R,G,B"  (ex: "255,0,128")
IFCE_caua/led_status   → "RGB(255,0,128)"
```

#### 🔊 Buzzer
```
IFCE_caua/buzzer_control  ← "frequencia_Hz"  (ex: "1000")
IFCE_caua/buzzer_status   → "Tocando 1000Hz"
```

#### 📺 Display
```
IFCE_caua/display_msg  ← "Mensagem para rolar"
```

#### 💰 Economia
```
IFCE_caua/credits       → "1000"  (créditos atuais)
IFCE_caua/bet           ← "50"    (valor apostado)
IFCE_caua/win           ← "100"   (valor ganho)
```

#### 🎰 Jogos
```
IFCE_caua/roulette_spin    ← "1"  (iniciar giro)
IFCE_caua/roulette_result  ← "VERMELHO"
IFCE_caua/casino_status    → "UNLOCKED" | "LOCKED"
```

#### 📊 Estatísticas
```
IFCE_caua/stats         → "jogos,vitorias,apostas_totais,maior_premio"
IFCE_caua/achievements  → "nome_conquista"
```

#### 🌡️ Sensores
```
IFCE_caua/temperatura    → "25.5"
IFCE_caua/umidade        → "60.0"
IFCE_caua/luminosidade   → "75"  (porcentagem)
```

### Exemplos de Uso

#### Publicar via JavaScript (Web)
```javascript
const client = mqtt.connect('wss://broker.hivemq.com:8884/mqtt');

client.on('connect', () => {
  // Mudar cor do LED para vermelho
  client.publish('IFCE_caua/led_control', '255,0,0');
  
  // Fazer uma aposta de 50 créditos
  client.publish('IFCE_caua/bet', '50');
  
  // Tocar buzzer em 1000Hz
  client.publish('IFCE_caua/buzzer_control', '1000');
});
```

#### Subscrever Tópicos (Web)
```javascript
client.subscribe('IFCE_caua/credits');
client.subscribe('IFCE_caua/botoes/#');

client.on('message', (topic, message) => {
  const msg = message.toString();
  
  if (topic === 'IFCE_caua/credits') {
    console.log('Créditos:', msg);
  }
  
  if (topic.startsWith('IFCE_caua/botoes/')) {
    const botao = topic.split('/')[2];
    console.log(`${botao} pressionado!`);
  }
});
```

---

## 💰 Sistema de Economia

### Créditos

- **Iniciais:** 1000 créditos
- **Persistência:** Salvo em EEPROM
- **Recuperação:** Automática ao reiniciar

### Apostas

- **Mínimo:** 10 créditos
- **Máximo:** 1000 créditos
- **Ajuste:** ±10 por vez

### Ganhos

Os ganhos dependem do jogo e multiplicador:
```
Ganho = Aposta × Multiplicador
```

### Estatísticas Salvas

```cpp
- playerCredits   // Créditos atuais
- totalBets       // Total apostado
- totalWins       // Total de vitórias
- gamesPlayed     // Jogos realizados
- biggestWin      // Maior prêmio
```

---

## 🏆 Conquistas (Achievements)

### Lista Completa

| Conquista | Descrição | Condição |
|-----------|-----------|----------|
| 🎰 **Primeiro Passo** | Faça sua primeira aposta | totalBets >= 1 |
| 🍀 **Sortudo** | Ganhe 5 jogos seguidos | winStreak >= 5 |
| 💰 **Magnata** | Acumule 10.000 créditos | credits >= 10000 |
| 🎖️ **Veterano** | Jogue 100 partidas | gamesPlayed >= 100 |
| 💎 **Jackpot** | Ganhe 1000 em uma aposta | biggestWin >= 1000 |
| 🔥 **Viciado** | Jogue 500 partidas | gamesPlayed >= 500 |

### Feedback ao Desbloquear

1. 🎵 **Melodia de vitória** no buzzer
2. 🎉 **Partículas** na tela web
3. 📺 **Notificação** no display OLED
4. 📡 **Publicação** via MQTT
5. 💾 **Salvamento** automático

---

## 📖 Guia de Uso

### Primeiro Acesso

1. ⚡ **Ligue o ESP32**
2. 📶 Aguarde conexão Wi-Fi (LED azul piscando)
3. 📡 Aguarde conexão MQTT (mensagem no display)
4. 🔐 Digite a sequência secreta nos botões
5. 🎵 Ouça a música do baú de Zelda
6. 🌐 Abra `casino_menu.html` no navegador

### Navegação Web

```
casino_menu.html (Hub Principal)
    ├── slot_machine.html (Caça-níqueis)
    ├── dados_craps.html (Dados)
    ├── blackjack_21.html (21)
    ├── roleta_apostas.html (Roleta RGB)
    └── russian_roulette.html (Roleta Russa)
```

### Controle pelos Botões

Todos os jogos seguem o mesmo padrão:

- **BTN1 (CIMA)** 🔼 - Aumentar/Navegar para cima
- **BTN2 (BAIXO)** 🔽 - Diminuir/Navegar para baixo
- **BTN3 (VOLTAR)** ⬅️ - Cancelar/Voltar
- **BTN4 (CONFIRMAR)** ✅ - Confirmar/Jogar

### Feedback Sensorial

#### Sons
- **Navegação:** Beep curto (800Hz, 80ms)
- **Confirmar:** Beep agudo (1200Hz, 150ms)
- **Voltar:** Beep grave (400Hz, 150ms)
- **Vitória:** Melodia ascendente
- **Erro:** Som grave prolongado

#### LED RGB
- **Vermelho:** Derrota/Erro
- **Verde:** Vitória
- **Azul:** Navegação
- **Dourado:** Jackpot/Conquista
- **Arco-íris:** Desbloqueio/Animação

---

## 🔧 Troubleshooting

### Problema: ESP32 não conecta ao Wi-Fi

**Soluções:**
```cpp
1. Verifique SSID e senha no código
2. Confirme que o Wi-Fi é 2.4GHz (ESP32 não suporta 5GHz)
3. Verifique distância do roteador
4. Reinicie o ESP32
5. Verifique Serial Monitor para mensagens de erro
```

### Problema: MQTT não conecta

**Soluções:**
```cpp
1. Verifique conexão de internet
2. Teste broker: ping broker.hivemq.com
3. Verifique firewall
4. Use outro broker (test.mosquitto.org)
5. Confirme porta 1883 aberta
```

### Problema: Display OLED não funciona

**Soluções:**
```cpp
1. Verifique conexões SDA/SCL
2. Confirme endereço I2C (0x3C ou 0x3D)
3. Execute I2C scanner
4. Verifique alimentação 3.3V/5V
5. Teste outro display
```

### Problema: Botões não respondem

**Soluções:**
```cpp
1. Verifique conexões dos botões
2. Confirme pull-up interno ativado
3. Teste com multímetro
4. Aumente DEBOUNCE_DELAY
5. Verifique pinos no código
```

### Problema: LED RGB cores erradas

**Soluções:**
```cpp
1. Verifique se é common anode/cathode
2. Inverta valores (255 - valor)
3. Teste cada cor individualmente
4. Confirme resistores 330Ω
5. Verifique alimentação
```

### Problema: Dados não salvam

**Soluções:**
```cpp
1. Verifique partição EEPROM no ESP32
2. Confirme Preferences.begin() é chamado
3. Teste com valores pequenos primeiro
4. Verifique Serial Monitor por erros
5. Apague e grave novamente o código
```

### Problema: Página web não conecta MQTT

**Soluções:**
```javascript
1. Use WSS (WebSocket Secure) na web
2. Broker: wss://broker.hivemq.com:8884/mqtt
3. Verifique console do navegador
4. Teste em navegador diferente
5. Desabilite bloqueador de anúncios
```

---

## 🚀 Melhorias Futuras

### Planejado para v3.0

- [ ] **Multiplayer Online** - Vários jogadores simultâneos
- [ ] **Sistema de Ranking** - Leaderboard global
- [ ] **Mais Jogos:**
  - Poker Texas Hold'em ♠️
  - Baccarat 🎴
  - Keno 🔢
- [ ] **Integração Discord/Telegram** - Notificações
- [ ] **App Mobile** - React Native
- [ ] **Torneios Programados** - Eventos automáticos
- [ ] **NFT Achievements** - Conquistas como NFTs
- [ ] **Voice Commands** - Controle por voz

---

## 📄 Licença

Este projeto está sob a licença **MIT**. Veja o arquivo `LICENSE` para mais detalhes.

---

## 🤝 Contribuindo

Contribuições são bem-vindas! Para contribuir:

1. Fork o projeto
2. Crie uma branch para sua feature (`git checkout -b feature/NovaFeature`)
3. Commit suas mudanças (`git commit -m 'Adiciona NovaFeature'`)
4. Push para a branch (`git push origin feature/NovaFeature`)
5. Abra um Pull Request

---

## 📞 Suporte

- **GitHub Issues:** [github.com/alessandro-caua/Franzininho_casino/issues](https://github.com/alessandro-caua/Franzininho_casino/issues)
- **Email:** [seu-email@exemplo.com]
- **Discord:** [Link do servidor]

---

## 🌟 Agradecimentos

- **Franzininho Community** - Hardware incrível
- **HiveMQ** - Broker MQTT gratuito
- **Adafruit** - Bibliotecas excelentes
- **Chart.js** - Gráficos lindos
- **Nintendo** - Inspiração Zelda 🎮

---

## 📸 Screenshots

### Menu Principal
![Menu](screenshots/menu.png)

### Slot Machine
![Slots](screenshots/slots.png)

### Dados
![Dados](screenshots/dados.png)

### Conquistas
![Achievements](screenshots/achievements.png)

---

**Desenvolvido com ❤️ por IkkiKuuro e Cauã Alessandro**

🎰 **BOA SORTE NO CASSINO!** 🎰
