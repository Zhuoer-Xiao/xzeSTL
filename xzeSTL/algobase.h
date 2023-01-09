#pragma once
#ifndef XZESTL_ALGOBASE_H
#define XZESTL_ALGOBASE_H
#include "iterator.h"
#include "util.h"
namespace xze {
//����������[first,last)������ȣ�����true
template<class InputIterator1,class InputIterator2>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
	for (; first1 != last1; ++first1, ++first2) {
		if (*first1 != *first2) {
			return false;
		}
	}
	return true;
}

template<class InputIterator1, class InputIterator2,class BinaryPredicate>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,BinaryPredicate binary_pred) {
	for (; first1 != last1; ++first1, ++first2) {
		if (!binary_pred( * first1 , *first2)) {
			return false;
		}
	}
	return true;
}
//��[first,last)������Ԫ�ظ�����ֵ
template<class ForwardIterator,class T>
void fill(ForwardIterator first, ForwardIterator last, const T& value) {
	for (; first != last; ++first) {
		*first = value;
	}
}
//��[first,last)ǰn����Ϊ��ֵ
template<class OutputIterator, class T,class Size>
void fill(OutputIterator first,Size n, const T& value) {
	for (; n>0;--n, ++first) {
		*first = value;
	}
}
//iter_swap������ForwardIterator��ָ����Ե�
template<class ForwardIterator1,class ForwardIteraor2>
inline void iter_swap(ForwardIterator1 a, ForwardIteraor2 b) {
	_iter_swap(a, b, value_type(a));
}
template<class ForwardIterator1, class ForwardIteraor2,class T>
inline void _iter_swap(ForwardIterator1 a, ForwardIteraor2 b,T*) {
	T tmp = *a;
	*a = *b;
	*b = tmp;
}
//�ֵ䷽ʽ���������н��бȽϣ�\
��һ����Ԫ�رȽ�С��true\
����last1δ����last2��true\
����last2δ����last1��false\
ͬʱ����last��false
template<class InputIterator1, class InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (*first1 < *first2) {
			return true;
		}
		if (*first2 < *first1) {
			return false;
		}
	}
	return first1 == last1 && first2 != last2;
}

template<class InputIterator1, class InputIterator2,class Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2,Compare comp) {
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (comp( * first1 , *first2)) {
			return true;
		}
		if (comp(*first2, *first1)) {  
			return false;
		}
	}
	return first1 == last1 && first2 != last2;
}
//���ԭ��ָ���ػ�
inline bool lexicographical_compare(const unsigned char* first1, const unsigned char* last1, const unsigned char* first2, const unsigned char* last2) 
{
	const size_t len1 = last1 - first1;
	const size_t len2 = last2 - first2;
	//�Ƚ���ͬ���ȵ�һ��
	const int result = memcmp(first1, first2, min(len1, len2));
	return result != 0 ? result < 0 : len1 < len2;
}
//max
template <class T>
inline const T& max(const T& a, const T& b) {
	return a < b ? b : a;
}
template <class T,class Compare>
inline const T& max(const T& a, const T& b,Compare comp) {
	return comp(a,b) ? b : a;
}
//min
template <class T>
inline const T& min(const T& a, const T& b) {
	return b < a ? b : a;
}
template <class T, class Compare>
inline const T& min(const T& a, const T& b, Compare comp) {
	return comp(b, a) ? b : a;
}
//�ҵ��������е�һ����ƥ���
template<class InputIteraotr1, class InputIteraotr2>
pair<InputIteraotr1, InputIteraotr2>mismatch(InputIteraotr1 first1, InputIteraotr1 last1, InputIteraotr2 first2) {
	while (first1 != last1 && *first1 == *first2) {
		++first1;
		++first2;
	}
	return pair<InputIteraotr1, InputIteraotr2>(first1, first2);
}

template<class InputIteraotr1, class InputIteraotr2,class BinaryPredicate>
pair<InputIteraotr1, InputIteraotr2>mismatch(InputIteraotr1 first1, InputIteraotr1 last1, InputIteraotr2 first2,BinaryPredicate binary_pred) {
	while (first1 != last1 && binary_pred(*first1 ,*first2)) {
		++first1;
		++first2;
	}
	return pair<InputIteraotr1, InputIteraotr2>(first1, first2);
}
//�����������������
template <class T>
inline void swap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}
template <class Tp, size_t N>
void swap(Tp(&a)[N], Tp(&b)[N])
{
	swap_range(a, a + N, b);
}

//copy dispatch����ȫ����������ָ���ػ����ڶ����װ
template<class InputIterator, class OutputIterator>
struct _copy_dispatch {
	OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
		return _copy(first, last, result, iterator_category(first));
	}
};
template<class T>
struct _copy_dispatch<T*,T*> {
	T* operator()(T* first, T* last, T* result) {
		typedef typename std::is_trivially_copy_assignable<T>::value t;
		return _copy_t(first, last, result, iterator_category(first),t());
	}
};
template<class T>
struct _copy_dispatch<const T*, T*> {
	T* operator()(const T* first, const T* last, T* result) {
		typedef typename std::is_trivially_copy_assignable<T>::value t;
		return _copy_t(first, last, result, iterator_category(first), t());
	}
};

template<class T>
inline T* _copy_t(const T* first, const T* last, T* result, std::true_type) {
	memmove(result, first, sizeof(T) * (last - first));
	return result + (last - first);
}
template<class T>
inline T* _copy_t(const T* first, const T* last, T* result, std::false_type) {
	return _copy_d(first,last,result,(ptrdiff_t*)0);
}

//�������װ
template<class InputIterator, class OutputIterator>
inline OutputIterator _copy(InputIterator first, InputIterator last, OutputIterator result,input_iterator_tag) {
	for (; first != last; ++result, ++first) {
		*result = *first;
	}
	return result;
};
template<class RandomAccessIterator, class OutputIterator>
inline OutputIterator _copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag) {
	return _copy_d(first,last,result,distance_type(first));
};
//���Ĳ��װ
template<class RandomAccessIterator, class OutputIterator,class Distance>
inline OutputIterator _copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*) {
	for (Distance n = last - first; n > 0; --n, ++result, ++first) {
		*result = *first;
	}
	return result;
};


//copy�ĵ�һ���װ
template<class InputIterator,class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
	return _copy_dispatch<InputIterator, OutputIterator>() (first, last, result);
}
//ָ������
inline char* copy(const char* first, const char* last, char* result) {
	memmove(result, first, last - first);
	return result + (last - first);
}
inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
	memmove(result, first, (last - first)*sizeof(wchar_t));
	return result + (last - first);
}
//todo
/*****************************************************************************************/
// copy_backward
// �� [first, last)�����ڵ�Ԫ�ؿ����� [result - (last - first), result)��
/*****************************************************************************************/
// unchecked_copy_backward_cat �� bidirectional_iterator_tag �汾
template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2
unchecked_copy_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
	BidirectionalIter2 result, bidirectional_iterator_tag)
{
	while (first != last)
		*--result = *--last;
	return result;
}

// unchecked_copy_backward_cat �� random_access_iterator_tag �汾
template <class RandomIter1, class BidirectionalIter2>
BidirectionalIter2
unchecked_copy_backward_cat(RandomIter1 first, RandomIter1 last,
	BidirectionalIter2 result, random_access_iterator_tag)
{
	for (auto n = last - first; n > 0; --n)
		*--result = *--last;
	return result;
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2
unchecked_copy_backward(BidirectionalIter1 first, BidirectionalIter1 last,
	BidirectionalIter2 result)
{
	return unchecked_copy_backward_cat(first, last, result,
		iterator_category(first));
}

// Ϊ trivially_copy_assignable �����ṩ�ػ��汾
template <class Tp, class Up>
typename std::enable_if<
	std::is_same<typename std::remove_const<Tp>::type, Up>::value&&
	std::is_trivially_copy_assignable<Up>::value,
	Up*>::type
	unchecked_copy_backward(Tp* first, Tp* last, Up* result)
{
	const auto n = static_cast<size_t>(last - first);
	if (n != 0)
	{
		result -= n;
		std::memmove(result, first, n * sizeof(Up));
	}
	return result;
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2
copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
{
	return unchecked_copy_backward(first, last, result);
}












}


#endif // !XZESTL_ALGOBASE_H