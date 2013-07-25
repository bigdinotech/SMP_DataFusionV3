//***************************************************
//Dino Tinitigan
//Smart Mobile Platform
//Data Fusion Module
//***************************************************
//libraries
#include <SoftwareSerial.h>
#include <Sabertooth.h>

//pinDefinitions
#define brk_sig 22
#define rst_all 23
#define bus_bsy 24
#define bus_chk 25
#define MS0 30
#define MS0 31
#define MS0 32
#define MS0 33
#define ledPin 34

//*******************************************Globals**********************************************
//objects
Sabertooth saber(128, Serial2);  //Sabertooth w/ addr 128

//buffers
byte sBuff;  //temporary buffer for serial data
byte packetBuffer[64];
byte writeBuffer[64];

//variables
byte headerValue = 170;  //0b10101010
int bus_state;
boolean validData = false;
boolean debugEnable = false;
int modID;

int spdSteps = 10;  //count of spd levels
int maxFspeed = spdSteps;
int maxBspeed = spdSteps;
int maxLspeed = spdSteps;
int maxRspeed = spdSteps;
boolean frontFlag = false;
boolean backFlag = false;
boolean leftFlag = false;
boolean rightFlag = false;

byte lastMoveDir;

int errorCount = 0;
int totalErrorCount = 0;
long currentErrorTime = 0;
long lastErrorTime = 0;
long checkStart = 0;
long currentTime = 0;
int rstCounter = 0;


int serialScheduler = 0;
int tempSpeed = 4;
int loopCounter = 0;
boolean loopTimeout = false;
boolean overrideCAS = false;
boolean tankControlMode = false;
//************************************************************************************************
//********************************************Main************************************************
//################################################################################################
void setup()
{
  Serial.begin(38400);  //debug
  Serial1.begin(38400);  //XBEE
  Serial2.begin(38400);  //used for Sabertooth motor controller
  Serial3.begin(115200);  //used for Modules
  saber.setTimeout(500);  //set timeout to 500ms
  saber.setRamping(18);    //slow ramp time
  
  //set pinModes
  pinMode(brk_sig, OUTPUT);
  pinMode(rst_all, OUTPUT);
  pinMode(bus_bsy, OUTPUT);
  pinMode(bus_chk, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(MS0, OUTPUT);
  digitalWrite(bus_chk, HIGH);
  digitalWrite(bus_bsy, LOW);
  
  validData = true;
  if(debugEnable)
  {
    Serial.println("Starting");
    Serial1.println("Starting");
  }
}
//################################################################################################
//################################################################################################
void loop()
{
  validData = true;
  
  modID = 0;
  digitalWrite(bus_bsy, LOW);  
  bus_state = digitalRead(bus_chk);
  loopTimeout = false;
  checkStart = millis();
  while((bus_state == HIGH) && !loopTimeout)
  {
    bus_state = digitalRead(bus_chk);
    if(bus_state)
    {
      readSerialData();
    }
    currentTime = millis();
    if((currentTime - checkStart) >10000)
    {
      loopTimeout = true;
    }
  }
  if(!loopTimeout)
  {
    //digitalWrite(bus_bsy, HIGH);  //lock the bus
    readSerial3();
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
        case 3:
        {
          CASModule(packetBuffer);
          break;
        }
        case 10:
        {
          LightModule(packetBuffer);
          break;
        }
        case 18:
        {
          SmartPowerModule(packetBuffer);
          break;
        }
        case 27:
        {
          RFIDModule(packetBuffer);
          break;
        }
        case 35:
        {
          GPSModule(packetBuffer);
          break;
        }
        case 38:
        {
          IMU_Module(packetBuffer);
          break;
        }
        case 39:
        {
          compassModule(packetBuffer);
          break;
        }
        case 40:
        {
          accelerometerModule(packetBuffer);
          break;
        }
        case 41:
        {
          environmentModule(packetBuffer);
          break;
        }
        case 42:
        {
          gasModule(packetBuffer);
          break;
        }
        case 100:
        {
          cameraModule(packetBuffer);
          break;
        }
        case 175:
        {
          heartRateModule(packetBuffer);
          break;
        }
        case 200:
        {
          break;
        }
        default:
        {
          if(debugEnable)
          {
            Serial.print("ID not found: ");
            Serial.println(modID);
            Serial1.print("ID not found: ");
            Serial1.println(modID);
          }
          break;
        }
      }
    }
    else
    {
      //checksum error
      if(debugEnable)
      {
        Serial.println("Checksum Error");
        Serial1.println("Checksum Error");
      }
      errorControl();
    }
    readSerialData();  //check if other serial ports for data
    //digitalWrite(bus_bsy, LOW);  //free the bus
  }
  else
  {
    //timeout!! no modules connected??
  }
  loopCounter++;
  if(loopCounter>10000)
  {
    loopCounter = 0;
  }
  if((loopCounter%100) == 0)
  {
    //reconditionMC();
    //systemModule();
  }
  if((loopCounter%1000) ==0)
  {
  }
}
//################################################################################################
//************************************************************************************************









