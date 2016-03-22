#include <Wire.h>
#include <Adafruit_PN532.h>
#define IRQ   (2)
#define RESET (3)  // Not connected by default on the NFC Shield

Adafruit_PN532 nfc(IRQ, RESET);

void setup(void) {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT); // When HIGH, will close relay connection
  pinMode(11, OUTPUT); // When HIGH, will open relay connection
  nfc.begin();
  nfc.SAMConfig();
}
boolean bikeOn = false;
uint8_t auth_uids[][7] = {
  // Scan your tag with a phone and place appropriate bytes below
  // prefxing each one with "0x" and delimiting with a comma ","
  {0x12, 0xFE, 0x12, 0x4E, 0x00, 0x00, 0x00}  // Description of Particluar Tag
  // ,{0xFE, 0xE2, 0x70, 0x5A, 0x00, 0x00, 0x00} // Add a second one like so
};

void loop(void) {
  //Authorized UIDs

  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;

  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
    Serial.print("Entry Request for UID: ");
    nfc.PrintHex(uid, uidLength);
    uint32_t i;
    uint32_t b;
    int num_uids = sizeof(auth_uids) / sizeof(auth_uids[0]);
    Serial.println(num_uids);
    for (i = 0; i < num_uids; i++) {
      Serial.print("Comparing UID to: ");
      nfc.PrintHex(auth_uids[i], 7);
      for (b = 0; b < 7; b++) {
        if (uid[b] == auth_uids[i][b]) {
          if (b == 6) {
            Serial.println("Read and matched!");
            if (!bikeOn) {
              if (bikeOn) {
                Serial.println("bikeOn = True");
              }
              if (!bikeOn) {
                Serial.println("bikeOn = False");
              }
              bikeOn = true;
              if (bikeOn) {
                Serial.println("bikeOn = True");
              }
              if (!bikeOn) {
                Serial.println("bikeOn = False");
              }
              digitalWrite(12, HIGH);
              digitalWrite(13, HIGH);
              Serial.println("Relay On (Closed/Tripped)");
              delay(2000);
              digitalWrite(12, LOW);
              Serial.println("Delay Done");
            } else {
              if (bikeOn) {
                Serial.println("bikeOn = True");
              }
              if (!bikeOn) {
                Serial.println("bikeOn = False");
              }
              bikeOn = false;
              if (bikeOn) {
                Serial.println("bikeOn = True");
              }
              if (!bikeOn) {
                Serial.println("bikeOn = False");
              }
              digitalWrite(11, HIGH);
              digitalWrite(13, LOW);
              Serial.println("Relay Off (Open/Untripped)");
              delay(2000);
              digitalWrite(11, LOW);
              Serial.println("Delay Done");
            }
            i = num_uids;
          }
        } else {
          Serial.println("Failed!");
          b = 7;
        }
      }
    }
  }
}
