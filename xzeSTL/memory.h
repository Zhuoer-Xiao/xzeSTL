#pragma once
#ifndef XZESTL_MEMORY_H
#define XZESTL_MEMORY_H
namespace xze {
template<class T>
class auto_ptr {
public:
	explicit auto_ptr(T*p=0):pointee(p){}
	template<class U>
	auto_ptr(auto_ptr<U>&rhs):pointee(rhs.release()){}
	~auto_ptr() { delete pointee; }


	auto_ptr<T>& operator=(auto_ptr& rhs) {
		if (this != &rhs)reset(rhs.release());
		return *this;
	}

	template<class U>
	auto_ptr<T>& operator=(auto_ptr<U>& rhs) {
		if (this->get() != &rhs.get())reset(rhs.release());
		return *this;
	}
	T* operator*()const { return *pointee; }
	T* operator->()const { return pointee; }
	T* get()const { return pointee; }
	T* release() {
		T* tmp = pointee;
		pointee = nullptr;
		return tmp;
	}
	void reset(T* p = nullptr)
	{
		if (pointee != p)
		{
			delete pointee;
			pointee = p;
		}
	}
private:
	T* pointee;
};
}

#endif // !XZESTL_MEMORY_H
