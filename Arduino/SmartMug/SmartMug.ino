#include <SPI.h>
#include <MFRC522.h>
#include <avr/wdt.h>

#define SS_PIN 10 //slave select pin
#define RST_PIN 9 //reset pin
#define RELAY_1 2
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
MFRC522::MIFARE_Key key; //Maak een MIFARE_Key genaamd key, dit zal alle kaart informatie bevatte

/*
 * Global variables
 */
//bool 
boolean isBusy;

//int
int block;
int coffeePump = 3, 
    chocolatePump = 4, 
    nozzle = 5,
    buttonBlackCoffee = 6,
    buttonChocolate = 7,
    pushPowerOut = A0;

//byte
byte readbackblock[18]; 
byte blockcontent[16]; //add here any cup you like and afterwards write it to the SmartMug

//String
String smartMugsFavoriteDrink, black_coffee, chocolate;

/*
 * 
 * 
 * 
 */
void setup()
{
    Serial.begin(9600); // Initialize serial communications with the PC
    SPI.begin(); // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522 card PCD = proximity coupling device.
   
    //set pins to output 
    pinMode(RELAY_1, OUTPUT);  
    pinMode(coffeePump, OUTPUT); 
    pinMode(chocolatePump, OUTPUT); 
    pinMode(nozzle, OUTPUT); 
    pinMode(buttonChocolate, OUTPUT); 
    pinMode(pushPowerOut, OUTPUT);

    //set pins to input 
    pinMode(buttonBlackCoffee, INPUT); 
    pinMode(buttonChocolate, INPUT); 
    
    Serial.println("Please place your SmartMug on the reader");

    for(byte i = 0; i < 6; i++) 
    {
        key.keyByte[i] = 0xFF; 
        //255, In a nutshell, “0xff” effectively masks the variable so it leaves only the value in the last 8 bits, 
        //and ignores all the rest of the bits.
    }    

    //initializing the Global variables on boot up to keep the heater turned off
    digitalWrite(RELAY_1, HIGH);
    
    isBusy = false;
    block = 2;
    smartMugsFavoriteDrink = "";

    //their names converted to numbers
    black_coffee =  "98108";
    chocolate =   "10910";
}

/*
 * 
 *   
 *   
 * 
 */
void loop()
{      
      /*
       digitalWrite(pushPowerOut, HIGH);

       int BC = digitalRead(buttonBlackCoffee);
       int CH = digitalRead(buttonChocolate);

       if(BC == HIGH)
       {
          Serial.println("BC"+ BC);
          Serial.println("Black Coffee Selected");
          delay(3000);
       }
       

       if(CH == HIGH)
       {
          Serial.println("CH"+ CH);
          Serial.println("Chocolate Selected");
          delay(3000);
       }
       */
       
       /*
       //back one
       digitalWrite(chocolatePump, HIGH);
       delay(3000);
       digitalWrite(chocolatePump, LOW);
       delay(3000);   

       //front one
       digitalWrite(coffeePump, HIGH);
       delay(3000);
       digitalWrite(coffeePump, LOW);
       delay(3000);    

       //nozzle
       digitalWrite(nozzle, HIGH);
       delay(3000);
       digitalWrite(nozzle, LOW);
       delay(3000);
       */
       
       //Look for new cards and if executed it will go back to the loop() without going underneath
       if(! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) 
       {  
          return;
       }

     
        if(isBusy==false)
        {
            turnHeatingElementOn();
            //read the block, send the block location
            readBlock(block,readbackblock);  
                     
            Serial.print("Uw smaak:\n");
            for(int j=0; j<16; j++)
            { 
                //Serial.write(readbackblock[j]); 
                smartMugsFavoriteDrink += readbackblock[j];   
            } 

            smartMugsFavoriteDrink.remove(5);
            Serial.print(smartMugsFavoriteDrink);
            //Serial.println("<--End of String");
            //Serial.print(smartMugsFavoriteDrink.length());
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

/*
 *  
 * 
 */
void writeNewDataIntoMug()
{ 
        //writeBlock(block, blockcontent);//the blockcontent array is written into the card block
}

/*
 * 
 * 
 */
void turn(int whichMotorToTurnOn)
{
  switch(whichMotorToTurnOn)
  {
    case 1: digitalWrite(coffeePump,HIGH);
            break;

    case 2: digitalWrite(chocolatePump, HIGH);
            break;
            
    default: break;        
  }
}

/*
 * 
 * 
 */
void timer()
{
       delay(20000); //40 seconds
       turnHeatingElementOff();
       delay(10000);
          wdt_enable(WDTO_2S);
}

/*
 * 
 * 
 */
void turnStirerOnorOff()
{

}

/*
 * 
 * 
 */
void turnHeatingElementOn()
{
  digitalWrite(RELAY_1, HIGH);
}

/*
 * 
 * 
 */
void turnHeatingElementOff()
{
  digitalWrite(RELAY_1, HIGH);
}

