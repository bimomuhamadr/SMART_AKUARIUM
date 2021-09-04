#include <Wire.h> 
#include <OneWire.h>
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
float pHVol;
float phValue;
const int analogInPin = A0; 
int sensorValue = 0; 
int sensorValue1 = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp; 
unsigned long int avgValue1; 
float b1;
int buf1[10],temp1;
int tombol;

void setup() {
 Serial.begin(9600);
}
 
void loop() {
   ph();
  delay(500);
}

void ph(){
 for(int i=0;i<10;i++) 
 { 
  buf[i]=analogRead(analogInPin);
  delay(10);
 }
 for(int i=0;i<9;i++)
 {
  for(int j=i+1;j<10;j++)
  {
   if(buf[i]>buf[j])
   {
    temp=buf[i];
    buf[i]=buf[j];
    buf[j]=temp;
   }
  }
 }
 avgValue=0;
 for(int i=2;i<8;i++)
 avgValue+=buf[i];
 pHVol=(float)avgValue*5.0/1024/6;
 phValue = -5.9375 * pHVol + 22.297;
 Serial.print("ADC = ");
 Serial.print(pHVol);
 Serial.print("  sensor = ");
 Serial.println(phValue);
}

