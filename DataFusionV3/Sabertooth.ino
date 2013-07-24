//******************************************Sabertooth********************************************
//################################################################################################
void moveMotor(byte dir, byte spd)
{
  //forward - w
  //reverse - s
  //left    - a
  //right   - d
  //stop    - x
  //eSTOP   - o
  //fLeft   - q
  //fRight  - e
  //rLeft   - z
  //rRight  - c
  if(spd != 0)
  {
    int stepValue = 127/spdSteps;
    spd *= stepValue;
  }
  else
  {
    spd = 1;
  }
  if(dir == 'w' || dir == 'W')  //forward
  {
    digitalWrite(brk_sig, HIGH);  //turns relay on releasing brakes
    saber.motor(1, spd);
    saber.motor(2, spd);
  }
  else if(dir == 'a' || dir == 'A')  //left
  {
    digitalWrite(brk_sig, HIGH);  //turns relay on releasing brakes
    if(spd < 32)
    {
      spd *= 0.6;
    }
    else if(spd < 64)
    {
      spd *= 0.5;
    }
    else if(spd < 89)
    {
      spd *= 0.4;
    }
    else
    {
      spd *= 0.3;
    }
    saber.motor(1, spd);
    saber.motor(2, -spd);
  }
  else if(dir == 'd' || dir == 'D')  //right
  {
    digitalWrite(brk_sig, HIGH);  //turns relay on releasing brakes
    if(spd < 32)
    {
      spd *= 0.6;
    }
    else if(spd < 64)
    {
      spd *= 0.5;
    }
    else if(spd < 89)
    {
      spd *= 0.4;
    }
    else
    {
      spd *= 0.3;
    }
    saber.motor(1, -spd);
    saber.motor(2, spd);
  }
  else if(dir == 's' || dir == 'S')  //reverse
  {
    digitalWrite(brk_sig, HIGH);  //turns relay on releasing brakes
    saber.motor(1, -spd);
    saber.motor(2, -spd);
  }
  else if(dir == 'e' || dir == 'E')  //forwardRight
  {
    digitalWrite(brk_sig, HIGH);  //turns relay on releasing brakes
    saber.motor(1, (spd * 0.75));
    saber.motor(2, spd);
  }
  else if(dir == 'q' || dir == 'Q')  //forwardLeft
  {
    digitalWrite(brk_sig, HIGH);  //turns relay on releasing brakes
    saber.motor(1, spd);
    saber.motor(2, (spd * 0.75));
  }
  else if(dir == 'z' || dir == 'Z')  //reverseLeft
  {
    digitalWrite(brk_sig, HIGH);  //turns relay on releasing brakes
    saber.motor(1, -spd);
    saber.motor(2, -(spd * 0.75));
  }
  else if(dir == 'c' || dir == 'C')  //reverseRight
  {
    digitalWrite(brk_sig, HIGH);  //turns relay on releasing brakes
    saber.motor(1, -(spd * 0.75));
    saber.motor(2, -spd);
  }
  else if(dir == 'x' || dir == 'X')
  {
    //digitalWrite(brk_sig, LOW);  //engage breaks
    saber.motor(1, 0);
    saber.motor(2, 0);
  }
  else if(dir == 'o' || dir == 'O')
  {
    saber.motor(1, 0);
    saber.motor(2, 0);
    digitalWrite(brk_sig, LOW);  //engage breaks
  }
}
//################################################################################################
//################################################################################################
void stopMotors()
{
  saber.motor(1, 0);
  saber.motor(2, 0);
  lastMoveDir = 'x';
}
//################################################################################################
//################################################################################################
void emergencyStop()
{
  digitalWrite(brk_sig, LOW);  //turns relay off engaging brakes
  saber.motor(1, 0);
  saber.motor(2, 0);
  if(debugEnable)
  {
    Serial.println("Emergency Stop!!!");
    Serial1.println("Emergency Stop!!!");
  }
}
//################################################################################################
void reconditionMC()
{
  saber.setTimeout(500);  //set timeout to 500 ms
  saber.setRamping(18);    //slow ramp time
}
//################################################################################################
//************************************************************************************************
