#include <Arduino.h>
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

//1. Change the following info
//1. Change the following info
#define WIFI_SSID "padep"
#define WIFI_PASSWORD "#ltc1qaz2wsx"
#define FIREBASE_HOST "smart-office-b5df8.firebaseio.com"
#define FIREBASE_AUTH "sVehAQzPkr8kgNnPfbBG08wZDENt0tufSIOlyOUR"

// Define I/O prot
#define SW 1
#define RL 4

// initialize essential variable
int state = LOW;
int reading = 0;
int previous = LOW;

//2. Define FirebaseESP8266 data object for data sending and receiving
FirebaseData fbo;

void setToFB(int data);
int getFromFB();

void setup()
{

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  //3. Set your Firebase info

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  //4. Enable auto reconnect the WiFi when connection lost
  Firebase.reconnectWiFi(true);

  // Set your I/O
  pinMode(SW, INPUT);
  pinMode(RL, OUTPUT);

  // set door state to active by defult
  setToFB(0);

  /*
  In case where you want to set other data types i.e. bool, float, double and String, you can use setBool, setFloat, setDouble and setString.
  If you want to get data which you known its type at specific node, use getInt, getBool, getFloat, getDouble, getString.
  If you don't know the data type at specific node, use get and check its type.
  The following shows how to get the variant data
  */

  if (Firebase.get(fbo, "/smart-doors/door-01"))
  {
    //Success
    Serial.print("Get variant data success, type = ");
    Serial.println(fbo.dataType());

    if (fbo.dataType() == "int")
    {
      Serial.print("data = ");
      Serial.println(fbo.intData());
    }
    else if (fbo.dataType() == "bool")
    {
      if (fbo.boolData())
        Serial.println("data = true");
      else
        Serial.println("data = false");
    }
    else if (fbo.dataType() == "json")
    {
      Serial.print("data = ");
      // Serial.println(fbo.jsonObject());
    }
  }
  else
  {
    //Failed?, get the error reason from fbo

    Serial.print("Error in get, ");
    Serial.println(fbo.errorReason());
  }

  /*
  If you want to get the data in realtime instead of using get, see the stream examples.
  If you want to work with JSON, see the FirebaseJson, jsonObject and jsonArray examples.
  If you have questions or found the bugs, feel free to open the issue here https://github.com/mobizt/Firebase-ESP8266
  */
}

void setToFB(int data)
{
  //5. Try to set int data to Firebase
  //The set function returns bool for the status of operation
  //fbo requires for sending the data
  if (Firebase.setInt(fbo, "/smart-doors/door-01", data))
  {
    //Success
    // Serial.println("Set int data success");
  }
  else
  {
    //Failed?, get the error reason from fbo

    Serial.print("Error in setInt, ");
    Serial.println(fbo.errorReason());
  }
}

int getFromFB()
{
  // int getData = Firebase.getInt(fbo, "/smart-doors/door-01");
  //6. Try to get int data from Firebase
  //The get function returns bool for the status of operation
  //fbo requires for receiving the data

  int getData;

  if (Firebase.getInt(fbo, "/smart-doors/door-01"))
  {
    //Success
    // Serial.print("Get int data success, int = ");
    // Serial.println(fbo.intData());
    getData = fbo.intData();
  }
  else
  {
    //Failed?, get the error reason from fbo
    Serial.print("Error in getInt, ");
    Serial.println(fbo.errorReason());
  }

  return getData;
}

void loop()
{
  if (getFromFB() == 1)
  {
    digitalWrite(RL, LOW);
    delay(3000);
    setToFB(0);
    digitalWrite(RL, HIGH);
  }
}