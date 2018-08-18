#pragma once

#include "EngineCore/EngineUtils.h"
#include <cstring>

#define GENERIC_MEMORY_SMALL_MEMSWAP_MAX 16

/*
 *	Helper struct to allow us to easily do manual memory management in a platform friendly way.
 *	Poggers
 *	
 *	I can't take all the credit for this, I drew a lot of help from the interwebz
 *	A lot of this is a reformatted copy/paste from Benny's GitHub
 **/
struct GenericMemory
{
	static FORCEINLINE void* Memmove(void* Destination, const void* Source, uint64 Amount)
	{
		return ::memmove(Destination, Source, Amount);
	}

	static FORCEINLINE int32 Memcmp(const void* Destination, const void* Source, uintptr Amount)
	{
		return ::memcmp(Destination, Source, Amount);
	}

	template<typename T>
	static FORCEINLINE void* Memset(void* InDestination, T Val, uintptr Amount)
	{
		auto* Destination = (T*)InDestination;
		uintptr AmountT = Amount / sizeof(T);
		uintptr remainder = Amount % sizeof(T);

		for (uintptr Index = 0; Index < AmountT; ++Index, ++Destination) 
		{
			Memcpy(Destination, &Val, sizeof(T));
		}

		Memcpy(Destination, &Val, remainder);
		return InDestination;
	}

	static FORCEINLINE void* Memzero(void* Destination, uintptr Amount)
	{
		return ::memset(Destination, 0, Amount);
	}

	static FORCEINLINE void* Memcpy(void* Destination, const void* Source, uintptr Amount)
	{
		return ::memcpy(Destination, Source, Amount);
	}

	/*
	 *	Swaps A and B
	 **/
	static void Memswap(void* A, void* B, uintptr Size)
	{
		if (Size <= GENERIC_MEMORY_SMALL_MEMSWAP_MAX)
		{
			SmallMemswap(A, B, Size);
		} 
		else 
		{
			BigMemswap(A, B, Size);
		}
	}

	template<typename T>
	static FORCEINLINE CONSTEXPR T Align(const T Ptr, uintptr Alignment)
	{
		return (T)(((intptr)Ptr + Alignment - 1) & ~(Alignment-1));
	}

	static void* Malloc(uintptr Amount, uint32 Alignment);
	static void* Realloc(void* Ptr, uintptr Amount, uint32 Alignment);
	static void* Free(void* Ptr);
	static uintptr GetAllocSize(void* Ptr);

private:

	static void BigMemswap(void* A, void* B, uintptr Size);
	static void SmallMemswap(void* A, void* B, uintptr Size)
	{
		assertCheck(Size <= GENERIC_MEMORY_SMALL_MEMSWAP_MAX);

		char _Data[GENERIC_MEMORY_SMALL_MEMSWAP_MAX];
		auto* _Temp = (void*)&_Data;
		GenericMemory::Memcpy(_Temp, A, Size);
		GenericMemory::Memcpy(A, B, Size);
		GenericMemory::Memcpy(B, _Temp, Size);
	}
};

template<>
FORCEINLINE void* GenericMemory::Memset(void* Destination, uint8 Val, uintptr Amount)
{
	return ::memset(Destination, Val, Amount);
}

