

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.


/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>


// Setup a feed called 'onoff' for subscribing to changes.
//Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Value");
#include "Connector.h"
#include "esp_utils.h"
#define WLAN_SSID       "WIFI名稱"
#define WLAN_PASS       "WIFI密碼"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "Adafruit使用者帳號"
#define AIO_KEY         "Adafruit使用者授權金鑰"

//建立一個跟adafruit溝通的物件
Connector* mqtt;

int LED_PIN = 2;

//新的訂閱訊息進來後該怎麼處理的副程式
void rcvValue(uint32_t current);
void rcvTime(uint32_t current);

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(LED_PIN, OUTPUT);
  
  //初始化與adafruit溝通的程式並且提供相關必須的資料
  mqtt = new Connector(WLAN_SSID, WLAN_PASS, AIO_USERNAME, AIO_KEY);

  //Subscribe(訂閱) : 跟adafruit程式說有以下的兩則訊息的通知請傳到ESP8266上面來
	//mqtt->Subscribe("訂閱的資訊名稱", 有更新訊息時處理的副程式)
  mqtt->Subscribe("BigDaddy2062424/feeds/Toggle", rcvValue);  //訂閱網路介面上的按鈕
  mqtt->Subscribe("time/seconds", rcvTime);  //訂閱Adafruit提供的時間
}



void loop() {
  //建立連線，倘若斷線則會重新建立連線	
  mqtt->Connect();

  //接收全部訂閱中有更新的資料內容
  mqtt->RecieveData(1000);
  
  //傳送一個整數資料到Adafruit.io雲端
  if (!mqtt->Publish("BigDaddy2062424/feeds/Value", 101)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
  
  //使用傳送後得使用publish_Delay來使得傳送頻率不會超過伺服器提供的上限
  publish_Delay(2000);
}

//接收線上介面傳來的0或是1,並且用此來開關ESP8266上面的LED燈
void rcvValue(uint32_t current){
  if(current == 0){
    digitalWrite(LED_PIN, HIGH);
  }
  else{
    digitalWrite(LED_PIN, LOW);
  }
}

//接收adafruit提供的時間並且轉換成易懂的訊息
void rcvTime(uint32_t current) {
  int timeZone = 8; //時區, +8 in Taiwan
  int hour = 0; 
  int minute = 0;
  int sec = 0;
  
  // adjust to local time zone
  current += (timeZone * 60 * 60);

  // calculate current time
  sec = current % 60;
  minute = current / 60 % 60;
  hour = (current / 60 / 60) % 24;
  String s_h = String(hour);
  String s_m = (minute<10)?"0" + String(minute): String(minute);
  String s_s = (sec<10)?"0" + String(sec):String(sec);
  
  Serial.println("Current Time : " + s_h + ":" + s_m + ":" + s_s);
}
