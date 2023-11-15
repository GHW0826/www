#pragma once
#ifndef __INTERLOCKED__
#define __INTERLOCKED__

#include "Types.h"

namespace Interlocked {
	int32 Increment(volatile int32* value);
	uint32 Increment(volatile uint32* value);

	int32 Decrement(volatile int32* value);
	uint32 Decrement(volatile uint32* value);

	int32 Exchange(volatile int32* dest, int32 value);
	uint32 Exchange(volatile uint32* dest, uint32 value);

	int32 CompareExchange(volatile int32* dest, int32 value, int32 comperand);
	uint32 CompareExchange(volatile uint32* dest, uint32 value, uint32 comperand);

#if defined (_WIN64) || defined(__LP64__)

	int64 Increment(volatile int64* value);
	uint64 Increment(volatile uint64* value);

	int64 Decrement(volatile int64* value);
	uint64 Decrement(volatile uint64* value);

	int64 Exchange(volatile int64* dest, int64 value);
	uint64 Exchange(volatile uint64* dest, uint64 value);

	int64 CompareExchange(volatile int64* dest, int64 value, int64 comperand);
	uint64 CompareExchange(volatile uint64* dest, uint64 value, uint64 comperand);

#endif // defined (_WIN64) || defined(__LP64__)
}


#endif // #ifndef __INTERLOCKED__