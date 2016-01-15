// Connect MB1043 sonar to PPZ AP IIC 
// by moses mail: moses.hao@gmail.com 2016
// just use arduino mini pro 6PIN to connect MB1043 pin2

#include <Wire.h>
#define ECHO_PIN 6
#define PULSE_TIMEOUT 200000
uint16_t tempread, reading_cm;
byte flag = 0;

//Blink LED
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

long DistanceAvg(int count) {
    long min, max, avg, d;
    min = 999;
    max = 0;
    avg = d = 0;
    if (count < 1) {
        count = 1;
    }

    for (int x = 0; x < count + 2; x++) {
        //d = sr04.Distance();
        if(d > 500) return 500;
        if (d < min) {
            min = d;
        }
        if (d > max) {
            max = d;
        }
        avg += d;
    }

    // substract highest and lowest value
    avg -= (max + min);
    // calculate average
    avg /= count;
    return avg;
    //return min;
}

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);          // LED PIN
  pinMode(ECHO_PIN, INPUT);     // ECHO PIN
  Wire.begin(0x38);             // i2c in slave mode address is 0x38(in ppz should be 0x70)
  Wire.onRequest(requestEvent); // register event
}


void loop()
{
  tempread = pulseIn(ECHO_PIN, HIGH, PULSE_TIMEOUT);
  reading_cm=tempread;
  if((reading_cm < 4000) && (reading_cm > 350))
  {
   Serial.println(reading_cm);
  }
}


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
  Wire.write(sendbyte,3); 
  blinkled(); 
}
