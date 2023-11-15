#pragma once
#ifndef __RECRITICALSECTION__
#define __RECRITICALSECTION__

class RWCriticalSection
{
public:
	RWCriticalSection(void);
	virtual ~RWCriticalSection(void);

	void ReadLock(void);
	void ReadUnlock(void);
	void WriteLock(void);
	void WriteUnlock(void);
private:
	void InternalUnlock(void);
private:
	RWCriticalSection(const RWCriticalSection&) {}
	const RWCriticalSection& operator=(const RWCriticalSection&) { return *this; };
private:
	struct Impl;
	Impl* impl_;
};

#endif // #ifndef __RECRITICALSECTION__
