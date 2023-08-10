#include <SPI.h>
#include "mcp2515_can.h"

#define CAN_DATA_COUNT 0x03

int SPI_CS_PIN = 10;
int CAN_INT_PIN = 2;
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
  Serial.print("CAN SEND Count data = ");
  counts = random(0, 200);
  Serial.println(counts);
  uint8_t data_count[] = { CAN_DATA_COUNT, (counts >> 16) & 0xFF, (counts >> 8) & 0xFF, counts & 0xFF };
  // send data:  id = 0x70, standard frame, data len = 8, stmp: data buf
  CAN.sendMsgBuf(0x70, 0, 4, data_count);
  delay(1000); // send data once per second

}
