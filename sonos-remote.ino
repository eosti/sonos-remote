/*
 * sonos-remote
 * 
 * Configures an ESP8266 such that when it is triggered, 
 * it will influence the local Sonos system. 
 * 
 * Requires a local server running a Sonos HTTP API
 * See: https://github.com/jishi/node-sonos-http-api
 * 
 * When the script is run (on reset), it will check the status of a speaker and: 
 *  If it is paused/stopped, start it playing a specific media
 *  If it is playing a specific media, pause it
 *  If it is playing something else, play the specific media
 * 
 * Configure WiFi, ESP8266, and media settings in config.h
 * 
 * Created sometime 2017
 * By Reid Sox-Harris (@eosti)
 * Modified 06 September 2020
 * By Reid Sox-Harris (@eosti)
 * 
 * https://github.com/eosti/sonos-remote
 * 
 */

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#include "config.h"

WiFiClient client;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  // Turn on LED to signify starting
  digitalWrite(LED_PIN, HIGH);
  
  Serial.begin(9600);
  delay(10);

  // Connect to WiFi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));   // Flash the led during connection
  }

  // Connected to WiFi, keep the LED on now
  digitalWrite(LED_PIN, HIGH);
  Serial.println("");
  Serial.println("WiFi connected");

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(HOST, HTTP_PORT)) {
    Serial.println("Connection to host failed");
    ledFailure();
    return;
  }

  // GET the kitchen state
  Serial.println("Obtaining SONOS status");
  client.print(String("GET ") + STATE_URL + " HTTP/1.1\r\n" +
               "Host: " + HOST + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      ledFailure();
      return;
    }
  }

  // Read the response
  String rawState;
  while (client.available()) {
    rawState = client.readStringUntil('\r');
  }
  client.stop();

  // Decode the JSON to figure out what we want the radio to do
  const int capacity = 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 2*JSON_OBJECT_SIZE(10) + 880;
  DynamicJsonDocument doc(capacity);
  DeserializationError err = deserializeJson(doc, rawState);

  if (err) {
    Serial.println("JSON parsing error!");
    Serial.println(err.c_str());
    ledFailure();
  }

  // Configured to play TuneIn radio
  // If the server isn't connected to SONOS, it will likely crash here.
  const char* stationName = doc["currentTrack"]["stationName"];
  const char* playbackState = doc["playbackState"];

  Serial.print("Station name: ");
  Serial.println(stationName);
  Serial.print("Playback status: ");
  Serial.println(playbackState);

  if (!client.connect(HOST, HTTP_PORT)) {
    Serial.println("Connection to host failed");
    ledFailure();
    return;
  }

  if(!strcmp(stationName, STATION_NAME) && !strcmp(playbackState, "PLAYING")) {
    // If it's already playing media, treat as a pause button
    Serial.println("Pausing media");
    client.print(String("GET ") + PAUSE_URL + " HTTP/1.1\r\n" +
               "Host: " + HOST + "\r\n" +
               "Connection: close\r\n\r\n");
    
  } else {
    // If it's doing anything else, play media
    Serial.println("Playing media");
    client.print(String("GET ") + PLAY_URL + " HTTP/1.1\r\n" +
               "Host: " + HOST + "\r\n" +
               "Connection: close\r\n\r\n");
  }

  // Make sure client is still available
  timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      ledFailure();
      return;
    }
  }

  // Read server response
  String response;
  while (client.available()) {
    response = client.readStringUntil('\r');
  }
  client.stop();

  // Make sure we get a success
  if(!(response.indexOf('{"status":"success"}')) > 0) {
    Serial.println("An error occured.");
    Serial.println(response);
    ledFailure();
  } else {
    Serial.println("Action completed successfully.");
  }

  // Go to sleep indefinetely, to be woken up by a reset
  Serial.println("Entering sleep mode. Goodnight!");
  digitalWrite(LED_PIN, LOW);
  ESP.deepSleep(0); 
  // Goodnight!
}

void loop() {

}

void ledFailure() {
  for(int i = 0; i<=5; i++) {     // Fast LED flashing to differentiate it from
    digitalWrite(LED_PIN, LOW);   //  the 'WiFi' connecting flashing.
    delay(100);
    digitalWrite(LED_PIN, HIGH);
    delay(100);
  }
}
