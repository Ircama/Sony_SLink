# Sony_SLink

**Arduino SONY S-LINK/Control-A1 Protocol Library.**

This library allows to drive Sony audio consumer devices connected through the
Sony S-Link bus-system.

It also includes trivial monitoring capabilities for the S-Link interface, to test
and dump received sequences.

(C) Ircama, 2017, CC-BY-SA 4.0
https://creativecommons.org/licenses/by-sa/4.0/

In standard operation, the code is targeted to the ATtiny85 microcontroller (e.g.
Digispark Digistump ATtiny85). As the ATtiny85 USB interface does not allow confortable
monitoring and debugging features, dumping the S-Link interface can be achieved through
an ATmega328P micro (e.g., Arduino Nano).

-------------------------------------------------------------------------------------------------------------------

# Installation
Create a new folder called "Sony_SLink" under the folder named "libraries" in your Arduino sketchbook folder.
Create the folder "libraries" in case it does not exist yet. Place all the files in the "Sony_SLink" folder.

# Usage
To use the library in your own sketch, select it from *Sketch > Import Library*.

# Example

Simplest example of using this library:

```
#define SLINK_PIN 2 // S-Link Control-A1 pin

#include "Sony_SLink.h"
Slink slink;

void setup() {
  slink.init(SLINK_PIN); // Set-up S-Link pin
  slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_POWER_ON);
}

void loop() {
}
```

Check the *examples* directory for further examples.

# Description of the functions

slink.init(slinkPin) - set-up S-Link pin

slink.sendCommand(device, command) - send a command to the device; check Sony_SLink.h for available commands

Examples:
```
slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_VOLUME_UP);
slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_VOLUME_DOWN);
slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_POWER_OFF);
slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_POWER_OFF);
slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_POWER_ON);
slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_SET_INPUT_CHAN, SLINK_CMDP_IN_TV);
slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_MUTE_ON);
slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_MUTE_OFF);
slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_SET_INPUT_CHAN, SLINK_CMDP_IN_TUNER);
slink.sendCommand(SLINK_DEVICE_AMP, SLINK_CMD_AMP_SET_INPUT_CHAN, SLINK_CMDP_IN_CD);
slink.sendCommand(SLINK_DEVICE_TUNER, SLINK_CMD_TUNER_PRESET_UP);
slink.sendCommand(SLINK_DEVICE_TUNER, SLINK_CMD_TUNER_PRESET_DOWN);
slink.sendCommand(SLINK_DEVICE_TUNER, SLINK_CMD_TUNER_PRESET_STATION, 1, 1);
```

slink.inputMonitor - monitor SLink input
Available options:
```
slink.inputMonitor(0); // measure timing of mark periods in microseconds (sync should be 2400, one about 1200, zero ab. 600)
slink.inputMonitor(1); // monitor bytes displaying binary and HEX format of each byte
slink.inputMonitor(2); // monitor bytes displaying HEX dump
slink.inputMonitor(0, true); // measure timing of idle periods (e.g., delimiter; all idle periods should be about 600 microseconds)
```

# Notes

  Code heavily based on
  - hajdbo's code:
    https://github.com/hajdbo/sony-jukebox-slink/blob/master/jukebox.pde
  - EdsterG's code:
    https://github.com/EdsterG/LANC_Library

  Reference documents:
  - Sony S-Link gateway document:
    http://ee.bradley.edu/projects/proj2001/sonyntwk/SLink.PDF

  - How SONY's S-LINK/CTRL-A(II) protocol works (waveform):
    http://boehmel.de/slink.htm

  - Codes for Sony STR-DA50ES receiver:
    http://www.hifi-remote.com/sony/Sony_dsp_192.htm

  - S-Link Parallel Device for Linux
    http://web.archive.org/web/20040722060841/http://www.undeadscientist.com/slink/index.html

  - Reverse Engineered Control-A1 codes (Using the Sony CDP-CX250) Written by BigDave (2/19/98)
    http://web.archive.org/web/19981202175957/http://www.cc.gatech.edu/people/home/bigdave/cdplayer/control-a1.txt

  - Working S-Link code - Background of the S-link protocol
    http://forums.parallax.com/discussion/70973/working-s-link-code-long

  - Slink Send and Receive Commands for Sony MiniDisc Recorders
    http://web.archive.org/web/20030419100153/http://web2.airmail.net/will/mdslink.txt

  - Jukebox Interface Guide
    http://web.archive.org/web/20030414200645/http://www.upl.cs.wisc.edu/~orn/jukebox/guide.html

  - Original Control-A1 document
    http://web.archive.org/web/20030414231523/http://www.upl.cs.wisc.edu/~orn/jukebox/controla1.html

  Tested with a Sony STR-DA50ES receiver/amplifier.
  Service manual: http://sportsbil.com/sony/STR/STR-D/STR-DA30ES_DA50ES_V55ES_v1.1.pdf
  
  This protocol is very slow: average 355 bps half duplex for a standard two-byte send transmission taking 45 millisecs (355=16/0,045).

  Feel free to share this source code, but include explicit mention to the author.
  Licensed under creative commons - see http://creativecommons.org/licenses/by-sa/4.0/
