
#include "HX711.h"
#include "Servo.h"

Servo dao; //Pin D1 GPIO 12
Servo can; //Pin D5 GPIO 14
Servo van; //Pin D0 GPIO 04
//Vi tri hien tai cua cac servo
 
//-----------------------------------------
//HX711 Loadcell 
const int LoadCellDT = 12;
const int LoadCellSCK = 13;
long times =0;
//-----------------------------------------
//Weight necessary
const int WeightBefore = 0;
const int WeightAfter = 0;
const int WeightNecessary = 0;

int calibration_factor = -375;
int KLcan = 10;
int KLBeforeBandau;
int KLBeforeThucte; 

int KLAfterBandau;
int KLAfterThucte;

HX711 scale;
  int i =0,j=0;
  String thongbao =" ";
  float SET = 1.0;   //Trọng lượng cài đặt ban đầu
void ServoInit() //Khoi tao ket noi servo va chuyen cac servo ve vi tri ban dau
{
  dao.attach(5);   /////chân các servo
  can.attach(14);
  van.attach(4);
  delay(300);
  AllServoStop();
  //Servo tro lai vi tri 0 do
/*  for (posKnife > 0; posKnife != 0; posKnife--) 
        { 
            dao.write(posKnife);              
            delay(15);
            if (posKnife == 0)
            {
              break;
            }  
        }
  for (posLoadCell > 0; posLoadCell != 0; posLoadCell--) 
        { 
            can.write(posLoadCell);              
            delay(15);
            if (posLoadCell == 0)
            {
              break;
            }   
        }    
  for (posValve > 0; posValve != 0; posValve--) 
        { 
            van.write(posValve);              
            delay(15); 
            if (posValve == 0)
            {
              break;
            } 
        }*/
}
//-----------------------------------------
void LoadCellInit() //Khoi tao cam bien khoi luong
{
  scale.begin(LoadCellDT, LoadCellSCK);
  scale.set_scale(calibration_factor);
  scale.tare();
}
/*
//-----------------------Code dieu khien----------------------------------------
void ServoKnifeControl()
{
for (posKnife = 0; posKnife <= 180; posKnife++) 
        { 
            dao.write(posKnife);              
            delay(15); 
        }
        delay(1000);
for (posKnife = 180; posKnife >= 0; posKnife--)
        { 
            dao.write(posKnife);              
            delay(15); 
        }
        delay(1000);
}
void ServoLoadCellControl()
{
for (posLoadCell = 0; posLoadCell <= 90; posLoadCell++) 
        { 
            can.write(posLoadCell);              
            delay(15); 
        }
        delay(1000);
for (posLoadCell = 90; posLoadCell >= 0; posLoadCell--)
        { 
            can.write(posLoadCell);              
            delay(15); 
        }
        delay(1000);
}
void ServoValveControl()
{
for (posValve = 0; posValve <= 180; posValve++) 
        { 
            van.write(posValve);              
            delay(15); 
        }
        delay(1000);
for (posValve = 180; posValve >= 0; posValve--)
        { 
            van.write(posValve);              
            delay(15); 
        }
        delay(1000);
}*/
void AllServoStop()
{
  dao.write(0); 
  can.write(180);
  van.write(180);
  delay(250);
}
 
void ReadWeightBefore()
{
  Serial.print("Reading Before:");
  KLBeforeBandau = scale.get_units(),10; 
    if (KLBeforeBandau < 0)
      {
        KLBeforeBandau = 0.00;
      }
  KLBeforeThucte = KLBeforeBandau - KLcan;
  if (KLBeforeThucte < 0)
      {
        KLBeforeThucte = 0.00;
      }
  Serial.println(KLBeforeThucte);
}
//-----------------------------------------
/*
void ReadWeightAfter() 
{
  Serial.print("Reading After:");
  KLAfterBandau = scale.get_units(),10; 
    if (KLAfterBandau < 0)
      {
        KLAfterBandau = 0.00;
      }
  Serial.print(KLAfterBandau + "   ");
  KLAfterThucte = KLAfterBandau - KLcan;
  if (KLAfterThucte < 0)
      {
        KLAfterThucte = 0.00;
      }
  Serial.println(KLAfterThucte);
  delay(1000);
}*/
//--------------------------------------------------------------------
#include <ESP8266WiFi.h>

const char* ssid = "Thu Thao";
const char* password = "917664ab";
WiFiServer server(80);
void setup() 
{
  Serial.begin(9600);
 Serial.print("Connecting to the Newtork");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected"); 
  server.begin();
  Serial.println("Server started");
  Serial.print("IP Address of network: "); // will IP address on Serial Monitor
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: https://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  LoadCellInit();
  ServoInit();
  
}
//----------------------------------------------------------
void loop() 
{  
   WiFiClient client = server.available();
 /* if (!client){
    return;}
    
  Serial.println("Waiting for new client");
while(!client.available())
  {
    delay(1);
  }*/
  
  String request = client.readStringUntil('\r');
  Serial.println(request);
  
  if(request.indexOf("/BAT") != -1){
   j=1;
  }

  if(request.indexOf("/TAT") != -1){
j=0;
  } 
  
  if(j==1){
    thongbao = " GIA TRI TRONG LUONG CAI DAT: ";
   ReadWeightBefore();
   if(KLBeforeThucte < SET && i ==0){
  van.write(0);///van mở   0 độ
  Serial.println("VAN MO");
   }
     if(KLBeforeThucte >= SET){ 
      i = 1;}
      if(i==1){
      van.write(180);///servo van đóng 180 độ
      Serial.println("VAN DONG");
  delay(500);
      can.write(0);//servo Can hoạt động 0 độ
      Serial.println("SERVO CAN HOAT DONG");
  delay(500);
      delay(100);
      i=2;
      times = millis();
     }
 if((millis() - times)/1000 >= 10 && i ==2){ //sau khoảng 120s
  if(KLBeforeThucte < 0.1){//bé hơn 1 hoặc gần bằng 0
  dao.write(180);///dao cắt hoạt động
  Serial.println("DAO CAT HOAT DONG");
  delay(5000);
  Serial.println("XONG");
  dao.write(0);//dao cắt dừng
  can.write(180);//servo Can dung hoạt động 180 do
  van.write(0);
  i=0;
   }
 }
if(request.indexOf("/TANG") != -1){
   SET = SET + 0.1;
   delay(200);
  }

  if(request.indexOf("/GIAM") != -1){
SET = SET - 0.1;
   delay(200);
  } 
  }
  if(j==0){ //////Dừng hệ thống
   thongbao = " HE THONG DANG TAM DUNG ";
  AllServoStop();  
  }
 
  if (!client){
    return;}
  client.flush();
   
  
//*------------------HTML Page Code---------------------*//

  client.println("HTTP/1.1 200 OK"); //
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print(thongbao);
  if(j==1){
    client.print(SET,1);
    client.print(" KG");}
    if(j==0){
      client.print(" ");
    }
    
  client.println("<br><br>");
  client.println("<a href=\"/GIAM\"\"><button>GIAM</button></a>");
  client.println("<a href=\"/TANG\"\"><button>TANG</button></a><br />");
  client.println("<a href=\"/BAT\"\"><button>BAT</button></a>");
  client.println("<a href=\"/TAT\"\"><button>TAT</button></a><br />");
  client.println("</html>");
   
        } 
/*
  if (request.indexOf("/TB=OFF") != -1) 
        {
            Serial.println ("Power off");
            server.stop();
            AllServoStop();
            scale.power_down();
        }      
}*/
