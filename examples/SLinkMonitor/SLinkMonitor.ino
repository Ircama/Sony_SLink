/*
 * SLink: SLinkMonitor - demonstrates monitoring S-Link protocol
 * The S-Link connector must be wired to Arduino SLINK_PIN.
 * Use with ATMEL devices provided with serial interface,

 * Version 1, April, 2017
 * Copyright 2017 Ircama
 * https://github.com/Ircama/Sony_SLink.git
 *
 */

#define SLINK_PIN 2 // S-Link Control-A1 pin
#define SERIAL_COM_PORT_SPEED 115200UL // 115 kbps

#include "Sony_SLink.h"
Slink slink;

void setup() {
  slink.init(SLINK_PIN); // Set-up S-Link pin
  Serial.begin(SERIAL_COM_PORT_SPEED);
  Serial.print("S-Link monitor on pin ");
  Serial.println(slink.pin());
  Serial.println("Enter command:");
}

void loop() {
  byte cmd;
  if (Serial.available()) {
    cmd = Serial.read();
    switch (cmd) {
      // Dump Control-A1 S-Link sequences
      case '1':
        slink.inputMonitor(0); // measure timing of mark periods in microseconds ('sync' should be 2400usecs, 'one' about 1200usecs, 'zero' ab. 600usecs)
        break;
      case '2':
        slink.inputMonitor(1); // monitor bytes decoded from S-Link input, displaying binary and HEX format of each byte
        break;
      case '3':
        slink.inputMonitor(2); // monitor bytes decoded from S-Link input, displaying HEX dump
        break;
      case '4':
        slink.inputMonitor(0, true); // measure timing of idle periods (e.g., delimiter; all idle periods should be about 600 microseconds)
        break;
    }
  }
}
