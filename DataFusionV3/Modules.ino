//----------------------------Module ID assignment and description--------------------------------
//  1:   Control Module
//  3:   CAS (Collision Avoidance System)
//  10:  Light Module
//  18:  Smart Power Module
//  27:  RFID Module
//  35:  GPS Module
//  38:  IMU Module
//  39:  Compass Module
//  40:  Accelerometer Module
//  41:  Environment Module
//  42:  Gas Module
//  100: Camera Module
//  101: Video Switcher Module
//  150: Turret Module
//  175: Heart Rate Module
//  200: MiniTankBase Module
//  255: System Module
//------------------------------------------------------------------------------------------------

//*******************************************Modules**********************************************
//################################################################################################
//ID = 1
void controlModule(byte packet[])
{
  byte pLength = packet[1];
  byte dir = packet[2];
  byte spd = packet[3];

  //check if there are any limits to speed on all directions
  if(spd > maxFspeed)
  {
    if(dir == 'W' || dir == 'Q' || dir == 'E' || dir == 'q' || dir == 'w' || dir == 'e')
    {
      spd = maxFspeed;
    }
  }
  if(spd> maxBspeed)
  {
    if(dir == 'Z' || dir == 'S' || dir == 'C' || dir == 'z' || dir == 's' || dir == 'c')
    {
      spd = maxBspeed;
    }
  }
  if(spd> maxLspeed)
  {
    if(dir == 'Q' || dir == 'A' || dir == 'Z' || dir == 'q' || dir == 'a' || dir == 'z')
    {
      spd = maxLspeed;
    }
  }
  if(spd> maxRspeed)
  {
    if(dir == 'E' || dir == 'D' || dir == 'C' || dir == 'e' || dir == 'd' || dir == 'c')
    {
      spd = maxRspeed;
    }
  }

  if(dir == 'W' || dir == 'Q' || dir == 'E' || dir == 'q' || dir == 'w' || dir == 'e')
  {
    if(!frontFlag)
    {
      moveMotor(dir, spd);
    }
  }
  else if(dir == 'Z' || dir == 'S' || dir == 'C' || dir == 'z' || dir == 's' || dir == 'c')
  {
    if(!backFlag)
    {
      moveMotor(dir, spd);
    }
  }
  else if(dir == 'E' || dir == 'D' || dir == 'C' || dir == 'e' || dir == 'd' || dir == 'c')
  {
    if(!rightFlag)
    {
      moveMotor(dir, spd);
    }
  }
  else if(dir == 'Q' || dir == 'A' || dir == 'Z' || dir == 'q' || dir == 'a' || dir == 'z')
  {
    if(!leftFlag)
    {
      moveMotor(dir, spd);
    }
  }
  else if(dir == 'o')
  {
    moveMotor(dir, spd);
  }

  lastMoveDir = dir;
  if(debugEnable)
  {
    Serial.println("*****Control Module*****");
    Serial.print("Dir: ");
    Serial.println(dir);
    Serial.print("Spd: ");
    Serial.println(spd);
    Serial.println();
    Serial1.println("*****Control Module*****");
    Serial1.print("Dir: ");
    Serial1.println(dir);
    Serial1.print("Spd: ");
    Serial1.println(spd);
    Serial1.println();
  }
  else
  {
    Serial1.write(headerValue);
    for(int i = 0; i < packet[1] + 3; i++)
    {
      Serial1.write(packet[i]);
    }
  }
}
//################################################################################################
//################################################################################################
//ID = 3
void CASModule(byte packet[])
{
  byte dist = packet[2];
  byte loc = packet[3];

  if(!overrideCAS)
  {
    if(loc == 'F')
    {
      if(dist <= 12)
      {
        if(lastMoveDir == 'q' || lastMoveDir == 'w' || lastMoveDir == 'e' || lastMoveDir == 'Q' || lastMoveDir == 'W' || lastMoveDir == 'E')
        {
          emergencyStop();
          //stopMotors();
        }
        maxFspeed = 0;
        frontFlag = true;
      }
      else if(dist <= 16)
      {
          maxFspeed = 1;
          frontFlag = false;
      }
      else if(dist <= 24)
      {
        maxFspeed = 2;
        frontFlag = false;
      }
      else if(dist <= 36)
      {
        maxFspeed = 3;
        frontFlag = false;
      }
      else if(dist <= 48)
      {
        maxFspeed = 4;
        frontFlag = false;
      }
      else
      {
        maxFspeed++;
        if(maxFspeed >= 10)
        {
          maxFspeed = 10;
          frontFlag = false;
        }
      }
    }
    else if(loc == 'B')
    {
      if(dist <= 12)
      {
        if(lastMoveDir == 'z' || lastMoveDir == 's' || lastMoveDir == 'c' || lastMoveDir == 'Z' || lastMoveDir == 'S' || lastMoveDir == 'C')
        {
          emergencyStop();
          //stopMotors();
        }
        maxBspeed = 0;
        backFlag = true;
      }
      else if(dist <= 16)
      {
        maxBspeed = 1;
        backFlag = false;
      }
      else if(dist <= 24)
      {
        maxBspeed = 2;
        backFlag = false;
      }
      else if(dist <= 36)
      {
        maxBspeed = 3;
        backFlag = false;
      }
      else if(dist <= 48)
      {
        maxBspeed = 4;
        backFlag = false;
      }
      else
      {
        maxBspeed++;
        if(maxBspeed >= 10)
        {
          maxBspeed = 10;
          backFlag = false;
        }
      }
    }
    else if(loc == 'L')
    {
      if(dist <= 12)
      {
        if(lastMoveDir == 'q' || lastMoveDir == 'a' || lastMoveDir == 'z' || lastMoveDir == 'Q' || lastMoveDir == 'A' || lastMoveDir == 'Z')
        {
          emergencyStop();
          //stopMotors();
        }
        maxLspeed = 0;
        leftFlag = true;
      }
      else if(dist <= 16)
      {
        maxLspeed = 1;
        leftFlag = false;
      }
      else if(dist <= 24)
      {
        maxLspeed = 2;
        leftFlag = false;
      }
      else if(dist <= 36)
      {
        maxLspeed = 3;
        leftFlag = false;
      }
      else if(dist <= 48)
      {
        maxLspeed = 4;
        leftFlag = false;
      }
      else
      {
        maxLspeed++;
        if(maxLspeed >= 10)
        {
          maxLspeed = 10;
          leftFlag = false;
        }
      }
    }
    else if(loc == 'R')
    {
      if(dist <= 12)
      {
        if(lastMoveDir == 'e' || lastMoveDir == 'd' || lastMoveDir == 'c' || lastMoveDir == 'E' || lastMoveDir == 'D' || lastMoveDir == 'C')
        {
          emergencyStop();
          //stopMotors();
        }
        maxRspeed = 0;
        rightFlag = true;
      }
      else if(dist <= 16)
      {
        maxRspeed = 1;
        rightFlag = false;
      }
      else if(dist <= 24)
      {
        maxRspeed = 2;
        rightFlag = false;
      }
      else if(dist <= 36)
      {
        maxRspeed = 3;
        rightFlag = false;
      }
      else if(dist <= 48)
      {
        maxRspeed = 4;
        rightFlag = false;
      }
      else
      {
        maxRspeed++;
        if(maxRspeed >= 10)
        {
          maxRspeed = 10;
          rightFlag = false;
        }
      }
    }
  }
  if(debugEnable)
  {
    Serial.println("*****CAS Module*****");
    Serial.print(char(loc)); 
    Serial.print(": ");
    Serial.print(int(dist));
    Serial.println(" inches");
    Serial.println();
    Serial1.println("*****CAS Module*****");
    Serial1.print(char(loc)); 
    Serial1.print(": ");
    Serial1.print(int(dist));
    Serial1.println(" inches");
    Serial1.println();
  }
  else
  {
    Serial1.write(headerValue);
    for(int i = 0; i < packet[1] + 3; i++)
    {
      Serial1.write(packet[i]);
    }
  }
  //Rear CAS not ocnnected
  maxBspeed = 10;
}
//################################################################################################
//################################################################################################
//ID = 10
void LightModule(byte packet[])
{
  byte loc = packet[2];
  byte lightValue = packet[3];
  byte modeStatus = packet[4];
  if(debugEnable)
  {
    Serial.println("*****Light Module*****");
    Serial.print(int(loc)); 
    Serial.print(": ");
    Serial.print((int)lightValue);
    Serial.print("    mode: ");
    Serial.println((int)modeStatus);
    Serial.println();
    Serial1.println("*****Light Module*****");
    Serial1.print(int(loc)); 
    Serial1.print(": ");
    Serial1.print((int)lightValue);
    Serial1.print("    mode: ");
    Serial1.println((char)modeStatus);
    Serial1.println();
  }
  else
  {
    Serial1.write(headerValue);
    for(int i = 0; i < packet[1] + 3; i++)
    {
      Serial1.write(packet[i]);
    }
  }
}
void toggleLightModule()
{
  byte check = 0;
  byte id = 10;
  check += id;
  byte length = 1;
  check += length;
  byte cmd = 't';
  check += cmd;
  Serial3.write(headerValue);
  Serial3.write(id);  //ID byte
  Serial3.write(length);   //length byte
  Serial3.write(cmd);
  Serial3.write(check);
  if(debugEnable)
  {
    Serial.println("Light Toggled");
    Serial1.println("Light Toggled");
  }

}
//################################################################################################
//################################################################################################
//ID = 18
void SmartPowerModule(byte packet[])
{
  if(debugEnable)
  {
    double temp = packet[2];
    temp *=4;
    temp = (temp * 5)/1024;
    temp *= 6.315;    //manually calibrated
    Serial.println("*****Smart Power Module*****");
    Serial.print("System Battery Voltage: ");
    Serial.print(temp);
    Serial.println(" V");
    Serial.println();
    Serial1.println("*****Smart Power Module*****");
    Serial1.print("System Battery Voltage: ");
    Serial1.print(temp);
    Serial1.println(" V");
    Serial1.println();
  }
  else
  {
    Serial1.write(headerValue);
    for(int i = 0; i < packet[1] + 3; i++)
    {
      Serial1.write(packet[i]);
    }
  }
}
//################################################################################################
//################################################################################################
//ID = 27
void RFIDModule(byte packet[])
{
  if(debugEnable)
  {
    Serial.println("*****RFID Module*****");
    Serial.print("RFID: ");
    for(int i = 1; i < packet[1]; i++)
    {
      Serial.print(packet[i]);
    }
    Serial.println();
    Serial1.println("*****RFID Module*****");
    Serial1.print("RFID: ");
    for(int i = 1; i < packet[1]; i++)
    {
      Serial1.print(packet[i]);
    }
    Serial1.println();
  }
  else
  {
    Serial1.write(headerValue);
    for(int i = 0; i < packet[1] + 3; i++)
    {
      Serial1.write(packet[i]);
    }
  }
}
//################################################################################################
//################################################################################################
//ID = 35
void GPSModule(byte packet[])
{
  byte longDir = packet[2];
  byte latDir = packet[7];
  byte longPacket[4];
  byte latPacket[4];
  uint32_t longitude;
  uint32_t latitude;
  for(int x=0; x<4; x++)
  {
    longPacket[x] = packet[3+x];
    latPacket[x] = packet[8+x];
  }
  longitude = ((int(packet[3])) * 16777216) + ((int(packet[4])) * 65536) + ((int(packet[5])) * 256) + (int(packet[6]));
  latitude = ((int(packet[8])) * 16777216) + ((int(packet[9])) * 65536) + ((int(packet[10])) * 256) + (int(packet[11]));
  if(debugEnable)
  {
    Serial.println("*****GPS Module*****");
    Serial.print("Latitude: ");
    Serial1.println("*****GPS Module*****");
    Serial1.print("Latitude: ");  
    if (latDir == 'N')
    {
      Serial.print('+');
      Serial1.print('+');
    }
    else if (latDir == 'S')
    {
      Serial.print('-');
      Serial1.print('-');
    }

    Serial.print(latitude/1000000, DEC); 
    Serial.print("* ");
    Serial.print((latitude/10000)%100, DEC); 
    Serial.print('\''); 
    Serial.print(' ');
    Serial.print((latitude%10000)*6/1000, DEC); 
    Serial.print('.');
    Serial.print(((latitude%10000)*6/10)%100, DEC); 
    Serial.println('"');
    Serial1.print(latitude/1000000, DEC); 
    Serial1.print("* ");
    Serial1.print((latitude/10000)%100, DEC); 
    Serial1.print('\''); 
    Serial1.print(' ');
    Serial1.print((latitude%10000)*6/1000, DEC); 
    Serial1.print('.');
    Serial1.print(((latitude%10000)*6/10)%100, DEC); 
    Serial1.println('"');

    Serial.print("Longitude: ");
    Serial1.print("Longitude: ");
    if (longDir == 'E')
    {
      Serial.print('+');
      Serial1.print('+');
    }
    else if (longDir == 'W')
    {
      Serial.print('-');
      Serial1.print('-');
    }

    Serial.print(longitude/1000000, DEC); 
    Serial.print("* ");
    Serial.print((longitude/10000)%100, DEC); 
    Serial.print('\''); 
    Serial.print(' ');
    Serial.print((longitude%10000)*6/1000, DEC); 
    Serial.print('.');
    Serial.print(((longitude%10000)*6/10)%100, DEC); 
    Serial.println('"');
    Serial.println();
    Serial1.print(longitude/1000000, DEC); 
    Serial1.print("* ");
    Serial1.print((longitude/10000)%100, DEC); 
    Serial1.print('\''); 
    Serial1.print(' ');
    Serial1.print((longitude%10000)*6/1000, DEC); 
    Serial1.print('.');
    Serial1.print(((longitude%10000)*6/10)%100, DEC); 
    Serial1.println('"');
    Serial1.println();
  }
  else
  {
    Serial1.write(headerValue);
    for(int i = 0; i < packet[1] + 3; i++)
    {
      Serial1.write(packet[i]);
    }
  }
}
//################################################################################################
//################################################################################################
//ID = 38
void IMU_Module(byte packet[])
{
  unsigned long temp;
  double rollValue;
  double pitchValue;
  double yawValue;

  byte rollMSB = packet[2];
  byte rollLSB = packet[3];
  byte pitchMSB = packet[4];
  byte pitchLSB = packet[5];
  byte yawMSB = packet[6];
  byte yawLSB = packet[7];

  temp = rollMSB * 256 + rollLSB;
  rollValue = temp * (360.0 / 65536.0);
  temp = pitchMSB * 256 + pitchLSB;
  pitchValue = temp * (360.0 / 65536.0);
  temp = yawMSB * 256 + yawLSB;
  yawValue = temp * (360.0 / 65536.0);

  if(debugEnable)
  {
    Serial.println("*****IMU Module*****");
    Serial.print("Roll: ");
    Serial.println(rollValue);
    Serial.print("Pitch: ");
    Serial.println(pitchValue);
    Serial.print("Yaw: ");
    Serial.println(yawValue);

    Serial1.println("*****IMU Module*****");
    Serial1.print("Roll: ");
    Serial1.println(rollValue);
    Serial1.print("Pitch: ");
    Serial1.println(pitchValue);
    Serial1.print("Yaw: ");
    Serial1.println(yawValue);
  }
  else
  {
    Serial1.write(headerValue);
    for(int i = 0; i < packet[1] + 3; i++)
    {
      Serial1.write(packet[i]);
    }
  }
}
//################################################################################################
//################################################################################################
//ID = 39
void compassModule(byte packet[])
{
  int hMSB = packet[2];
  int hLSB = packet[3];
  int headingValue = 0;
  if(hMSB == 1)
  {
    headingValue += 256;
  }
  else if(hMSB == 0)
  {
  }
  headingValue += hLSB;

  if(debugEnable)
  {
    Serial.println("*****Compass Module*****");
    Serial.print("Heading: ");
    Serial.println(headingValue);
    Serial.println();
    Serial1.println("*****Compass Module*****");
    Serial1.print("Heading: ");
    Serial1.println(headingValue);
    Serial1.println();
  }
  else
  {
    Serial1.write(headerValue);
    for(int i = 0; i < packet[1] + 3; i++)
    {
      Serial1.write(packet[i]);
    }
  }
}
//################################################################################################
//################################################################################################
//ID = 40
void accelerometerModule(byte packet[])
{
  int xVal = packet[2];
  int yVal = packet[3];
  int zVal = packet[4];
  xVal *=4;
  yVal *=4;
  zVal *=4;
  int gVal = 66;

  if(debugEnable)
  {

    double tempxVal = (double)xVal/25.0;
    double tempyVal = (double)yVal/25.0;
    double tempzVal = (double)zVal/25.0;

    tempxVal -= 5.0;
    tempyVal -= 5.0;
    tempzVal -= 5.0;

    Serial.println("*****Accelerometer Module*****");
    Serial.print("X:");
    Serial.println(tempxVal );
    Serial.print("Y:");
    Serial.println(tempyVal );
    Serial.print("Z:");
    Serial.println(tempzVal );
    Serial.println();
    Serial1.println("*****Accelerometer Module*****");
    Serial1.print("X:");
    Serial1.println(tempxVal );
    Serial1.print("Y:");
    Serial1.println(tempyVal );
    Serial1.print("Z:");
    Serial1.println(tempzVal );
    Serial1.println();
  }
  else
  {
    Serial1.write(headerValue);
    for(int i = 0; i < packet[1] + 3; i++)
    {
      Serial1.write(packet[i]);
    }
  }

}
//################################################################################################
//################################################################################################
//ID = 41
void environmentModule(byte packet[])
{
  int t, h;
  h = packet[2];
  t = packet[3];
  if(debugEnable)
  {
    Serial.println("*****Environment Module*****");
    Serial.print("Humidity: ");
    Serial.println((int)h);
    Serial.print("Temperature: ");
    Serial.println((int)t);
    Serial.println();
    Serial1.println("*****Environment Module*****");
    Serial1.print("Humidity: ");
    Serial1.println((int)h);
    Serial1.print("Temperature: ");
    Serial1.println((int)t);
    Serial1.println();
  }
  else
  {
    Serial1.write(headerValue);
    for(int i = 0; i < packet[1] + 3; i++)
    {
      Serial1.write(packet[i]);
    }
  }
}
//################################################################################################
//################################################################################################
//ID = 42
//Gas Module
void gasModule(byte packet[])
{
  unsigned int COValue = packet[2]*256 + packet[3];
  unsigned int AlcoholValue = (packet[4]*256 + packet[5])/1000.0;
  
  unsigned int MethaneValue = packet[6]*256 + packet[7];
  unsigned int FlamableValue = packet[8]*256 + packet[9];

  if(debugEnable)
  {
    Serial.println("*****Gas Module*****");
    Serial.print("CO: ");
    Serial.print(COValue);
    Serial.println(" PPM");
    Serial.print("Alocohol: ");
    Serial.print(AlcoholValue);
    Serial.println(" mg/L");
    Serial.print("Methane: ");
    Serial.print(MethaneValue);
    Serial.println(" PPM");
    Serial.print("Flamable: ");
    Serial.print(FlamableValue);
    Serial.println(" PPM");
    
    Serial1.println("*****Gas Module*****");
    Serial1.print("CO: ");
    Serial1.print(COValue);
    Serial1.println(" PPM");
    Serial1.print("Alocohol: ");
    Serial1.print(AlcoholValue);
    Serial1.println(" mg/L");
    Serial1.print("Methane: ");
    Serial1.print(MethaneValue);
    Serial1.println(" PPM");
    Serial1.print("Flamable: ");
    Serial1.print(FlamableValue);
    Serial1.println(" PPM");
  }
  else
  {
    Serial1.write(headerValue);
    for(int i = 0; i < packet[1] + 3; i++)
    {
      Serial1.write(packet[i]);
    }
  }
}
//################################################################################################
//################################################################################################
//ID = 100
//uses HackHD
void cameraModule(byte packet[])
{
  byte dataType = packet[2];
  if((dataType == 'M') || (dataType == 'm'))
  {
    int mode = packet[3];
    if(debugEnable)
    {
      Serial.println("*****Camera Module*****");
      Serial.print("Mode: ");
      Serial.println(mode);
      Serial.println();
      Serial1.println("*****Camera Module*****");
      Serial1.print("Mode: ");
      Serial1.println(mode);
      Serial1.println();
    }
    else
    {
      Serial1.write(headerValue);
      for(int i = 0; i < packet[1] + 3; i++)
      {
        Serial1.write(packet[i]);
      }
    }
  }

}

void HackHDStandby()
{
  //place camera into standby mode
}
void HackHDRecord()
{
  //start recording
}
void HackHDStopRecord()
{
  //stop recording
}
//################################################################################################
//################################################################################################
//ID = 101
void videoSwitchModule(byte mCmd)
{
  byte mID = 101;
  byte mLength = 1;
  byte mChecksum = 0;
  writeBuffer[0] = headerValue;
  writeBuffer[1] = mID;
  writeBuffer[2] = mLength;
  writeBuffer[3] = mCmd;
  mChecksum += mID;
  mChecksum += mLength;
  mChecksum += mCmd;
  writeBuffer[4] = mChecksum;

  writeSerial3(5, writeBuffer);
  if(debugEnable)
  {
    Serial.println("Video Source Toggled");
    Serial1.println("Video Source Toggled");
  }
}
//################################################################################################
//################################################################################################
//ID = 150
void turretModule(byte dir, byte steps)
{  
  byte mID = 150;
  byte mLength = 2;
  byte mChecksum = 0;
  writeBuffer[0] = headerValue;
  writeBuffer[1] = mID;
  writeBuffer[2] = mLength;
  writeBuffer[3] = dir;
  writeBuffer[4] = steps;
  mChecksum += mID;
  mChecksum += mLength;
  mChecksum += dir;
  mChecksum += steps;
  writeBuffer[5] = mChecksum;

  writeSerial3(6, writeBuffer);

}
//################################################################################################
//################################################################################################
//ID = 175
void heartRateModule(byte packet[])
{
  byte HR = packet[2];

  if(debugEnable)
  {
    Serial.println("*****Heart Rate Module*****");
    Serial.print("Heart Rate: ");
    Serial.println((int)HR);
    Serial1.println("*****Heart Rate Module*****");
    Serial1.print("Heart Rate: ");
    Serial1.println((int)HR);
  }
  else
  {
    Serial1.write(headerValue);
    for(int i = 0; i < packet[1] + 3; i++)
    {
      Serial1.write(packet[i]);
    }
  }
}
//################################################################################################
//################################################################################################
//ID = 200
void tankBaseModule(byte cmd)
{
  byte mID = 200;
  byte mLength = 1;
  byte mChecksum = 0;
  writeBuffer[0] = headerValue;
  writeBuffer[1] = mID;
  writeBuffer[2] = mLength;
  writeBuffer[3] = cmd;
  mChecksum += mID;
  mChecksum += mLength;
  mChecksum += cmd;
  writeBuffer[4] = mChecksum;

  writeSerial3(5, writeBuffer);
}
//################################################################################################
//################################################################################################
//ID = 255
//Outputs system information for diagnosis
void systemModule()
{
  if(errorCount > 65535)
  {
    errorCount = 65535;
  }
  byte modChecksum = 0;
  byte modID = 255;
  byte errorCmd = 'e';
  byte errorCountMSB = errorCount/256;
  byte errorCountLSB = errorCount%256;
  
  modChecksum += errorCmd;
  modChecksum += errorCountMSB;
  modChecksum += errorCountLSB;
  if(debugEnable)
  {
  }
  else
  {
    Serial1.write(headerValue);
    Serial1.write(modID);
    Serial1.write(errorCmd);
    Serial1.write(errorCountMSB);
    Serial1.write(errorCountLSB);
  }
}
//################################################################################################
//################################################################################################
void unidentifiedModule(byte packet[])
{
  if(debugEnable)
  {
    Serial.println("*****Unidentified Module*****");
    Serial.println((int)packet[1]);
    Serial.println();
    Serial1.println("*****Unidentified Module*****");
    Serial1.println((int)packet[1]);
    Serial1.println();
  }
  else
  {
    Serial1.write(headerValue);
    for(int i = 0; i < packet[1] + 3; i++)
    {
      Serial1.write(packet[i]);
    }
  }
}
//################################################################################################
//************************************************************************************************

