#include <ESP8266WiFi.h>
#include <Wire.h> 
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); //LCD
Servo servo;
#define relay1 14 //pompa 1
#define relay2 12 //lampu
#define relay3 13 //kipas
#define relay4 15 //pompa 2
//#define kabel1 0 // kabel rendah 
//#define kabel2 2 // kabel tinggi

//const char* ssid     = "acle1234";
//const char* password = "n1n2n3n4";
//const char* host = "nissandatsunmobil.com";

const char* ssid     = "HUAWEI E5673s-609";
const char* password = "b1b2b3b4";
const char* host = "nissandatsunmobil.com";

//const char* ssid     = "KUMALA5";
//const char* password = "DANISH06042012";
//const char* host = "nissandatsunmobil.com";
//const char* streamId   = "....................";
//const char* privateKey = "....................";

String command;
float a,c;
int b,d;
void setup() {
  Serial.begin(115200);
  Wire.begin(D2, D1);
  lcd.init();
  lcd.backlight();
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
//  pinMode(kabel1, INPUT_PULLUP);
//  pinMode(kabel2, INPUT_PULLUP);
  
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);

  servo.attach(D0);
  servo.write(0);
  delay(10);
  // We start by connecting to a WiFi network

  lcd.setCursor(3,0);
  lcd.print("Connecting");
  delay(2000);
  lcd.clear();
  
  lcd.setCursor(1,0);
  lcd.print("Connecting to");
  lcd.setCursor(1,1);
  lcd.print(ssid);
  delay(2000);
  lcd.clear();
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  lcd.setCursor(1,0);
  lcd.print("IP address : ");
  lcd.setCursor(1,1);
  lcd.print(WiFi.localIP());
  delay(2000);
  lcd.clear();

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  lcd.setCursor(1,0);
  lcd.print("Connecting to ");
  lcd.setCursor(1,1);
  lcd.print(host);
  delay(2000);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("SMART AKUARIUM");
  lcd.setCursor(5,1);
  lcd.print("BIMO MR");
  
}

void loop() {
  Serial.print("connecting to ");
  Serial.println(host);

  while(Serial.available()) {
    char f = Serial.read();
    if(f=='\n'){
//      Serial.println(command);
      parseCommand(command);
      command="";
    }
    else {
      command += f;
    }
  }
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ph:");
  lcd.setCursor(4,0);
  lcd.print(a);
  delay(1500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ntu:");
  lcd.setCursor(5,0);
  lcd.print(c);
  delay(1500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("suhu:");
  lcd.setCursor(6,0);
  lcd.print(b);
  delay(1500);
  lcd.clear();
  
  Serial.print("ph : ");
  Serial.println(a);
  Serial.print("suhu : ");
  Serial.println(b);
  Serial.print("turbinity : ");
  Serial.println(c);
  Serial.print("ultra : ");
  Serial.println(d);
  
  if(d >= 10 && d <= 14){
    //pompa mati
    lcd.setCursor(2,0);
    lcd.print("Pompa Kuras");
    lcd.setCursor(2,1);
    lcd.print("Kondisi Mati");
    digitalWrite(relay1, LOW);
    update2();
    delay(2000);
    lcd.clear();  
  }
  if(d >= 0 && d <= 5){
    //pompa mati
    lcd.setCursor(2,0);
    lcd.print("Pompa Mengisi");
    lcd.setCursor(2,1);
    lcd.print("Kondisi Mati");
    digitalWrite(relay4, LOW);
    update1();
    delay(2000);
    lcd.clear();  
  }
  if(b >= 30){
    //kipas nyala mendinginkan
    lcd.setCursor(5,0);
    lcd.print("Kondisi");
    lcd.setCursor(1,1);
    lcd.print("Kipas Menyala");
    digitalWrite(relay3, HIGH);
    delay(2000);
    lcd.clear();
  }
  if(b < 30){
    //kipas mati 
    lcd.setCursor(5,0);
    lcd.print("Kondisi");
    lcd.setCursor(5,1);
    lcd.print("Kipas Mati");
    digitalWrite(relay3, LOW);
    delay(2000);
    lcd.clear();
  }
  
  lcd.setCursor(1,0);
  lcd.print("SMART AKUARIUM");
  lcd.setCursor(5,1);
  lcd.print("BIMO MR");
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/tambah-proses.php?lampu&";
  url += "a=";
  url += a;
  url += "&b=";
  url += b;
  url += "&c=";
  url += c;
  url += "&d=";
  url += d;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
//      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial


  String line;
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
   if (line.indexOf("pakan") != -1) {
//      Serial.println("Pakan aktif");
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("Pakan Ikan");
      lcd.setCursor(2,1);
      lcd.print("Sedang Aktif");
      servo.write(30);
      delay(200);
      servo.write(0);
      delay(300);
  }
  if (line.indexOf("aktifl") != -1) {
//      Serial.println("Lampu aktif");
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Kondisi Lampu");
      lcd.setCursor(2,1);
      lcd.print("Sedang Aktif");
      digitalWrite(relay2, HIGH);
      delay(3000);
  }else if (line.indexOf("matil") != -1) {
//      Serial.println("Lampu aktif");
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Kondisi Lampu");
      lcd.setCursor(3,1);
      lcd.print("Sedang mati");
      digitalWrite(relay2, LOW);
      delay(3000);
  }
  if (line.indexOf("hurungp1") != -1) {
//      Serial.println("Lampu aktif");
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Pompa Kuras");
      lcd.setCursor(1,1);
      lcd.print("Kondisi Nyala");
      digitalWrite(relay1, HIGH);
      delay(3000);
  }else if (line.indexOf("modarp1") != -1) {
//      Serial.println("Lampu aktif");
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Pompa Kuras");
      lcd.setCursor(2,1);
      lcd.print("Kondisi Mati");
      digitalWrite(relay1, LOW);
      delay(3000);
  }
  if (line.indexOf("caangp2") != -1) {
//      Serial.println("Lampu aktif");
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Pompa Mengisi");
      lcd.setCursor(1,1);
      lcd.print("Kondisi Nyala");
      digitalWrite(relay4, HIGH);
      delay(3000);
  }else if (line.indexOf("inditp2") != -1) {
//      Serial.println("Lampu aktif");
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Pompa Mengisi");
      lcd.setCursor(2,1);
      lcd.print("Kondisi Mati");
      digitalWrite(relay4, LOW);
      delay(3000);
  }
 }
  
  Serial.println();
  Serial.println("closing connection");
}

void update1(){
  Serial.println("masuk sini update1"); 
  digitalWrite(relay4, LOW);
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/update-proses2.php?pompa2=";
  url += "inditp2";
  digitalWrite(relay4, LOW);
  Serial.print("Requesting URL: ");
  Serial.println(url);
  digitalWrite(relay4, LOW);
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 1000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
//    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
}

void update2(){
  Serial.println("masuk sini update2"); 
  digitalWrite(relay1, LOW);
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/update-proses.php?pompa1=";
  url += "modarp1";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 1000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
//    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
}

void parseCommand(String com) {
  String part1;
  String part2;

  part1 = com.substring(0, com.indexOf(" "));
  part2 = com.substring(com.indexOf(" ")+1);
   
  if(part1.equalsIgnoreCase("ph")){
    a = part2.toFloat();
//    Serial.print("ph ");
//    Serial.println(a,1);
  }
  if(part1.equalsIgnoreCase("suhu")){
    b = part2.toInt();
//    Serial.print("suhu ");
//    Serial.println(b);
  }
  if(part1.equalsIgnoreCase("ntu")){
    c = part2.toFloat();
//    Serial.print("ntu ");
//    Serial.println(c);
  }
  if(part1.equalsIgnoreCase("ultra")){
    d = part2.toInt();
//    Serial.print("ultra ");
//    Serial.println(d);
  } 
}
