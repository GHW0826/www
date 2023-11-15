#pragma once
#ifndef __ITHREAD__
#define __ITHREAD__
#include <Windows.h>

class IThread
{
private:
	struct Impl;
	Impl* impl_;
public:
	bool Start(void) {}
	void Stop(void) {}

	virtual bool Suspend(void) = 0;
	virtual bool Resume(void) = 0;
	virtual void Terminate(void) = 0;
	virtual bool WaitFor(unsigned int timeout = INFINITE) = 0;

	virtual void SetFreeOnTerminate(bool value) = 0;
	virtual bool GetFreeOnTerminate(void) const = 0;
	virtual void SetFinished(bool value) = 0;
	virtual bool GetFinished(void) const = 0;
	virtual bool SetPriority(int value) = 0;
	virtual int GetPriority(void) = 0;
	virtual void SetSuspended(bool value) = 0;
	virtual bool GetSuspended(void) const = 0;

	virtual HANDLE GetHandle(void) const = 0;
	virtual unsigned int GetThreadID(void) const = 0;
protected:
	virtual bool Terminated(void) const = 0;
	virtual void SetThreadName(const char* name) = 0;
	virtual void SetReturnValue(int value) = 0;
	virtual int GetReturnValue(void) const = 0;

	virtual void Execute(void) = 0;

private:
	static unsigned int __stdcall InternalThreadProc(IThread* thread);
};

bool SetThreadName(unsigned int threadID, const char* name);

#endif __ITHREAD__
