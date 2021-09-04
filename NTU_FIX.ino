#include <Wire.h> 
#include <OneWire.h>
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
float ntuVol;
float ntuValue;
const int analogInPin1= A1; 
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
      ntu();
  delay(100);
}

void ntu(){
 for(int i=0;i<10;i++) 
 { 
  buf1[i]=analogRead(analogInPin1);
  delay(10);
 }
 for(int i=0;i<9;i++)
 {
  for(int j=i+1;j<10;j++)
  {
   if(buf1[i]>buf1[j])
   {
    temp1=buf1[i];
    buf1[i]=buf1[j];
    buf1[j]=temp1;
   }
  }
 }
 avgValue1=0;
 for(int i=2;i<8;i++)
 avgValue1+=buf1[i];
 ntuVol=(float)avgValue1*5.0/1024/6;
 ntuValue = -25.556* ntuVol + 111.11;
 Serial.print("ADC ntu = ");
 Serial.print(ntuVol);
 Serial.print("  sensor ntu = ");
 Serial.println(ntuValue);
}


