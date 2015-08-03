//*************************************Communication**********************************************
//################################################################################################
void readSerialData()
{
  readSerial0();
  readSerial1();
  /**
  switch(serialScheduler)
  {
    case 0:
    {
      readSerial0();
      break;
    }
    case 1:
    {
      readSerial1();
      break;
    }
    case 2:
    {
      readSerial2();
      break;
    }
  }
  serialScheduler++;
  if(serialScheduler >= 3)
  {
    serialScheduler = 0;
  }
  **/
}
//################################################################################################
//################################################################################################
void readSerial0()
{
  //int i = 0;
  //byte remoteBuffer[64];
  int i = 0;
  sBuff = 0;
  byte dataLength = 0;
  unsigned long startingTime = millis();
  unsigned long currentTime;
  boolean readFlag = true;
  if(Serial.available()>=5)
  {
    sBuff = Serial.read();    //Header Byte
    while((sBuff != headerValue) && (Serial.available())) 
    {
      if(debugEnable)
      {
        Serial.print("Module header error: ");
        Serial.println(sBuff);
        Serial1.print("Module header error:");
        Serial1.println(sBuff);
      }
      sBuff = Serial.read(); //read a byte
    }
    if(sBuff == headerValue)
    {
      sBuff = Serial.read();   //read the ID byte
      packetBuffer[i] = sBuff;  //place module ID into packet buffer
      i++;
      dataLength = Serial.read();   //read the dataLength byte
      packetBuffer[i] = dataLength;  //place data length into packet buffer
      i++;
      startingTime = millis();
      //read the rest of the packet
      while(((i<(dataLength + 3)) && readFlag))
      {
        if(Serial.available())
        {
          packetBuffer[i] = Serial.read();
          i++;
        }
        else
        {
          //wait for next byte
          currentTime = millis();
          if((currentTime - startingTime) > 50)  //50 ms timeout
          {
            readFlag = false;
            if(debugEnable)
            {
              Serial.print("Data Timout!");              
              Serial1.print("Data Timout!"); 
            }
          }
        }
      }
      modID = processData(packetBuffer, validData);
    } 
    else
    {
      if(debugEnable)
      {
        Serial.print("Packet Error");
        Serial1.print("Packet Error");
        errorControl();
      }
    }
  }
  if(validData)
  {
    switch(modID)
    {
      case 0:
      {
        break;
      }
      case 1:
      {
        controlModule(packetBuffer);
        break;
      }
    }
  }
  /**
  if(Serial.available()>=5)
  {
    while(Serial.available())
    {
      remoteBuffer[i] = Serial.read();
      delayMicroseconds(21); //delay some time since Serial has a lower baud rate right now
      i++;
    }
    if(remoteBuffer[1] == 1)
    {
      Serial.println("remote motor command");
      byte dir = remoteBuffer[3];
      byte spd = remoteBuffer[4];
      moveMotor(dir, spd);
    }
  }
  **/
}
//################################################################################################
//################################################################################################
void readSerial1()
{
  if(Serial1.available())
  {
    digitalWrite(ledPin, HIGH);
    //digitalWrite(bus_bsy, HIGH);  //lock the bus
    sBuff = Serial1.read();
    processSimpleCommand(sBuff);
    /**
    while(Serial1.available())  //flush the Serial port
    {
      sBuff = Serial1.read();
    }
    **/
    //digitalWrite(bus_bsy, LOW);  //free the bus
    digitalWrite(ledPin, LOW);
  }
}
//################################################################################################
//################################################################################################
void readSerial2()
{
}
//################################################################################################
//################################################################################################
void readSerial3()
{
  int i = 0;
  sBuff = 0;
  byte dataLength = 0;
  unsigned long startingTime = millis();
  unsigned long currentTime;
  boolean readFlag = true;
  if(Serial3.available()>=5)
  {
    digitalWrite(bus_bsy, HIGH);  //lock the bus 
    sBuff = Serial3.read();    //Header Byte
    while((sBuff != headerValue) && (Serial3.available())) 
    {
      if(debugEnable)
      {
        Serial.print("Module header error: ");
        Serial.println(sBuff);
        Serial1.print("Module header error:");
        Serial1.println(sBuff);
      }
      sBuff = Serial3.read(); //read a byte
    }
    if(sBuff == headerValue)
    {
      sBuff = Serial3.read();   //read the ID byte
      packetBuffer[i] = sBuff;  //place module ID into packet buffer
      i++;
      dataLength = Serial3.read();   //read the dataLength byte
      packetBuffer[i] = dataLength;  //place data length into packet buffer
      i++;
      startingTime = millis();
      //read the rest of the packet
      while(((i<(dataLength + 3)) && readFlag))
      {
        if(Serial3.available())
        {
          packetBuffer[i] = Serial3.read();
          i++;
        }
        else
        {
          //wait for next byte
          currentTime = millis();
          if((currentTime - startingTime) > 50)  //50 ms timeout
          {
            readFlag = false;
            if(debugEnable)
            {
              Serial.print("Data Timout!");              
              Serial1.print("Data Timout!"); 
            }
          }
        }
      }
      modID = processData(packetBuffer, validData);
    } 
    else
    {
      if(debugEnable)
      {
        Serial.print("Packet Error");
        Serial1.print("Packet Error");
        errorControl();
      }
    }
  }
  digitalWrite(bus_bsy, LOW);  //free the bus
}
//################################################################################################
//################################################################################################
void writeSerial3(int packetLength, byte packet[])
{
  for(int i =0; i<packetLength; i++)
  {
    Serial3.write(packet[i]);
  }
}
//################################################################################################
//################################################################################################
int processData(byte packet[], boolean &validData)
{
  byte check = 0;
  //byte header = packet[0];
  byte ID = packet[0];
  check += ID;
  byte length = packet[1];
  check += length;
  for(int x=0; x<length; x++)
  {
    check += packet[2+x];
  }
  if(packet[2+length] == check)
  {
    validData = true;
  }
  else
  {
    validData = false;
  }
  return ID;
}
//################################################################################################
//################################################################################################
void processCommand(byte packet[])
{
}
//################################################################################################
//################################################################################################
void processSimpleCommand(byte tBuff)
{
  if(tankControlMode)
  {
    if(tBuff == 'y')
    {    
      tankControlMode = false;
      if(debugEnable)
      {
        Serial.println("Tank control disabled");
        Serial1.println("Tank control disabled");
      }
    }
    else
    {
      tankBaseModule(tBuff); 
    }
  }
  else
  {
    if((tBuff == 'u') || tBuff == 'j')
    {
      simpleSpeedChange(tBuff);
    }
    else if(tBuff == 'r')
    {
      recover();
    }
    else if(tBuff == 'f')
    {
      toggleDebugMode(); 
    }
    else if(tBuff == 'p')
    {
      CASOverride();
    }
    else if(tBuff == 't')
    {
      toggleLightModule();
    }
    else if((tBuff == 'm') || (tBuff == 'n') || (tBuff == 'b') || (tBuff == 'h'))
    {
      if(debugEnable)
      {
        Serial.println("Turret Cmd Received");
        Serial1.println("Turret Cmd Received");
      }
      byte tStep = 10;
      byte tCmd;
      if(tBuff == 'n')
      {
        tCmd = 'l';
        turretModule(tCmd, tStep);
      }
      else if(tBuff == 'm')
      {
        tCmd = 'r';
        turretModule(tCmd, tStep);
      }
      else if(tBuff == 'b')
      {
        tCmd = 'a';
        turretModule(tCmd, tStep);
      }
      else if(tBuff == 'h')
      {
        tCmd = 'c';
        turretModule(tCmd, tStep);
      }
    }
    else if(tBuff == 'v')
    {
      videoSwitchModule('t');
    }
    else if((tBuff == 'q') || (tBuff == 'w') || (tBuff == 'e') || (tBuff == 'a') || (tBuff == 's') || (tBuff == 'd') || (tBuff == 'z') || (tBuff == 'x') || (tBuff == 'c') || (tBuff == 'o'))
    {
      //controlModule
      simpleControl(tBuff);
    }
    else if(tBuff == 'y')
    {
      tankControlMode = true;
      if(debugEnable)
      {
        Serial.println("Tank control enabled");
        Serial1.println("Tank control enabled");
        
      }
    }
    else if(tBuff == 'i')
    {
      printCommandList();
    }
    else
    {
      if(debugEnable)
      {
        Serial.println("Invalid Command");
        Serial1.println("Invalid Command");
      }
    }  
  }
}
//################################################################################################
//################################################################################################
void remoteTransmit(byte packet[])
{
    Serial1.write(headerValue);
    for(int i = 0; i < packet[1] + 3; i++)
    {
      Serial1.write(packet[i]);
    }
    //Serial1.flush();  //allow transmission to be finished before doing anything
}
//################################################################################################
//################################################################################################
//************************************************************************************************
