#pragma once
#ifndef __SCOPEDRWLOCK__
#define __SCOPEDRWLOCK__

#include "NonCopyable.h"
#include "RWCriticalSection.h"

#include <cassert>

enum class RWLockState
{
	Unlocked = 0,
	ReadLocked = 1,
	WriteLocked = 2
};

template<typename T>
class ScopedRWLock : private NonCopyable
{
public:
	using LockGuardRWCS = ScopedRWLock<RWCriticalSection>;
public:
	ScopedRWLock(T* mutex, RWLockState initialState)
		: mutex_(*mutex),
		state_(RWLockState::Unlocked) {
		if (initialState == RWLockState::ReadLocked) {
			ReadLock();
		}
		else if (initialState == RWLockState::WriteLocked) {
			WriteLock();
		}
	}
	ScopedRWLock(T& mutex, RWLockState initialState)
		: mutex_(mutex),
		state_(RWLockState::Unlocked) {
		if (initialState == RWLockState::ReadLocked) {
			ReadLock();
		}
		else if (initialState == RWLockState::WriteLocked) {
			WriteLock();
		}
	}
	~ScopedRWLock(void) {
		if (state_ != RWLockState::Unlocked) {
			Unlock();
		}
	}


	void ReadLock(void) {
		if (state_ != RWLockState::Unlocked) {
			assert(!"[ScopedRWLock::ReadLock()] state_ must be ScopedRWLockState::Unlocked.");
			throw "[ScopedRWLock::ReadLock()] state_ must be ScopedRWLockState::Unlocked.";
		}

		mutex_.ReadLock();
		state_ = RWLockState::ReadLocked;
	}
	void WriteLock(void) {
		if (state_ != RWLockState::Unlocked) {
			assert(!"[ScopedRWLock::WriteLock()] state_ must be ScopedRWLockState::Unlocked.");
			throw "[ScopedRWLock::WriteLock()] state_ must be ScopedRWLockState::Unlocked.";
		}

		mutex_.WriteLock();
		state_ = RWLockState::WriteLocked;
	}
	void Unlock(void) {
		if (state_ == RWLockState::Unlocked) {
			assert(!"[ScopedRWLock::Unlock()] state_ must not be ScopedRWLockState::Unlocked.");
			throw "[ScopedRWLock::Unlock()] state_ must not be ScopedRWLockState::Unlocked.";
		}

		if (state_ == RWLockState::ReadLocked) {
			mutex_.ReadUnlock();
		}
		else if (state_ == RWLockState::WriteLocked) {
			mutex_.WriteUnlock();
		}
	}

	bool IsLocked(void)			const	{ return state_ != RWLockState::Unlocked; }
	bool IsReadLocked(void)		const	{ return state_ == RWLockState::ReadLocked; }
	bool IsWriteLocked(void)	const	{ return state_ == RWLockState::WriteLocked; }
	RWLockState GetState(void)	const	{ return state_; }
private:
	T& mutex_;
	RWLockState state_;
};

#endif // #ifndef __SCOPEDRWLOCK__
