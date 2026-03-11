#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_ILI9341.h> // Biblioteca especifica para o simulador Wokwi
#include <Fonts/FreeSansBold12pt7b.h>
#include "PixelFont.h"

#define MAINBUTTON_PIN 13 // This button will make sure to advance to the next phase

#define ANIMALBUTTON1_PIN 14 // This button will make sure to get if the children got the animal correct
#define ANIMALBUTTON2_PIN 25 // This button will make sure to get if the children got the animal correct
#define ANIMALBUTTON3_PIN 26 // This button will make sure to get if the children got the animal correct
#define ANIMALBUTTON4_PIN 27 // This button will make sure to get if the children got the animal correct

#define DFPLAYER_RX2_PIN 16
#define DFPLAYER_TX2_PIN 17

#define DISPLAY_SCK_PIN 18
#define DISPLAY_SDA_PIN 23
#define DISPLAY_RES_PIN 4
#define DISPLAY_RS_DC_PIN 2
#define DISPLAY_CS_PIN 5

// Inicializa o ecrã ILI9341 (Apenas para Simulação)
Adafruit_ILI9341 display = Adafruit_ILI9341(DISPLAY_CS_PIN, DISPLAY_RS_DC_PIN, DISPLAY_RES_PIN);

// Variáveis para controlar o botão
int lastButtonState = 1; // HIGH porque estamos a usar INPUT_PULLUP
int currentPhase = 0;   // This will indicate in what phase are we in

void setup()
{
    Serial.begin(115200);
    
    //Buttons Mode
    pinMode(MAINBUTTON_PIN, INPUT_PULLUP);
    pinMode(ANIMALBUTTON1_PIN, INPUT_PULLUP);
    pinMode(ANIMALBUTTON2_PIN, INPUT_PULLUP);
    pinMode(ANIMALBUTTON3_PIN, INPUT_PULLUP);
    pinMode(ANIMALBUTTON4_PIN, INPUT_PULLUP);

    //DFPlayer inicialized
    //Serial2.begin(9600, SERIAL_8N1, DFPLAYER_RX2_PIN, DFPLAYER_TX2_PIN);

    //Display inicialized
    display.begin(); // Comando correto para o ILI9341
    display.setRotation(1); 
    
    Serial.println("Sistema pronto!");
    
    // Configuracoes de texto para o efeito "Pixel Art" e visibilidade
    display.setFont(&PressStart2P_Regular8pt7b);
    display.setTextSize(1); // Tamanho maior para preencher melhor o ecrã
    display.setTextColor(ILI9341_WHITE); // Garante que o texto é branco
}

void loop()
{
    // Lê o estado atual do botão principal
    int currentButtonState = digitalRead(MAINBUTTON_PIN);

    // Verifica se o botão acabou de ser pressionado (mudou de HIGH para LOW)
    if (lastButtonState == 1 && currentButtonState == 0)
    {

        switch (currentPhase)
        {
        //Não esquecer de colocar o case 0 no setup assim ao iniciar vai logo fazer oque o case 0 faz
        case 0:
            display.fillScreen(ILI9341_BLACK);
            display.setCursor(10, 50);
            display.print("Ola bora lavar as");
            display.setCursor(10, 80);
            display.print("maos juntos?!!!"); // Trigraph e acentos resolvidos
            
            delay(3000);
            display.fillScreen(ILI9341_BLACK);
            delay(1000);
            
            display.setCursor(10, 80);
            display.print("Clica no botao para");
            display.setCursor(10, 110);
            display.print("comecarmos!!!!");
            break;
            
        case 1:
            display.fillScreen(ILI9341_BLACK);
            display.setCursor(10, 50);
            delay(3000);
            display.print("Antes de comecar,");
            display.setCursor(10, 80);
            display.print("que tal algo divertido?");
            
            delay(3000);
            display.fillScreen(ILI9341_BLACK);
            display.setCursor(10, 50);
            display.print("Vai aparecer um animal");
            display.setCursor(10, 80);
            display.print("e tu nao te podes");
            display.setCursor(10, 110);
            display.print("esquecer qual foi, ok?");
            
            delay(4000);
            display.fillScreen(ILI9341_BLACK);
            display.setCursor(10, 80);
            display.print("Clica para avancar!");
            break;
            
        case 2:
            display.fillScreen(ILI9341_BLACK);
            //ira ser escolhido um animal qualquer ( entre 4 opcoes)
            display.setCursor(10, 80);
            display.print("Clica quando tiveres");
            display.setCursor(10, 110);
            display.print("decorado o animal!!!!");
            break;
            
        case 3:
             display.fillScreen(ILI9341_BLACK);
             display.setCursor(10, 50);
             display.print("Bora la comecar!");
             display.setCursor(10, 80);
             display.print("Primeiro, sabao na mao!");
             
             delay(3000); // 3 segundos de pausa
             display.setCursor(10, 140);
             display.print("E agora esfrega esfrega!");
            break;
            
        case 4:
            display.fillScreen(ILI9341_BLACK);
            /* code */
            break;
            
        case 5:
            display.fillScreen(ILI9341_BLACK);
            display.setCursor(10, 80);
            display.print("Parabens conseguiste!!!!");
            break;

        default:
            currentPhase = -1; // Volta a -1 para que a proxima soma o coloque na fase 0
            break;
        }

        delay(50); // Debounce
        currentPhase++;
        
        // Agora os prints estão DENTRO do if e não vão fazer spam!
        Serial.print("Mudou para CurrentPhase: ");
        Serial.println(currentPhase);
    }

    // Atualiza o estado anterior do botão para a próxima volta do loop
    lastButtonState = currentButtonState;
}