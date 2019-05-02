#include <Adafruit_NeoPixel.h>
#include <Ticker.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define PIN            25 // GPIO
#define G35            35
#define NUMPIXELS      59

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
uint8_t value = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
//
// Version 4 UUID
#define SERVICE_UUID        "f6740539-e7ee-47bc-926b-adfa5150e1a2"
#define CHARACTERISTIC_UUID "3d7c94ca-0522-4287-94a7-8913e7e63beb"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Ticker ticker;

int delayval = 50;
int val = 0;
int counter = 1;
int punchCounter = 0;

void setup() {
  pinMode(PIN, OUTPUT);
  Serial.begin(115200);
  // Create the BLE Device
  BLEDevice::init("knockmitten");

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");

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
    if (deviceConnected) {
      char buffer[32];
      sprintf(buffer, "{\"pulse\":%d,\"counter\":%d}", val, punchCounter);
      Serial.printf(buffer);
      pCharacteristic->setValue(buffer);
      pCharacteristic->notify();
      delay(100);
    }
  }

  if (punchCounter == 5) {
    for(int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(65, 1, 200));
      pixels.show();
    }
  }

  if (punchCounter == 10) {
    for(int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(130, 1, 200));
      pixels.show();
    }
  }

  if (punchCounter == 15 ) {
    for(int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(200, 1, 200));
      pixels.show();
    }
  }

  if (punchCounter == 20) {
    finish();
  }
}
