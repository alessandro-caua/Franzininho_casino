# 🎮 GUIA DE CONTROLES - FRANZININHO CASINO

## 🔘 MAPEAMENTO DOS BOTÕES

### Botões Principais (1-4)
- **BTN 1** 🔼 - **CIMA** (Navegação para cima, aumentar aposta)
- **BTN 2** 🔽 - **BAIXO** (Navegação para baixo, diminuir aposta)
- **BTN 3** ⬅️ - **VOLTAR** (Cancelar, voltar ao menu)
- **BTN 4** ✅ - **CONFIRMAR/SELECIONAR** (Confirmar ação, apostar, atirar)

### Botões Extras (5-6)
- **BTN 5** ⚙️ - **EXTRA 1** (Reservado para funções futuras)
- **BTN 6** ⚙️ - **EXTRA 2** (Reservado para funções futuras)

---

## 🎰 COMO JOGAR COM OS BOTÕES

### 🎡 ROLETA DE APOSTAS (roleta_apostas.html)
1. Use **BTN1 (CIMA)** e **BTN2 (BAIXO)** para ajustar o valor da aposta
2. Use **BTN4 (CONFIRMAR)** para fazer a aposta na cor selecionada
3. Use **BTN3 (VOLTAR)** para cancelar ou voltar
4. Pressione **BTN4** novamente para girar a roleta

### 🃏 BLACKJACK 21 (blackjack_21.html)
1. Use **BTN1 (CIMA)** e **BTN2 (BAIXO)** para navegar entre as opções
2. Use **BTN4 (CONFIRMAR)** para:
   - Selecionar "HIT" (pedir carta)
   - Selecionar "STAND" (parar)
   - Confirmar aposta
3. Use **BTN3 (VOLTAR)** para voltar ao menu

### 💀 ROLETA RUSSA (russian_roulette.html)
1. Use **BTN1 (CIMA)** e **BTN2 (BAIXO)** para escolher o alvo
2. Use **BTN4 (CONFIRMAR)** para confirmar o alvo e atirar
3. Use **BTN3 (VOLTAR)** para voltar ao menu

---

## 🔊 FEEDBACK SONORO

Cada botão tem um som característico:
- **BTN1/BTN2 (Navegação)**: Som curto e agudo (800Hz, 80ms) 🎵
- **BTN3 (Voltar)**: Som grave (400Hz, 150ms) 🔉
- **BTN4 (Confirmar)**: Som agudo de confirmação (1200Hz, 150ms) 🔔
- **BTN5/BTN6**: Som neutro (700Hz, 100ms) 🎶

---

## 🌈 FEEDBACK VISUAL (LED RGB)

O LED RGB muda de cor conforme o contexto do jogo:

### Roleta de Apostas
- **Vermelho** (255,0,0) - Venceu na cor vermelha
- **Azul** (0,0,255) - Venceu na cor azul
- **Branco** (255,255,255) - Venceu na cor branca (2x prêmio!)

### Blackjack 21
- **Verde** (0,255,0) - Sua vez de jogar
- **Amarelo** (255,255,0) - Dealer jogando
- **Ciano** (0,255,255) - Blackjack natural!
- **Vermelho** (255,0,85) - Bust!

### Roleta Russa
- **Cor do jogador atual** - Indica vez de jogar
- **Vermelho piscante** - Tiro letal
- **Cinza** - Tiro em branco
- **Verde** - Vencedor

---

## 📺 MENSAGENS NO DISPLAY OLED

O display mostra informações contextuais:
- Nome do jogador atual
- Valor da aposta
- Cartas na mão (Blackjack)
- Vidas restantes (Roleta Russa)
- Resultado das rodadas

---

## 🔄 DEBOUNCE E PROTEÇÃO

- Tempo de debounce: **200ms** (evita leituras duplas)
- Feedback visual: **300ms** (confirmação visual)
- Som de confirmação: **80-150ms** (não interfere no jogo)

---

## 📡 TÓPICOS MQTT UTILIZADOS

### Publicados pelo ESP32:
- `IFCE_caua/botoes/BOTAO_1` - Botão 1 pressionado
- `IFCE_caua/botoes/BOTAO_2` - Botão 2 pressionado
- `IFCE_caua/botoes/BOTAO_3` - Botão 3 pressionado
- `IFCE_caua/botoes/BOTAO_4` - Botão 4 pressionado
- `IFCE_caua/temperatura` - Leitura DHT11
- `IFCE_caua/umidade` - Leitura DHT11

### Recebidos pelo ESP32:
- `IFCE_caua/led_control` - Controle RGB (formato: "R,G,B")
- `IFCE_caua/buzzer_control` - Frequência do buzzer (Hz)
- `IFCE_caua/display_msg` - Mensagem para o display
- `IFCE_caua/roulette_spin` - Roleta girando
- `IFCE_caua/roulette_result` - Resultado da roleta
- `IFCE_caua/blackjack_turn` - Turno do Blackjack
- `IFCE_caua/russian_turn` - Turno da Roleta Russa
- `IFCE_caua/russian_shot` - Resultado do tiro

---

## 🛠️ SOLUÇÃO DE PROBLEMAS

### Botões não respondem?
1. Verifique conexão MQTT (LED verde no status)
2. Confirme que os pinos estão corretos (BTN1=2, BTN2=3, BTN3=4, BTN4=5)
3. Verifique se o debounce não está muito alto

### LED não muda de cor?
1. Confirme conexões RGB (R=14, G=13, B=12)
2. Verifique se os valores RGB estão corretos (0-255)
3. Teste manualmente via mqtt_viewer.html

### Display não mostra nada?
1. Verifique I2C (SDA=8, SCL=9)
2. Confirme endereço 0x3C
3. Reinicie o ESP32

### Som não toca?
1. Verifique buzzer no pino 17
2. Teste via mqtt_viewer.html com frequências 50-5000Hz
3. Confirme que o buzzer não está invertido

---

## 🎯 DICAS DE JOGO

### Roleta de Apostas
- Branco paga 2x! Mas tem apenas 2 casas.
- Azul e Vermelho têm 4 casas cada, mais seguro.

### Blackjack 21
- Dealer sempre tira até 17 ou mais.
- Blackjack natural (A + figura) paga mais!
- Não ultrapasse 21 ou perde na hora.

### Roleta Russa
- Atirar em si mesmo com tiro vazio = turno extra! 🎯
- Máximo de 3 balas letais por rodada
- Último sobrevivente leva 2000 fichas! 💰

---

## ✅ CHECKLIST DE HARDWARE

- [ ] ESP32 Franzininho WiFi LAB conectado
- [ ] 6 botões nos pinos 2-7 (pull-up interno ativo)
- [ ] LED RGB nos pinos 14 (R), 13 (G), 12 (B)
- [ ] Buzzer no pino 17
- [ ] Display OLED I2C (SDA=8, SCL=9, 0x3C)
- [ ] DHT11 no pino 15
- [ ] WiFi configurado (SSID e senha no código)
- [ ] Broker MQTT: broker.hivemq.com:1883

---

**Desenvolvido por IkkiKuuro + Cauã** 🤖
**Versão: 2.0 - Casino Edition** 🎰
**Data: 2025-01-30** 📅
