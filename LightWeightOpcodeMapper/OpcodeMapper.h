#pragma once

#if defined(WINDOWS)  
	#include <windows.h>
	#define valloc VirtualAllocEx
	#define vfree VirtualFree
#elif defined(LINUX)  
	#include <sys/mman.h>
	#define valloc mmap
	#define vfree munmap
#endif

#include <vector>

using namespace std;

template <typename T>
class OpcodeMapper
{
public:
	OpcodeMapper();
	OpcodeMapper(INT32 allocated_memory);
	~OpcodeMapper();
	int Map(BYTE* opcode_buf, INT32 opcode_length);
	T Invoke();

private:
	BYTE* opcode_buf;
	INT32 opcode_length;
	INT32 allocated_memory;
};


////////////////////////////////////////////////////////////////
//Templatemethods in header to avoid LNK2019////////////////////
////////////////////////////////////////////////////////////////

template <typename T>
OpcodeMapper<T>::OpcodeMapper()
{
	allocated_memory = 4096;
}

template <typename T>
OpcodeMapper<T>::OpcodeMapper(INT32 allocated_memory)
{
	if (allocated_memory < 4096)
		throw;

	this->allocated_memory = allocated_memory;
}

template <typename T>
OpcodeMapper<T>::~OpcodeMapper()
{
	if (opcode_buf!=NULL)
		vfree(opcode_buf, 0, MEM_RELEASE);
}

// Copies opcode to executable memory
template <typename T>
int OpcodeMapper<T>::Map(BYTE * opcode_buf, INT32 opcode_length)
{
	if (opcode_buf == NULL || opcode_length > allocated_memory)
		return -1;

	this->opcode_buf = (BYTE*)valloc(GetCurrentProcess(), 0, allocated_memory, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	for (size_t i = 0; i < opcode_length; i++)
		this->opcode_buf[i] = opcode_buf[i];

	return 0;
}

template <typename T>
T OpcodeMapper<T>::Invoke()
{
	if (opcode_buf == NULL)
		throw;

	DWORD dummy;
	VirtualProtect(opcode_buf, opcode_length, PAGE_EXECUTE_READ, &dummy);
	auto const function_ptr = (T(*)())(opcode_buf);
	T result = function_ptr();
	
	return result;
}