#pragma once
#ifndef XZESTL_UNINITIALIZED_H
#define XZESTL_UNINITIALIZED_H
//�����ڴ����������
#include "iterator.h"
#include "construct.h"

namespace xze {
	//pod���Ͳ�ȡ��ͬ����
	//�� [first, last) �ϵ����ݸ��Ƶ��� result Ϊ��ʼ���Ŀռ䣬���ظ��ƽ�����λ��
template<class InputIterator,class ForwardIterator>
ForwardIterator
uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
	return _uninitialized_copy_aux(first, last, result, std::is_trivially_copy_assignable<
		typename iterator_traits<ForwardIterator>::
		value_type>{});
}

template<class InputIterator, class ForwardIterator>
ForwardIterator
_uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, std::true_type) {
	return copy(first, last, result);
}

template<class InputIterator, class ForwardIterator>
ForwardIterator
_uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, std::false_type) {
	ForwardIterator cur = result;
	for (; first != last; ++first, ++cur) {
		construct(&*cur, *first);
	}
	return cur;
}


//x��ʾ��ֵ,�� [first, last) ���������Ԫ��ֵ
template<class ForwardIterator, class T>
ForwardIterator
uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x) {
	return _uninitialized_fill_aux(first, last, x, std::is_trivially_copy_assignable<
		typename iterator_traits<ForwardIterator>::
		value_type>{});
}
	
template<class ForwardIterator, class T>
inline void
_uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, std::true_type){
	fill(first,last,x);
}
template<class ForwardIterator, class T>
inline void
_uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, std::false_type) {
	ForwardIterator cur = first;
	for (; cur != last; ++cur) {
		construct(&*cur, x);
	}
}

//x��ʾ��ֵ,�� first λ�ÿ�ʼ����� n ��Ԫ��ֵ��������������λ��
template<class ForwardIterator, class Size,class T>
inline ForwardIterator
uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
	return _uninitialized_fill_aux(first, n, x, value_type(first));
}

template<class ForwardIterator, class Size, class T,class T1>
inline void
_uninitialized_fill_n(ForwardIterator first, Size n, const T& x,T1*) {
	return _uninitialized_fill_n_aux(first,n,x,std::is_trivially_copy_assignable<
	typename iterator_traits<ForwardIterator>::
	value_type>{});
}
template<class ForwardIterator, class Size, class T>
ForwardIterator
_uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, std::false_type) {
	ForwardIterator cur = first;
	for (; n>0; --n,++cur) {
		construct(&*cur, x);
	}
}

template<class ForwardIterator, class Size, class T>
ForwardIterator
_uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, std::true_type) {
	return fill_n(first, n, x);
}




}

#endif // !XZESTL_UINITIALIZED_H
