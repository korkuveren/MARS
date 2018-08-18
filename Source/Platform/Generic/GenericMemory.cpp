#include "GenericMemory.h"
#include "Math/Math.h"
#include <cstdlib>
#include <stdio.h>

void* GenericMemory::Malloc(uintptr Amount, uint32 Alignment)
{
	Alignment = Math::Max(Amount >= 16 ? 16u : 8u, Alignment);
	void* _ptr = ::malloc(Amount + Alignment + sizeof(void*) + sizeof(uintptr));
	void* _Result = Align((uint8*)_ptr + sizeof(void*) + sizeof(uintptr), (uintptr)Alignment);
	*((void**)((uint8*)_Result - sizeof(void*))) = _ptr;
	*((uintptr*)((uint8*)_Result - sizeof(void*) - sizeof(uintptr))) = Amount;

	return _Result;
}

void* GenericMemory::Realloc(void* Ptr, uintptr Amount, uint32 Alignment)
{
	Alignment = Math::Max(Amount >= 16 ? 16u : 8u, Alignment);
	if (Ptr == nullptr) 
	{
		return GenericMemory::Malloc(Amount, Alignment);
	}

	if (Amount == 0) 
	{
		GenericMemory::Free(Ptr);

		return nullptr;
	}

	void* _Result = Malloc(Amount, Alignment);
	uintptr _Size = GenericMemory::GetAllocSize(Ptr);
	GenericMemory::Memcpy(_Result, Ptr, Math::Min(_Size, Amount));
	Free(Ptr);

	return _Result;
}

void* GenericMemory::Free(void* Ptr)
{
	if (Ptr) 
	{
		::free(*((void**)((uint8*)Ptr - sizeof(void*))));
	}

	return nullptr;
}

uintptr GenericMemory::GetAllocSize(void* Ptr)
{
	return *((uintptr*)((uint8*)Ptr - sizeof(void*) - sizeof(uintptr)));
}

void GenericMemory::BigMemswap(void* A, void* B, uintptr Size)
{
	uint64* _ptr1 = (uint64*)A;
	uint64* _ptr2 = (uint64*)B;
	while (Size > GENERIC_MEMORY_SMALL_MEMSWAP_MAX) 
	{
		uint64 _Temp = *_ptr1;
		*_ptr1 = *_ptr2;
		*_ptr2 = _Temp;
		Size -= 8;
		_ptr1++;
		_ptr2++;
	}
	SmallMemswap(_ptr1, _ptr2, Size);
}
