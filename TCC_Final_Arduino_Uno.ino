#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <SPI.h> //INCLUSÃO DE BIBLIOTECA
#include <MFRC522.h> //INCLUSÃO DE BIBLIOTECA


//===================DEFINIÇÃO DE PINOS RFID=============================================
#define SS_PIN 10 //PINO SDA
#define RST_PIN 9 //PINO DE RESET

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
char st[20];
//=======================================================================================

//Inicializa o display no endereco 0x3F

LiquidCrystal_I2C lcd(0x3F,16,2);


//===================DEFINIÇÃO DAS ENTRADAS===============================================

int BOT_MENU = 2; //Verde
int BOT_BAIXO = 3; //Azul
int BOT_CIMA= 4; //Roxo
int BOT_OK= 5; // Cinza

//========================================================================================


//================================Variáveis================================================

int BOT;            // Direita=1 / Cima=2 / Baixo=3 / Esquerda=4 / Select=5 /
int LIDO=0;         // Variavel utilizada para entrar no loop de receber valor e escolher se é entrada ou saída
int LIDO_POS=0;     // Variável utilizada para saber qual seta esta apertando na escolha se é entrada ou saída
int ESCOLHIDO;      // Variável utilizada para entrar no loop de escolher a quantidade
int OPERACAO;       // Variável para armazenar qual foi a opção escolha: 1 Saída ///// 2 Entrada
int QUANTIDADE;     // Variável utilizada para armazenar a quantidade escolhida 
int QTD_ESC;        // Variável utilizada para habilitar o loop de enviar os dados
int MENU;           // Variável que verifica se foi acessado o MENU
int CANCELA;        //
int ESCOLHA_CANCELA;
int PRODUTO;

//========================================================================================


//================================CONFIGURAÇÃO============================================
void setup() {
    SPI.begin(); //INICIALIZA O BARRAMENTO SPI
    mfrc522.PCD_Init();   // Inicia MFRC522
  
  Serial.begin(9600);
   lcd.init();
   lcd.backlight(); 
   lcd.begin(16,2); //SETA A QUANTIDADE DE COLUNAS(16) E O NÚMERO DE LINHAS(2) DO DISPLAY. EM SUMA: UMA MATRIZ DE 16 COLUNAS E 2 LINHAS
   lcd.setCursor(0,0); //SETA A POSIÇÃO EM QUE O CURSOR INCIALIZA(LINHA 1) 
   lcd.clear();
   lcd.print("Aguardando...");

pinMode (BOT_MENU, INPUT_PULLUP);
pinMode (BOT_BAIXO, INPUT_PULLUP);
pinMode (BOT_CIMA, INPUT_PULLUP);
pinMode (BOT_OK, INPUT_PULLUP);

}
//========================================================================================

void loop() {

INICIO:
 //==================================Leitura======================================

  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  String TAG= "";
  byte letra;
  for (byte i = 0; i < 4; i++) 
  {
    TAG.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    TAG.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
    mfrc522.PICC_HaltA(); //PARADA DA LEITURA DO CARTÃO
    mfrc522.PCD_StopCrypto1(); //PARADA DA CRIPTOGRAFIA NO PCD
    TAG.toUpperCase();
//===================================================================================

//==============================Verifica Produtos===================================
  if (TAG.substring(1) == "D9 03 26 B5") // PRODUTO 1
  {
  PRODUTO=1;
  BOT=5;
  delay(100);
  }

  else if (TAG.substring(1) == "86 72 83 25") //PRODUTO 2
  {
  PRODUTO=2;
  BOT=5;
  delay(100);
  }
    else  //PRODUTO NÃO CADASTRADO
  {
    lcd.clear();
    lcd.setCursor(0,0); //SETA A POSIÇÃO EM QUE O CURSOR INCIALIZA(LINHA 1) 
    lcd.print("Nao cadastrado!");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0); //SETA A POSIÇÃO EM QUE O CURSOR INCIALIZA(LINHA 1) 
    lcd.print("Aguardando...");
    }
    
//==================================================================================

//==================================CANCELA OPERAÇÃO================================

CANCELA1:
if(CANCELA == 1){
      ESCOLHA_CANCELA=2;
      BOT=0;
      lcd.clear();
      lcd.setCursor(0,0); 
     lcd.print("SAIR?");
     lcd.setCursor(10,0); 
     lcd.print("SIM");
     lcd.setCursor(10,1); 
     lcd.print("NAO");

CANCELA2:
//////////////////////////BOTÕES///////////////////////////////////////                           
if (digitalRead(BOT_BAIXO) == LOW){ESCOLHA_CANCELA=1;}                                                 
if (digitalRead(BOT_CIMA) == LOW){ESCOLHA_CANCELA=2; }                                                
if (digitalRead(BOT_OK) == LOW){BOT=5;}                                                               
////////////////////////////////////////////////////////////////////////

if(ESCOLHA_CANCELA == 2){      
  lcd.setCursor(7,0); 
  lcd.print("==>");
  lcd.setCursor(7,1); 
  lcd.print("   ");  
  }

if(ESCOLHA_CANCELA ==1){
  lcd.setCursor(7,1); 
  lcd.print("==>");
  lcd.setCursor(7,0); 
  lcd.print("   ");  
  
  }

if (BOT==5 && ESCOLHA_CANCELA==2){         //CASO SELECIONADO SAIR. SERÁ ZERADO TODAS AS VARIÁVEIS
  LIDO=0;
  BOT=0;
  ESCOLHIDO=0; 
  CANCELA=0;  
  OPERACAO=0;
  QUANTIDADE=0;                         

     lcd.clear();
     lcd.setCursor(0,0); 
     delay(500);
     lcd.print("Aguardando...");
     
     goto INICIO;
} 

if (BOT==5 && ESCOLHA_CANCELA==1 && LIDO==1)
{CANCELA=0; ESCOLHA_CANCELA=0; delay(200); goto ESCOLHA1; }  //SELECIONADO NÃO. VOLTA PARA O LOCAL QUE APERTOU SAIR

if (BOT==5 && ESCOLHA_CANCELA==1 && ESCOLHIDO==1)
{CANCELA=0; ESCOLHA_CANCELA=0; delay(200); goto QTD; }   //SELECIONADO NÃO. VOLTA PARA O LOCAL QUE APERTOU SAIR

  goto CANCELA2;
}
  
//==================================================================================

//===============================Botão de Leitura Apertado===========================
if (BOT==5){
  BOT =0;
  LIDO=1;
 lcd.clear();
 lcd.setCursor(0,0); 
 lcd.print("Lendo...");
 delay(1000);
 lcd.clear();
   lcd.setCursor(0,0); 
  lcd.print("==>");
}
//=================================================================================


//================================Escolha se é entrada ou saida===================

if(LIDO==1){

  ESCOLHA1:
BOT=0;
  lcd.clear();
  lcd.setCursor(4,0); 
  lcd.print("Entrada?");
  lcd.setCursor(4,1); 
  lcd.print("Saida?");
  LIDO_POS=1;

ESCOLHA:

/////////////////////////BOTÕES///////////////////////////////////////
if (digitalRead(BOT_MENU) == LOW){CANCELA=1; delay(400); goto CANCELA1;}
if (digitalRead(BOT_BAIXO) == LOW){LIDO_POS=2; delay(400);}
if (digitalRead(BOT_CIMA) == LOW){LIDO_POS=1; delay(400);}
if (digitalRead(BOT_OK) == LOW){BOT=5; delay(400);}
//////////////////////////////////////////////////////////////////////

if (LIDO_POS==1){
  
       lcd.setCursor(0,1); 
     lcd.print("   ");
     lcd.setCursor(0,0); 
  lcd.print("==>");

  }

  
if (LIDO_POS==2){

       lcd.setCursor(0,0); 
     lcd.print("   ");
     lcd.setCursor(0,1); 
  lcd.print("==>");

  }

  if (BOT==5 && LIDO_POS==1){         //Caso a opção escolha for entrada
    LIDO_POS=0;
    LIDO=0;
    BOT=0;
    OPERACAO = 2;
    lcd.clear();
     lcd.setCursor(0,0); 
     lcd.print("OP. ESCOLHIDA:");
          lcd.setCursor(5,1);
        lcd.print("ENTRADA");   
     delay(2000);
          ESCOLHIDO=1;
     goto QTD;
   }

  if (BOT==5 && LIDO_POS==2){       //Caso a opção escolha for entrada
    LIDO_POS=0;
    LIDO=0;
    BOT=0;
     OPERACAO = 1;
     lcd.clear();
     lcd.setCursor(0,0); 
     lcd.print("OP. ESCOLHIDA:");
     lcd.setCursor(6,1);
     lcd.print("SAIDA");  
     delay(2000);
     ESCOLHIDO=1;
     goto QTD;
   }

  goto ESCOLHA;
}
//==============================================================================

//==============================Escolhe quantidade===============================
if(ESCOLHIDO==1){
  
QTD:
    BOT=0;
     lcd.clear();
     lcd.setCursor(0,0); 
     lcd.print("Quantidade: ");

QTD1:
     lcd.setCursor(13,0); 
     lcd.print("    ");
     lcd.setCursor(13,0);
     lcd.print(QUANTIDADE);
   delay(150);
   
/////////////////////////BOTÕES///////////////////////////////////////
if (digitalRead(BOT_MENU) == LOW){CANCELA= 1; delay(400); goto CANCELA1;}              
if (digitalRead(BOT_BAIXO) == LOW){QUANTIDADE=QUANTIDADE-1;}            
if (digitalRead(BOT_CIMA) == LOW){QUANTIDADE=QUANTIDADE+1; }          
if (digitalRead(BOT_OK) == LOW){BOT=5;}                               
////////////////////////////////////////////////////////////////////////

if(BOT==5){
  BOT=0;
  ESCOLHIDO=0;
     lcd.clear();
     lcd.setCursor(0,0); 
     lcd.print("Espere...");
     QTD_ESC=1;
     delay(1000);
  
  goto ENVIASINAL;
  }

  if(QUANTIDADE<0){QUANTIDADE=0;}

goto QTD1;
}
//=====================================================================================

//=================================Envia Sinal========================================
if(QTD_ESC==1){
  
ENVIASINAL:
     QTD_ESC=0;
     lcd.clear();
     lcd.setCursor(0,0); 
     lcd.print("Atualizando");
     lcd.setCursor(0,1); 
     lcd.print("Estoque...");
///////////////Informações enviadas//////////////     
     Serial.print(PRODUTO);
     delay(1000);
     Serial.print(OPERACAO);
     delay(1000);
     Serial.print(QUANTIDADE);
     delay(1000);
///////////////////////////////////////////////
     lcd.clear();
     lcd.setCursor(0,0); 
     lcd.print("Atualizado!");
     QUANTIDADE=00000;
     OPERACAO=0;
     delay(2000);
     lcd.clear();
     lcd.setCursor(0,0); 
     lcd.print("Aguardando...");
     goto INICIO;

}
//=====================================================================================


}
