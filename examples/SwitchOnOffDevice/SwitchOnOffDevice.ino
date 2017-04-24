/*
 * SLink: SwitchOnOffDevice - demonstrates sending S-Link codes with Slink
 * The S-Link connector must be wired to Arduino SLINK_PIN.
 * Version 1, April, 2017
 * Copyright 2017 Ircama
 * https://github.com/Ircama/Sony_SLink.git
 *
 * Test Sony Amplifier power-on and power-off via S-Link bus
 * using the SONY S-LINK/Control-A1 Protocol Library.
 * Each 10 secs the device is powered on and then off.
 */

#define SLINK_PIN 2 // S-Link Control-A1 pin

#include "Sony_SLink.h"
Slink slink;

void setup() {
  slink.init(SLINK_PIN); // Set-up S-Link pin
}

void loop() {
  slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_POWER_ON);
	delay(10000); //10 second delay between each command
  slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_POWER_OFF);
	delay(10000); //10 second delay between each command
}
