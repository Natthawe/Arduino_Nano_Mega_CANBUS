#include <SPI.h>
#include "mcp2515_can.h"

#define CAN_DATA_COUNT 0x03

const int SPI_CS_PIN = 53;
const int CAN_INT_PIN = 2;
int counts = 0;

mcp2515_can CAN(SPI_CS_PIN);

void setup() {
  Serial.begin(115200);
  while (CAN_OK != CAN.begin(CAN_500KBPS)) // init can bus : baudrate = 500k
  {
    Serial.println("CAN INIT FAIL!");
    delay(100);
  }
  Serial.println("CAN INIT SUCCESS!");
}

void loop() {
  unsigned char len = 0;
  unsigned char rxBuf[8];

  if (CAN_MSGAVAIL == CAN.checkReceive()) {         // check if data coming
    CAN.readMsgBuf(&len, rxBuf);    // read data,  len: data length, buf: data buf

    unsigned long canId = CAN.getCanId();

    Serial.println("-----------------------------");
    Serial.print("get data from ID: 0x");
    Serial.println(canId, HEX);

    if (len < 1) {
      return;
    }

    // check cmd
    uint8_t cmd = rxBuf[0];

    switch (cmd) {
      case CAN_DATA_COUNT: // 3 data bytes
        {
          if (len < 4) {
            break;  // pb
          }
          int32_t data_count = (rxBuf[1] << 16) + (rxBuf[2] << 8) + rxBuf[3];

          // check if negative
          if (data_count & (1 << 15)) {
            data_count = -1 * ((~data_count + 1) & 0xFFFF);
          }

          counts = data_count;
          Serial.print("data_count: ");
          Serial.println(counts);
        }
        break;
    }
  }
}
