#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

const char* ssid = "Router";       
const char* password = "123456789";  

#define BOT_TOKEN "7971343934:AAHiH51NyM627izdfGD1F6TMdFgnhji3FQ0"
#define CHAT_ID "1049560108"

const int deliverySensorPin = 4;  // GPIO pin connected to sensor/button
bool parcelDelivered = false;

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

void setup() {
  Serial.begin(115200);

  pinMode(deliverySensorPin, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  client.setInsecure(); // Allow insecure connection (no cert check)
}

void loop() {
  int sensorState = digitalRead(deliverySensorPin);

  if (sensorState == LOW && !parcelDelivered) {
    parcelDelivered = true;
    Serial.println("Parcel Delivered! Sending Telegram message...");

    String message = "📦 Parcel has been delivered!";
    bool success = bot.sendMessage(CHAT_ID, message, "Markdown");
    if(success) {
      Serial.println("Message sent successfully!");
    } else {
      Serial.println("Failed to send message!");
    }
  } 
  else if (sensorState == HIGH && parcelDelivered) {
    parcelDelivered = false;
    Serial.println("Parcel status reset.");
  }

  delay(200);
}