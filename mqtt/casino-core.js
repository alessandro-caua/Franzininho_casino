/**
 * FRANZININHO CASINO - Core Library
 * Biblioteca compartilhada para todas as páginas do cassino
 * Versão: 2.1
 */

// ====== SISTEMA DE NOTIFICAÇÕES TOAST ======
class ToastNotification {
  constructor() {
    this.container = null;
    this.init();
  }

  init() {
    // Cria container se não existir
    if (!document.getElementById('toast-container')) {
      this.container = document.createElement('div');
      this.container.id = 'toast-container';
      this.container.style.cssText = `
        position: fixed;
        top: 80px;
        right: 20px;
        z-index: 10000;
        display: flex;
        flex-direction: column;
        gap: 10px;
        pointer-events: none;
      `;
      document.body.appendChild(this.container);
    } else {
      this.container = document.getElementById('toast-container');
    }

    // Adiciona estilos CSS
    if (!document.getElementById('toast-styles')) {
      const style = document.createElement('style');
      style.id = 'toast-styles';
      style.textContent = `
        .toast {
          display: flex;
          align-items: center;
          gap: 12px;
          padding: 15px 20px;
          border-radius: 10px;
          border: 2px solid;
          box-shadow: 0 4px 20px rgba(0, 0, 0, 0.5);
          font-family: 'Orbitron', monospace;
          font-weight: 700;
          font-size: 0.95em;
          min-width: 300px;
          max-width: 400px;
          opacity: 0;
          transform: translateX(400px);
          transition: all 0.4s cubic-bezier(0.68, -0.55, 0.265, 1.55);
          pointer-events: auto;
          cursor: pointer;
        }
        .toast.show {
          opacity: 1;
          transform: translateX(0);
        }
        .toast-icon {
          font-size: 1.5em;
          flex-shrink: 0;
        }
        .toast-message {
          flex: 1;
          line-height: 1.4;
        }
        .toast-close {
          font-size: 1.2em;
          opacity: 0.7;
          transition: opacity 0.2s;
          cursor: pointer;
        }
        .toast-close:hover {
          opacity: 1;
        }
        .toast-success {
          background: rgba(0, 255, 65, 0.15);
          border-color: #00ff41;
          color: #00ff41;
        }
        .toast-error {
          background: rgba(255, 0, 85, 0.15);
          border-color: #ff0055;
          color: #ff0055;
        }
        .toast-warning {
          background: rgba(255, 170, 0, 0.15);
          border-color: #ffaa00;
          color: #ffaa00;
        }
        .toast-info {
          background: rgba(0, 255, 255, 0.15);
          border-color: #00ffff;
          color: #00ffff;
        }
      `;
      document.head.appendChild(style);
    }
  }

  show(message, type = 'info', duration = 4000) {
    const icons = {
      success: '✅',
      error: '❌',
      warning: '⚠️',
      info: 'ℹ️'
    };

    const toast = document.createElement('div');
    toast.className = `toast toast-${type}`;
    toast.innerHTML = `
      <div class="toast-icon">${icons[type]}</div>
      <div class="toast-message">${message}</div>
      <div class="toast-close">×</div>
    `;

    this.container.appendChild(toast);

    // Animação de entrada
    requestAnimationFrame(() => {
      toast.classList.add('show');
    });

    // Fechar ao clicar
    const closeBtn = toast.querySelector('.toast-close');
    closeBtn.addEventListener('click', () => {
      this.close(toast);
    });

    // Fechar automaticamente
    if (duration > 0) {
      setTimeout(() => {
        this.close(toast);
      }, duration);
    }

    return toast;
  }

  close(toast) {
    toast.classList.remove('show');
    setTimeout(() => {
      toast.remove();
    }, 400);
  }

  success(message, duration = 4000) {
    return this.show(message, 'success', duration);
  }

  error(message, duration = 5000) {
    return this.show(message, 'error', duration);
  }

  warning(message, duration = 4500) {
    return this.show(message, 'warning', duration);
  }

  info(message, duration = 3500) {
    return this.show(message, 'info', duration);
  }
}

// Instância global
const toast = new ToastNotification();

// ====== STATUS DE CONEXÃO MQTT ======
class ConnectionStatus {
  constructor() {
    this.banner = null;
    this.init();
  }

  init() {
    // Cria banner de status
    if (!document.getElementById('connection-status')) {
      this.banner = document.createElement('div');
      this.banner.id = 'connection-status';
      this.banner.style.cssText = `
        position: fixed;
        top: 0;
        left: 0;
        width: 100%;
        padding: 12px 20px;
        text-align: center;
        z-index: 9999;
        font-family: 'Orbitron', monospace;
        font-weight: 700;
        font-size: 0.9em;
        text-transform: uppercase;
        letter-spacing: 2px;
        transition: all 0.3s ease;
        transform: translateY(-100%);
        display: flex;
        align-items: center;
        justify-content: center;
        gap: 10px;
      `;
      document.body.appendChild(this.banner);

      // Adiciona estilos
      if (!document.getElementById('connection-styles')) {
        const style = document.createElement('style');
        style.id = 'connection-styles';
        style.textContent = `
          .connection-dot {
            width: 10px;
            height: 10px;
            border-radius: 50%;
            animation: pulse 2s infinite;
          }
          @keyframes pulse {
            0%, 100% { opacity: 1; }
            50% { opacity: 0.5; }
          }
          #connection-status.show {
            transform: translateY(0);
          }
          #connection-status.connected {
            background: linear-gradient(90deg, #00ff41, #00ffff);
            color: #000;
            box-shadow: 0 4px 20px rgba(0, 255, 65, 0.5);
          }
          #connection-status.connecting {
            background: linear-gradient(90deg, #ffaa00, #ff8800);
            color: #000;
            box-shadow: 0 4px 20px rgba(255, 170, 0, 0.5);
          }
          #connection-status.disconnected {
            background: linear-gradient(90deg, #ff0055, #ff4444);
            color: #fff;
            box-shadow: 0 4px 20px rgba(255, 0, 85, 0.5);
          }
        `;
        document.head.appendChild(style);
      }
    } else {
      this.banner = document.getElementById('connection-status');
    }
  }

  setStatus(status, message) {
    this.banner.className = `show ${status}`;
    
    let icon = '';
    if (status === 'connected') icon = '🟢';
    else if (status === 'connecting') icon = '🟡';
    else if (status === 'disconnected') icon = '🔴';

    this.banner.innerHTML = `
      <div class="connection-dot" style="background: currentColor;"></div>
      ${icon} ${message}
    `;
  }

  hide() {
    this.banner.classList.remove('show');
  }
}

// Instância global
const connectionStatus = new ConnectionStatus();

// ====== GERENCIADOR MQTT ======
class CasinoMQTT {
  constructor(config = {}) {
    this.broker = config.broker || 'broker.hivemq.com';
    this.port = config.port || 8884;
    this.useSSL = config.useSSL !== false;
    this.clientId = config.clientId || `casino-${Math.random().toString(16).slice(2, 10)}`;
    this.client = null;
    this.isConnected = false;
    this.reconnectAttempts = 0;
    this.maxReconnectDelay = 30000; // 30 segundos
    this.messageHandlers = {};
  }

  connect() {
    return new Promise((resolve, reject) => {
      connectionStatus.setStatus('connecting', 'Conectando ao servidor...');

      const protocol = this.useSSL ? 'wss' : 'ws';
      const port = this.useSSL ? 8884 : 8000;
      const url = `${protocol}://${this.broker}:${port}/mqtt`;

      this.client = mqtt.connect(url, {
        clientId: this.clientId,
        clean: true,
        reconnectPeriod: this.getReconnectDelay(),
        connectTimeout: 10000,
        keepalive: 60
      });

      this.client.on('connect', () => {
        this.isConnected = true;
        this.reconnectAttempts = 0;
        connectionStatus.setStatus('connected', 'Conectado ao servidor');
        toast.success('Conectado ao servidor MQTT!');
        
        // Oculta banner após 3 segundos
        setTimeout(() => {
          connectionStatus.hide();
        }, 3000);

        resolve();
      });

      this.client.on('message', (topic, message) => {
        const msg = message.toString();
        
        // Chama handlers registrados
        if (this.messageHandlers[topic]) {
          this.messageHandlers[topic].forEach(handler => handler(msg, topic));
        }

        // Handler curinga (todos os tópicos)
        if (this.messageHandlers['*']) {
          this.messageHandlers['*'].forEach(handler => handler(msg, topic));
        }
      });

      this.client.on('error', (err) => {
        console.error('[MQTT] Erro:', err);
        connectionStatus.setStatus('disconnected', 'Erro de conexão');
        toast.error(`Erro MQTT: ${err.message}`);
        reject(err);
      });

      this.client.on('reconnect', () => {
        this.reconnectAttempts++;
        connectionStatus.setStatus('connecting', `Reconectando (tentativa ${this.reconnectAttempts})...`);
        console.log(`[MQTT] Tentando reconectar (${this.reconnectAttempts})...`);
      });

      this.client.on('close', () => {
        this.isConnected = false;
        connectionStatus.setStatus('disconnected', 'Desconectado do servidor');
        console.log('[MQTT] Conexão fechada');
      });

      this.client.on('offline', () => {
        this.isConnected = false;
        connectionStatus.setStatus('disconnected', 'Servidor offline');
        toast.warning('Servidor offline. Tentando reconectar...');
      });
    });
  }

  getReconnectDelay() {
    // Backoff exponencial
    const baseDelay = 2000;
    const delay = Math.min(baseDelay * Math.pow(2, this.reconnectAttempts), this.maxReconnectDelay);
    return delay;
  }

  subscribe(topic, handler) {
    if (!this.client) {
      console.error('[MQTT] Cliente não iniciado!');
      return;
    }

    this.client.subscribe(topic, { qos: 0 }, (err) => {
      if (err) {
        console.error(`[MQTT] Erro ao subscrever ${topic}:`, err);
        toast.error(`Erro ao subscrever tópico: ${topic}`);
      } else {
        console.log(`[MQTT] Inscrito em: ${topic}`);
      }
    });

    // Registra handler
    if (!this.messageHandlers[topic]) {
      this.messageHandlers[topic] = [];
    }
    this.messageHandlers[topic].push(handler);
  }

  publish(topic, message, options = { qos: 0, retain: false }) {
    if (!this.client || !this.isConnected) {
      console.error('[MQTT] Não conectado!');
      toast.error('Não conectado ao servidor. Tente novamente.');
      return false;
    }

    this.client.publish(topic, message, options, (err) => {
      if (err) {
        console.error(`[MQTT] Erro ao publicar em ${topic}:`, err);
        toast.error('Erro ao enviar comando');
      }
    });

    return true;
  }

  disconnect() {
    if (this.client) {
      this.client.end();
      this.isConnected = false;
      connectionStatus.hide();
    }
  }
}

// ====== HELPER FUNCTIONS ======

// Converter Hex para RGB
function hexToRgb(hex) {
  const result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
  return result 
    ? `${parseInt(result[1], 16)},${parseInt(result[2], 16)},${parseInt(result[3], 16)}` 
    : '255,0,0';
}

// Formatar número com separador de milhares
function formatNumber(num) {
  return num.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ".");
}

// Play sound
function playSound(frequency, duration = 100, type = 'sine') {
  try {
    const audioContext = new (window.AudioContext || window.webkitAudioContext)();
    const osc = audioContext.createOscillator();
    const gain = audioContext.createGain();
    
    osc.connect(gain);
    gain.connect(audioContext.destination);
    
    osc.type = type;
    osc.frequency.value = frequency;
    gain.gain.value = 0.15;
    gain.gain.exponentialRampToValueAtTime(0.01, audioContext.currentTime + duration / 1000);
    
    osc.start();
    osc.stop(audioContext.currentTime + duration / 1000);
  } catch (e) {
    console.error('[SOUND] Erro:', e);
  }
}

// Cleanup automático ao sair
window.addEventListener('beforeunload', () => {
  if (window.casinoMQTT && window.casinoMQTT.isConnected) {
    // Reset LED e buzzer
    window.casinoMQTT.publish('IFCE_caua/led_control', '0,0,0');
    window.casinoMQTT.publish('IFCE_caua/buzzer_control', '0');
  }
});

// Exporta para uso global
window.CasinoMQTT = CasinoMQTT;
window.toast = toast;
window.connectionStatus = connectionStatus;
window.casinoUtils = {
  hexToRgb,
  formatNumber,
  playSound
};

console.log('[CASINO CORE] Biblioteca carregada ✅');
