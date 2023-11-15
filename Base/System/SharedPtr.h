#pragma once
#ifndef __SHARDPTR__
#define __SHARDPTR__

#include "Types.h"
#include "Interlocked.h"

#include <algorithm>
#include <cassert>

template<typename T>
class SharedPtr
{
private:
	// 모든 템플릿 인스턴스끼리 친구로 지정
	template<typename U> friend class SharedPtr;
public:
	SharedPtr(void)
		: p_(NULL),
		count_(static_cast<unsigned int*>(MPHeap::Alloc(nullptr, sizeof(uint32)))) {
		if (count_ == nullptr) {
			throw std::bad_alloc();
		}
		*count_ = 1;
	}
	explicit SharedPtr(T* p)
		: p_(p),
		count_(static_cast<unsigned int*>(MPHeap::Alloc(nullptr, sizeof(uint32)))) {
		if (count_ == nullptr) {
			throw std::bad_alloc();
		}
		*count_ = 1;
	}
	SharedPtr(const SharedPtr& rhs)
		: p_(rhs.p_),
		count_(rhs.count_) {
		Interlocked::Increment(rhs.count_);
	}
	template<typename U>
	SharedPtr(const SharedPtr<U>& rhs)
		: p_(rhs.p_),
		count_(rhs.count_) {
		Interlocked::Increment(rhs.count_);
	}
	~SharedPtr(void) {
		if (Interlocked::Decrement(count_) == 0) {
			// MPHeap::Free(NULL, count_);
			count_ = nullptr;

			if (p_) {
				delete p_;
				p_ = nullptr;
			}
		}
	}

	// Enables "if (!sp) ..."
	bool operator!() const	{ return p_ == nullptr; }
	T* operator->() const	{ return p_; }
	T& operator*() const	{ return *p_; }
	friend bool operator==(const SharedPtr<T>& rhs, const SharedPtr<T>& lhs) {
		return (rhs.GetRawPtr() == lhs.GetRawPtr());
	}
	SharedPtr& operator=(const SharedPtr& rhs) {
		SharedPtr(rhs).Swap(*this);
		return *this;
	}
	template<typename U>
	SharedPtr& operator=(const SharedPtr<U>& rhs) {
		SharedPtr(rhs).Swap(*this);
		return *this;
	}

	template<typename U>
	void Reset(U* p) {
		assert(p == 0 || p != p_); // catch self-reset errors
		SharedPtr(p).Swap(*this);
	}
	void Reset(void) {
		SharedPtr().Swap(*this);
	}

	void Swap(SharedPtr& rhs) {
		std::swap(p_, rhs.p_);
		std::swap(count_, rhs.count_);
	}

	SharedPtr<T> lock() const {
		SharedPtr<T> result;
		if (p_ != nullptr) {
			int old_count;
			do {
				old_count = *count_;
				if (old_count == 0) {
					break;
				}
			} while (old_count != Interlocked::CompareExchange(count_, old_count + 1, old_count));

			if (old_count > 0) {
				result.p_ = p_;
				result.count_ = count_;
			}
		}
		return result;
	}

	T* GetRawPtr(void) const { return p_; }
private:
	T* p_;
	uint32* count_;
};

#endif
