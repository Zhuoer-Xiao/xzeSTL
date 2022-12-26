#pragma once
#ifndef XZESTL_UNINITIALIZED_H
#define XZESTL_UNINITIALIZED_H
//定义内存基本处理工具
#include "iterator.h"
#include "construct.h"

namespace xze {
	//pod类型采取不同策略
	//把 [first, last) 上的内容复制到以 result 为起始处的空间，返回复制结束的位置
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


//x表示初值,在 [first, last) 区间内填充元素值
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

//x表示初值,从 first 位置开始，填充 n 个元素值，返回填充结束的位置
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
