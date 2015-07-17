#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <memory>

template <typename T>
class Singleton {
public:
	typedef T singleton_type;

public:
	template <typename... Args>
	static singleton_type* getInstance(Args&&... args);

	static void destroyInstance();

private:
	static std::unique_ptr<singleton_type> instance_;

};

#include "Singleton.inl"

#endif//__SINGLETON_H__