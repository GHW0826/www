#pragma once
#ifndef __SCOPEDLOCK__
#define __SCOPEDLOCK__

#include "CriticalSection.h"
#include "NonCopyable.h"

#include <cassert>

template<typename T>
class ScopedLock : private NonCopyable
{
public:
	using LockGuardCS = ScopedLock<CriticalSection>;

	explicit ScopedLock(T* mutex, bool initiallyLocked = true)
		: mutex_(mutex),
		looked_(false) {
		if (initiallyLocked == true)
			Lock();
	}	
	explicit ScopedLock(T& mutex, bool initiallyLocked = true)
		: mutex_(mutex),
		looked_(false) {
		if (initiallyLocked == true)
			Lock();
	}
	~ScopedLock(void) {
		if (locked_ == true)
			Unlock();
	}

	void Lock(void) {
		if (locked_ == true) {
			assert(!"[ScopedLock::Lock()] locked_ must be false.");
			throw "[ScopedLock::Lock()] locked_ must be false.";
		}

		mutex_.Lock();
		locked_ = true;
	}
	void Unlock(void) {
		if (locked_ == false) {
			assert(!"[ScopedLock::Unlock()] locked_ must be true.");
			throw "[ScopedLock::Unlock()] locked_ must be true.";
		}

		locked_ = false;
		mutex_.Unlock();
	}

	bool IsLocked(void) const { return locked_; }
private:
	T& mutex_;
	bool looked_;
};


#endif
