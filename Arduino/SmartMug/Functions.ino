
int readBlock(int blockNumber, byte arrayAddress[])
{
  int largestModulo4Number=blockNumber/4*4;
  int trailerBlock=largestModulo4Number+3;
  
                          /* authentication begins here */ 

   byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));

   if(status!= MFRC522::STATUS_OK)
    { 
       Serial.print("PCD_Authenicate() failed: ");
       Serial.println(mfrc522.GetStatusCodeName(status));
       return 3; 
    }

    byte buffersize = 18; 
    status = mfrc522.MIFARE_Read(blockNumber, arrayAddress, &buffersize);

 if(status!= MFRC522::STATUS_OK)
   { 
       Serial.print("MIFARE_Write() failed: ");
       Serial.println(mfrc522.GetStatusCodeName(status));
       return 4; 
    }
    
   return 0;
}


