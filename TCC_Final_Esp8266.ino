
//------------------Variáveis do sistema-------------------------------
char RECEBEQT[5]="";
char RECEBEOP[5]="";
char RECEBEPR[5]="";

#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "Vancine"
#define STAPSK  "socorro1"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "djxmmx.net";
const uint16_t port = 17;

//const char* X[5]="";

//#include <WiFi.h>

//const char* usuario = "Vancine";
//const char* senha = "socorro1";

#include <PubSubClient.h>
WiFiClient espClient;
PubSubClient client(espClient);

const char* Cliente = "ESP32";
const char* Servidor = "mqtt.dioty.co";
const int Porta = 1883;
const char* Usuario_MQTT = "vini.f.carreia@gmail.com";
const char* Senha_MQTT = "c1a07db5";


//--------------------------------------------------------------------

//-------------------Configurações Iniciais---------------------------

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  
  /* Conexão ao MQTT Broker */
  client.setServer(Servidor, Porta);
  while(!client.connected())
  {
    Serial.println("Conectando ao MQTT...");
    if(client.connect(Cliente, Usuario_MQTT, Senha_MQTT))
    {
      Serial.println("MQTT Conectado");
    }
    else
    {
      Serial.println("Falha na Conexão");
      Serial.print(client.state());
      delay(2000);
    }}}}
//-------------------------------------------------------------------------


void loop() { 
  client.loop();

//--------------Recebe Informação de qual é o produto----------------------  
  Produto:
  
   if (Serial.available() > 0) {
      Serial.readBytes(RECEBEPR,1);
      delay(100);
      goto Operacao;
   }
   goto Produto;   
//------------------------------------------------------------------------

//-------------Recebe Informação de qual a operação selecionada-----------
  Operacao:

   if (Serial.available() > 0) {
      Serial.readBytes(RECEBEOP,1);
    delay(100);   
      goto Quantidade;
   }
  goto Operacao;

//-----------------------------------------------------------------------

//-------------Recebe Informação da quantidade e envia o sinal-----------
Quantidade:

if (Serial.available()>0) {
    Serial.readBytes(RECEBEQT,5);
    delay(100);
  client.setServer(Servidor, Porta);
  while(!client.connected())
  {
    Serial.println("Conectando ao MQTT...");
    if(client.connect(Cliente, Usuario_MQTT, Senha_MQTT))
    {
     Serial.println("MQTT Conectado");

    }
    else
    {
      Serial.println("Falha na Conexão");
      Serial.print(client.state());
      delay(2000);
    }
  }
   client.publish("/vini.f.carreia@gmail.com/produto",RECEBEPR);
delay(100);
   client.publish("/vini.f.carreia@gmail.com/operacao",RECEBEOP);
delay(100);
   client.publish("/vini.f.carreia@gmail.com/quantidade",RECEBEQT);

   
   delay(1000);
   goto Fim;
   
 }
 goto Quantidade;

Fim:

delay(10);
char RECEBEQT[5]="";
char RECEBEOP[5]="";
char RECEBEPR[5]="";

//---------------------------------------------------------------------


}
