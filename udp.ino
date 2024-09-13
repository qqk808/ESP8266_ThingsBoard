/*
作者：我系xun悟空
QQ交流群：85387868
博客：www.armfun.cn
*/
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
byte mac1[] = {0x74,0x56,0x3c,0x38,0xf2,0xfe};//唤醒目标电脑的mac为40 B8 76 A8 38 A6 ,替换为自己的电脑mac地址
byte preamble[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
IPAddress ip(255, 255, 255, 255); //幻数据包需要在局域网中广播发送 要控制的电脑的网段的255地址
//int pingResult;
WiFiUDP UDP; //建立一个WiFiUDP对象 UDP

void sendWakeUp()
{
  UDP.beginPacket(ip, 7); //UDP发送到目标（IP，端口）
  UDP.write(preamble, sizeof preamble); //写入包头(FF,FF,FF,FF,FF,FF)
  for (byte i = 0; i < 16; i++)
  {
    UDP.write(mac1, sizeof mac1);
  }
  Serial.println("唤醒主机");
  UDP.endPacket();
}