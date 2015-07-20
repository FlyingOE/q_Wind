#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <memory>
#include <mutex>

template <typename T>
class Singleton {
public:
	typedef T singleton_type;

public:
	template <typename... Args>
	static singleton_type* getInstance(Args&&... args);

	static void destroyInstance();

private:
	static std::mutex mutex_;
	static std::unique_ptr<singleton_type> instance_;

	typedef std::lock_guard<std::mutex> lock_guard;
};

#include "Singleton.inl"

#endif//__SINGLETON_H__