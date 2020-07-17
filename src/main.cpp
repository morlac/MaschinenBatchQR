#include <Arduino.h>

// for c++-style cout and "<<" operator
#include <Streaming.h>

#define QR_VERSION 8

#define LOCK_VERSION 8

#include <qrcode.h>

#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

//U8G2_ST7565_PE12864_004_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
U8G2_ST7565_PE12864_004_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);

//U8G2_ST7565_PE12864_004_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);

QRCode maschinenQR;

#define QR_BUFFER_SIZE_1 (4 * QR_VERSION + 17)
#define QR_BUFFER_SIZE ((QR_BUFFER_SIZE_1 * QR_BUFFER_SIZE_1) + 7) / 8

static uint8_t qrcodeData[QR_BUFFER_SIZE];

/**
 *
 */
void setup() {
	u8g2.begin();
	u8g2.enableUTF8Print();

	Serial.begin(57600);
}

/**
 *
 */
void loop() {
	uint32_t m = millis();

	qrcode_initText(&maschinenQR, qrcodeData, QR_VERSION, ECC_HIGH, String(String(F("https://maschinendeck.org/?")) + String(m)).c_str());

//	u8g2.clearBuffer();

	Serial << m << F(" millies") << endl;

	// Top quiet zone
//	Serial << endl << endl << endl << endl;

	u8g2.firstPage();

	do {
		for (uint8_t y = 0; y < maschinenQR.size; y++) {
			// Left quiet zone
			//		Serial << F("        ");

			// Each horizontal module
			for (uint8_t x = 0; x < maschinenQR.size; x++) {
				// Print each module (UTF-8 \u2588 is a solid block)
				//			Serial << (qrcode_getModule(&maschinenQR, x, y) ? F("\u2588\u2588"): F("  "));

				if (qrcode_getModule(&maschinenQR, x, y)) {
					//				Serial << F("\u2588\u2588");
					u8g2.drawPixel(1 + x, 1 + y);
					//			} else {
					//				Serial << F("  ");
				}
			}

			//		Serial << endl;
		}
	} while (u8g2.nextPage());

//	u8g2.sendBuffer();

	// Bottom quiet zone
//	Serial << endl << endl << endl << endl;

	delay(1000);
}
