//********************************************General*********************************************
//################################################################################################
void errorControl()
{
  totalErrorCount++;
  if(errorCount == 0)
  {
    lastErrorTime = millis();
  }
  currentErrorTime = millis();
  if((currentErrorTime - lastErrorTime) > 5000)
  {
    //reset errorCount if no errors for 5s
    errorCount = 0;
  }
  else if((currentErrorTime - lastErrorTime) > 1000)
  {
    //decrement errorCount if no errors in last second
    errorCount--;
  }
  else
  {
    errorCount++;
  }
  if(errorCount >= 10)
  {
    // attempt to recover by reseting all the modules
    digitalWrite(rst_all, LOW);
    delay(1);
    digitalWrite(rst_all, HIGH);
    rstCounter++;
  }
  if(debugEnable)
  {
    Serial.print("Total Error Count: ");
    Serial.println(totalErrorCount);
    Serial1.print("Total Error Count: ");
    Serial1.println(totalErrorCount);
  }  
}
//################################################################################################
//################################################################################################
void lockOut()
{
  //initiated when there are to many error resets
  if(rstCounter>10)
  {
    //lock everything
  }
}
//################################################################################################
//################################################################################################
void systemDiagnosis()
{
  
}
//################################################################################################
//################################################################################################
void recover()
{
  stopMotors();
  reconditionMC();
  //reintialize variables;
  spdSteps = 10;  //count of spd levels
  maxFspeed = spdSteps;  
  maxBspeed = spdSteps;
  maxLspeed = spdSteps;
  maxRspeed = spdSteps;
  tempSpeed = 4;
  frontFlag = false;
  backFlag = false;
  leftFlag = false;
  rightFlag = false;
  lastMoveDir = 'x';
  overrideCAS = false;
  pinMode(brk_sig, OUTPUT);
  pinMode(rst_all, OUTPUT);
  pinMode(bus_bsy, OUTPUT);
  pinMode(bus_chk, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(MS0, OUTPUT);
  if(debugEnable)
  {
    Serial.println("Recovery");
    Serial1.println("Recovery");
  }
}
//################################################################################################
//################################################################################################
void toggleDebugMode()
{
  if(debugEnable)
  {
    Serial.println("Debug Mode Disabled");
    Serial1.println("Debug Mode Disabled");
    debugEnable = false;
  }
  else
  {
    debugEnable = true;
    Serial.println("Debug Mode Enabled");
    Serial1.println("Debug Mode Enabled");
  } 
}
//################################################################################################
//################################################################################################
void printCommandList()
{
  if(debugEnable)
  {
    Serial.println("*****SMP Command List*****");
    Serial.println("*f: Enable/Disable Debug Mode");
    Serial.println("*Movement Commands");
    Serial.println("*  w: Forward");
    Serial.println("*  s: Reverse");
    Serial.println("*  a: Turn left");
    Serial.println("*  d: Turn right");
    Serial.println("*  q: Forward left");
    Serial.println("*  e: Forward right");
    Serial.println("*  z: Reverse left");
    Serial.println("*  c: Reverse right");
    Serial.println("*  x: Stop");
    Serial.println("*  o: Emergency stop");
    Serial.println("*  u: Increase speed");
    Serial.println("*  j: Decrease speed");
    Serial.println("*r: System recovery");
    Serial.println("*v: Toggle video source");
    Serial.println("*t: Toggle light");
    Serial.println("*y: Switch to mini tank control");
    Serial.println("*p: CAS Override");
    Serial.println("*Turret Commands");
    Serial.println("*  m: Turn turret right");
    Serial.println("*  n: Turn turret left");
    Serial.println("*  h: Auto target heading mode");
    Serial.println("*  b: Set target heading");
    
    Serial1.println("*****SMP Command List*****");
    Serial1.println("*f: Enable/Disable Debug Mode");
    Serial1.println("*Movement Commands");
    Serial1.println("*  w: Forward");
    Serial1.println("*  s: Reverse");
    Serial1.println("*  a: Turn left");
    Serial1.println("*  d: Turn right");
    Serial1.println("*  q: Forward left");
    Serial1.println("*  e: Forward right");
    Serial1.println("*  z: Reverse left");
    Serial1.println("*  c: Reverse right");
    Serial1.println("*  x: Stop");
    Serial1.println("*  o: Emergency stop");
    Serial1.println("*  u: Increase speed");
    Serial1.println("*  j: Decrease speed");
    Serial1.println("*r: System recovery");
    Serial1.println("*v: Toggle video source");
    Serial1.println("*t: Toggle light");
    Serial1.println("*y: Switch to mini tank control");
    Serial1.println("*p: CAS Override");
    Serial1.println("*Turret Commands");
    Serial1.println("*  m: Turn turret right");
    Serial1.println("*  n: Turn turret left");
    Serial1.println("*  h: Auto target heading mode");
    Serial1.println("*  b: Set target heading");
  }
}
//################################################################################################
//################################################################################################
void CASOverride()
{
  if(!overrideCAS)
  {
    overrideCAS = true;
    if(debugEnable)
    {
      Serial.println("CAS Override!");
      Serial1.println("CAS Override!");
      maxFspeed = spdSteps;
      maxBspeed = spdSteps;
      maxLspeed = spdSteps;
      maxRspeed = spdSteps;
    }
  }
  else
  {
    overrideCAS = false;
    if(debugEnable)
    {
      Serial.println("CAS Enabled!");
      Serial1.println("CAS Enabled!");
    }
  }
}
//################################################################################################
//################################################################################################
void simpleControl(byte cmd)
{
  //controlModule
  byte controlChecksum = 0;
  controlChecksum += 3; //add ID and data length Bytes
  packetBuffer[0] = 1;
  packetBuffer[1] = 2;
  packetBuffer[2] = cmd;
  packetBuffer[3] = tempSpeed;
  controlChecksum += cmd;
  controlChecksum += tempSpeed;
  packetBuffer[4] = controlChecksum;
  controlModule(packetBuffer);
}
//################################################################################################
//################################################################################################
//################################################################################################
void simpleSpeedChange(byte cmd)
{
  if(cmd == 'u')
  {
    if(tempSpeed<10)
    {
      tempSpeed++;
    }
    if(debugEnable)
    {
      Serial.print("Speed: ");
      Serial.println(tempSpeed);
      Serial1.print("Speed: ");
      Serial1.println(tempSpeed);
    }
  }
  else if(cmd == 'j')
  {
    if(tempSpeed > 0)
    {
      tempSpeed--;
    }
    if(debugEnable)
    {
      Serial.print("Speed: ");
      Serial.println(tempSpeed);
      Serial1.print("Speed: ");
      Serial1.println(tempSpeed);
    }
  }
}
//################################################################################################
//################################################################################################
void switchControl()
{
}
//################################################################################################
//################################################################################################
