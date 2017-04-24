/*
  Arduino SONY S-LINK/Control-A1 Protocol Library

  https://github.com/Ircama/Sony_SLink.git

  (C) Ircama, 2017, CC-BY-SA 4.0
  https://creativecommons.org/licenses/by-sa/4.0/


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
  Licensed under creative commons - see http://creativecommons.org/licenses/by-sa/3.0/

*/

#include "Sony_SLink.h"
#include <util/atomic.h>

#define bitWidth 3000 //Default bit width (in micro seconds)

/* Initialize protocol */

void Slink::init(int slinkPin){
  _slinkPin=slinkPin;
  pinMode(_slinkPin, INPUT); // Define pin mode INPUT for slinkPin.
                             // Will be changed to output when writing.

/*
 * Notice that activating the internal pullup resistor is not needed,
 * as S-LINK is already pulled up to +5.1V with a 5kOhm resistor (ref.
 * Sony STR-DA50ES; other devices might have sightly different pull-up
 * resistors and voltage). Besides, S-LINK is a bidirectional protocol.
 * Do not forget a diode and a 220 Ohm resistor between slinkPin and
 * the Control-A1 jack, to limit the maximum current drawn by the
 * microcontroller in case of transmission collision.
 */

}

// We advise to issue every command twice, in case the first time collided
// with an incoming response sequence from the other device

void Slink::sendCommand(unsigned int deviceId, unsigned int commandId1, int commandId2, int commandId3){
  unsigned long Start;

  pinMode(_slinkPin, INPUT);
  _lineReady();          // Check line availability to write a frame
  Start = micros();
  pinMode(_slinkPin, OUTPUT); // Change pin mode to OUTPUT for slinkPin.
  _writeSync();          // Beginning of new transmission
  _writeByte(deviceId);  // Select device (one byte)
  _writeByte(commandId1); // Send the actual Command-Code (one byte)
  if (commandId2>=0)
    _writeByte(commandId2); // Send the actual Command-Code (one byte)
  if (commandId3>=0)
    _writeByte(commandId3); // Send an additional Command-Code (one byte)
  pinMode(_slinkPin, INPUT); // Return to INPUT
  do { // The command sequence must be padded (by 5V) at the end to make it 45 millisec. long
  delayMicroseconds(SLINK_LOOP_DELAY);
  if (digitalRead(_slinkPin)==LOW)
    break; // break padding if the other device starts transmitting meanwhile
  } //  (but the padding rule is apparently not very strict)
  while (micros()-Start < SLINK_WORD_DELIMITER);
}

/* Check line availability before transmitting or receiving

 * S-Link has no bus arbitration logic. Messages can
 * be sent at any time by any participant. Bus
 * collision may occur. Because there is not much
 * traffic, this happens extremly rare. Checking the
 * receiver state immediately before starting a new
 * transmission prevents from collisions in nearly all cases.
 */

void Slink::_lineReady(){
  unsigned long Start = micros();
  unsigned long beginTimeout = Start;

  //Search for a 3ms=3000uSec gap (at least), being HIGH continuosly.

  do {
    delayMicroseconds(SLINK_LOOP_DELAY);
    if (digitalRead(_slinkPin)==LOW)
      Start = micros(); // reset the loop each time traffic is detected
  }
  while ( (micros()-Start < SLINK_LINE_READY) && (micros()-beginTimeout < SLINK_LOOP_TIMEOUT) );
}

/*
 * Synchronisation:
 * holding the line low for SLINK_MARK_SYNC uS indicates the beginning of new transmission
 */
void Slink::_writeSync(){
  pinMode(_slinkPin, OUTPUT); // Change pin mode to OUTPUT for slinkPin.
  digitalWrite(_slinkPin, LOW); // start sync (line low)
  delayMicroseconds(SLINK_MARK_SYNC); // keep line low for SLINK_MARK_SYNC uS
  digitalWrite(_slinkPin, HIGH); // release the default HIGH state
  delayMicroseconds(SLINK_MARK_DELIMITER); // Delimiter (between Sync, Ones and Zeros, the line must be released (return to high level) for SLINK_MARK_DELIMITER uS)
}

//-------------------------------------------------------------------------------------

void Slink::_writeByte(byte value){
/*
 * Zero bit: holding the line low for SLINK_MARK_ZERO uS indicates a 0
 * One bit: holding the line low for SLINK_MARK_ONE uS indicates a 1
 * Delimiter between Sync, Ones and Zeros, the line must be released (return to high level) for SLINK_MARK_DELIMITER uS
 */

  pinMode(_slinkPin, OUTPUT); // Change pin mode to OUTPUT for slinkPin.
  for (int i=7; i>=0; i--) {
    if (value & 1<<i) {
      digitalWrite(_slinkPin, LOW);
      delayMicroseconds(SLINK_MARK_ONE);
      digitalWrite(_slinkPin, HIGH);
      delayMicroseconds(SLINK_MARK_DELIMITER);
      }
    else {
      digitalWrite(_slinkPin, LOW);
      delayMicroseconds(SLINK_MARK_ZERO);
      digitalWrite(_slinkPin, HIGH);
      delayMicroseconds(SLINK_MARK_DELIMITER);
      }
  }
}

//-------------------------------------------------------------------------------------

int Slink::pin(){
  return _slinkPin;
}

//-------------------------------------------------------------------------------------

// Read, dump and debug S-Link protocol
// This function is for debugging purpose and is valid for ATMEL devices provided with serial interface,
// which is used to output the monitoring information related to S-Link timings and data, read from the input port.
#if !defined (__AVR_ATtiny85__)
void Slink::inputMonitor(int type, // 0=measure timing, 1=decode and dump binary & hex, 2=decode and dump hex
                         boolean idle, // false(default)=measure mark (=data), true=measure idle timing (=delimiters)
                         // for S-Link protocol, marks can return sync (2400usec), zero (600usec), one (1200usec)
                         // idle=true can only be used with type=0 and timing should always return 600 usec (delimiter)
                         unsigned long uSecTimeout, // monitoring timeout; defaults to 10 seconds
                         // information read from S-Link is buffered until timeout and then dumped to the serial port
                         unsigned long serialSpeed // baud rate of the serial port (defaults to 115.2kbps)
                         ){
  unsigned long value = 0;
  unsigned long Start = micros();
  int nl=0;
  int count=0;
  int byte=0;
  String buffer = "";
  
  Serial.begin(serialSpeed);
  Serial.println("Start monitor");
    do {
      value = pulseIn(_slinkPin, idle, 3000UL); // timeout to 3 milliseconds=3000 uSec
      if (value==0) {
        if (nl==0)
          buffer += String("\n");
        nl=1;
        count=0;
        byte=0;
      }
      else {
        switch(type) {
         case 0: // timing
            buffer += String(",");
            buffer += String(value);
            break;
         case 1: // binary - HEX
         case 2: // HEX
            if ((value > (SLINK_MARK_SYNC / SLINK_MARK_RANGE)) && (value < SLINK_MARK_SYNC * SLINK_MARK_RANGE)) {
              buffer += String("\n");
              buffer += String("START,");
              count=0;
              byte=0;
            }
            else {
              if ((value > SLINK_MARK_ONE / SLINK_MARK_RANGE) && (value < SLINK_MARK_ONE * SLINK_MARK_RANGE)) {
                byte |= 128>>count;
                
                if (type == 1)
                  buffer += String("1,");
              }
              if ((value > SLINK_MARK_ZERO / SLINK_MARK_RANGE) && (value < SLINK_MARK_ZERO * SLINK_MARK_RANGE))
                if (type == 1)
                  buffer += String("0,");
              
              if (count++ == 7) {
                if (type == 1)
                  buffer += String(" = ");
                buffer += String(byte, HEX) + String(",");
                if (type == 1)
                  buffer += String("  ");
                count=0;
                byte=0;
              }
            }
            break;
        }
        nl=0;
      } // else
    } // do
    while (micros()-Start < uSecTimeout);
  Serial.println(buffer);
  Serial.println("End monitor");
}
#endif
