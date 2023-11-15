#pragma once
#ifndef __ATOMIC__
#define __ATOMIC__

#include "Interlocked.h"

template <typename T>
class Atomic
{
public:
	Atomic() 
		: n_()
	{}
	explicit Atomic(const T& v) {
		Interlocked::Increment((volatile int*)&n_, v);
	}
	// Copy construction 
	Atomic(const Atomic<T>& rhs) {
		T v = (T)rhs;
		Interlocked::Increment((volatile int*)&n_, v);
	}
	~Atomic() {}
private:
	volatile T n_;
};


template <>
class Atomic<bool>
{
public:
	Atomic() 
		: n_(false)
	{}
	explicit Atomic(const bool& v) {
		Interlocked::Exchange((volatile int32*)&n_, v);
	}
	// Copy construction 
	Atomic(const Atomic<bool>& rhs) {
		bool v = (bool)rhs;
		Interlocked::Exchange((volatile int*)&n_, (long)v);
	}
	~Atomic()
	{}

	// Assignment with a raw value 
	const Atomic& operator=(const bool& v) {
		Interlocked::Exchange((volatile int*)&n_, (long)v);
		return *this;
	}
	operator bool() const {
		return (Interlocked::CompareExchange((volatile int*)&n_, n_, n_) != 0);
	}
	// Comparison of raw values
	bool operator==(const bool& v) {
		bool ev = (Interlocked::CompareExchange((volatile int*)&n_, n_, n_) != 0);
		return (ev == v);
	}
private:
	volatile bool n_;
};

#endif
