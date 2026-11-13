#include <DHT.h>
#include <Arduino.h>

DHT dht(4, DHT11);

int led1 = 18;
int led2 = 19;
int led3 = 21;

int ledVermelho = 22;
int ledAzul = 23;

int brilho = 0;
int passo = 5;
int ledDaVez = 1;

unsigned long tempoAnteriorLed = 0;
unsigned long tempoAnteriorSensor = 0;

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledAzul, OUTPUT);
}

void loop() {
  unsigned long tempoAtual = millis();

  if (tempoAtual - tempoAnteriorLed >= 15) {
    tempoAnteriorLed = tempoAtual;

    if (ledDaVez == 1) {
      analogWrite(led1, brilho);
    } else if (ledDaVez == 2) {
      analogWrite(led2, brilho);
    } else if (ledDaVez == 3) {
      analogWrite(led3, brilho);
    }

    brilho = brilho + passo;

    if (brilho >= 255) {
      brilho = 255;
      passo = -5;
    }

    if (brilho <= 0) {
      brilho = 0;
      passo = 5;
      ledDaVez = ledDaVez + 1;

      if (ledDaVez > 3) {
        ledDaVez = 1;
      }
    }
  }

  if (tempoAtual - tempoAnteriorSensor >= 2000) {
    tempoAnteriorSensor = tempoAtual;

    float umidade = dht.readHumidity();
    float temperatura = dht.readTemperature();

    if (!isnan(umidade) && !isnan(temperatura)) {
      int brilhoTemp = map(temperatura, 20, 35, 0, 255);
      brilhoTemp = constrain(brilhoTemp, 0, 255);

      int brilhoUmid = map(umidade, 30, 90, 0, 255);
      brilhoUmid = constrain(brilhoUmid, 0, 255);

      analogWrite(ledVermelho, brilhoTemp);
      analogWrite(ledAzul, brilhoUmid);
    }
  }
}
