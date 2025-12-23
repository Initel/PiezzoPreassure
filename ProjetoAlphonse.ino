#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mp3Serial(10,11);
DFRobotDFPlayerMini mp3;

const int footSensor = A0;

const int minImpacto = 120;
const int maxImpacto = 800;
const unsigned long pausa = 250;

unsigned long ultimoPasso = 0;

void setup() {
  pinMode(footSensor, INPUT);

  Serial.begin(115200);
  mp3Serial.begin(9600);

  if(!mp3.begin(mp3Serial)){
    Serial.println("Erro DFPlayer");
    while (true);
  }

  mp3.volume(20);

  Serial.pintln("Sistema iniciado");
  

}

void loop() {
  int impact = lendoOsPassos();

  if(impact >= minImpacto && millis() - ultimoPasso > pausa ){
    ultimoPasso = millis();

    int volume = map(impact, minImpacto, maxImpacto, 10, 30);
    volume = constrain(volume, 10, 30);

    Serial.print("Impacto: ");
    Serial.print(impact);
    Serial.print(" | Volume: ")
    Serial.pritnln(volume);

    mp3.volume(volume);
    mp3.play(1);
  }

}

void lendoOsPassos(){
  int pico = 0;
  unsigned long inicio = millis();

  while(millis() - inicio < 8){
    int leitura = analogRead(footSensor);
    if (leitura > pico){
      pico = leitura;
    }
  }

  return pico;
}