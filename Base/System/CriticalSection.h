#pragma once
#ifndef __CRITICALSECTION__
#define __CRITICALSECTION__

class CriticalSection
{
public:
	explicit CriticalSection(unsigned int spinCount = 4000) {}
	virtual	~CriticalSection(void) {};

	virtual void Enter(void) = 0;
	virtual bool TryEnter(void) = 0;
	virtual void Leave(void) = 0;

	virtual void Lock(void) = 0;
	virtual void Unlock(void) = 0;

	virtual unsigned long SetSpinCount(unsigned long spinCount) = 0;
private:
	CriticalSection(const CriticalSection&) {}
	const CriticalSection& operator=(const CriticalSection&) { return *this; }
private:
	struct Impl;
	Impl* impl_;
};

#endif 