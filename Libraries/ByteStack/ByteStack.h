// Scott Camarena
// IEEE Micromouse 2012
// ByteStack Class
//============================================================

#ifndef _BYTESTACK_H_
#define _BYTESTACK_H_

#include "Arduino.h"

typedef unsigned char byte;

class ByteStack
{
	private:
		short int size;
		short int cap;
		byte* array;
	
	public:
		ByteStack( short int cp = 128 );
		~ByteStack();

		bool empty(){ return size == 0 ? true : false; }
		short int getSize(){ return size; }
		short int getCap(){ return cap; }
		void cxxPrint();

		void push( byte );
		byte pop();
		byte& top();
		void extendCap( short int newCap );

};

ByteStack::ByteStack( short int cp )
	: size( 0 ), cap(cp), array( (byte*) malloc(cp) )
{}


ByteStack::~ByteStack()
{
	free( array );
}

void ByteStack::push( byte val )
{
	if( size == cap ) extendCap( cap+64 );
	array[size++] = val;
}

byte ByteStack::pop()
{
	if( size != 0 )
		return array[--size];
	Serial.println("\n**ERROR: ByteStack - Invalid Pop**\n");
	return 255;
}

byte& ByteStack::top()
{
	return array[size-1];
}


void ByteStack::extendCap( short int newCap )
{
	byte* newArray = (byte*) malloc( newCap );
	for( short int i = size-1; i >= 0; --i )
		newArray[i] = array[i];
	
	free( array );
	array = newArray;
	cap = newCap;
}

















#endif
