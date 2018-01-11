#include <iostream>
#include "OpcodeMapper.h"

void main()
{
	BYTE buf[] = 
	{
		0xb8,                   // move value to EAX:
		0xF0, 0x00, 0x00, 0x00, // 240
		0xc3                    // return EAX
	};

	OpcodeMapper<int>* om1 = new OpcodeMapper<int>();
	om1->Map(buf, 6);
	int i = om1->Invoke();

	OpcodeMapper<char>* om2 = new OpcodeMapper<char>();
	om2->Map(buf, 6);
	unsigned char c = om2->Invoke();

	std::system("PAUSE");
}