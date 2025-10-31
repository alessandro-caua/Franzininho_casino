#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ====== CONFIG Wi-Fi ======
const char* WIFI_SSID = "brisa-463544";
const char* WIFI_PASS = "oyfhnkvp";

//const char* WIFI_SSID = "IFCE";
//const char* WIFI_PASS = "IFCE1234";

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
const char* TOPIC_LED_CONTROL = "IFCE_caua/led_control";      // NOVO: Controle do LED
const char* TOPIC_LED_STATUS = "IFCE_caua/led_status";
const char* TOPIC_DISPLAY_MSG = "IFCE_caua/display_msg";
const char* TOPIC_TEMP = "IFCE_caua/temperatura";             // NOVO
const char* TOPIC_HUMIDITY = "IFCE_caua/umidade";             // NOVO
const char* TOPIC_LUMINOSITY = "IFCE_caua/luminosidade";      // NOVO: Sensor LDR
const char* TOPIC_ROULETTE_SPIN = "IFCE_caua/roulette_spin";  // NOVO: Roleta girando
const char* TOPIC_ROULETTE_RESULT = "IFCE_caua/roulette_result"; // NOVO: Resultado da roleta

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
unsigned long lastBtnPress[6] = {0, 0, 0, 0, 0, 0};  // Debounce timing
const unsigned long DEBOUNCE_DELAY = 200;  // 200ms debounce
bool ledState = false;
int currentRed = 0, currentGreen = 0, currentBlue = 0;  // Valores RGB atuais
unsigned long lastLedToggle = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long lastSensorPublish = 0;  // NOVO: para publicar sensores
bool displayOK = false;
String scrollingMsg = "";
unsigned long scrollTimer = 0;
int scrollX = SCREEN_WIDTH;

// Botões com funções específicas
const char* BTN_NAMES[6] = {"CIMA", "BAIXO", "VOLTAR", "CONFIRMAR", "EXTRA1", "EXTRA2"};

// ====== FUNÇÕES ======

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
      mqtt.publish(TOPIC_BUZZER_STATUS, "Frequencia invalida!");
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
      Serial.println("[LED] Formato invalido! Use: R,G,B");
    }
  }

  // ====== MENSAGEM NO DISPLAY ======
  else if (String(topic) == TOPIC_DISPLAY_MSG) {
    showScrollingMessage(msg);
  }

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
    int melody[] = {523, 659, 784, 1047}; // Dó, Mi, Sol, Dó (oitava acima)
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
      delay(3000); // Mostra resultado por 3 segundos
    }
  }
}

void ensureMqtt() {
  while (!mqtt.connected()) {
    Serial.print("[MQTT] Conectando...");
    if (mqtt.connect(clientId.c_str())) {
      Serial.println(" conectado!");
      mqtt.subscribe(TOPIC_BUZZER_CONTROL);
      mqtt.subscribe(TOPIC_LED_CONTROL);      // NOVO
      mqtt.subscribe(TOPIC_DISPLAY_MSG);
      mqtt.subscribe(TOPIC_ROULETTE_SPIN);    // NOVO: Roleta
      mqtt.subscribe(TOPIC_ROULETTE_RESULT);  // NOVO: Roleta

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
        
        String topic = String(TOPIC_BTN) + "/BOTAO_" + String(i + 1);
        mqtt.publish(topic.c_str(), "PRESSIONADO");
        Serial.printf("[BOTAO %d - %s] Pressionado\n", i + 1, BTN_NAMES[i]);
        
        // Feedback sonoro diferenciado
        if (i == 0 || i == 1) {
          // BTN1 (CIMA) e BTN2 (BAIXO) - som curto de navegação
          tone(BUZZER_PIN, 800, 80);
        } else if (i == 2) {
          // BTN3 (VOLTAR) - som grave de cancelar
          tone(BUZZER_PIN, 400, 150);
        } else if (i == 3) {
          // BTN4 (CONFIRMAR) - som agudo de confirmação
          tone(BUZZER_PIN, 1200, 150);
        } else {
          // BTN5 e BTN6 - som neutro
          tone(BUZZER_PIN, 700, 100);
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
          delay(300); // Mostra por 300ms
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
  
  display.setCursor(0, 0);
  display.println("=== FRANZININHO ===");
  
  display.setCursor(0, 12);
  display.print("WiFi: ");
  display.println(WiFi.status() == WL_CONNECTED ? "OK" : "OFF");
  
  display.setCursor(0, 22);
  display.print("MQTT: ");
  display.println(mqtt.connected() ? "OK" : "OFF");
  
  display.setCursor(0, 32);
  display.printf("RGB:%d,%d,%d", currentRed, currentGreen, currentBlue);
  
  display.setCursor(0, 42);
  display.print("Uptime: ");
  display.print(millis() / 1000);
  display.println("s");
  
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
  // Converte para porcentagem (0-4095 -> 0-100%)
  int luminosity = map(ldrValue, 0, 4095, 0, 100);
  
  mqtt.publish(TOPIC_LUMINOSITY, String(luminosity).c_str());
  Serial.printf("[SENSOR] Luminosidade: %d%% (ADC: %d)\n", luminosity, ldrValue);
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

  Serial.println("[DISPLAY] ERRO! OLED nao encontrado!");
  displayOK = false;
}

// ====== SETUP ======
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n=== Franzininho WiFi LAB - MQTT Controle ===");
  Serial.println("Autor: IkkiKuuro + Cauã");
  Serial.println("Data: 2025-10-30");

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
  
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(mqttCallback);
  
  ensureMqtt();

  dht.begin();
  
  Serial.println("\n[SETUP] Concluído!");
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
