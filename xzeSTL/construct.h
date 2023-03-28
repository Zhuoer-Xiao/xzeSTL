#pragma once
#ifndef XZESTL_CONSTRUCT_H
#define XZESTL_CONSTRUCT_H
//�����������Ļ������ߣ������Ǵ�������ʱ���������ڴ棬�ٹ������ɾ������ʱ��������������ɾ���ڴ�
#include<new>
#include"iterator.h"
namespace xze {
	template<class T1, class T2>
	inline void construct(T1* p, const T2& value) {
		new(p) T1(value);
	}
	template<class T>
	inline void construct(T* ptr) {
		new((void)*ptr) T();
	}
	template <class T, class... Args>
	void construct(T* ptr, Args&&... args)
	{
		new ((void*)ptr) T(forward<Args>(args)...);
	}



	template<class T>
	inline void destroy(T* pointer) {
		pointer->~T();
	}

	template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last) {
		_destroy(first, last, value_type(first));
	}
	//������ȡ�����Ƿ���trivial destructorѡ��汾
	template<class ForwardIterator, class T>
	inline void _destroy(ForwardIterator first, ForwardIterator last, T*) {
		_destroy_aux(first, last, std::is_trivially_destructible<typename iterator_traits<ForwardIterator>::value_type>{});
	}


	template<class ForwardIterator>
	inline void _destroy_aux(ForwardIterator first, ForwardIterator last, std::true_type) {
		
	}

	template<class ForwardIterator>
	inline void _destroy_aux(ForwardIterator first, ForwardIterator last, std::false_type) {
		for (; first < last; ++first) {
			destroy(&*first);
		}
	}

	inline void destroy(char*,char*) {

	}

	inline void destroy(wchar_t*, wchar_t*) {

	}
}
#endif 
