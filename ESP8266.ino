/*
作者：我系xun悟空
QQ交流群：85387868
博客：www.armfun.cn
*/
#include <ArduinoJson.h>
const int ledPin = 0;

int ledSta = 0;
int time1s = 0;
void setLedSta(int sta)
{
  ledSta =sta;
  if(ledSta)
    digitalWrite(ledPin,LOW);
  else
    digitalWrite(ledPin,HIGH);
}
int getLedSta()
{
  return ledSta;
}
void uploadtele()
{
  StaticJsonDocument<200> doc;
  if(ledSta==0)
    doc["ledSta"] = false;
  else
    doc["ledSta"] = true;
  // 将 JSON 对象转换为字符串
  String jsonString;
  serializeJson(doc, jsonString);
  telePublish(jsonString.c_str());
}
// the setup function runs once when you press reset or power the board
void timeTask10ms()
{
  time1s+=10;
  wifi_loop();
  if(time1s>=1000)
  {
    time1s = 0;
    Serial.println("upload tele data");
    uploadtele();
  }
}
void setup() 
{
  Serial.begin(115200);
  Serial.println("start");
  pinMode(ledPin,OUTPUT);
  setLedSta(0);
  wifi_setup();
}
void loop() 
{
  delay(10);
  timeTask10ms();
}