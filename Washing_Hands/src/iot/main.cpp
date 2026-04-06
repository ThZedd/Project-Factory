#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h> 
#include <DFRobotDFPlayerMini.h>

//Include das imagens
#include "sabao_img.h"
#include "open_water_img.h"
#include "water_img.h"
#include "close_water_img.h"
#include "secar_img.h"
#include "crab_img.h"
#include "fish_img.h"
#include "PixelFont.h"

// Dimensões do Display
#define IMG_W 160
#define IMG_H 128

// Definição dos pinos

// Botões
#define BTN_MAIN 26  
#define BTN_SIM 14   
#define BTN_NAO 25 

//Display
#define TFT_CS 5
#define TFT_DC 2
#define TFT_RST 4

Adafruit_ST7735 display = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
DFRobotDFPlayerMini player;
bool playerPronto = false; 

const int AUDIO_INTRODUCAO = 5; // 0001_Boas_Vindas
const int audioPrestaAtencar = 15; // Áudio de chamar a atenção

// Perguntas para o Quiz: 2, 3, 4, 5, 6
int audiosPerguntas[] = {11, 12, 13, 14, 1}; 

// Etapas normais: 7, 8, 9, 10, 11
int audiosEtapas[] = {6, 7, 8, 9, 10}; 

// Feedbacks do Quiz: 14, 15, 16
const int AUDIO_FEEDBACK_1 = 2; // Acertaste
const int AUDIO_FEEDBACK_2 = 3; // Errou_Nao_Fez_Isso
const int AUDIO_FEEDBACK_3 = 4; // Errou_Fez_Isso

int animalQueFezAetapa[5]; 
int faseAtual = 0;
unsigned long tempoBloqueioSurgimento = 0;

void tocarAudio(int faixa, String texto) {
  Serial.println("A tentar tocar: " + texto + " (Faixa: " + String(faixa) + ")");
  if (playerPronto) {
    player.play(faixa);
    Serial.println("Comando enviado ao DFPlayer.");
  } else {
    Serial.println("DFPlayer nao esta pronto!");
  }
  delay(1400); 
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n\nA iniciar o sistema.");
  
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  
  pinMode(BTN_MAIN, INPUT_PULLUP);
  pinMode(BTN_SIM, INPUT_PULLUP);
  pinMode(BTN_NAO, INPUT_PULLUP);


  display.initR(INITR_BLACKTAB); 
  display.setRotation(1); 
  display.fillScreen(ST7735_BLACK); 
  Serial.println("Display iniciado.");
  
  Serial.println("A aguardar DFPlayer.");
  if (player.begin(Serial2)) {
    playerPronto = true;
    player.volume(20); // alterar volume se necessário
    Serial.println("DFPlayer pronto e volume definido para 20.");
  } else {
    Serial.println("Falha ao iniciar DFPlayer! Verificar as ligacoes.");
  }
  
  display.setCursor(0, 50);
  display.setFont(&PressStart2P_Regular7pt7b);
  display.setTextSize(1);
  display.setTextColor(ST7735_WHITE);
  display.println(" Carrega no");
  display.println(" botao para");
  display.println("  comecar");
  
  // Define o temporizador inicial para evitar bloqueio no primeiro clique
  tempoBloqueioSurgimento = millis() - 3000; 
}

void quizFinal() {
  Serial.println("\n--- Inicio do quizz ---");
  int etapaAlvo = random(0, 5); 
  bool acertouGeral = false;
  int animalSorteado = random(0, 2); 
  
  Serial.print("Etapa sorteada para a pergunta: ");
  Serial.println(etapaAlvo);

  while (!acertouGeral) {
    bool eOanimalCorreto = (animalSorteado == animalQueFezAetapa[etapaAlvo]);
    
    Serial.print("Animal desenhado na tela: ");
    if (animalSorteado == 0) { 
        Serial.println("Caranguejo");
        display.drawRGBBitmap(0, 0, (const uint16_t*)crab_img, IMG_W, IMG_H);
    } else { 
        Serial.println("Peixe");
        display.drawRGBBitmap(0, 0, (const uint16_t*)fish_img, IMG_W, IMG_H);
    }

    // --- REMOVIDO O ÁUDIO "PRESTA ATENÇÃO" DAQUI ---
    // Deixei apenas uma pausa silenciosa para a imagem ser vista
    delay(1500); 

    Serial.print("O animal correto para esta etapa era o: ");
    Serial.println(animalQueFezAetapa[etapaAlvo] == 0 ? "Caranguejo" : "Peixe");

    tocarAudio(audiosPerguntas[etapaAlvo], "Pergunta do Quiz " + String(etapaAlvo));

    Serial.println("A aguardar resposta do utilizador (botao sim ou botao nao)...");

    while (true) {
      if (digitalRead(BTN_SIM) == LOW) {
        Serial.println("Botao sim pressionado.");
        if (eOanimalCorreto) {
          Serial.println("acertou (Disse sim e era o animal correto)");
          display.fillScreen(ST7735_GREEN);
          tocarAudio(AUDIO_FEEDBACK_1, "Acertaste!"); 
          acertouGeral = true;
        } else {
          Serial.println("errou (Disse sim, mas nao era este animal)");
          display.fillScreen(ST7735_RED);
          tocarAudio(AUDIO_FEEDBACK_2, "Nao, ele nao fez isso."); 
          animalSorteado = random(0, 2); 
          Serial.println("A repetir a pergunta...");
        }
        delay(800); 
        break;
      }
      if (digitalRead(BTN_NAO) == LOW) {
        Serial.println("Botao nao pressionado.");
        if (!eOanimalCorreto) {
          Serial.println("acertou (Disse nao e o animal estava errado)");
          display.fillScreen(ST7735_GREEN);
          tocarAudio(AUDIO_FEEDBACK_1, "Acertaste!");
          acertouGeral = true;
        } else {
          Serial.println("errou (Disse nao, mas era este animal)");
          display.fillScreen(ST7735_RED);
          tocarAudio(AUDIO_FEEDBACK_3, "Nao, ele fez isso."); 
          Serial.println("A repetir a pergunta.");
        }
        delay(800); 
        break;
      }
    }
  }
  Serial.println("--- Fim do Quizz ---");
}

void loop() {
  bool botaoPressionado = (digitalRead(BTN_MAIN) == LOW);
  static bool botaoAnterior = false;

  if (botaoPressionado && !botaoAnterior) {
    Serial.println("\nBotao principal pressionado");
    
    if (millis() - tempoBloqueioSurgimento > 2000) {
      
      Serial.print("A processar Etapa atual: ");
      Serial.println(faseAtual);
      
      if (faseAtual == 0) {
        Serial.println("Etapa 0 - A tocar audio de introducao.");
        tocarAudio(AUDIO_INTRODUCAO, "Ola! Vamos lavar as maos");
        delay(1000); 
      }

      if (faseAtual < 5) {
        int bicho = random(0, 2);
        animalQueFezAetapa[faseAtual] = bicho;
        
        Serial.print("O animal sorteado para esta etapa: ");
        Serial.println(bicho == 0 ? "Caranguejo" : "Peixe");

        
        Serial.println("A desenhar do animal sorteado.");
        if (bicho == 0) {
            display.drawRGBBitmap(0, 0, (const uint16_t*)crab_img, IMG_W, IMG_H);
        } else {
            display.drawRGBBitmap(0, 0, (const uint16_t*)fish_img, IMG_W, IMG_H);
        }
        
        tocarAudio(audioPrestaAtencar, "Presta atencao (fase de ensino)");
        delay(4000); // Pausa extra depois do áudio para criança ter tempo para olhar para a imagem
       

        if (faseAtual == 0) {
            Serial.println("A desenhar: Sabao");
            display.drawRGBBitmap(0, 0, (const uint16_t*)sabao_img, IMG_W, IMG_H);
        } else if (faseAtual == 1) {
            Serial.println("A desenhar: Abrir Agua");
            display.drawRGBBitmap(0, 0, (const uint16_t*)open_water_img, IMG_W, IMG_H);
        } else if (faseAtual == 2) {
            Serial.println("A desenhar: Agua Corrente");
            display.drawRGBBitmap(0, 0, (const uint16_t*)water_img, IMG_W, IMG_H);
        } else if (faseAtual == 3) {
            Serial.println("A desenhar: Fechar Agua");
            display.drawRGBBitmap(0, 0, (const uint16_t*)close_water_img, IMG_W, IMG_H);
        } else if (faseAtual == 4) {
            Serial.println("A desenhar: Secar");
            display.drawRGBBitmap(0, 0, (const uint16_t*)secar_img, IMG_W, IMG_H);
        }
        
        if (faseAtual < 5) {
            tocarAudio(audiosEtapas[faseAtual], "Audio da Etapa indice " + String(faseAtual)); 
        } else {
            Serial.println("A fase atual é 5.");
        }

        tempoBloqueioSurgimento = millis(); // Reinicia o cronómetro de segurança
        faseAtual++;
        Serial.print("Etapa concluida. A proxima Etapa será: ");
        Serial.println(faseAtual);
        
      } else {
        Serial.println("Todas as fases foram concluidas! A iniciar Quiz.");
        quizFinal();
        
        Serial.println("A reiniciar o sistema para comecar de novo.");
        faseAtual = 0;
        
        display.fillScreen(ST7735_BLACK);
        display.setCursor(0, 50);
        display.setFont(&PressStart2P_Regular7pt7b);
        display.setTextSize(1);
        display.setTextColor(ST7735_WHITE);
        display.println(" Carrega no");
        display.println(" botao para");
        display.println("  comecar");

        tempoBloqueioSurgimento = millis(); // Previne toques acidentais a sair do quiz
      }
    } else {
      Serial.println("O clique no botao foi ignorado, os 2 segundos de seguranca ainda nao passaram.");
    }
  } 
  
  botaoAnterior = botaoPressionado;
  delay(50); // Estabilidade e debounce do botão
}