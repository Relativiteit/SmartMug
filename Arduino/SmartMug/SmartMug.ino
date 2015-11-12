#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10 //slave select pin
#define RST_PIN 9 //reset pin
MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance.
MFRC522::MIFARE_Key key; //Maak een MIFARE_Key genaamd key, dit zal alle kaart informatie bevatte

void setup()
{
  Serial.begin(9600);         // Initialize serial communications with the PC
  SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();	// Init MFRC522 card PCD = proximity coupling device.
	Serial.println("Please place your SmartMug on the reader");

   for(byte i = 0; i < 6; i++) 
   {
        key.keyByte[i] = 0xFF;
   }      
}

int block=2;
byte readbackblock[18]; 
void loop()
{                
      //Look for new cards 
      if(! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) 
      {  
          return;
      }
                  
      readBlock(block,readbackblock);
            
      Serial.print("Uw smaak:\n");
      for(int j=0; j<16; j++)
      { 
          Serial.write(readbackblock[j]);    
      } 
      
       /*
        Print out the UID 
        Serial.print("Card UID:\t");
        for(byte i = 0; i < mfrc522.uid.size; i++) 
        {
          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(mfrc522.uid.uidByte[i], HEX);
        }
       */      
}   
