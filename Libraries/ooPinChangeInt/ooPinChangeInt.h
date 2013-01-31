// We use 4-character tabstops, so IN VIM:  <esc>:set ts=4
// ...that's: ESCAPE key, colon key, then "s-e-t SPACE key t-s-=-4"
//
//-------- define these in your sketch, if applicable ----------------------------------------------------------
//-------- This must go ahead of the #include ooPinChangeInt.h statement in your sketch ------------------------
// You can reduce the memory footprint of this handler by declaring that there will be no pin change interrupts
// on any one or two of the three ports.  If only a single port remains, the handler will be declared inline
// reducing the size and latency of the handler.
// #define NO_PORTB_PINCHANGES // to indicate that port b will not be used for pin change interrupts
// #define NO_PORTC_PINCHANGES // to indicate that port c will not be used for pin change interrupts
// #define NO_PORTD_PINCHANGES // to indicate that port d will not be used for pin change interrupts
// You can reduce the code size by maybe 20 bytes, and you can speed up the interrupt routine
// slightly by declaring that you don't care if the static variable PCintPort::pinState
// is made available to your interrupt routine.
// #define NO_PIN_STATE        // to indicate that you don't need the pinState
//
// define DISABLE_PCINT_MULTI_SERVICE below to limit the handler to servicing a single interrupt per invocation.
// #define       DISABLE_PCINT_MULTI_SERVICE
//-------- define the above in your sketch, if applicable ------------------------------------------------------

/*
	ooPinChangeInt.h
	---- VERSIONS ----------------------------------------------------------------------------
	Library begins with the PinChangeInt v 1.3 code.  See http://code.google.com/p/arduino-pinchangeint/

	Version 1.00 Sat Dec  3 22:56:20 CST 2011
	Modified to use the new() operator and symbolic links instead of creating a pre-populated
	array of pointers to the pins.  This consumes more flash, but makes possible some
	additional C++ style functionality later.

	Version 1.01 Thu Dec  8 21:29:11 CST 2011
	Modified to use a C++ callback function.  The arduinoPin variable is no longer necessary,
	as this creates a new methodology for using the library.

	Version 1.02 Tue Mon Mar  5 18:37:28 CST 2012
	All code moved into this .h file so as to make it possible to recognize #define's in the
	user's sketch.

	Added #ifdef LIBCALL_OOPINCHANGEINT.  Programmers using this library in another library
	should define this macro, because this will allow you to #include it in your sketch AND
	#include it in the library.
    (As a matter of act, you must always #include this file in your sketch, even if it's only
	used to support another library.  See the Tigger library and example, for an example.)

	Code uses the cbiface library, which is a much simplified and renamed version of cb.h
	---- VERSIONS ----------------------------------------------------------------------------

	See google code project for latest, bugs and info http://code.google.com/p/arduino-pinchangeint/
	For more information Refer to avr-gcc header files, arduino source and atmega datasheet.

	This library was inspired by and derived from "johnboiles" (it seems)
	PCInt Arduino Playground example here: http://www.arduino.cc/playground/Main/PcInt
	If you are the original author, please let us know at the google code page

	It provides an extension to the interrupt support for arduino by
	adding pin change interrupts, giving a way for users to have
	interrupts drive off of any pin.

	Please make any configuration changes in the accompanying PinChangeIntConfig.h file.
	This will help avoid having to reset your config in the event of changes to the
	library code (just don't replace that file when you update).

	Modified Thu Sep  8 07:33:17 CDT 2011 by GreyGnome.  See GreyGnome comment, below.

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	*/

#ifndef ooPinChangeInt_h
#define	ooPinChangeInt_h

#include "stddef.h"

// Thanks to Maurice Beelen, nms277, Akesson Karlpetter, and Orly Andico for these fixes.
#if defined(ARDUINO) && ARDUINO >= 100
   #include <Arduino.h>
   #include <new.h>
#else
   #include <pins_arduino.h>
   #ifndef   LIBCALL_OOPINCHANGEINT
     #include "../cppfix/cppfix.h"
   #endif
   #include "WProgram.h"
#endif

#include "../cbiface/cbiface.h"

#undef DEBUG

/*
* Theory: all IO pins on Atmega168 are covered by Pin Change Interrupts.
* The PCINT corresponding to the pin must be enabled and masked, and
* an ISR routine provided.  Since PCINTs are per port, not per pin, the ISR
* must use some logic to actually implement a per-pin interrupt service.
*/

/* Pin to interrupt map:
* D0-D7 = PCINT 16-23 = PCIR2 = PD = PCIE2 = pcmsk2
* D8-D13 = PCINT 0-5 = PCIR0 = PB = PCIE0 = pcmsk0
* A0-A5 (D14-D19) = PCINT 8-13 = PCIR1 = PC = PCIE1 = pcmsk1
*/

#define	INLINE_PCINT
#if ((defined(NO_PORTB_PINCHANGES) && defined(NO_PORTC_PINCHANGES)) || \
			(defined(NO_PORTC_PINCHANGES) && defined(NO_PORTD_PINCHANGES)) || \
			(defined(NO_PORTD_PINCHANGES) && defined(NO_PORTB_PINCHANGES)))
#undef INLINE_PCINT
#define	INLINE_PCINT inline
#endif

class PCintPort {
public:
	PCintPort(int index,volatile uint8_t& maskReg) :
	portInputReg(*portInputRegister(index + 2)),
	portPCMask(maskReg),
	firstPin(NULL),
	PCICRbit(1 << index)
	{ }
	volatile	uint8_t&		portInputReg;
	// cbIface should be an object instantiated from a subclass of CallBackInterface
	static		void attachInterrupt(uint8_t pin, CallBackInterface* cbIface, int mode);
	static		void detachInterrupt(uint8_t pin);
	INLINE_PCINT void PCint();
	static uint8_t curr;

protected:
	class PCintPin {
	public:
		PCintPin() :
		mode(0) {}
		CallBackInterface* 	pinCallBack;
		uint8_t 	mode;
		uint8_t		mask;
		#ifndef NO_PIN_STATE
		uint8_t		state;
		#endif
		PCintPin* next;
	};
	void		addPin(uint8_t arduinoPin,uint8_t mode,CallBackInterface* cbIface);
	void		delPin(uint8_t mask);
	volatile	uint8_t&		portPCMask;
	const		uint8_t			PCICRbit;
	uint8_t		lastPinView;
	PCintPin*	firstPin;
};
#endif
// **********************************************************************************************************

#ifdef   LIBCALL_OOPINCHANGEINT
extern PCintPort portB;
extern PCintPort portC;
extern PCintPort portD;
#else // LIBCALL_OOPINCHANGEINT
uint8_t PCintPort::curr=0;
#ifndef NO_PORTB_PINCHANGES
PCintPort portB=PCintPort(0,PCMSK0); // port PB==2  (from Arduino.h, Arduino version 1.0)
#endif

#ifndef NO_PORTC_PINCHANGES
PCintPort portC=PCintPort(1,PCMSK1); // port PC==3  (also in pins_arduino.c, Arduino version 022)
#endif

#ifndef NO_PORTD_PINCHANGES
PCintPort portD=PCintPort(2,PCMSK2); // port PD==4
#endif
#endif // LIBCALL_OOPINCHANGEINT

#ifndef   LIBCALL_OOPINCHANGEINT
void PCintPort::addPin(uint8_t arduinoPin, uint8_t mode,CallBackInterface* cbIface)
{
	// Create pin p:  fill in the data
	PCintPin* p=new PCintPin;
	p->mode = mode;
	p->next=NULL;
	//portPCMask |= p->mask = digital_pin_to_bit_mask[arduinoPin]; // the mask - MIKE
	p->mask = digitalPinToBitMask(arduinoPin); // the mask
	// ...Pin created

	if (p == NULL) return;
	// Add to linked list, starting with firstPin
	if (firstPin == NULL) firstPin=p;
	else {
		PCintPin* tmp=firstPin;
		while (tmp->next != NULL) {
				tmp=tmp->next;
		};
		tmp->next=p;
	}
	// Now we attach the object which has subclassed the CallBackInterface
	p->pinCallBack=cbIface;
#ifdef DEBUG
	Serial.print("addPin. pin given: "); Serial.print(arduinoPin, DEC), Serial.print (" pin stored: ");
	int addr = (int) p;
	Serial.print(" instance addr: "); Serial.println(addr, HEX);
#endif
	portPCMask |= p->mask;
	PCICR |= PCICRbit;
}

void PCintPort::delPin(uint8_t mask)
{
		PCintPin* current=firstPin;
		PCintPin* prev=NULL;
		while (current) {
				if (current->mask == mask) { // found the target
						uint8_t oldSREG = SREG;
						cli(); // disable interrupts
						portPCMask &= ~mask; // disable the mask entry.
						if (portPCMask == 0) PCICR &= ~(PCICRbit);
						// Link the previous' next to the found next. Then remove the found.
						if (prev != NULL) prev->next=current->next; // linked list skips over current.
			else firstPin=current->next; // at the first pin; save the new first pin
			delete current;
			SREG = oldSREG; // Restore register; reenables interrupts
			return;
		}
		prev=current;
		current=current->next;
	}
}

/*
 * attach an interrupt to a specific pin using pin change interrupts.
 */
void PCintPort::attachInterrupt(uint8_t arduinoPin, CallBackInterface* cbIface, int mode)
{
	PCintPort *port;
	uint8_t portNum = digitalPinToPort(arduinoPin);
	if ((portNum == NOT_A_PORT) || (cbIface == NULL)) return;

	switch (portNum) {
#ifndef NO_PORTB_PINCHANGES
	case 2:
		port=&portB;
		break;
#endif
#ifndef NO_PORTC_PINCHANGES
	case 3:
		port=&portC;
		break;
#endif
#ifndef NO_PORTD_PINCHANGES
	case 4:
		port=&portD;
#endif
	}
	// Added by GreyGnome... must set the initial value of lastPinView for it to be correct on the 1st interrupt.
	// ...but even then, how do you define "correct"?  Ultimately, the user must specify (not provisioned for yet).
	port->lastPinView=port->portInputReg;

	// map pin to PCIR register
	port->addPin(arduinoPin,mode,cbIface);
#ifdef DEBUG
	Serial.print("attachInterrupt FUNC: "); Serial.println(arduinoPin, DEC);
#endif
}

void PCintPort::detachInterrupt(uint8_t arduinoPin)
{
	PCintPort *port;
#ifdef DEBUG
	Serial.print("detachInterrupt: "); Serial.println(arduinoPin, DEC);
#endif
	uint8_t portNum = digitalPinToPort(arduinoPin);
	if (portNum == NOT_A_PORT) {
		return;
	}
	switch (portNum) {
#ifndef NO_PORTB_PINCHANGES
	case 2:
		port=&portB;
		break;
#endif
#ifndef NO_PORTC_PINCHANGES
	case 3:
		port=&portC;
		break;
#endif
#ifndef NO_PORTD_PINCHANGES
	case 4:
		port=&portD;
#endif
	}
	port->delPin(digitalPinToBitMask(arduinoPin)); // MIKE
}

// common code for isr handler. "port" is the PCINT number.
// there isn't really a good way to back-map ports and masks to pins.
void PCintPort::PCint() {
	#ifndef DISABLE_PCINT_MULTI_SERVICE
	uint8_t pcifr;
	do {
	#endif
		// get the pin states for the indicated port.
		uint8_t changedPins = PCintPort::curr ^ lastPinView;
		lastPinView = PCintPort::curr;
		changedPins &= portPCMask;
		// This code removed because the only pins that can trigger an interrupt are
		// pins that are enabled via the PCMASK[0-2], therefore, we are here because of
		// an interrupt.  Other pins on that port that have not been assigned an interrupt
		// will not trigger an interrupt. We assume the pins have not been enabled by
		// another method outside of the PinChangeInt code. Even if they have, the worst
		// that will happen is that this interrupt will be called but no user method
		// will be called; the extra pin will not be chosen in any event.
		//
		// screen out non pcint pins.
		//if ((changedPins &= portPCMask) == 0) {
		//	return;
		//}

		PCintPin* p = firstPin;
		/* Example: Do it the array way.  Note that the pinArray has not been created.
		 * This is left as an exercise for the programmer.
		 */
		/*
		uint8_t i=0;
		uint8_t mode;
		while (changedPins) {
			if ( changedPins & 0b00001111 ) i=0; else { i=4; changedPins >>= 4; } // optimization
			if (changedPins & 0x1) {
				mode=pinArray[i]->mode;
				if (     mode == CHANGE
					|| ((mode == RISING)  &&  (curr & p->mask))
					|| ((mode == FALLING) && !(curr & p->mask)) ) {
				(*(pinArray[i]->pinCallBack))(); // pinArray is an ordered array of the pins for this port
			}
			changedPins >> 1; i++;
		}
		*/
		/* Do it the pointer way */
		while (p) {
			if (p->mask & changedPins) { // a changed bit
				// Trigger interrupt if mode is CHANGE, or if mode is RISING and
				// the bit is currently high, or if mode is FALLING and bit is low.
				if (     p->mode == CHANGE
					|| ((p->mode == RISING)  &&  (PCintPort::curr & p->mask))
					|| ((p->mode == FALLING) && !(PCintPort::curr & p->mask)) ) {
					#ifndef NO_PIN_STATE
					p->state=PCintPort::curr & p->mask ? HIGH : LOW;
					#endif
					(*(p->pinCallBack)).cbmethod();
				}
			}
			//changedPins ^= p->mask;  // MIKE:  Check on this optimization.
			//if (!changedPins) break;
			p=p->next;
		}
	#ifndef DISABLE_PCINT_MULTI_SERVICE
		pcifr = PCIFR & PCICRbit;
		PCIFR = pcifr;	// clear the interrupt if we will process it (no effect if bit is zero)
	} while(pcifr);
	#endif
}

#ifndef NO_PORTB_PINCHANGES
ISR(PCINT0_vect) {
	PCintPort::curr = portB.portInputReg;
	portB.PCint();
}
#endif

#ifndef NO_PORTC_PINCHANGES
ISR(PCINT1_vect) {
	PCintPort::curr = portC.portInputReg;
	portC.PCint();
}
#endif

#ifndef NO_PORTD_PINCHANGES
ISR(PCINT2_vect) {
	PCintPort::curr = portD.portInputReg;
	portD.PCint();
}
#endif
#endif // LIBCALL_OOPINCHANGEINT
