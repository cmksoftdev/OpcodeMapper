#include <iostream>
#define WINDOWS
#include "OpcodeMapper.h"

// Test

void main()
{
	BYTE buf[] = 
	{
		0xb8,                   // move value to EAX:
		0xF0, 0x00, 0x00, 0x00, // 240
		0x40,					// Increment by 1
		0xc3                    // return EAX
	};

	OpcodeMapper<int>* om1 = new OpcodeMapper<int>();
	om1->Map(buf, sizeof(buf));
	int i = om1->Invoke();

	OpcodeMapper<char>* om2 = new OpcodeMapper<char>(8192);
	om2->Map(buf, sizeof(buf));
	unsigned char c = om2->Invoke();
	c = om2->Invoke();
	c = om2->Invoke();

	std::system("PAUSE");
}