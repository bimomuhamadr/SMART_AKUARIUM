#include <OneWire.h>
float pHVol;
float phValue;
int temperature;
static float kekeruhan;
static float teg;
float To = 0;
const int pH_Pin = A0;
int DS18S20_Pin = A2; 
OneWire ds(DS18S20_Pin);
int sensorValue = 0; 
int echo = 7; //atas
int trig = 6; //bawah
long sonar;
unsigned long int avgValue; 
int buf[10],temp; 
float ntuVol;
float ntuValue;
const int analogInPin1= A1; 
float b;
int buf1[10],temp1; 
unsigned long int avgValue1; 
float b1;
int buf2[10],temp2;
void setup() {
  Serial.begin(115200);
  pinMode(trig, OUTPUT); 
  pinMode(echo, INPUT);

}

void loop() {
  ph();
  suhu();
  turbidity();
  ultra();
  delay(2000);
}

void ultra(){
long durasi, sonar;
digitalWrite(trig, LOW);
delayMicroseconds(2);
digitalWrite(trig, HIGH);
delayMicroseconds(5);
digitalWrite(trig, LOW);
durasi = pulseIn(echo, HIGH);
sonar = durasi/29/2;
Serial.print("ultra ");
Serial.println(sonar);
}

void ph(){
 for(int i=0;i<10;i++) 
 { 
  buf[i]=analogRead(pH_Pin);
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
 phValue = abs(-5.9375 * pHVol + 22.297);
 Serial.print("ADC PH = ");
 Serial.println(pHVol);
 Serial.print("pH ");
 Serial.println(phValue,1);
}

void suhu(){
  temperature = getTemp();
  Serial.print("suhu ");
  Serial.println(temperature);
}

float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }
  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }
  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end
  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad  
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }  
  ds.reset_search();  
  byte MSB = data[1];
  byte LSB = data[0];
  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = abs(tempRead / 16);  
  return TemperatureSum;  
}


void turbidity(){
    for(int i=0;i<10;i++) 
   { 
    buf2[i]=analogRead(analogInPin1);
    delay(10);
   }
   for(int i=0;i<9;i++)
   {
    for(int j=i+1;j<10;j++)
    {
     if(buf2[i]>buf2[j])
     {
      temp2=buf2[i];
      buf2[i]=buf2[j];
      buf2[j]=temp2;
     }
    }
   }
   avgValue1=0;
   for(int i=2;i<8;i++)
   avgValue1+=buf2[i];
   ntuVol=(float)avgValue1*5.0/1024/6;
   ntuValue = abs(-25.556* ntuVol + 106.25);
   Serial.print("ADC ntu = ");
   Serial.println(ntuVol);
   Serial.print("ntu ");
   Serial.println(ntuValue);
   delay(100);
}
