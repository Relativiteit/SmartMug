#include <SPI.h>
#include <MFRC522.h>
#include <avr/wdt.h>

#define SS_PIN 10 //slave select pin
#define RST_PIN 9 //reset pin
#define RELAY_ON 1
#define RELAY_OFF 0
#define RELAY_1  2
MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance.
MFRC522::MIFARE_Key key; //Maak een MIFARE_Key genaamd key, dit zal alle kaart informatie bevatte

/*
 * Global variables
 */
//bool 
boolean isBusy;

//int
int block;
int motorNumber;
int latchPin = 5; //Pin connected to ST_CP of 74HC595
int clockPin = 4; //Pin connected to SH_CP of 74HC595
int dataPin = 3; ////Pin connected to DS of 74HC595

//byte
byte readbackblock[18]; 
byte blockcontent[16]; //add here any cup you like and afterwards write it to the SmartMug
String cappuccino, black_coffee, milk_coffee;
const byte motor[6]= {B10000000, B01000000, B00100000, B00000000, B00010000, B00001000}; //motorSequence: water, milk, coffee, off, stirer & nozzle

//String
String smartMugsFavoriteDrink;

/*
 * 
 * 
 * 
 */
void setup()
{
    Serial.begin(9600); // Initialize serial communications with the PC
    SPI.begin(); // Init SPI bus
	  mfrc522.PCD_Init();	// Init MFRC522 card PCD = proximity coupling device.
   
    //set pins to output so you can control the shift register
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(RELAY_1, OUTPUT); 
     
    // Initialize relay and other components on off so that on reset it would be off by default
    motorNumber = 3; //default state of the motors is 0
    setStateOfMotor();
    motorNumber = 0;
    
	  Serial.println("Please place your SmartMug on the reader");

    for(byte i = 0; i < 6; i++) 
    {
        key.keyByte[i] = 0xFF; 
        //255, In a nutshell, “0xff” effectively masks the variable so it leaves only the value in the last 8 bits, 
        //and ignores all the rest of the bits.
    }    

    //initializing the Global variables on boot up
    isBusy = false;
    block = 2;
    smartMugsFavoriteDrink = "";

    cappuccino =    "99971";
    black_coffee =  "98108";
    milk_coffee =   "10910";
}

/*
 * 
 * 
 * 
 */
void loop()
{                
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
            prepareBitsSoThatTheCorrectMotorsWillTurnForThisMug(smartMugsFavoriteDrink);
            isBusy = true;
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
void setStateOfMotor()
{
        digitalWrite(latchPin, LOW);
    
        // shift out the bits: i.e 10000000 is actually 00000001 
        shiftOut(dataPin, clockPin, LSBFIRST, motor[motorNumber]);  

        //take the latch pin high so the LEDs will light up:
        digitalWrite(latchPin, HIGH);
}

/*
 * 
 * 
 */
void  prepareBitsSoThatTheCorrectMotorsWillTurnForThisMug(String temp)
{
        if(temp.equals(cappuccino))
        {
              //water
              setStateOfMotor();
              delay(5000); //5 sec
              motorNumber++; // 1

              //milk
              setStateOfMotor();
              delay(5000); //5 sec
              motorNumber++; //2
              
              //coffee
              setStateOfMotor();
              delay(3000); //5 sec
              motorNumber++; //3
              
        }
        
        //reset motors state to 0, in other words turn pumps off
        setStateOfMotor();  //motorNumber = 3 at the moment
        
        //20 extra cooking + 15 second for the total amount
        timer();
}

/*
 * 
 * 
 */
void timer()
{
       turnStirerOnorOff();
       delay(20000); //40 seconds
       turnHeatingElementOff();

       //open the nozzle to pour in the mug
       motorNumber++; //5
       setStateOfMotor();
       delay(10000);

       wdt_enable(WDTO_2S);
}

/*
 * 
 * 
 */
void turnStirerOnorOff()
{
    motorNumber++; //4
    setStateOfMotor();
}

/*
 * 
 * 
 */
void turnHeatingElementOn()
{
  digitalWrite(RELAY_1, RELAY_ON);
}

/*
 * 
 * 
 */
void turnHeatingElementOff()
{
  digitalWrite(RELAY_1, RELAY_OFF);
}

