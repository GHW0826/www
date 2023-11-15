#pragma once
#ifndef __NONCOPYABLE__
#define __NONCOPYABLE__

class NonCopyable
{
protected:
	NonCopyable(void)
	{}
	~NonCopyable(void)
	{}
private:
	NonCopyable(const NonCopyable&);
	const NonCopyable& operator=(const NonCopyable&) {
		return *this;
	}
};

#endif