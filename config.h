/*
 * config.h for sonos-remote.ino
 *
 * Set WiFi details, server information, ESP8266 configuratiom, and media information here
 *
 * Created 06 September 2020
 * By Reid Sox-Harris (@eosti)
 *
 */

// Set WiFi credentials
#define SSID            "WIFI_SSID"
#define PASSWORD        "WIFI_PSK"

// Server that the http-sonos-api is running on
#define HOST            "SERVER_IP"
#define HTTP_PORT       5005        // Default

/* 
 * Set media information (likely just PLAY_URL and STATION_NAME)
 *
 * To find the PLAY_URL, go to tunein.com and find your station of choice
 *  Then, copy just the number in the URL and put that after /play/ in the #define
 *
 *  The STATION_NAME is the station name in tunein.com
 *
 *  For each URL, change the base parameter to the name of the targeted device
 *
 *  It currently is configured for KQED-FM, a local NPR station, to play on the kitchen speaker
 *  Configure to your station of choice!
 *
 */

#define STATE_URL       "/kitchen/state"
#define PLAY_URL        "/kitchen/tunein/play/34804"
#define PAUSE_URL       "/kitchen/pause"
#define STATION_NAME    "KQED-FM"

// Pin that the LED is connected to
#define LED_PIN         5
