/*
作者：我系xun悟空
QQ交流群：85387868
博客：www.armfun.cn
*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
 
// 设置wifi接入信息(请根据您的WiFi信息进行修改)
const char* ssid = "youwifiname";
const char* password = "youwifipw";

const char* mqttServer = "121.89.200.200";
// MQTT服务端连接用户名密码
const char* mqttUserName = "WA8HRKxqVKeNd2FHliuxu";
const char* mqttPassword = NULL;
 
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void wifi_setup() {
  //设置ESP8266工作模式为无线终端模式
  WiFi.mode(WIFI_STA);
  // 连接WiFi
  connectWifi();
  // 设置MQTT服务器和端口号
   mqttClient.setServer(mqttServer, 11883);
  // 连接MQTT服务器
  connectMQTTServer();
   mqttClient.setCallback(callback);        // 设置回调，控制led灯
}

void wifi_loop() { 
  if (mqttClient.connected()) { // 如果开发板成功连接服务器    
    mqttClient.loop();          // 保持客户端心跳
  } else {                  // 如果开发板未能成功连接服务器
    connectMQTTServer();    // 则尝试连接服务器
  }
}

void connectMQTTServer(){
  // 根据ESP8266的MAC地址生成客户端ID（避免与其它ESP8266的客户端ID重名）
  String clientId = "esp8266-" + WiFi.macAddress();
  // 连接MQTT服务器
  if (mqttClient.connect(clientId.c_str(), mqttUserName, mqttPassword)) { 
    //连接成功后就订阅主题
    mqttClient.subscribe("v1/devices/me/rpc/request/+");//订阅主题
    Serial.print("订阅主题成功！！");
  } else {
    Serial.println(mqttClient.state());
    delay(3000);
  }   
}
// ESP8266连接wifi
void connectWifi(){
  WiFi.begin(ssid, password);
  //等待WiFi连接,成功连接后输出成功信息
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected!");  
  Serial.println(""); 
}
 //回调函数
void callback(char* topic, byte* payload, unsigned int length) {
   Serial.println("begin callback");
   char s[length];
   for (int i=0;i<length;i++) {
    s[i]=(char)payload[i];
   }
    std::string str;
    str=s;
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, str);
    JsonObject obj = doc.as<JsonObject>();
   
    const char* methodName;
    bool value;
   
    methodName=obj["method"];
    if(strcmp(methodName,"getValue")==0)
    {
      int id ;
      sscanf(topic,"v1/devices/me/rpc/request/%d",&id);
      Serial.println("req id:"+String(id));
      prcResponse(id,String(getLedSta()).c_str());
    }
    else if(strcmp(methodName,"setValue")==0)
    {
      value=obj["params"];
      Serial.println(methodName);
      Serial.println(value);
      setLedSta(value);
      if(value)
        sendWakeUp();
    }
    
}
void telePublish(const char * msg)
{
  mqttClient.publish("v1/devices/me/telemetry",msg);
}
void attrPublish(const char * msg)
{
  mqttClient.publish("v1/devices/me/attributes",msg);
}
void prcResponse(int id,const char *msg)
{
  mqttClient.publish(("v1/devices/me/rpc/response/"+ String(id)).c_str(),msg); 
}