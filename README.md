Sony_SLink - Arduino SONY S-LINK/Control-A1 Protocol Library.

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
