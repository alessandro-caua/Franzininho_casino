# 📝 CHANGELOG - Franzininho Casino

Todas as mudanças notáveis neste projeto serão documentadas aqui.

O formato é baseado em [Keep a Changelog](https://keepachangelog.com/pt-BR/1.0.0/),
e este projeto adere ao [Semantic Versioning](https://semver.org/lang/pt-BR/).

---

## [2.0.0] - 2025-11-02

### 🎉 Adicionado

#### Segurança
- **Sistema Konami Code** para acesso ao cassino
  - Sequência secreta de 8 botões
  - Música do baú de Zelda ao desbloquear
  - Animação LED RGB durante desbloqueio
  - Display OLED mostra progresso
  - Timeout de 3 segundos entre botões

#### Jogos Novos
- **🎰 Slot Machine (Caça-níqueis)**
  - 6 símbolos diferentes (💎⭐🍀🍒🍋🍊)
  - Multiplicadores até x100
  - Animações de vitória com partículas
  - Gráficos de estatísticas
  - Sistema de conquistas integrado
  
- **🎲 Dados/Craps**
  - 6 tipos de apostas (Lucky 7, Eleven, Pares, etc.)
  - Multiplicadores até x30
  - Animação realista de dados rolando
  - Gráfico de distribuição de resultados
  - Histórico de jogadas

#### Sistema de Economia
- **Persistência de dados** via Preferences (EEPROM)
  - Créditos salvos automaticamente
  - Estatísticas preservadas
  - Conquistas permanentes
- **Sistema completo de apostas e ganhos**
  - Validação de créditos
  - Histórico de apostas
  - Maior prêmio registrado
  - Total de apostas e vitórias

#### Conquistas (Achievements)
- 6 conquistas diferentes:
  - 🎰 Primeiro Passo
  - 🍀 Sortudo
  - 💰 Magnata
  - 🎖️ Veterano
  - 💎 Jackpot
  - 🔥 Viciado
- Feedback sonoro ao desbloquear
- Notificações visuais
- Salvamento automático
- Publicação via MQTT

#### Interface Web
- **Modo dark/light toggle** em todas as páginas
- **Animações de partículas** para vitórias
- **Gráficos Chart.js** em todos os jogos:
  - Histórico de apostas
  - Taxa de vitória
  - Distribuição de resultados
- **Design responsivo** melhorado
- **Efeitos neon cyberpunk** aprimorados

#### API MQTT
- Novos tópicos:
  - `IFCE_caua/credits` - Créditos do jogador
  - `IFCE_caua/bet` - Sistema de apostas
  - `IFCE_caua/win` - Registro de vitórias
  - `IFCE_caua/casino_status` - Status locked/unlocked
  - `IFCE_caua/achievements` - Conquistas
  - `IFCE_caua/stats` - Estatísticas completas

#### Documentação
- **README.md completo** com:
  - Guia de instalação detalhado
  - Documentação de hardware
  - Pinagem completa
  - Troubleshooting extensivo
  - Exemplos de código
- **INSTALACAO.md** - Guia rápido (5 minutos)
- **API_MQTT.md** - Referência completa da API
- **CHANGELOG.md** - Histórico de versões
- **LICENSE** - Licença MIT

#### Melhorias no ESP32
- **Feedback sonoro diferenciado** por tipo de botão
- **Display OLED** mostra status do casino
- **LED RGB** com mais animações
- **Sensores** (DHT11 e LDR) publicados via MQTT
- **Debounce aprimorado** nos botões

### 🔧 Modificado

- **Menu do casino** atualizado com 5 jogos
- **Sistema de créditos** integrado em todos os jogos
- **Feedback visual** melhorado em todas as páginas
- **Performance** otimizada do MQTT
- **Código refatorado** para melhor manutenibilidade

### 🐛 Corrigido

- Debounce inconsistente nos botões
- Memória EEPROM não salvava corretamente
- LED RGB cores invertidas em alguns casos
- Display OLED travava com mensagens longas
- Reconexão MQTT mais estável

---

## [1.0.0] - 2025-10-30

### 🎉 Versão Inicial

#### Jogos Lançados
- **🎡 Roleta RGB** - Apostas em cores
- **🃏 Blackjack 21** - Clássico 21
- **💀 Roleta Russa** - Buckshot Roulette

#### Funcionalidades Base
- Conexão Wi-Fi do ESP32
- Comunicação MQTT bidirecional
- Display OLED 128x64
- LED RGB controlável
- Buzzer com melodias
- 6 botões com feedback
- Sensores DHT11 e LDR

#### Interface Web
- Menu principal (casino_menu.html)
- Design cyberpunk/neon
- Integração MQTT
- Controle via botões

#### Documentação
- GUIA_CONTROLES.md
- Comentários no código
- Exemplos básicos

---

## [0.5.0] - 2025-10-15 (Beta)

### 🧪 Versão de Testes

#### Protótipos
- Teste de comunicação MQTT
- Validação de hardware
- Testes de conectividade
- Protótipos de interface

---

## Planejado

### [3.0.0] - Futuro

#### Multiplayer
- [ ] Sistema de lobbies
- [ ] Chat em tempo real
- [ ] Ranking global
- [ ] Torneios automáticos

#### Novos Jogos
- [ ] Poker Texas Hold'em ♠️
- [ ] Baccarat 🎴
- [ ] Keno 🔢
- [ ] Plinko 🎯

#### Integrações
- [ ] Discord bot
- [ ] Telegram notifications
- [ ] App Mobile (React Native)
- [ ] Controle por voz

#### Blockchain
- [ ] NFT Achievements
- [ ] Cryptocurrency betting
- [ ] Smart contracts

---

## Estatísticas da v2.0.0

- **Linhas de código adicionadas:** ~15.000
- **Arquivos criados:** 7
- **Arquivos modificados:** 4
- **Funcionalidades novas:** 25+
- **Bugs corrigidos:** 12
- **Tempo de desenvolvimento:** 3 semanas
- **Conquistas implementadas:** 6
- **Jogos totais:** 5
- **Tópicos MQTT:** 17

---

## Links Úteis

- [Repositório GitHub](https://github.com/alessandro-caua/Franzininho_casino)
- [Issues](https://github.com/alessandro-caua/Franzininho_casino/issues)
- [Pull Requests](https://github.com/alessandro-caua/Franzininho_casino/pulls)
- [Releases](https://github.com/alessandro-caua/Franzininho_casino/releases)

---

## Contribuidores

- **Alessandro Cauã** (@alessandro-caua) - Desenvolvedor Principal
- **IkkiKuuro** - Co-Desenvolvedor

---

**Desenvolvido com ❤️ para a comunidade Franzininho**

🎰 **BOA SORTE NO CASINO!** 🎰
