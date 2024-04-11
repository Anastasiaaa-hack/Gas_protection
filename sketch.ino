#include <ESP8266WiFi.h> 
#include <TelegramBot.h>
#include <WiFiClientSecure.h> 

const int button1 = 3; //пин для кнопки на конце болта
const int button2 = 4; //пин для кнопки на окне
int cnt = 0; //счетчик для отправки сообщений
#define smokeSensor (0) //пин для датчика газа
//пины для мотора:
byte ena = 5;
byte in1 = 6;
byte in2 = 7;

int sensorValue; //значение датчика
int buttonState1; //значение кнопки 1
int buttonState2; //значение кнопки 2

// данные для подключения к сети Wifi  
char ssid[] = "Keenetic-9667"; //название сети  
char pass[] = "aLTY2wkf"; //пароль 
// инициализация Telegram BOT 
const char* BOTtoken = ""; //токен бота  
WiFiClientSecure client;  
TelegramBot bot(BOTtoken,client); 

#define CHAT_ID = "-518163870"

void setup() {
  pinMode(button1, OUTPUT); 
  pinMode(button2, OUTPUT); 
  pinMode(ena, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  Serial.begin(115200);
  Serial.print("Connecting Wifi: ");   
  Serial.println(ssid);   
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {   
    Serial.print(".");   
    delay(500);   
  }   
  Serial.println("");   
  Serial.println("WiFi connected");   
  bot.begin();
}

void loop() {
  message m = bot.getUpdates();
  sensorValue = analogRead(smokeSensor); //считываем данные с датчика газа
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  analogWrite(ena, 255);
  if (sensorValue > 300) {
    cnt = cnt + 1;
    if (cnt == 1) {
      bot.sendMessage(m.chat_id, "Внимание! Обнаружена утечка газа!");
    }
    buttonState1 = digitalRead(button1); //пока кнопка на конце болта не нажата крутим мотор
    if (!buttonState1){
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      delay (50);
    }
    digitalWrite(in1, LOW);
  }
  else {
    cnt = 0;
  }
  m = bot.getUpdates();
  if (m.text.equals("close")){
    buttonState2 = digitalRead(button2); //пока кнопка на окне не нажата крутим мотор
    while (!buttonState2){
      analogWrite(ena, 150);
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      delay (50);
      buttonState2 = digitalRead(button2);
    }
    digitalWrite(in2, LOW);
  }
}
