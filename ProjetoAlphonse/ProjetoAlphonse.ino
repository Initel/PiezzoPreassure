#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// Pinos de comunicação com DFPlayer
static const uint8_t DFPLAYER_RX = 10; // Arduino RX para DFPlayer TX
static const uint8_t DFPLAYER_TX = 11; // Arduino TX para DFPlayer RX

SoftwareSerial dfSerial(DFPLAYER_RX, DFPLAYER_TX);
DFRobotDFPlayerMini dfplayer;

// Sensor FSR
const int fsrPin = A0;
bool isPisando = false;

// Limiares de detecção (ajuste se necessário)
const int LIMIAR_PISOU = 450;
const int LIMIAR_LEVANTOU = 300;

void setup() {
  Serial.begin(115200);
  
  // Começa comunicação DFPlayer
  dfSerial.begin(9600);
  if (!dfplayer.begin(dfSerial)) {
    Serial.println("Erro ao iniciar DFPlayer!");
    while (true) {} // Para se falhar
  }

  Serial.println("DFPlayer iniciado.");

  // Definir volume inicial
  dfplayer.volume(10); // 0 a 30

  // Não tocar nada inicialmente
  dfplayer.stop();
}

void loop() {
  int fsrValue = analogRead(fsrPin);
  
  // Ajusta volume conforme força da pisada
  int volume = map(fsrValue, 0, 1023, 5, 30);
  volume = constrain(volume, 5, 30);
  dfplayer.volume(volume);

  // Detectar pisou (transição de não pisando para pisando)
  if (fsrValue > LIMIAR_PISOU && !isPisando) {
    isPisando = true;
    Serial.println("Pisou");
    dfplayer.play(1); // Tocar som 0001.mp3
    delay(50); // Debounce simples
  }

  // Detectar levantou (transição de pisando para não pisando)
  if (fsrValue < LIMIAR_LEVANTOU && isPisando) {
    isPisando = false;
    Serial.println("Levantou");
    dfplayer.play(2); // Tocar som 0002.mp3
    delay(50);
  }

  delay(5); // Loop rápido
}
