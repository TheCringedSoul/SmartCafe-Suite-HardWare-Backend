#include <Arduino.h>
#include <WiFiManager.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

const char *ssid = "Kanishk singhal 2.4 G";
const char *password = "kanishk1";

const String localServerUrl = "http://192.168.1.44:3000/addCard";

#define SS_PIN 5
#define RST_PIN 22
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup()
{
  Serial.begin(115200);

  WiFiManager wm;
  if (!wm.autoConnect("RFID-ESP32-AP"))
  {
    Serial.println("Failed to connect to WiFi");
    return;
  }
  Serial.println("Connected to WiFi!");

  SPI.begin();
  mfrc522.PCD_Init();

  Serial.println("Place your card on the scanner!");
}

void sendToLocalServer(String uid)
{

  HTTPClient http;
  http.begin(localServerUrl);

  http.addHeader("Content-Type", "application/json");

  DynamicJsonDocument doc(1024);
  doc["uid"] = uid;
  String jsonData;
  serializeJson(doc, jsonData);

  int httpCode = http.POST(jsonData);

  if (httpCode > 0)
  {
    Serial.print("POST request sent, response code: ");
    Serial.println(httpCode);
    String payload = http.getString();
    Serial.println("Response: ");
    Serial.println(payload);
  }
  else
  {
    Serial.print("Error sending POST request, error code: ");
    Serial.println(httpCode);
    String errorMessage = http.errorToString(httpCode);
    Serial.println("Error message: ");
    Serial.println(errorMessage);
  }

  http.end();
}

void loop()
{
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    uidString += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    uidString += String(mfrc522.uid.uidByte[i], HEX);
  }

  Serial.print("UID: ");
  Serial.println(uidString);

  sendToLocalServer(uidString);

  delay(1000);
}
