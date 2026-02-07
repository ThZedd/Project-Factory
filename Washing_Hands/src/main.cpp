#include <Arduino.h>

// O LED azul integrado na placa costuma ser o pino 2
#define LED_PIN 2 

void setup() {
  Serial.begin(115200);
  delay(1000); 
  Serial.println("Teste de Toque Iniciado!");
}

void loop() {
  // O pino D4 (GPIO 4) suporta toque capacitivo (Touch 0)
  // Toca com o dedo no pino metálico D4 da tua placa
  int valorToque = touchRead(4); 
  
  Serial.print("Valor do Sensor no Pino D4: ");
  Serial.println(valorToque);
  
  // Quanto mais tocas, menor o número fica (perto de 0 ou 10)
  // Sem tocar, costuma estar acima de 70 ou 80
  
  delay(500);
}