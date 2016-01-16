// Connect sr04 sonar to PPZ AP IIC 
// by moses mail: moses.hao@gmail.com 2016
#include <SR04.h>
#include <Wire.h>

#define DEBUG

//=======Hardware connect ===========
#define TRIG_PIN 3
#define ECHO_PIN 2
//=======SONAR range min -- max======
#define SONAR_MIN_RANGE 1   //1cm
#define SONAR_MAX_RANGE 400 //400cm
//======I2c address==================
#define I2C_ADDRESS 0X38    //i2c address 
#define ERROR_VALUE 0X4FFF  //ERROR VALUE 
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
uint16_t tempread, reading_cm;
byte dataflag = true;

int Sonarfilter(int filterlen) 
{
    int min = SONAR_MAX_RANGE, max = 0, avg = 0, d = 0;
    if (filterlen < 3) 
    {
        return ERROR_VALUE;
    }
    for (int x = 0; x < filterlen; x++) 
    {
        d = sr04.Distance();
        if((d < SONAR_MIN_RANGE) || (d > SONAR_MAX_RANGE))
          return ERROR_VALUE;   //get a error data
        if (d < min) 
        {
            min = d;
        }
        if (d > max) 
        {
            max = d;
        }
        avg += d;
    }
    // substract highest and lowest value
    avg -= (max + min);
    // calculate average
    avg /= (filterlen-2);
    return avg;
}

void setup()
{
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
  pinMode(13, OUTPUT);          // LED PIN
  Wire.begin(I2C_ADDRESS);      // i2c in slave mode address is 0x38(in ppz should be 0x70)
  Wire.onRequest(requestEvent); // register event
}

//====================Main loop process sonar data ===================
void loop()
{
  //just read 5 times for filter
  tempread=Sonarfilter(5); 
  if(tempread != ERROR_VALUE)
  {
    reading_cm = tempread;
    dataflag = true;
  }
  else
  {
    dataflag = false;
  }
  #ifdef DEBUG  //In debug mode, data will send to serialport
  if(dataflag == true)
  {
    Serial.println(reading_cm);
    blinkled();
  }
  #endif
}

//==========if sonar data is invalid should return a err checkshum=====
void requestEvent()
{
  byte sendhi = 0, sendli = 0;
  byte sendbyte[3];
  uint16_t tempreading_cm;
  tempreading_cm=reading_cm;
  sendhi=tempreading_cm>>8;
  sendli=tempreading_cm&0xff;
  sendbyte[0]=sendhi;
  sendbyte[1]=sendli;
  sendbyte[2]=(sendhi+sendli)&0xff;
  if(dataflag == false)
    sendbyte[2] = !sendbyte[2];  
  Wire.write(sendbyte,3); 
  blinkled(); 
}

//============================Blink LED=================================
byte flag = 0;
void blinkled()
{
  if(flag)
  {
    flag = 0;
    digitalWrite(13, LOW);
  }
  else
  {
    flag = 1;
    digitalWrite(13, HIGH);
  }
}
