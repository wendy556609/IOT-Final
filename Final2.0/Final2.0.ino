#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Wire.h>
#include <SPI.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "SSD1306Wire.h"

SSD1306Wire  display(0x3c, 21, 22);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

const char* ssid = "Sapido_RB-1602G3_d526bf";
const char* password = "";
//const char* ssid = "OAO";
//const char* password = "123456788";
//const char* ssid = "thomas";
//const char* password = "1qaz2wsx";

String formattedDate;
String dayStamp;
String timeStamp;
String hourStamp;
String minStamp;
int h;
int m;
String a;

String website;

bool ledstate=0;
bool lightstate=1;
int light=0;

WebServer server(80);

int photocellPin = 32; // 光敏電阻 (photocell) 接在 anallog pin 2
int photocellVal = 0; // photocell variable
int minLight = 500;   // 最小光線門檻值

int cbuttonpin=12;
int cbuttonstate=1;
int buttonpin=13;
int buttonstate=1;
int ledpin=4;

bool cbcurbutton=0;
bool curbutton=0;

#define DEMO_DURATION 3000
typedef void (*Demo)(void);

Demo demos[] = {draw,alarm};
int demoLength = (sizeof(demos) / sizeof(Demo));
long timeSinceLastModeSwitch = 0;

int demoMode = 0;
int counter = 1;

// use first channel of 16 channels (started from zero)
#define LEDC_CHANNEL_0     0

// use 13 bit precission for LEDC timer
#define LEDC_TIMER_13_BIT  13

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     5000

// SPEAKER PIN
#define SPEAKER_PIN        17
#define SPEAKER_PIN1       2
#define SPEAKER_PIN2       32

const int NOTE_NONE = NOTE_MAX;

int MIC_PIN = A0;

bool isclose=false;
bool voice=false;

void createSite(){
  website+="<!DOCTYPE html><html lang=\"en\"><head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <title>小夜燈世界</title> <script src=\"https://code.jquery.com/jquery-3.3.1.js\"></script> <script>$(document).ready(function(){$(\"[type=range]\").change(function(){var ledvalue=$(\"#led\").val(); $(\".ledvalue\").text(ledvalue);});}); $(document).ready(function(){$(\"[type=select]\").change(function(){var Hourvalue=$(\"#Hour\").val(); $(\".Hourvalue\").text(Hourvalue);});}); $(document).ready(function(){$(\"[type=select]\").change(function(){var Minvalue=$(\"#Date\").val(); $(\".Minvalue\").text(Minvalue);});}); $(document).ready(function(){$(\"[type=select]\").change(function(){var Monthvalue=$(\"#Month\").val(); $(\".Monthvalue\").text(Monthvalue);});}); $(document).ready(function(){$(\"[type=select]\").change(function(){var Dayvalue=$(\"#Day\").val(); $(\".Dayvalue\").text(Dayvalue);});}); </script><style>.whole1{margin:20px 10px 10px 10px; padding:10px; color:yellow; text-align:center;}.whole2{margin:30px 10px 10px 10px; padding:10px; color:yellow; text-align:center;}.whole3{margin:20px 10px 5px 10px; padding:10px; color:yellow; text-align:center;}body{background-color: black;}.container{display: flex; flex-direction: column; align-items: center;}.btn{padding: 0.3em 0.5em; font-size: 1.5em;}.btn6{padding: 0.3em 0.5em; font-size: 1.5em;}.btn7{padding: 0.3em 0.5em; font-size: 1.5em;}.btn8{padding: 0.1em 0.5em; font-size: 1.5em;}.btn9{padding: 0.1em 0.5em; font-size: 1.5em;}.btn10{padding: 0.1em 0.5em; font-size: 1.5em;}.title3{text-align: center;color:yellow;text-shadow:0px 3px 5px #000;margin-bottom:50px;}.title4{text-align: center;color:yellow;text-shadow:0px 3px 5px #000;margin-bottom:50px;}.title5{text-align: center;color:yellow;text-shadow:0px 3px 5px #000;margin-bottom:50px;}.title1{text-align:center;color:yellow;text-shadow:0px 3px 5px #000; margin-bottom: 40px;}a{display:block;text-decoration:none;background-color:#FFFFFF;radius:3px;width:150px;text-align:center;margin:0 auto;margin-bottom:30px;font-size:2em;box-shadow: 0px 3px 6px #000;}.btn1{color:black;background-color: white}.btn2{color:black;background-color: gray}.title2{text-align:center;color:yellow;text-shadow:0px 3px 5px #000; margin-bottom: 50px;}a{display:block;text-decoration:none;background-color:#FFFFFF;radius:3px;width:150px;text-align:center;margin:0 auto;margin-bottom:30px;font-size:2em;box-shadow: 0px 3px 6px #000;}.btn3{color:white;background-color: red}.btn4{color:white;background-color: rgb(252, 122, 30)}.btn5{color:white;background-color: blue}.topic{font-size: 3em; color: rgb(99, 146, 255);}</style></head> <body> <div class=\"container\"> <h1 class=\"topic\"> 小夜燈</h1> <h1 style=\"color:white;\">Alarm Clock Setting</h1> <form class=\"whole3\"> <lable> <h2>時間:</h2> </lable> </form> <table> <tr> <th> <form action=\"HourDiming\" method=\"get\" id=\"Hour\"> <select type=\"option\" name=\"Hours\" class=\"Hourvalue\"> <option value=\"0\">00</option> <option value=\"1\">01</option> <option value=\"2\">02</option> <option value=\"3\">03</option> <option value=\"4\">04</option> <option value=\"5\">05</option> <option value=\"6\">06</option> <option value=\"7\">07</option> <option value=\"8\">08</option> <option value=\"9\">09</option> <option value=\"10\">10</option> <option value=\"11\">11</option> <option value=\"12\">12</option> <option value=\"13\">13</option> <option value=\"14\">14</option> <option value=\"15\">15</option> <option value=\"16\">16</option> <option value=\"17\">17</option> <option value=\"18\">18</option> <option value=\"19\">19</option> <option value=\"20\">20</option> <option value=\"21\">21</option> <option value=\"22\">22</option> <option value=\"23\">23</option> </select> </form> </th> <th> <lable style=\"color:white;\"> ： </lable> </th> <th> <form action=\"timeDiming\" method=\"get\" id=\"Date\"> <select type=\"option\" name=\"Mins\" class=\"Minvalue\"> <option value=\"0\">00</option> <option value=\"1\">01</option> <option value=\"2\">02</option> <option value=\"3\">03</option> <option value=\"4\">04</option> <option value=\"5\">05</option> <option value1=\"6\">06</option> <option value=\"7\">07</option> <option value=\"8\">08</option> <option value=\"9\">09</option> <option value=\"10\">10</option> <option value=\"11\">11</option> <option value=\"12\">12</option> <option value=\"13\">13</option> <option value=\"14\">14</option> <option value=\"15\">15</option> <option value=\"16\">16</option> <option value=\"17\">17</option> <option value=\"18\">18</option> <option value=\"19\">19</option> <option value=\"20\">20</option> <option value=\"21\">21</option> <option value=\"22\">22</option> <option value=\"23\">23</option> <option value=\"24\">24</option> <option value=\"25\">25</option> <option value=\"26\">26</option> <option value=\"27\">27</option> <option value=\"28\">28</option> <option value=\"29\">29</option> <option value=\"30\">30</option> <option value=\"31\">31</option> <option value=\"32\">32</option> <option value=\"33\">33</option> <option value=\"34\">34</option> <option value=\"35\">35</option> <option value=\"36\">36</option> <option value=\"37\">37</option> <option value=\"38\">38</option> <option value=\"39\">39</option> <option value=\"40\">40</option> <option value=\"41\">41</option> <option value=\"42\">42</option> <option value=\"43\">43</option> <option value=\"44\">44</option> <option value=\"45\">45</option> <option value=\"46\">46</option> <option value=\"47\">47</option> <option value=\"48\">48</option> <option value=\"49\">49</option> <option value=\"50\">50</option> <option value=\"51\">51</option> <option value=\"52\">52</option> <option value=\"53\">53</option> <option value=\"54\">54</option> <option value=\"55\">55</option> <option value=\"56\">56</option> <option value=\"57\">57</option> <option value=\"58\">58</option> <option value=\"59\">59</option> </select> </form> </th> </tr></table> <br><table> <tr> <th> <button type=\"submit\" form=\"Hour\" value=\"HourSubmit\" class=\"btn7\">時</button> </th> <th><button type=\"submit\" form=\"Date\" value=\"timeSubmit\" class=\"btn6\">分</button></th> </tr></table> <br><br><br><br><h1 style=\"color:white;\">LED Setting</h1> <br><h2 style=\"color:yellow;\">亮度=<span class='ledvalue'>128</span></h2> <form action=\"ledDiming\" method=\"get\" id=\"form1\"> <input type=\"range\" name=\"ledval\" id=\"led\" min=\"0\" max=\"255\"> </form> <br><button type=\"submit\" form=\"form1\" value=\"Submit\" class=\"btn\">Submit</button> <br><br><br><h2 class=\"title1\">開關</h2> <a class=\"btn1\" href=\"/H\">ON</a> <a class=\"btn2\" href=\"/L\">OFF</a> <br><br><a class=\"btn\" href=\"/page\">頁面切換</a> <br></div></table> </body> </html>";
}

void handleRoot() {
  server.send(200, "text/html",website); 
  
}
void ledDiming(){
  int led_Diming=server.arg("ledval").toInt(); 
  ledcWrite(0,led_Diming);  
  server.send(200,"text/html",website);
}
void LEDON(){
  digitalWrite(ledpin, HIGH);
  //digitalWrite(led[color], HIGH);
   server.send(200, "text/html",website);
  ledstate=1; 
  ledcAttachPin(ledpin,0);
}
void LEDOFF(){
   server.send(200, "text/html",website);
   digitalWrite(ledpin, LOW); 
  //digitalWrite(led[color], LOW); 
  ledstate=0;
  ledcAttachPin(ledpin,1);
}

void hourset() {
  Serial.println("2");
  h = server.arg("Hours").toInt();
  //ledcWrite(0,led_Diming);
  server.send(200, "text/html", website);
}
void timeset() {
  Serial.println("1");
  m = server.arg("Mins").toInt();
  //ledcWrite(0,led_Diming);
  server.send(200, "text/html", website);
  //   h=server.arg("Hours").toInt();
  //  //ledcWrite(0,led_Diming);
  //  server.send(200,"text/html",website);
  //
}

void Time(){
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  formattedDate = timeClient.getFormattedDate();
  //Serial.println(formattedDate);

  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  //Serial.print("DATE: ");
  //Serial.println(dayStamp);
  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-4);
  //Serial.print("HOUR: ");
  //Serial.println(timeStamp);
  hourStamp = formattedDate.substring(splitT+1, formattedDate.length()-7);
  minStamp  = formattedDate.substring(splitT+4, formattedDate.length()-4);
 // Serial.println(minStamp);
  if(hourStamp==String(h)&&minStamp==String(m)&&isclose==false){
    //Serial.println("123");
    if(isclose==false){
    voice=true;
    }
  }
  if(hourStamp!=String(h)||minStamp!=String(m)){
    voice=false;
  }
  
}
void alarm(){
  display.setFont(ArialMT_Plain_16);
  display.drawString(25, 0 ,"Alarm Clock");
  a="";
  if (h < 10) {
    a += "0";
  }
  a+=String(h);
  a+=":";
  if (m < 10) {
    a += "0";
  }
  a+=String(m);
  display.setFont(ArialMT_Plain_24);
   display.drawString(35, 30 ,a);
}
void draw(){
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0 ,dayStamp);
  display.setFont(ArialMT_Plain_24);
  display.drawString(35, 30 ,timeStamp);
}

void buttonset(){
  if(ledstate==0){
    if(buttonstate==1&&curbutton==0){
      Serial.println("1111");
      digitalWrite(ledpin, HIGH);
    
    //server.send(200, "text/html",website);
    ledstate=1; 
    ledcAttachPin(ledpin,0);
    curbutton=buttonstate;
    //lightstate=1;
    }
  }

   if(ledstate==1){
    if(buttonstate==1&&curbutton==0){
     Serial.println("0000");
   // server.send(200, "text/html",website);
   digitalWrite(ledpin, LOW); 
    ledstate=0;
    ledcAttachPin(ledpin,1); 
    curbutton=buttonstate;
    lightstate=0;
   }
   }

  if(buttonstate==0&&curbutton==1){
    curbutton=buttonstate;
  }
}

void lightdetect(){
  //&& ledstate == 0
  // 光線不足時打開 LED
  //Serial.println(photocellVal); 
  if (photocellVal < minLight&&lightstate==1) {
  if(lightstate==1&&ledstate==0){
  
    Serial.println(photocellVal); 
    //Serial.println("2222"); 
      digitalWrite(ledpin, HIGH); // turn on LED    
      ledstate = 1;
      ledcAttachPin(ledpin,0);
      lightstate = 0;
   }   
    
    //curbutton=0;
  }
  
//  // 光線充足時關掉 LED
  if (photocellVal > minLight ) {
    //Serial.println(photocellVal);
    lightstate = 1;
    //curbutton=0;
  }  
}

void cbuttonset(){
    if(cbuttonstate==1&&cbcurbutton==0){
    isclose=true;
    voice=false;
    
    }

  if(cbuttonstate==0&&cbcurbutton==1){
    cbcurbutton=cbuttonstate;
  }
}

void PlayMelody0(){
  //for (int thisNote = 0; thisNote < 204; thisNote++) {

if(voice==true){
  //Serial.println("bb");
  ledcAttachPin(SPEAKER_PIN, 0);
  ledcWriteTone(SPEAKER_PIN,35);
  Serial.print("lo");
  //delay(3000);
}
if(voice==false){
  //Serial.println("cc");
  ledcAttachPin(SPEAKER_PIN, 1);
  ledcWriteTone(SPEAKER_PIN,0);
}      
  break1:;
}

void changepage(){
   server.send(200, "text/html",website);
   demoMode++;
   if(demoMode>=3){
    demoMode=0;
   }
}

void handleNotFound() {
  //Serial.print("12345");
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
   pinMode(cbuttonpin,INPUT);
  digitalWrite(cbuttonpin,HIGH);
  pinMode(buttonpin,INPUT);
  digitalWrite(buttonpin,HIGH);
  pinMode(ledpin,OUTPUT);
  ledcSetup(0,5000,8);
  ledcWrite(0,128); 
  //ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT); 
  pinMode(MIC_PIN, INPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  createSite();
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  timeClient.begin();
  timeClient.setTimeOffset(28800);

  server.on("/", handleRoot);
  server.on("/page", changepage);
  server.on("/timeDiming", timeset);
  server.on("/HourDiming", hourset);
  server.on("/H", LEDON);
  server.on("/L", LEDOFF);
  server.on("/ledDiming",ledDiming);
  server.on("/inline", []() {
    server.send(200, "text/html", website);
  });

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  display.init();//初始化(init)
  display.setContrast(255); //數值介於0~255，調整對比
   display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.clear();//清除螢幕和緩衝區(clear)
  display.display();
  h=0;
  m=0;
   
}

void loop(void) {
  server.handleClient();
  cbuttonstate=digitalRead(cbuttonpin);
  buttonstate=digitalRead(buttonpin);
  photocellVal = analogRead(photocellPin);
  Time();  
  display.clear();
  demos[demoMode]();
  display.display();
  buttonset();
  cbuttonset();
  lightdetect();
  PlayMelody0();
}
