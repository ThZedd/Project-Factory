#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>


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

Adafruit_ST7735 display = Adafruit_ST7735(DISPLAY_CS_PIN, DISPLAY_RS_DC_PIN, DISPLAY_RES_PIN);
// Variáveis para controlar o botão
int lastButtonState = 1; // HIGH porque estamos a usar INPUT_PULLUP
int currentPhase = 0;   // This will indicate in what phase are we in
// Variáveis para controlar o Timer

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
    display.initR(INITR_BLACKTAB); 
    display.setRotation(1); 
    Serial.println("Sistema pronto!");
    
}

void loop()
{
    // Lê o estado atual do botão
    int currentButtonState = digitalRead(MAINBUTTON_PIN);

    // Verifica se o botão acabou de ser pressionado (mudou de HIGH para LOW)
    if (lastButtonState == 1 && currentButtonState == 0)
    {

        switch (currentPhase)
        {
        //Não esquecer de colocar o case 0 no setup assim ao iniciar vai logo fazer oque o case 0 faz
        case 0:
            display.setCursor(50, 85);
            display.print("Olá bora lavar as mãos juntos??!!!");
            delay(3000);
            display.fillScreen(ST77XX_BLACK);
            delay(1000);
            display.setCursor(50, 85);
            display.print("Clica no botão denovo para começarmos!!!!");
            break;
        case 1:
            display.fillScreen(ST77XX_BLACK);
            display.setCursor(50, 85);
            delay(3000);
            display.print("Antes de começarmos que tal tornarmos isto um pouco mais divertido?");
            delay(3000);
            display.fillScreen(ST77XX_BLACK);
            display.print("Vai aparecer um animal no ecrã e tu não te podes esquecer qual foi, ok?");
            delay(3000);
            display.fillScreen(ST77XX_BLACK);
            display.print("Clica no botão denovo para avançarmos!!!!");
            break;
        case 2:
            //ira ser escolhido um animal qualquer ( entre 4 opcoes)
            display.print("Clica no botão denovo quando tiveres o decorado!!!!");
            break;
        case 3:
             display.print("Bora lá começar primeiro pomos o sabão na mão");
             delay(3000); // 3 segundos de pausa, tempo medio que uma criança demora a colocar o sabão
             display.print("E agora esfrega esfrega esfrega!!!");
            break;
        case 4:
            /* code */
            break;
        case 5:
            display.print("Parabéns conseguiste!!!!");
            break;

        default:
            currentPhase = 0;
            break;
        }

        delay(50);
        currentPhase++;
    }



    // Atualiza o estado anterior do botão para a próxima volta do loop
    lastButtonState = currentButtonState;
    Serial.print("CurrentPhase: ");
    Serial.println(currentPhase);
}
