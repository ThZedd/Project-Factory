#include <Arduino.h>
#include <WiFi.h>
// O LED azul integrado na placa costuma ser o pino 2
#define LED_PIN 2

const char *ssid = "ESP32-Access-Point";
const char *password = "12345678";

WiFiServer server(80);

void setup()
{
  Serial.begin(115200);

  delay(1000);
  Serial.println("Setting AP...");
  Serial.println("Teste de Toque Iniciado!");

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.println(IP);

  server.begin();

  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  WiFiClient client = server.available();

  if (client)
  {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected())
    {

      if (client.available())
      {
        char c = client.read();
        Serial.write(c);
        currentLine += c;
        if (c == '\n')
        {
          if (currentLine.length() == 2)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Send your "Hello World" HTML response
            client.println("<html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head>");
            client.println("<body><h1>Hello World</h1></body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          }
          else
          { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }
      }
    }
  }
  // O pino D4 (GPIO 4) suporta toque capacitivo (Touch 0)
  // Toca com o dedo no pino metálico D4 da tua placa
  /*
  int valorToque = touchRead(4);

  Serial.print("Valor do Sensor no Pino D4: ");
  Serial.println(valorToque);
  if (valorToque <= 60)
  {
    digitalWrite(LED_PIN, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(1000);                 // wait for a second
    digitalWrite(LED_PIN, LOW);  // turn the LED off by making the voltage LOW
    delay(1000);
  }
    */
}