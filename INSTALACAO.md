# 🚀 GUIA RÁPIDO DE INSTALAÇÃO - FRANZININHO CASINO

## ⚡ Instalação Rápida (5 minutos)

### 1️⃣ Instalar Arduino IDE

Baixe e instale: https://www.arduino.cc/en/software

### 2️⃣ Adicionar Suporte ESP32

1. Abra Arduino IDE
2. File → Preferences
3. Em "Additional Board Manager URLs", adicione:
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
4. Tools → Board → Boards Manager
5. Procure por "esp32" e instale **"esp32 by Espressif Systems"**

### 3️⃣ Instalar Bibliotecas

Vá em: **Sketch → Include Library → Manage Libraries**

Instale as seguintes bibliotecas (procure pelo nome exato):

```
✅ PubSubClient         (versão 2.8 ou superior)
✅ DHT sensor library   (versão 1.4 ou superior)
✅ Adafruit GFX Library (versão 1.11 ou superior)
✅ Adafruit SSD1306     (versão 2.5 ou superior)
```

**IMPORTANTE:** As bibliotecas `WiFi`, `Wire` e `Preferences` já vêm incluídas no ESP32 core, não precisa instalar!

### 4️⃣ Configurar a Placa

1. **Tools → Board → ESP32 Arduino → ESP32 Dev Module** (ou Franzininho WiFi se aparecer)
2. **Tools → Port → COMx** (Windows) ou **/dev/ttyUSB0** (Linux/Mac)
3. **Tools → Upload Speed → 115200**

### 5️⃣ Editar Credenciais Wi-Fi

Abra o arquivo: `codigo_esp32_atualizado/codigo_esp32_atualizado.ino`

Altere as linhas 9-10:

```cpp
const char* WIFI_SSID = "SEU_WIFI_AQUI";
const char* WIFI_PASS = "SUA_SENHA_AQUI";
```

### 6️⃣ Fazer Upload

1. Conecte o ESP32 via USB
2. Clique no botão **Upload** (→)
3. Aguarde a mensagem "Done uploading"

### 7️⃣ Verificar Funcionamento

1. Abra o **Serial Monitor** (Tools → Serial Monitor)
2. Configure para **115200 baud**
3. Você deve ver:
   ```
   === Franzininho WiFi LAB - MQTT Controle ===
   Conectando WiFi...
   WiFi conectado: 192.168.x.x
   [MQTT] Conectando... conectado!
   Digite a sequencia secreta para desbloquear o casino:
   CIMA, CIMA, BAIXO, BAIXO, VOLTAR, CONFIRMAR, VOLTAR, CONFIRMAR
   ```

### 8️⃣ Desbloquear o Casino

Pressione os botões da placa nesta ordem:
```
BTN1 → BTN1 → BTN2 → BTN2 → BTN3 → BTN4 → BTN3 → BTN4
```

Você ouvirá a música do baú de Zelda! 🎵

### 9️⃣ Abrir as Páginas Web

1. Navegue até a pasta `mqtt/`
2. Abra `casino_menu.html` em um navegador moderno (Chrome/Firefox/Edge)
3. Aguarde a conexão MQTT (●✅ aparecerá)
4. Escolha um jogo e divirta-se! 🎰

---

## 🔧 Resolução de Problemas Comuns

### Erro: "Preferences.h: No such file or directory"

**Solução:** Certifique-se de que selecionou a placa ESP32 correta em Tools → Board.

### Erro: "WiFi.h: No such file or directory"

**Solução:** Você precisa ter o ESP32 core instalado (passo 2).

### Erro: "Port not found" ou "Serial port unavailable"

**Soluções:**
1. **Windows:** Instale o driver CH340/CP2102 
   - Download: https://sparks.gogo.co.nz/ch340.html
2. **Linux:** Adicione seu usuário ao grupo dialout
   ```bash
   sudo usermod -a -G dialout $USER
   ```
   (depois, faça logout e login novamente)
3. Teste outro cabo USB (muitos cabos são apenas para carga)

### Erro: "A fatal error occurred: Failed to connect"

**Solução:** 
1. Segure o botão **BOOT** na placa
2. Clique em Upload
3. Solte o botão BOOT quando aparecer "Connecting..."

### Display OLED não funciona

**Solução:**
1. Verifique as conexões SDA (GPIO 8) e SCL (GPIO 9)
2. O endereço I2C pode ser 0x3C ou 0x3D. Se não funcionar, troque no código:
   ```cpp
   #define OLED_ADDRESS 0x3D  // Tente 0x3D se 0x3C não funcionar
   ```

### LED RGB cores invertidas

**Solução:** Seu LED pode ser Common Anode. Na função `setLEDColor()`, inverta os valores:

```cpp
void setLEDColor(int r, int g, int b) {
  currentRed = constrain(r, 0, 255);
  currentGreen = constrain(g, 0, 255);
  currentBlue = constrain(b, 0, 255);
  
  // ADICIONE ESTAS LINHAS para inverter (Common Anode):
  analogWrite(LED_R_PIN, 255 - currentRed);
  analogWrite(LED_G_PIN, 255 - currentGreen);
  analogWrite(LED_B_PIN, 255 - currentBlue);
  // ...
}
```

---

## 📝 Checklist Final

Antes de usar o casino, verifique:

- [ ] ESP32 conectado ao Wi-Fi (LED azul aceso)
- [ ] MQTT conectado (mensagem no Serial Monitor)
- [ ] Display OLED funcionando (mostra mensagens)
- [ ] Botões respondem (som ao pressionar)
- [ ] LED RGB muda de cor
- [ ] Sequência secreta foi digitada (música tocou)
- [ ] Página web conectou ao MQTT (✅ no status)

---

## 🆘 Ainda com Problemas?

1. **Verifique o Serial Monitor** - Mensagens de erro aparecem lá
2. **Teste componente por componente** - Isole o problema
3. **Use o código exemplo** - Teste bibliotecas separadamente
4. **Abra uma Issue** no GitHub com:
   - Mensagens de erro completas
   - Fotos da montagem
   - Versão da Arduino IDE
   - Modelo do ESP32

---

## 💡 Dicas Profissionais

### Performance
- Use alimentação externa (5V, 2A) para projetos maiores
- Evite cabos USB muito longos
- Mantenha a placa longe de fontes de interferência

### Debugging
- Use `Serial.println()` liberalmente
- Teste cada função isoladamente
- Monitore o consumo de memória

### Expansão Futura
O código está preparado para:
- Adicionar mais jogos (6 botões disponíveis)
- Integrar mais sensores
- Controlar mais LEDs (PWM disponível)
- Adicionar speaker melhor

---

## 🎉 Pronto!

Agora você tem um **cassino IoT completo** funcionando! 

🎰 **BOA SORTE NOS JOGOS!** 🎰

Para mais informações, consulte o `README.md` principal.
