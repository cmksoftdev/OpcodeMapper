#pragma once
#include <windows.h>
#include <vector>

using namespace std;

template <typename T>
class OpcodeMapper
{
public:
	OpcodeMapper();
	~OpcodeMapper();
	int Map(BYTE* opcode_buf, INT32 opcode_length);
	T Invoke();

private:
	BYTE* opcode_buf;
	INT32 opcode_length;
};


////////////////////////////////////////////////////////////////
//Templatemethods in header to avoid LNK2019////////////////////
////////////////////////////////////////////////////////////////

template <typename T>
OpcodeMapper<T>::OpcodeMapper()
{
	this->opcode_buf = (BYTE*)VirtualAllocEx(GetCurrentProcess(), 0, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE); // 4kbyte allocated. increase it if needed.
}

template <typename T>
OpcodeMapper<T>::~OpcodeMapper()
{
	free(opcode_buf);
}

// Copies opcode to executable memory
template <typename T>
int OpcodeMapper<T>::Map(BYTE * opcode_buf, INT32 opcode_length)
{
	if (this->opcode_buf == NULL)
		return -1;

	for (size_t i = 0; i < opcode_length; i++)
	{
		this->opcode_buf[i] = opcode_buf[i];
	}
	return 0;
}

template <typename T>
T OpcodeMapper<T>::Invoke()
{
	DWORD dummy;
	VirtualProtect(opcode_buf, opcode_length, PAGE_EXECUTE_READ, &dummy);
	auto const function_ptr = (T(*)())(opcode_buf);
	T result = function_ptr();
	VirtualFree(opcode_buf, 0, MEM_RELEASE);
	return result;
}