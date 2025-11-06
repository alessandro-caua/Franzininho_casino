# 🎰 Como Ativar o Casino Escondido

## 🔐 Sistema de Segurança em 3 Camadas

O **Franzininho Casino** está completamente oculto! Por padrão, o dispositivo funciona como um MQTT normal com sensores, LED e display. Para acessar o casino, você precisa seguir os passos secretos:

---

## 📋 Passo a Passo de Ativação

### **ETAPA 1: MQTT Normal** ⚙️
Por padrão, o sistema funciona normalmente:
- ✅ Sensores publicam dados (temperatura, umidade, luminosidade)
- ✅ LED pode ser controlado via MQTT
- ✅ Display mostra mensagens
- ✅ Buzzer funciona
- ❌ Botões **NÃO** fazem nada especial (modo casino desativado)

---

### **ETAPA 2: Ativar Modo Casino** 🎯

Para revelar o casino escondido, você precisa fazer **DUAS COISAS** na ordem correta:

#### 1️⃣ **Enviar a mensagem secreta no display**
Via MQTT, publique no tópico `IFCE_caua/display_msg`:
```
casino hub
```
**Importante:** Pode ser maiúscula ou minúscula, mas precisa ser exatamente "casino hub"

#### 2️⃣ **Mudar o LED para vermelho**
Via MQTT, publique no tópico `IFCE_caua/led_control`:
```
255,0,0
```
**Importante:** Precisa ser EXATAMENTE vermelho (R=255, G=0, B=0)

#### ✅ **Confirmação da Ativação**
Quando você completar os dois requisitos, o sistema vai:
- 🔊 Tocar 3 beeps (800Hz → 1000Hz → 1200Hz)
- 💚 LED pisca VERDE por 0.5s
- 📺 Display mostra:
  ```
  CASINO
  ATIVADO!
  Insira Konami Code:
  ^ ^ v v < > < >
  ```
- ✅ Botões agora estão prontos para receber o Konami Code!

---

### **ETAPA 3: Inserir Konami Code** 🎮

Agora que o casino foi ativado, pressione a sequência de botões:

| Ordem | Botão | Nome      | Símbolo |
|-------|-------|-----------|---------|
| 1     | BTN1  | CIMA      | ↑       |
| 2     | BTN1  | CIMA      | ↑       |
| 3     | BTN2  | BAIXO     | ↓       |
| 4     | BTN2  | BAIXO     | ↓       |
| 5     | BTN3  | VOLTAR    | ←       |
| 6     | BTN4  | CONFIRMAR | →       |
| 7     | BTN3  | VOLTAR    | ←       |
| 8     | BTN4  | CONFIRMAR | →       |

**⏱️ ATENÇÃO:** Você tem apenas **3 segundos** entre cada botão! Se demorar muito, a sequência reseta.

#### ✅ **Desbloqueio Completo**
Quando você inserir a sequência correta, o sistema vai:
- 🎵 Tocar a **música do baú de Zelda** (9 notas)
- 🌈 LED pisca em arco-íris durante a música
- 🟡 LED fica DOURADO no final
- 📺 Display mostra:
  ```
  CASINO
  UNLOCKED!
  Credits: 1000
  ```
- 🎰 **CASINO TOTALMENTE LIBERADO!**

---

## 🎰 Após o Desbloqueio

Agora você pode:
- ✅ Usar os **6 botões** para controlar os jogos via MQTT
- ✅ Acessar os **5 jogos** via interface web:
  - 🎰 Slot Machine (Caça-níqueis)
  - 🎲 Dados/Craps
  - ♠️ Blackjack 21
  - 🌈 Roleta RGB
  - 🔫 Roleta Russa
- ✅ Gerenciar seus **créditos** (sistema de economia)
- ✅ Desbloquear **6 conquistas**
- ✅ Ver **estatísticas** em tempo real

---

## 🔄 Como Resetar

Para voltar ao modo MQTT normal e ocultar o casino novamente:
1. **Reinicie o ESP32** (botão RESET ou desplugue/replugue)
2. Todos os estados são resetados:
   - ❌ `casinoActivated = false`
   - ❌ `casinoUnlocked = false`
   - ❌ `casinoHubReceived = false`
   - ❌ `ledIsRed = false`

**OBS:** Seus créditos e estatísticas são salvos na memória EEPROM e **NÃO são perdidos**!

---

## 🛡️ Por Que Esse Sistema?

Este sistema de 3 camadas garante que:
1. **O casino fica completamente oculto** - Ninguém suspeita que existe
2. **MQTT funciona normalmente** - Você pode usar como projeto IoT comum
3. **Apenas quem souber o segredo** consegue acessar
4. **Não há rastros visuais** - Nada indica a existência do casino até ativar

---

## 📊 Resumo Visual

```
┌─────────────────────────────────────────┐
│      MODO NORMAL (Padrão)               │
│  ✅ Sensores funcionando                │
│  ✅ LED controlável                     │
│  ✅ Display funcionando                 │
│  ❌ Botões sem função especial          │
└─────────────────────────────────────────┘
                  ↓
          (Enviar "casino hub"
           + LED vermelho)
                  ↓
┌─────────────────────────────────────────┐
│    CASINO ATIVADO (Aguardando Konami)   │
│  ✅ Display mostra Konami Code          │
│  ✅ Botões habilitados para sequência   │
│  ❌ Jogos ainda bloqueados              │
└─────────────────────────────────────────┘
                  ↓
      (Inserir Konami Code correto)
                  ↓
┌─────────────────────────────────────────┐
│      CASINO DESBLOQUEADO 🎰             │
│  ✅ Música de Zelda                     │
│  ✅ LED dourado                         │
│  ✅ 5 jogos liberados                   │
│  ✅ Sistema de créditos ativo           │
│  ✅ Conquistas desbloqueáveis           │
└─────────────────────────────────────────┘
```

---

## 🎯 Testes Rápidos

### Teste 1: Verificar modo normal
```bash
# Sistema deve funcionar normalmente
# Botões não fazem nada especial
```

### Teste 2: Ativar casino
```bash
# Via MQTT:
Tópico: IFCE_caua/display_msg
Mensagem: "casino hub"

Tópico: IFCE_caua/led_control  
Mensagem: "255,0,0"

# Esperar beeps + display mostrar Konami Code
```

### Teste 3: Inserir Konami Code
```bash
# Pressionar botões físicos:
BTN1 → BTN1 → BTN2 → BTN2 → BTN3 → BTN4 → BTN3 → BTN4
# (em menos de 3s entre cada)

# Esperar música de Zelda + display "UNLOCKED"
```

### Teste 4: Acessar jogos
```bash
# Abrir navegador:
file:///c:/Users/Aless/Documents/GitHub/Franzininho_casino/mqtt/casino_menu.html

# Escolher qualquer um dos 5 jogos
```

---

## 🔧 Troubleshooting

### "Inseri 'casino hub' mas nada aconteceu"
- ✅ Certifique-se de que o LED está VERMELHO (255,0,0)
- ✅ Ambos os requisitos precisam estar ativos

### "LED vermelho mas nada aconteceu"  
- ✅ Certifique-se de enviar "casino hub" no display
- ✅ A ordem não importa, mas ambos precisam estar OK

### "Inseri Konami Code mas não desbloqueou"
- ❌ Você ativou o casino primeiro? (Etapa 2 completa?)
- ❌ Demorou mais de 3 segundos entre botões?
- ❌ Pressionou a sequência errada?

### "Sistema não reseta"
- 🔄 Pressione o botão RESET do ESP32
- 🔄 Ou desligue e ligue novamente

---

**🎰 Boa sorte descobrindo o casino escondido! 🍀**
