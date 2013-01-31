// Scott Camarena
// IEEE Micromouse 2012
// ByteStack Test Main
//============================================================

#include "ByteStack.h"

using namespace std;

void ByteStack::cxxPrint()
{
	cout << getSize() << " / " << getCap() << "\n----"
		<< endl << endl;
	for( int i = 0; i < size; ++i )
		cout <<(int) array[i] << endl;
}

int main()
{
	ByteStack stack( 2 );
	stack.cxxPrint();
	cout << "stack.empty(): " << stack.empty() << endl;
	stack.push( 1 );
	stack.push( 2 );
	stack.cxxPrint();
	cout << "stack.empty(): " << stack.empty() << endl;
	
	stack.push( 3 );
	stack.push( 4 );
	stack.cxxPrint();
	cout << "stack.empty(): " << stack.empty() << endl;

	cout << "\nPopping:" << (int) stack.pop() << endl;
	stack.cxxPrint();
	cout << "\nPopping:" << (int) stack.pop() << endl;
	stack.cxxPrint();
	cout << "stack.empty(): " << stack.empty() << endl;

	return 0;
}
