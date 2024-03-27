// main.cpp

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPL4-nokWJrm"
#define BLYNK_TEMPLATE_NAME         "ESP32"
#define BLYNK_AUTH_TOKEN            "4o1rwIekteF8LLCa_cQUM9_S9IfJ6uDw"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

// Slots
#define drm01Red 12
#define drm01Green 13
#define slot2Red 19
#define slot2Green 18
#define slot3Red 17
#define slot3Green 16

// Libraries
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BluetoothSerial.h>
#include <Slot.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


HTTPClient http;
const char* serverUrl = "http://192.168.172.253:5000/";

const char* user1ID = "U001";
const char* user2ID = "U002";
const char* user3ID = "U003";
const char* user6ID = "U006";


// Your WiFi credentials.
// Set password to "" for open networks.

// Home
//char ssid[] = "Toffe-2G";
//char pass[] = "JoffeTaattuss";

// ESP8266 AP
//char ssid[] = "VEPS-AP";
//char pass[] = "123456789";

// Pixel Hotspot
//char ssid[] = "Joffe";
//char pass[] = "john1234";

// Samsung Hotspot
char ssid[] = "Ilir";
char pass[] = "ilirilir";

//LCD
int lcdColumns = 16;
int lcdRows = 2;
bool lastLcdVal;
char c;

BluetoothSerial SerialBT;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
Slot drm01("Drammen", "D001", "Available", drm01Green, drm01Red);



void fetchAllLocationData(){
  HTTPClient http;
  http.begin(String(serverUrl) + "locations");
  int httpCode = http.GET();

  if(httpCode > 0){
    String payload = http.getString();
    Serial.print("The /locations returns the following HTTP code: ");
    Serial.println(httpCode);
    Serial.print("And the following payload: ");
    Serial.println(payload);

    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, payload);
    

    if(error){
      Serial.print(F("deserializeJson() failed:"));
      Serial.print(error.f_str());
      return;
    }

    JsonArray locations = doc.as<JsonArray>();
    for(JsonObject location : locations){
      String unitID = location[" Unit"].as<String>();
      unitID.trim();
      String status = location[" Status"].as<String>();
      status.trim();

      if(unitID.equalsIgnoreCase(drm01.getSlotID())){
        Serial.print("Found unit: ");
        Serial.print(unitID);
        Serial.print(" with status: ");
        Serial.println(status);

        if(!status.equalsIgnoreCase(drm01.getStatus())){
          Serial.println("Status mismatch, updating...");
          drm01.updateStatus(status);
        }
        break;
      }
    }
  } else{
    Serial.println("Error on HTTP request when fetching location data");
  }
  http.end();
}

void fetchSpecificLocationData(Slot slot){
  HTTPClient http;
  http.begin(String(serverUrl)+"locations/"+String(slot.getSlotID()));
  int httpCode = http.GET();

  if(httpCode > 0){
    String payload = http.getString();
    Serial.print("The locations/" + String(slot.getSlotID()) + " returns the following HTTP code: ");
    Serial.println(httpCode);
    Serial.print("And the following payload: ");
    Serial.println(payload);
  } else{
    Serial.print("Error on HTTP request when fetching specific location data - exiting with code: ");
    Serial.println(httpCode);
  }
  http.end();
}

void fetchAllUserData(){
  HTTPClient http;
  http.begin(String(serverUrl) + "users");
  int httpCode = http.GET();

  if(httpCode > 0){
    String payload = http.getString();
    Serial.print("The /getusers returns the following HTTP code: ");
    Serial.println(httpCode);
    Serial.print("And the following payload: ");
    Serial.println(payload);
  }else{
    Serial.print("Error on HTTP request when fetching all user data - exiting with code: ");
    Serial.println(httpCode);
  }
  http.end();
}

void fetchSpecificUserData(const char* userID){
  HTTPClient http;
  http.begin(String(serverUrl)+"users/"+String(userID));
  int httpCode = http.GET();

  if(httpCode > 0){
    String payload = http.getString();
    Serial.print("The /users/" + String(userID) + " returns the following HTTP code: ");
    Serial.println(httpCode);
    Serial.print("And the following payload: ");
    Serial.println(payload);
  } else{
        Serial.print("Error on HTTP request when fetching specific user data - exiting with code: ");
        Serial.println(httpCode);
  }
  http.end();
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  SerialBT.begin("ESP32");
  // HTTP
  http.begin(serverUrl);  
  // Unit LEDs
  pinMode(drm01Red, OUTPUT);
  pinMode(drm01Green, OUTPUT);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  WiFi.begin(ssid, pass);
  //lcd.init();
  //lcd.backlight();
  WiFi.printDiag(Serial);
  if(drm01.getStatus() == "Available"){
    //lcd.printstr("Unit D01 is available!");
    digitalWrite(drm01Green, HIGH);
    digitalWrite(drm01Red, LOW);
  }else if (drm01.getStatus() == "Unavailable"){
    //lcd.print("Unit D01 is unavailable!");
    digitalWrite(drm01Red, HIGH);
    digitalWrite(drm01Green, LOW);
  }
  drm01.Print();

  Serial.print("User 1 have the ID: ");
  Serial.println(user1ID);
    Serial.print("User 2 have the ID: ");
  Serial.println(user2ID);
    Serial.print("User 3 have the ID: ");
  Serial.println(user3ID);
    Serial.print("User 6 have the ID: ");
  Serial.println(user6ID);
}

void loop()
{
  if(WiFi.status() == WL_CONNECTED){
    if(!http.connected()){
      http.begin(serverUrl);
    }

    int httpCode = http.GET();
    if(httpCode > 0){
      String payload = http.getString();
      Serial.print("The index.html returns the following HTTP code: ");
      Serial.println(httpCode);
      Serial.print("And the following payload: ");
      Serial.println(payload);
    }
    else{
      Serial.println("Error on HTTP request");
      http.end();
    }
  }else{
    Serial.println("WiFi not connected");
  }

  //fetchUserData();
  //fetchAllLocationData();

  fetchSpecificUserData(user1ID);
  delay(2500);
  fetchSpecificUserData(user2ID);
  delay(2500);
  fetchSpecificUserData(user3ID);
  delay(2500);
  fetchSpecificUserData(user6ID);
  delay(2500);
  fetchAllUserData();
  delay(2500);

  /*Bluetooth part 
  if(SerialBT.available()){
    char c = SerialBT.read();
    if(c == '1' && lastLcdVal != HIGH){
      lcd.clear();
      lcd.print("Slot one is occupied!");
      digitalWrite(slot1Red, HIGH);
      digitalWrite(slot1Green, LOW);
      lastLcdVal = HIGH;
    }
    else if(c == '0' && lastLcdVal != LOW){
      lcd.clear();
      lcd.print("Slot one is available!");
      digitalWrite(slot1Red, LOW);
      digitalWrite(slot1Green, HIGH);
      lastLcdVal = LOW;
    }
  }
  */
}
