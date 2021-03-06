#ifndef ESP32_H
#define ESP32_H

// SPI_DRIVER_SELECT must be set to 0 in SdFat/SdFatConfig.h

SdFat SD;
#define SPIINIT 14,2,15,13 // TTGO_T1,LOLIN32_Pro (sck, miso, mosi, cs)
//#define SPIINIT 18,19,23,5 // ePaper,ESP32_DevKit
#define SDINIT SS, SD_SCK_MHZ(SDMHZ)
#define SDMHZ 25 // TTGO_T1,LOLIN32_Pro=25 ePaper,ESP32_DevKit=20
#define LED 5 // TTGO_T1=22 LOLIN32_Pro=5(inverted) DOIT_Esp32=2 ESP32-PICO-KIT=no led
#define LEDinv 1
#define BOARD "LOLIN32_Pro (clone)"
#define board_esp32
#define board_digital_io

uint8 esp32bdos(uint16 dmaaddr) {
	return(0x00);
}

#endif