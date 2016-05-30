/**
   @file    nfc_mifare_mf1s50_reader.ino
   @author  www.elechouse.com
   @brief   example of reading mf1s50 card for NFC_MODULE

    For this demo, waiting for a MF1S50 card or tag, after reading a card/tag UID,
    then try to read the block 4/5/6/7 ..

   @section  HISTORY

   V1.0 initial version

    Copyright (c) 2012 www.elechouse.com  All right reserved.
 */

#define IRQ   (2)

/** include library */

#include "application.h"

#include "nfc.h"

/** define a nfc class */
NFC_Module nfc;

void setup(void)
{
        Serial.begin(9600);
        nfc.begin();
        Serial.println("MF1S50 Reader Demo From Elechouse!");

        uint32_t versiondata = nfc.get_version();
        if (!versiondata) {
                Serial.print("Didn't find PN53x board");
                while (1);  // halt
        }

        // Got ok data, print it out!
        Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);
        Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);
        Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
        pinMode(13, OUTPUT);
        pinMode(12, OUTPUT); // When HIGH, will close relay connection
        pinMode(11, OUTPUT); // When HIGH, will open relay connection
        /** Set normal mode, and disable SAM */
        nfc.SAMConfiguration();
}

boolean bikeOn = false;
uint8_t auth_uids[][7] = {
        // Scan your tag with a phone and place appropriate bytes below
        // prefxing each one with "0x" and delimiting with a comma ","
        {0x12, 0xFE, 0x12, 0x4E, 0x00, 0x00, 0x00} // Description of Particluar Tag
        // ,{0xFE, 0xE2, 0x70, 0x5A, 0x00, 0x00, 0x00} // Add a second one like so
};


void loop(void)
{
        u8 buf[32],sta;


        /** Polling the mifar card, buf[0] is the length of the UID */
        sta = nfc.InListPassiveTarget(buf);

        /** check state and UID length */
        if(sta && buf[0] == 4) {
                /*Serial.print("Entry Request for UID: ");
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
                }*/
                /** the card may be Mifare Classic card, try to read the block */
                Serial.print("UUID length:");
                Serial.print(buf[0], DEC);
                Serial.println();
                Serial.print("UUID:");
                nfc.puthex(buf+1, buf[0]);
                Serial.println();
                /** factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF */
                u8 key[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
                u8 blocknum = 4;
                /** Authentication blok 4 */
                sta = nfc.MifareAuthentication(0, blocknum, buf+1, buf[0], key);
                if(sta) {
                        /** save read block data */
                        u8 block[16];
                        Serial.println("Authentication success.");
                        /** read block 4 */
                        sta = nfc.MifareReadBlock(blocknum, block);
                        if(sta) {
                                Serial.println("Read block successfully:");

                                nfc.puthex(block, 16);
                                Serial.println();
                        }

                        /** read block 5 */
                        sta = nfc.MifareReadBlock(blocknum+1, block);
                        if(sta) {
                                Serial.println("Read block successfully:");

                                nfc.puthex(block, 16);
                                Serial.println();
                        }
                        /** read block 6 */
                        sta = nfc.MifareReadBlock(blocknum+2, block);
                        if(sta) {
                                Serial.println("Read block successfully:");

                                nfc.puthex(block, 16);
                                Serial.println();
                        }
                        /** read block 7 */
                        sta = nfc.MifareReadBlock(blocknum+3, block);
                        if(sta) {
                                Serial.println("Read block successfully:");

                                nfc.puthex(block, 16);
                                Serial.println();
                        }
                }
        }
}
