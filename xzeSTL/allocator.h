#pragma once
#ifndef XZESTL_ALLOCATOR_H
#define XZESTL_ALLOCATOR_H
//空间配置器,只是new和delete的薄薄包装，效率底下
#include <cstddef>
#include <stdlib.h>
#include <iostream>
using namespace std;
//配置空间
template<class T>
inline T* allocate(ptrdiff_t size,T*) {
	//set_new_handle(0);
	T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
	if (tmp == 0) {
		cerr << "out of memory" << endl;
		exit(1);
	}
	return tmp;
}
//归还空间
template<class T>
inline void deallocate(T* buffer) {
	::operator delete(buffer);
}
template<class T>
class allocator {
public:
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	pointer allocate(size_type n) {
		return ::allocate((difference_type)n, (pointer)0);
	}
	void deallocate(pointer p) {
		::deallocate(p);
	}
	pointer address(reference x) {
		return (pointer)&x;
	}
	const_pointer const_address(const_reference x) {
		return (const_reference)&x;
	}
	size_type init_page_size() {
		return max(size_type(1), size_type(4096 / sizeof(T)));
	}
	size_type max_size() const {
		return max(size_type(1), size_type(UINT_MAX / sizeof(T)));
	}

	
	/*class allocator<void> {
	public:
		typedef void* pointer;
	};*/
};








#endif 