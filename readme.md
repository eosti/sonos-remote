# sonos-remote

A simple ESP8266 button for starting or stopping a [TuneIn](https://tunein.com) radio stream on a Sonos system.

## Prerequisites

* An ESP8266 with an LED connected to any I/O pin and a reset button wired to it
* A server running [node-sonos-http-api](https://github.com/jishi/node-sonos-http-api)
    * Server can either be located on the same subnet as Sonos (easiest)
    * If server is on a different subnet, you will need an IGMP proxy between the subnets for multicast forwarding
    * ESP8266 can be on any subnet, as long as it has access to port 5005/TCP on the server
* A radio station on TuneIn that you particularly enjoy

## Usage

Modify `config.h` to suit your network, ESP8266, media preferences, etc.
Then, upload to the ESP8266

You should see the LED slowly blink (connecting to WiFi) and then hold still on first powerup.
After a little bit, the LED should turn off and, if all is configured right, you should hear your radio station start up!

If that does not happen, or the LED begins to rapidly blink, check the serial output to see what's happening. 
Ensure that you can hit the Sonos API from the same network as the ESP8266, and that you can control Sonos from there.  
