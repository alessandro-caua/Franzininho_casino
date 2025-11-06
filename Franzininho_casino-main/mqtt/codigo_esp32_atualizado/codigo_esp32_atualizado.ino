#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Preferences.h>

// ====== CONFIG Wi-Fi ======
//const char* WIFI_SSID = "brisa-463544";
//const char* WIFI_PASS = "oyfhnkvp";

const char* WIFI_SSID = "IFCE";
const char* WIFI_PASS = "IFCE1234";

// ====== CONFIG MQTT ======
const char* MQTT_HOST = "broker.hivemq.com";
const uint16_t MQTT_PORT = 1883;
String clientId = "franzininho-" + String((uint32_t)ESP.getEfuseMac(), HEX);

// ====== DHT ======
#define DHTPIN 15
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

// ====== TOPICOS MQTT ======
const char* TOPIC_BTN = "IFCE_caua/botoes";
const char* TOPIC_BUZZER_CONTROL = "IFCE_caua/buzzer_control";
const char* TOPIC_BUZZER_STATUS = "IFCE_caua/buzzer_status";
const char* TOPIC_LED_CONTROL = "IFCE_caua/led_control";
const char* TOPIC_LED_STATUS = "IFCE_caua/led_status";
const char* TOPIC_DISPLAY_MSG = "IFCE_caua/display_msg";
const char* TOPIC_TEMP = "IFCE_caua/temperatura";
const char* TOPIC_HUMIDITY = "IFCE_caua/umidade";
const char* TOPIC_LUMINOSITY = "IFCE_caua/luminosidade";
const char* TOPIC_ROULETTE_SPIN = "IFCE_caua/roulette_spin";
const char* TOPIC_ROULETTE_RESULT = "IFCE_caua/roulette_result";
const char* TOPIC_CREDITS = "IFCE_caua/credits";              // Créditos do jogador
const char* TOPIC_BET = "IFCE_caua/bet";                      // Apostas
const char* TOPIC_WIN = "IFCE_caua/win";                      // Vitórias
const char* TOPIC_CASINO_STATUS = "IFCE_caua/casino_status";  // Status do cassino (locked/unlocked)
const char* TOPIC_CASINO_REDIRECT = "IFCE_caua/casino_redirect"; // Comando para abrir casino no navegador
const char* TOPIC_ACHIEVEMENTS = "IFCE_caua/achievements";    // Conquistas
const char* TOPIC_STATS = "IFCE_caua/stats";                  // Estatísticas

WiFiClient espClient;
PubSubClient mqtt(espClient);

// ====== PINOS ======
#define LED_R_PIN 14
#define LED_G_PIN 13
#define LED_B_PIN 12
#define BUZZER_PIN 17

#define BTN1_PIN 2
#define BTN2_PIN 3
#define BTN3_PIN 4
#define BTN4_PIN 5
#define BTN5_PIN 6
#define BTN6_PIN 7

#define LDR_PIN 1  // Sensor de luminosidade (ADC1_CH0)

// ====== DISPLAY OLED ======
#define OLED_SDA_PIN 8
#define OLED_SCL_PIN 9
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ====== VARIÁVEIS ======
bool lastBtn[6] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
unsigned long lastBtnPress[6] = {0, 0, 0, 0, 0, 0};
const unsigned long DEBOUNCE_DELAY = 200;
bool ledState = false;
int currentRed = 0, currentGreen = 0, currentBlue = 0;
unsigned long lastLedToggle = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long lastSensorPublish = 0;
bool displayOK = false;
String scrollingMsg = "";
unsigned long scrollTimer = 0;
int scrollX = SCREEN_WIDTH;

// ====== CALIBRAÇÃO DO LDR ======
int ldrMin = 4095;  // Valor no escuro (máximo ADC)
int ldrMax = 0;     // Valor com luz máxima (mínimo ADC)
bool ldrCalibrated = false;

// Botões com funções específicas
const char* BTN_NAMES[6] = {"CIMA", "BAIXO", "VOLTAR", "CONFIRMAR", "EXTRA1", "EXTRA2"};

// ====== SISTEMA DE SENHA "KONAMI CODE" ======
const int SECRET_SEQUENCE[] = {0, 0, 1, 1, 2, 3, 2, 3};  // CIMA, CIMA, BAIXO, BAIXO, VOLTAR, CONFIRMAR, VOLTAR, CONFIRMAR
const int SECRET_LENGTH = 8;
int currentSequence[SECRET_LENGTH];
int sequenceIndex = 0;
bool casinoActivated = false;   // Etapa 1: "casino hub" + LED vermelho
bool casinoUnlocked = false;    // Etapa 2: Konami code completo
unsigned long lastSequenceTime = 0;
const unsigned long SEQUENCE_TIMEOUT = 3000;  // 3 segundos para completar a sequência

// ====== ATIVAÇÃO DO CASSINO (REQUISITOS) ======
bool casinoHubReceived = false;  // Recebeu mensagem "casino hub"
bool ledIsRed = false;            // LED está vermelho (255, 0, 0)

// ====== SISTEMA DE ECONOMIA ======
Preferences preferences;
unsigned long playerCredits = 1000;  // Créditos iniciais
unsigned long totalBets = 0;
unsigned long totalWins = 0;
unsigned long gamesPlayed = 0;
unsigned long biggestWin = 0;

// ====== CONQUISTAS ======
struct Achievement {
  const char* name;
  const char* description;
  bool unlocked;
  unsigned long requirement;
};

Achievement achievements[] = {
  {"Primeiro Passo", "Faça sua primeira aposta", false, 1},
  {"Sortudo", "Ganhe 5 jogos seguidos", false, 5},
  {"Magnata", "Acumule 10.000 créditos", false, 10000},
  {"Veterano", "Jogue 100 partidas", false, 100},
  {"Jackpot", "Ganhe 1000 créditos em uma aposta", false, 1000},
  {"Viciado", "Jogue 500 partidas", false, 500}
};
const int NUM_ACHIEVEMENTS = 6;

// ====== MÚSICA DO BAÚ DE ZELDA ======
// Notas: C, D, E, F, G, A, B
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_A5  880

// Melodia do baú de Zelda
int zeldaChestMelody[] = {NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5};
int zeldaChestDurations[] = {200, 200, 200, 200, 200, 200, 200, 200, 600};

// ====== FUNÇÕES ======

// Verifica se o cassino foi ativado (mensagem "casino hub" + LED vermelho)
void checkCasinoActivation() {
  if (casinoHubReceived && ledIsRed && !casinoActivated) {
    casinoActivated = true;
    
    Serial.println("[CASSINO] 🔓 MODO CASSINO ATIVADO! Agora insira o Konami Code nos botões!");
    
    if (displayOK) {
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(10, 5);
      display.println("CASSINO");
      display.setCursor(5, 25);
      display.println("ATIVADO!");
      display.setTextSize(1);
      display.setCursor(0, 48);
      display.println("Insira Konami Code:");
      display.setCursor(0, 56);
      display.println("^ ^ v v < > < >");
      display.display();
    }
    
    // Som de ativação
    tone(BUZZER_PIN, 800, 200);
    delay(250);
    tone(BUZZER_PIN, 1000, 200);
    delay(250);
    tone(BUZZER_PIN, 1200, 400);
    
    // LED pisca verde para confirmar
    setLEDColor(0, 255, 0);
    delay(500);
    setLEDColor(255, 0, 0);  // Volta pro vermelho
  }
}

void playZeldaChestMusic() {
  Serial.println("[CASSINO] 🎵 Tocando música do baú de Zelda!");
  
  for (int i = 0; i < 9; i++) {
    tone(BUZZER_PIN, zeldaChestMelody[i], zeldaChestDurations[i]);
    
    // LED pisca em arco-íris durante a música
    int r = (i % 3 == 0) ? 255 : 0;
    int g = (i % 3 == 1) ? 255 : 0;
    int b = (i % 3 == 2) ? 255 : 0;
    setLEDColor(r, g, b);
    
    delay(zeldaChestDurations[i] + 50);
  }
  
  // LED dourado no final
  setLEDColor(255, 215, 0);
  delay(500);
}

// Verifica se a sequência secreta foi inserida
void checkSecretSequence(int buttonIndex) {
  unsigned long now = millis();
  
  // Reset se passou muito tempo desde o último botão
  if (now - lastSequenceTime > SEQUENCE_TIMEOUT) {
    sequenceIndex = 0;
  }
  
  lastSequenceTime = now;
  
  // Verifica se o botão está correto na sequência
  if (buttonIndex == SECRET_SEQUENCE[sequenceIndex]) {
    currentSequence[sequenceIndex] = buttonIndex;
    sequenceIndex++;
    
    // Feedback visual
    tone(BUZZER_PIN, 600 + (sequenceIndex * 100), 50);
    
    Serial.printf("[SEQUENCIA] Progresso: %d/%d\n", sequenceIndex, SECRET_LENGTH);
    
    // Sequência completa! (Só funciona se cassino foi ativado)
    if (sequenceIndex >= SECRET_LENGTH) {
      if (casinoActivated) {
        casinoUnlocked = true;
        sequenceIndex = 0;
        
        Serial.println("[CASSINO] 🎰 DESBLOQUEADO! Bem-vindo ao Franzininho Cassino!");
        
        // Toca a música do baú de Zelda
        playZeldaChestMusic();
        
        // Mostra mensagem no display
        if (displayOK) {
          display.clearDisplay();
          display.setTextSize(2);
          display.setTextColor(SSD1306_WHITE);
          display.setCursor(5, 10);
          display.println("CASSINO");
          display.setCursor(0, 30);
          display.println("UNLOCKED!");
          display.setTextSize(1);
          display.setCursor(10, 50);
          display.printf("Creditos:%lu", playerCredits);
          display.display();
          delay(3000);
        }
        
        // Publica status e comando de redirecionamento
        mqtt.publish(TOPIC_CASINO_STATUS, "UNLOCKED", true);
        mqtt.publish(TOPIC_CASINO_REDIRECT, "OPEN", false); // Não retained - só válido uma vez
        publishCredits();
        
        Serial.println("[CASSINO] 🌐 Comando enviado: Abrindo cassino no navegador...");
      } else {
        // Tentou Konami code mas não ativou o cassino ainda
        Serial.println("[CASSINO] Você precisa ativar o modo cassino primeiro!");
        sequenceIndex = 0;
      }
    }
  } else {
    // Sequência errada - reset
    if (sequenceIndex > 0) {
      Serial.println("[SEQUENCIA] Errado! Resetando...");
      tone(BUZZER_PIN, 200, 300);  // Som de erro
      setLEDColor(255, 0, 0);  // LED vermelho
      delay(300);
      setLEDColor(0, 0, 0);
    }
    sequenceIndex = 0;
  }
}

// ====== SALVAR/CARREGAR DADOS ======
void saveData() {
  preferences.begin("casino", false);
  
  preferences.putULong("credits", playerCredits);
  preferences.putULong("totalBets", totalBets);
  preferences.putULong("totalWins", totalWins);
  preferences.putULong("gamesPlayed", gamesPlayed);
  preferences.putULong("biggestWin", biggestWin);
  
  for (int i = 0; i < NUM_ACHIEVEMENTS; i++) {
    String key = "ach" + String(i);
    preferences.putBool(key.c_str(), achievements[i].unlocked);
  }
  
  preferences.end();
  Serial.println("[SAVE] � Dados salvos!");
}

void loadData() {
  preferences.begin("casino", true);
  
  playerCredits = preferences.getULong("credits", 1000);
  totalBets = preferences.getULong("totalBets", 0);
  totalWins = preferences.getULong("totalWins", 0);
  gamesPlayed = preferences.getULong("gamesPlayed", 0);
  biggestWin = preferences.getULong("biggestWin", 0);
  
  for (int i = 0; i < NUM_ACHIEVEMENTS; i++) {
    String key = "ach" + String(i);
    achievements[i].unlocked = preferences.getBool(key.c_str(), false);
  }
  
  preferences.end();
  
  Serial.printf("[CARREGAR] Créditos: %lu | Jogos: %lu\n", playerCredits, gamesPlayed);
}

// Publica créditos
void publishCredits() {
  mqtt.publish(TOPIC_CREDITS, String(playerCredits).c_str());
}

// Publica estatísticas
void publishStats() {
  String stats = String(gamesPlayed) + "," + String(totalWins) + "," + 
                 String(totalBets) + "," + String(biggestWin);
  mqtt.publish(TOPIC_STATS, stats.c_str());
}

// Verifica e desbloqueia conquistas
void checkAchievements() {
  bool anyUnlocked = false;
  
  for (int i = 0; i < NUM_ACHIEVEMENTS; i++) {
    if (achievements[i].unlocked) continue;
    
    bool unlock = false;
    
    switch(i) {
      case 0: unlock = (totalBets >= 1); break;                    // Primeira aposta
      case 1: unlock = false; break;                               // 5 vitórias seguidas (implementar contador)
      case 2: unlock = (playerCredits >= 10000); break;            // 10k créditos
      case 3: unlock = (gamesPlayed >= 100); break;                // 100 partidas
      case 4: unlock = (biggestWin >= 1000); break;                // Jackpot 1000
      case 5: unlock = (gamesPlayed >= 500); break;                // 500 partidas
    }
    
    if (unlock) {
      achievements[i].unlocked = true;
      anyUnlocked = true;
      
      Serial.printf("[CONQUISTA] 🏆 %s desbloqueada!\n", achievements[i].name);
      
      // Feedback sonoro
      int victoryMelody[] = {NOTE_C5, NOTE_E5, NOTE_G5, NOTE_C5};
      for (int j = 0; j < 4; j++) {
        tone(BUZZER_PIN, victoryMelody[j], 150);
        delay(180);
      }
      
      // Publica conquista
      mqtt.publish(TOPIC_ACHIEVEMENTS, achievements[i].name);
      
      // Mostra no display
      if (displayOK) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("CONQUISTA!");
        display.setTextSize(1);
        display.setCursor(0, 20);
        display.println(achievements[i].name);
        display.setCursor(0, 35);
        display.println(achievements[i].description);
        display.display();
        delay(3000);
      }
    }
  }
  
  if (anyUnlocked) {
    saveData();
  }
}

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi conectado: " + WiFi.localIP().toString());
}

void showScrollingMessage(String msg) {
  if (!displayOK) return;
  scrollingMsg = msg;
  scrollX = SCREEN_WIDTH;
  Serial.println("[DISPLAY] Nova mensagem: " + msg);
}

void drawScrollingMessage() {
  if (!displayOK || scrollingMsg == "") return;

  if (millis() - scrollTimer > 50) {
    scrollTimer = millis();

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(scrollX, 28);
    display.print(scrollingMsg);
    display.display();

    scrollX--;
    if (scrollX < -((int)scrollingMsg.length() * 6)) {
      scrollX = SCREEN_WIDTH;
    }
  }
}

// NOVA FUNÇÃO: Define cor RGB do LED
void setLEDColor(int r, int g, int b) {
  currentRed = constrain(r, 0, 255);
  currentGreen = constrain(g, 0, 255);
  currentBlue = constrain(b, 0, 255);
  
  // Inverte valores porque LEDs podem ser common anode
  // Se não funcionar corretamente, remova os "255 -"
  analogWrite(LED_R_PIN, currentRed);
  analogWrite(LED_G_PIN, currentGreen);
  analogWrite(LED_B_PIN, currentBlue);
  
  String status = "RGB(" + String(currentRed) + "," + String(currentGreen) + "," + String(currentBlue) + ")";
  mqtt.publish(TOPIC_LED_STATUS, status.c_str());
  Serial.println("[LED] Cor definida: " + status);
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) msg += (char)payload[i];
  msg.trim();

  Serial.printf("[MQTT] %s => %s\n", topic, msg.c_str());
  
  // Verifica se o cassino está desbloqueado (exceto para alguns comandos)
  if (!casinoUnlocked && 
      String(topic) != TOPIC_BUZZER_CONTROL && 
      String(topic) != TOPIC_LED_CONTROL && 
      String(topic) != TOPIC_DISPLAY_MSG) {
    Serial.println("[CASSINO] Bloqueado! Digite a sequência secreta nos botões.");
    mqtt.publish(TOPIC_CASINO_STATUS, "LOCKED");
    return;
  }

  // ====== CONTROLE DO BUZZER ======
  if (String(topic) == TOPIC_BUZZER_CONTROL) {
    int freq = msg.toInt();
    if (freq >= 50 && freq <= 5000) {
      tone(BUZZER_PIN, freq, 500);
      mqtt.publish(TOPIC_BUZZER_STATUS, ("Tocando " + String(freq) + "Hz").c_str());

      if (displayOK && scrollingMsg == "") {
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 20);
        display.printf("Buzzer\n%d Hz", freq);
        display.display();
        delay(500);
      }
    } else {
      mqtt.publish(TOPIC_BUZZER_STATUS, "Frequência inválida!");
    }
  }

  // ====== CONTROLE DO LED RGB ======
  else if (String(topic) == TOPIC_LED_CONTROL) {
    // Formato esperado: "R,G,B" exemplo: "255,0,128"
    int r = 0, g = 0, b = 0;
    int firstComma = msg.indexOf(',');
    int secondComma = msg.indexOf(',', firstComma + 1);
    
    if (firstComma > 0 && secondComma > firstComma) {
      r = msg.substring(0, firstComma).toInt();
      g = msg.substring(firstComma + 1, secondComma).toInt();
      b = msg.substring(secondComma + 1).toInt();
      
      setLEDColor(r, g, b);
      
      // Verifica se LED está vermelho (requisito 2 para ativar cassino)
      if (r == 255 && g == 0 && b == 0) {
        ledIsRed = true;
        Serial.println("[CASSINO] ✓ LED vermelho detectado!");
        checkCasinoActivation();
      } else {
        ledIsRed = false;
      }
      
      if (displayOK && scrollingMsg == "") {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 10);
        display.println("LED RGB:");
        display.setTextSize(2);
        display.printf("R:%d\nG:%d\nB:%d", r, g, b);
        display.display();
      }
    } else {
      Serial.println("[LED] Formato inválido! Use: R,G,B");
    }
  }

  // ====== MENSAGEM NO DISPLAY ======
  else if (String(topic) == TOPIC_DISPLAY_MSG) {
    // Verifica se é a mensagem secreta "casino hub" (requisito 1 para ativar cassino)
    if (msg.equalsIgnoreCase("casino hub")) {
      casinoHubReceived = true;
      Serial.println("[CASSINO] ✓ Mensagem 'casino hub' detectada!");
      showScrollingMessage("*** CASSINO HUB ATIVANDO ***");
      checkCasinoActivation();
    } else {
      showScrollingMessage(msg);
    }
  }
  
  // ====== GERENCIAMENTO DE JOGADORES ======
  
  // ====== ROLETA - SPINNING ======
  else if (String(topic) == TOPIC_ROULETTE_SPIN) {
    // Tocar som de roleta girando
    for (int i = 0; i < 10; i++) {
      tone(BUZZER_PIN, 800 + (i * 50), 50);
      delay(100);
    }
    
    if (displayOK && scrollingMsg == "") {
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(10, 20);
      display.println("GIRANDO");
      display.setCursor(10, 40);
      display.println("ROLETA!");
      display.display();
    }
  }

  // ====== ROLETA - RESULTADO ======
  else if (String(topic) == TOPIC_ROULETTE_RESULT) {
    // Tocar melodia de vitória
    int melody[] = {523, 659, 784, 1047};
    int noteDurations[] = {200, 200, 200, 400};
    
    for (int i = 0; i < 4; i++) {
      tone(BUZZER_PIN, melody[i], noteDurations[i]);
      delay(noteDurations[i] + 50);
    }
    
    if (displayOK && scrollingMsg == "") {
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(5, 15);
      display.println("RESULT:");
      display.setTextSize(3);
      display.setCursor(5, 35);
      display.println(msg);
      display.display();
      delay(3000);
    }
  }
  
  // ====== APOSTA ======
  else if (String(topic) == TOPIC_BET) {
    unsigned long betAmount = msg.toInt();
    
    // ✅ VALIDAÇÃO 1: Créditos suficientes
    if (betAmount > playerCredits) {
      Serial.println("[APOSTA] ❌ Créditos insuficientes!");
      mqtt.publish(TOPIC_CASINO_STATUS, "INSUFFICIENT_CREDITS");
      mqtt.publish("IFCE_caua/bet_error", "Créditos insuficientes");
      tone(BUZZER_PIN, 200, 500);  // Som de erro
      
      if (displayOK) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 20);
        display.println("ERRO:");
        display.println("Creditos");
        display.println("insuficientes!");
        display.display();
        delay(2000);
      }
      return;
    }
    
    // ✅ VALIDAÇÃO 2: Valor mínimo
    if (betAmount < 10) {
      Serial.println("[APOSTA] ❌ Aposta muito baixa! Mínimo: 10");
      mqtt.publish("IFCE_caua/bet_error", "Aposta mínima: 10 créditos");
      tone(BUZZER_PIN, 200, 300);
      return;
    }
    
    // ✅ VALIDAÇÃO 3: Valor máximo
    if (betAmount > 1000) {
      Serial.println("[APOSTA] ❌ Aposta muito alta! Máximo: 1000");
      mqtt.publish("IFCE_caua/bet_error", "Aposta máxima: 1000 créditos");
      tone(BUZZER_PIN, 200, 300);
      return;
    }
    
    // ✅ VALIDAÇÃO 4: Múltiplo de 10
    if (betAmount % 10 != 0) {
      Serial.println("[APOSTA] ❌ Aposta deve ser múltiplo de 10!");
      mqtt.publish("IFCE_caua/bet_error", "Use múltiplos de 10");
      tone(BUZZER_PIN, 200, 300);
      return;
    }
    
    // ✅ Aposta válida - processar
    playerCredits -= betAmount;
    totalBets += betAmount;
    gamesPlayed++;
    
    Serial.printf("[APOSTA] ✅ Aposta aceita: %lu | Créditos restantes: %lu\n", betAmount, playerCredits);
    
    // Confirma aposta via MQTT
    mqtt.publish("IFCE_caua/bet_confirmed", String(betAmount).c_str());
    
    publishCredits();
    saveData();
    checkAchievements();
    
    // Feedback sonoro de sucesso
    tone(BUZZER_PIN, 1000, 100);
    delay(120);
    tone(BUZZER_PIN, 1200, 100);
  }
  
  // ====== VITÓRIA ======
  else if (String(topic) == TOPIC_WIN) {
    unsigned long winAmount = msg.toInt();
    
    // ✅ VALIDAÇÃO: Valor razoável (anti-hack)
    if (winAmount > 100000) {
      Serial.println("[VITÓRIA] ⚠️ Valor suspeito! Limitando...");
      winAmount = 10000; // Limita ganho máximo
      mqtt.publish("IFCE_caua/win_error", "Valor limitado por segurança");
    }
    
    // ✅ VALIDAÇÃO: Não pode ganhar 0
    if (winAmount == 0) {
      Serial.println("[VITÓRIA] ❌ Valor inválido!");
      return;
    }
    
    playerCredits += winAmount;
    totalWins++;
    
    if (winAmount > biggestWin) {
      biggestWin = winAmount;
    }
    
    Serial.printf("[VITÓRIA] ✅ Ganhou: %lu | Créditos: %lu\n", winAmount, playerCredits);
    
    // Animação de vitória
    for (int i = 0; i < 5; i++) {
      setLEDColor(255, 215, 0);  // Dourado
      delay(100);
      setLEDColor(0, 0, 0);
      delay(100);
    }
    
    publishCredits();
    publishStats();
    saveData();
    checkAchievements();
  }
}

void ensureMqtt() {
  while (!mqtt.connected()) {
    Serial.print("[MQTT] Conectando...");
    if (mqtt.connect(clientId.c_str())) {
      Serial.println(" conectado!");
      
      // Tópicos originais
      mqtt.subscribe(TOPIC_BUZZER_CONTROL);
      mqtt.subscribe(TOPIC_LED_CONTROL);
      mqtt.subscribe(TOPIC_DISPLAY_MSG);
      mqtt.subscribe(TOPIC_ROULETTE_SPIN);
      mqtt.subscribe(TOPIC_ROULETTE_RESULT);
      mqtt.subscribe(TOPIC_BET);
      mqtt.subscribe(TOPIC_WIN);
      
      if (displayOK) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 30);
        display.println("MQTT: Conectado!");
        display.display();
      }
    } else {
      Serial.println(" falhou, tentando novamente...");
      delay(2000);
    }
  }
}

void checkButtons() {
  int btnPins[6] = {BTN1_PIN, BTN2_PIN, BTN3_PIN, BTN4_PIN, BTN5_PIN, BTN6_PIN};
  unsigned long now = millis();
  
  for (int i = 0; i < 6; i++) {
    bool current = digitalRead(btnPins[i]);
    
    // Detecta press com debounce
    if (lastBtn[i] == HIGH && current == LOW) {
      // Verifica debounce
      if (now - lastBtnPress[i] > DEBOUNCE_DELAY) {
        lastBtnPress[i] = now;
        
        // SEMPRE publica no MQTT (modo normal funciona sempre)
        String topic = String(TOPIC_BTN) + "/BOTAO_" + String(i + 1);
        mqtt.publish(topic.c_str(), "PRESSIONADO");
        Serial.printf("[BOTAO %d - %s] Pressionado\n", i + 1, BTN_NAMES[i]);
        
        // Se cassino ativado mas não desbloqueado -> verifica Konami code
        if (casinoActivated && !casinoUnlocked) {
          checkSecretSequence(i);
        }
        
        // Feedback visual no display
        if (displayOK && scrollingMsg == "") {
          display.clearDisplay();
          display.setTextSize(2);
          display.setTextColor(SSD1306_WHITE);
          display.setCursor(10, 15);
          display.print("BTN ");
          display.println(i + 1);
          display.setTextSize(1);
          display.setCursor(10, 35);
          display.println(BTN_NAMES[i]);
          display.display();
          delay(300);
        }
        
        // Feedback sonoro diferenciado
        if (i == 0 || i == 1) {
          tone(BUZZER_PIN, 800, 80);
        } else if (i == 2) {
          tone(BUZZER_PIN, 400, 150);
        } else if (i == 3) {
          tone(BUZZER_PIN, 1200, 150);
        } else {
          tone(BUZZER_PIN, 700, 100);
        }
      }
    }
    lastBtn[i] = current;
  }
}

void updateDisplay() {
  if (!displayOK || scrollingMsg != "") return;
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  if (!casinoUnlocked) {
    // Mostra mensagem de bloqueio
    display.setCursor(0, 0);
    display.println("=== FRANZININHO ===");
    display.setCursor(0, 15);
    display.println("CASSINO BLOQUEADO");
    display.setCursor(0, 30);
    display.println("Digite sequencia");
    display.setCursor(0, 40);
    display.println("nos botoes...");
    display.setCursor(0, 55);
    display.printf("%d/%d", sequenceIndex, SECRET_LENGTH);
  } else {
    // Mostra informações do cassino
    display.setCursor(0, 0);
    display.println("=== CASSINO ===");
    
    display.setCursor(0, 12);
    display.printf("Creditos: %lu", playerCredits);
    
    display.setCursor(0, 22);
    display.printf("Jogos: %lu", gamesPlayed);
    
    display.setCursor(0, 32);
    display.printf("Vitorias: %lu", totalWins);
    
    display.setCursor(0, 42);
    display.print("WiFi: ");
    display.print(WiFi.status() == WL_CONNECTED ? "OK" : "OFF");
    
    display.setCursor(0, 52);
    display.print("MQTT: ");
    display.print(mqtt.connected() ? "OK" : "OFF");
  }
  
  display.display();
}

// NOVA FUNÇÃO: Publica leituras dos sensores
void publishSensors() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  
  if (!isnan(temp)) {
    mqtt.publish(TOPIC_TEMP, String(temp, 1).c_str());
    Serial.printf("[SENSOR] Temperatura: %.1f°C\n", temp);
  }
  
  if (!isnan(hum)) {
    mqtt.publish(TOPIC_HUMIDITY, String(hum, 1).c_str());
    Serial.printf("[SENSOR] Umidade: %.1f%%\n", hum);
  }
  
  // Lê sensor de luminosidade (LDR)
  int ldrValue = analogRead(LDR_PIN);
  int luminosity;
  
  if (ldrCalibrated) {
    // Usa valores calibrados
    luminosity = map(ldrValue, ldrMax, ldrMin, 0, 100);
  } else {
    // Fallback: mapeamento padrão invertido
    luminosity = map(ldrValue, 0, 4095, 100, 0);
  }
  
  luminosity = constrain(luminosity, 0, 100);  // Garante valores entre 0-100
  
  mqtt.publish(TOPIC_LUMINOSITY, String(luminosity).c_str());
  Serial.printf("[SENSOR] Luminosidade: %d%% (ADC: %d) %s\n", 
                luminosity, ldrValue, ldrCalibrated ? "[Calibrado]" : "[Padrão]");
}

// Calibra o sensor LDR
void calibrateLDR() {
  Serial.println("\n[LDR] ⚙️ Iniciando calibração automática...");
  Serial.println("[LDR] Aguarde 10 segundos...");
  
  if (displayOK) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 10);
    display.println("CALIBRANDO LDR");
    display.setCursor(0, 25);
    display.println("Cubra/ilumine");
    display.setCursor(0, 35);
    display.println("o sensor...");
    display.display();
  }
  
  unsigned long startTime = millis();
  int readings = 0;
  
  // LED pisca durante calibração
  while (millis() - startTime < 10000) {
    int reading = analogRead(LDR_PIN);
    
    if (reading < ldrMin) ldrMin = reading;
    if (reading > ldrMax) ldrMax = reading;
    
    readings++;
    
    // LED pisca azul
    if ((millis() / 200) % 2 == 0) {
      setLEDColor(0, 0, 255);
    } else {
      setLEDColor(0, 0, 0);
    }
    
    // Atualiza display a cada segundo
    if ((millis() - startTime) % 1000 < 50) {
      if (displayOK) {
        display.fillRect(0, 48, 128, 16, BLACK);
        display.setCursor(0, 48);
        display.printf("Min:%d Max:%d", ldrMin, ldrMax);
        display.display();
      }
    }
    
    delay(50);
  }
  
  setLEDColor(0, 0, 0);
  
  // Validação
  if (ldrMax - ldrMin < 100) {
    Serial.println("[LDR] ⚠️ Variação muito pequena! Usando valores padrão.");
    ldrMin = 3500;  // Escuro típico
    ldrMax = 100;   // Luz típica
  } else {
    ldrCalibrated = true;
  }
  
  Serial.printf("[LDR] ✅ Calibrado! Min=%d (escuro), Max=%d (luz) | Leituras: %d\n", 
                ldrMin, ldrMax, readings);
  
  if (displayOK) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 15);
    display.println("LDR CALIBRADO!");
    display.setCursor(0, 30);
    display.printf("Min: %d", ldrMin);
    display.setCursor(0, 40);
    display.printf("Max: %d", ldrMax);
    display.display();
    delay(2000);
  }
  
  // Feedback sonoro
  tone(BUZZER_PIN, 1000, 100);
  delay(120);
  tone(BUZZER_PIN, 1200, 100);
  delay(120);
  tone(BUZZER_PIN, 1500, 150);
}

void initDisplay() {
  Serial.println("\n[DISPLAY] Inicializando...");
  
  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);
  delay(100);
  
  if (display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.printf("[DISPLAY] OK! (SDA=%d, SCL=%d)\n", OLED_SDA_PIN, OLED_SCL_PIN);
    displayOK = true;
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Franzininho WiFi");
    display.println("LAB - IoT");
    display.println("");
    display.println("Inicializando...");
    display.display();
    return;
  }

  Serial.println("[DISPLAY] ERRO! OLED não encontrado!");
  displayOK = false;
}

// ====== SETUP ======
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n=== Franzininho WiFi LAB - MQTT Controle ===");
  Serial.println("Autor: IkkiKuuro + Cauã");
  Serial.println("Data: 2025-11-02");
  Serial.println("\n[INFO] MQTT funcionando normalmente");
  Serial.println("[INFO] Cassino oculto - requer ativação especial");

  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);
  pinMode(BTN4_PIN, INPUT_PULLUP);
  pinMode(BTN5_PIN, INPUT_PULLUP);
  pinMode(BTN6_PIN, INPUT_PULLUP);

  pinMode(LDR_PIN, INPUT);  // Sensor de luminosidade

  // Inicializa LED desligado
  setLEDColor(0, 0, 0);

  initDisplay();
  delay(2000);

  if (displayOK) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Conectando WiFi...");
    display.display();
  }
  
  connectWiFi();
  
  // Calibra o LDR automaticamente
  calibrateLDR();
  
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(mqttCallback);
  
  ensureMqtt();

  dht.begin();
  
  // Carrega dados salvos
  loadData();
  
  // Publica status inicial do cassino
  mqtt.publish(TOPIC_CASINO_STATUS, "LOCKED", true);
  Serial.println("[CASSINO] Status inicial: LOCKED");
  
  Serial.println("\n[SETUP] Concluído!");
  Serial.println("[INFO] Cassino oculto - requer ativação especial");
}

// ====== LOOP ======
void loop() {
  if (WiFi.status() != WL_CONNECTED) connectWiFi();
  if (!mqtt.connected()) ensureMqtt();
  mqtt.loop();

  unsigned long now = millis();
  
  // Publica sensores a cada 5 segundos
  if (now - lastSensorPublish > 5000) {
    lastSensorPublish = now;
    publishSensors();
    
    // Publica status do cassino periodicamente
    if (casinoUnlocked) {
      mqtt.publish(TOPIC_CASINO_STATUS, "UNLOCKED", true); // Retained message
    } else {
      mqtt.publish(TOPIC_CASINO_STATUS, "LOCKED", true); // Retained message
    }
  }

  // Atualiza display a cada 1 segundo
  if (now - lastDisplayUpdate > 1000) {
    lastDisplayUpdate = now;
    updateDisplay();
  }

  drawScrollingMessage();
  checkButtons();

  delay(20);
}
