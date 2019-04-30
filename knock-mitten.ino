#include <Adafruit_NeoPixel.h>
#include <Ticker.h>
#include <WiFi.h>
#include <Ambient.h>

#define PIN            25 // GPIO
#define G35            35
#define NUMPIXELS      59

#define WIFI_SSID "********"
#define WIFI_PASSWORD "********"

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Ticker ticker;
Ambient ambient;
WiFiClient client;

unsigned int channelId = "****"; // AmbientのチャネルID
const char* writeKey = "****************"; // Ambientライトキー

int delayval = 50;
int val = 0;
int counter = 1;
int punchCounter = 0;

void setup() {
  pinMode(PIN, OUTPUT);
  Serial.begin(115200);

  // WiFi接続
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  ambient.begin(channelId, writeKey, &client);

  // This initializes the NeoPixel library.
  pixels.begin();
  pixels.setBrightness(255);  // MAX: 255
  pixels.show();
  for(int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 200));
    pixels.show();
  }
}


// めでたいモード
void blink() {
  counter = counter + 1;
  Serial.println(counter);
  int r = (counter * 255) % 255;
  int g = (counter * 100) % 255;
  int b = (counter * 150) % 255;
  for(int i = 0; i < NUMPIXELS; i++) {
    if (i % 2 == 0) {
      pixels.setPixelColor(i, pixels.Color(r, g, b));
      pixels.show();
    } else {
      pixels.setPixelColor(i, pixels.Color(188, 100, 100));
      pixels.show();
    }
  }
}

void finish() {
  // ticker.detach();
  delay(500);
  pixels.clear();
  pixels.show();
  ticker.attach_ms(200, blink);
}

// loop内の処理を繰り返します
void loop(){
  // ADCって書いてある番号だけanalogReadできるらしい
  val = analogRead(G35); // read the input pin
  Serial.println(val);
  if (val > 50) {
    delay(100);
    Serial.println("count!!!");
    punchCounter = punchCounter + 1;
    Serial.println(punchCounter);
    ambient.set(1, String(val).c_str());
    ambient.send();
  }

  if (punchCounter >= 10 && punchCounter < 20) {
    for (int i = 1; i <= 10; i++){
      pixels.setPixelColor(i, pixels.Color(255, 1, 1));
    }
    pixels.show();
  }

  if (punchCounter >= 20 & punchCounter < 30) {
    for (int i = 11; i <= 20; i++){
      pixels.setPixelColor(i, pixels.Color(255, 1, 1));
    }
    pixels.show();
  }

  if (punchCounter >= 30 & punchCounter < 40) {
    for (int i = 21; i <= 30; i++){
      pixels.setPixelColor(i, pixels.Color(255, 1, 1));
    }
    pixels.show();
  }

  if (punchCounter >= 40 & punchCounter < 50) {
    for (int i = 31; i <= 40; i++){
      pixels.setPixelColor(i, pixels.Color(255, 1, 1));
    }
    pixels.show();
  }


  if (punchCounter >= 50 & punchCounter <= 60) {
    for (int i = 41; i <= 50; i++){
      pixels.setPixelColor(i, pixels.Color(255, 1, 1));
    }
    pixels.show();
  }

  if (punchCounter > 60) {
    finish();
  }
}
