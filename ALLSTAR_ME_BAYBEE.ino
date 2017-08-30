/*
  ESP8266 // ESP32 Auto-caster

  Because sometimes you never know if you don't go.
  You'll never shine if you don't glow.
 */

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

#include <ESPmDNS.h>

#include <HTTPClient.h>
#include <ssl_client.h>

typedef struct WiFiNetwork {
    char* ssid;
    char* password;
} WiFiNetwork;

// Add Wi-Fi Configs here!
const int wifi_networks = 1;
WiFiNetwork networks[wifi_networks] = {
  { "SSID", "Password" },
};

// Hey now, You're an all-star.
String youtube_id = "jiczZVF2d0s";

WiFiMulti wifi;
HTTPClient http;

void connect_to_wifi() {
  while (wifi.run() != WL_CONNECTED) {
    Serial.println("Unable to connect to wifi. Waiting for a connection.");
    delay(5000);
  }
  MDNS.begin("ESP32_Browser");
}

void FUKIN_ALLSTAR_ME_BAYBEE() {
  char service[] = "googlecast";
  char proto[] = "tcp";
  Serial.printf("Browsing for service _%s._%s.local. ... ", service, proto);
  int n = MDNS.queryService(service, proto);
  if (n == 0) {
    Serial.println("no chromecasts found");
    return;
  } else {
    Serial.print(n);
    Serial.println(" chromecast(s) found");
  }
  
  for (int i=0; i<n; ++i) {
    String cc = MDNS.IP(i).toString();
    int port = 8008;

    String uri = "http://" + cc + ":" + port + "/apps/YouTube";
    Serial.println("Requesting " + uri);

    http.begin(uri);
    http.setUserAgent("arduino/esp32");
    http.addHeader("Accept", "*/*");
    http.addHeader("Content-Type", "application/json");
    int response_code = http.sendRequest("POST", "v=" + youtube_id);
    http.end();
    
    Serial.print("Http Response Code: ");
    Serial.println(response_code);
    delay(5000);
  }
}

void WiFiEvent(WiFiEvent_t event) {
    switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.println("Lost connection to hotspot.");
        break;
    }
}

void setup() {
    Serial.begin(115200);
    // Forget about previous wifi connections.
    WiFi.disconnect(true);
    delay(1000);
    WiFi.onEvent(WiFiEvent);

    for (int i=0; i<wifi_networks; i++) {
      char* ssid     = networks[i].ssid;
      char* password = networks[i].password;
      wifi.addAP(ssid, password);
    }
}

void loop() {
    connect_to_wifi();
    FUKIN_ALLSTAR_ME_BAYBEE();
    delay(60000);
}
