# 🔓 GUIA VISUAL - SEQUÊNCIA SECRETA

## 🎮 Como Desbloquear o Casino

### Sequência dos Botões

Pressione os botões **nesta ordem exata**:

```
┌─────────────────────────────────────┐
│   SEQUÊNCIA KONAMI CODE CASINO      │
└─────────────────────────────────────┘

1️⃣  Pressione: BTN1 (CIMA) 🔼
    Som: Beep curto

2️⃣  Pressione: BTN1 (CIMA) 🔼
    Som: Beep curto (2x)

3️⃣  Pressione: BTN2 (BAIXO) 🔽
    Som: Beep curto

4️⃣  Pressione: BTN2 (BAIXO) 🔽
    Som: Beep curto (2x)

5️⃣  Pressione: BTN3 (VOLTAR) ⬅️
    Som: Beep grave

6️⃣  Pressione: BTN4 (CONFIRMAR) ✅
    Som: Beep agudo

7️⃣  Pressione: BTN3 (VOLTAR) ⬅️
    Som: Beep grave

8️⃣  Pressione: BTN4 (CONFIRMAR) ✅
    Som: Beep agudo

✨ DESBLOQUEADO! ✨
🎵 Música do baú de Zelda toca
🌈 LED RGB faz animação arco-íris
💰 Display mostra seus créditos
```

---

## ⚠️ IMPORTANTE

### Regras da Sequência

- ⏱️ **Timeout:** Você tem **3 segundos** entre cada botão
- ❌ **Erro:** Se errar, a sequência reseta automaticamente
- 🔁 **Tentativas:** Ilimitadas! Continue tentando
- 📺 **Display:** Mostra progresso (X/8) na tela OLED

### Dicas

✅ **FAÇA:**
- Pressione os botões com calma
- Observe o display OLED
- Escute os beeps de confirmação
- Espere o som de cada botão antes de pressionar o próximo

❌ **NÃO FAÇA:**
- Não pressione muito rápido
- Não pressione dois botões ao mesmo tempo
- Não espere mais de 3 segundos entre botões
- Não desanime se errar!

---

## 🎵 Música de Desbloqueio

Quando você completar a sequência correta, ouvirá a música do **baú de Zelda**:

```
♪ Sol - Lá - Si - Dó - Ré - Mi - Fá - Sol - Lá ♪
  G4   A4  B4  C5  D5  E5  F5  G5   A5
```

Ao mesmo tempo:
- 🔴 LED pisca em **vermelho**
- 🟢 LED pisca em **verde**
- 🔵 LED pisca em **azul**
- 🟡 LED fica **dourado** no final
- 📺 Display mostra "**CASINO UNLOCKED!**"

---

## 📊 Indicadores Visuais

### No Display OLED

#### Antes de Desbloquear:
```
═══ FRANZININHO ═══
  CASINO BLOQUEADO
  Digite sequencia
    nos botoes...
       [0/8]
```

#### Durante a Sequência:
```
═══ FRANZININHO ═══
  CASINO BLOQUEADO
  Digite sequencia
    nos botoes...
       [3/8]  ← Progresso!
```

#### Após Desbloquear:
```
    === CASINO ===
  Creditos: 1000
   Jogos: 0
   Vitorias: 0
  WiFi: OK
  MQTT: OK
```

### LED RGB

| Estado | Cor | Significado |
|--------|-----|-------------|
| Bloqueado | Apagado | Aguardando sequência |
| Digitando | Azul piscando | Processando entrada |
| Erro | Vermelho | Sequência incorreta |
| Desbloqueando | Arco-íris | Sequência correta! |
| Desbloqueado | Dourado | Casino liberado |

---

## 🔊 Sons de Feedback

### Botões

| Botão | Som | Frequência |
|-------|-----|------------|
| BTN1 (CIMA) | Beep curto | 800Hz, 80ms |
| BTN2 (BAIXO) | Beep curto | 800Hz, 80ms |
| BTN3 (VOLTAR) | Beep grave | 400Hz, 150ms |
| BTN4 (CONFIRMAR) | Beep agudo | 1200Hz, 150ms |

### Estados

| Estado | Som | Descrição |
|--------|-----|-----------|
| Progresso | Beep ascendente | 600Hz + (100Hz × índice) |
| Erro | Beep prolongado | 200Hz, 300ms |
| Desbloqueado | Melodia Zelda | 9 notas ascendentes |

---

## 🎯 Passo a Passo Completo

### 1. Preparação

```bash
✅ ESP32 ligado e conectado ao Wi-Fi
✅ MQTT conectado
✅ Display OLED funcionando
✅ Botões respondendo
```

### 2. Visualização

Veja no Serial Monitor (115200 baud):

```
[SETUP] Concluído!
Digite a sequencia secreta para desbloquear o casino:
CIMA, CIMA, BAIXO, BAIXO, VOLTAR, CONFIRMAR, VOLTAR, CONFIRMAR
```

### 3. Execução

Siga a sequência conforme mostrado acima.

A cada botão correto, você verá:

```
[SEQUENCIA] Progresso: 1/8
[SEQUENCIA] Progresso: 2/8
[SEQUENCIA] Progresso: 3/8
...
```

### 4. Sucesso!

Quando completar:

```
[CASINO] 🎰 DESBLOQUEADO! Bem-vindo ao Franzininho Casino!
[CASINO] 🎵 Tocando musica do bau de Zelda!
[LED] Cor definida: RGB(255,215,0)
[SAVE] Dados salvos!
```

### 5. Confirmação Web

Abra `casino_menu.html` no navegador e você verá:
- ✅ Status MQTT conectado
- 💰 Seus créditos (1000 iniciais)
- 🎮 Todos os 5 jogos disponíveis

---

## 🐛 Troubleshooting da Sequência

### Problema: Sequência não funciona

**Sintomas:**
- Botões pressionados mas nada acontece
- Contador não avança
- Sem som de feedback

**Soluções:**

1. **Verifique o Serial Monitor:**
   ```
   [BOTAO X - NOME] Pressionado
   ```
   Se não aparecer, problema nos botões.

2. **Teste botão por botão:**
   - Pressione BTN1 e veja se imprime no Serial
   - Repita para BTN2, BTN3, BTN4

3. **Verifique debounce:**
   - Se contador pula (0 → 2), aumente `DEBOUNCE_DELAY`
   ```cpp
   const unsigned long DEBOUNCE_DELAY = 300;  // Era 200
   ```

4. **Reset manual:**
   - Aperte o botão RESET no ESP32
   - Aguarde reconexão

### Problema: Contador volta para 0

**Causa:** Timeout de 3 segundos excedido

**Solução:** Pressione mais rápido ou aumente o timeout:

```cpp
const unsigned long SEQUENCE_TIMEOUT = 5000;  // Era 3000 (5 segundos agora)
```

### Problema: Som não toca

**Verificar:**

1. **Buzzer conectado?**
   - GPIO 17 → Buzzer (+)
   - GND → Buzzer (-)

2. **Buzzer correto?**
   - Use buzzer **passivo** (não ativo)
   - Teste com `tone(BUZZER_PIN, 1000, 500);`

3. **Volume baixo?**
   - Alguns buzzers precisam de transistor
   - Teste com resistor menor

---

## 📸 Checklist Visual

Antes de tentar a sequência, confirme:

- [ ] 🔌 ESP32 alimentado (LED azul aceso)
- [ ] 📡 WiFi conectado (mensagem no Serial)
- [ ] 🌐 MQTT conectado (● no display)
- [ ] 📺 Display mostra "CASINO BLOQUEADO"
- [ ] 🔘 Botões respondem (teste cada um)
- [ ] 🔊 Buzzer funciona (teste com código)
- [ ] 💡 LED RGB muda de cor (teste RGB)

---

## 🏆 Após Desbloquear

### O que você pode fazer:

1. **Abrir qualquer jogo:**
   - Slot Machine 🎰
   - Dados/Craps 🎲
   - Blackjack 21 🃏
   - Roleta RGB 🎡
   - Roleta Russa 💀

2. **Controlar pelo botões:**
   - BTN1/BTN2: Ajustar apostas
   - BTN4: Jogar/Confirmar
   - BTN3: Voltar

3. **Ver estatísticas:**
   - Créditos
   - Vitórias
   - Jogos realizados
   - Conquistas

4. **Acompanhar pelo display:**
   - Status em tempo real
   - Mensagens do jogo
   - Créditos atualizados

---

## 💡 Curiosidades

### Por que Konami Code?

O **Konami Code** é uma sequência famosa de botões usada em jogos clássicos da Konami (Contra, Gradius, etc.) para desbloquear cheats. A sequência original era:

```
↑ ↑ ↓ ↓ ← → ← → B A
```

Adaptamos para o nosso projeto! 🎮

### Por que a música de Zelda?

A música que toca quando você abre um **baú** em *The Legend of Zelda* é icônica e representa perfeitamente a sensação de descoberta e recompensa - exatamente como desbloquear o casino! 🗝️

---

**🎰 Boa sorte desbloqueando o casino! 🎰**

*Se conseguir na primeira tentativa, você é um verdadeiro gamer!* 🏆
