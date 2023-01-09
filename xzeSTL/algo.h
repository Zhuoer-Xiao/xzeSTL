#pragma once
#ifndef XZESTL_ALGO_H
#define XZESTL_ALGO_H
#include "iterator.h"
#include "heap.h"
#include "util.h"
#include "algobase.h"
#include "adaptor.h"
#include "function.h"
namespace xze {
const int _stl_threshold = 16;
//�ҵ���һ����������������Ԫ��
template<class ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) {
    if (first == last)return last;
    ForwardIterator next = first;
    while (++next != last) {
        if (*first == *next)return first;
        first = next;
    }
    return last;
}
template<class ForwardIterator,class BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate binary_pred) {
    if (first == last)return last;
    ForwardIterator next = first;
    while (++next != last) {
        if (binary_pred(*first, *next))return first;
        first = next;
    }
    return last;
}
//count����
template<class InputIterator,class T>
typename iterator_traits<InputIterator>::difference_type
count(InputIterator first, InputIterator last, const T& value) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    for (; first != last; ++first) {
        if (*first == value) {
            ++n;
        }
    }
    return n;
}
template<class InputIterator, class Predicate>
typename iterator_traits<InputIterator>::difference_type
count_if(InputIterator first, InputIterator last, Predicate pred) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    for (; first != last; ++first) {
        if (pred( * first)) {
            ++n;
        }
    }
    return n;
}
//���������ĵ�һ��
template<class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last, const T& value) {
    while (first != last && *first != value)++first;
    return first;
}
template<class InputIterator, class Predicate>
InputIterator find_if(InputIterator first, InputIterator last, Predicate pred) {
    while (first != last && !pred(*first))++first;
    return first;
}
//������1���ҵ�����2�����λ��
template<class ForwardIterator1,class ForwardIterator2>
inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2) {
    typedef typename iterator_traits<ForwardIterator1>::iterator_category category1;
    typedef typename iterator_traits<ForwardIterator2>::iterator_category category2;
    return _find_end(first1, last1, first2, last2, category1(), category2());
}
//ForwardIterator���ػ�
template<class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1 _find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2,forward_iterator_tag, forward_iterator_tag) {
    if (first2 == last2) {
        return last1;//����Ŀ���
    }
    else {
        ForwardIterator1 result = last1;
        while (true) {
            ForwardIterator1 new_result = search(first1, last1, first2, last2);
            if (new_result == last1) {
                return result;//û�ҵ�
            }
            else {
                result = new_result;//����ƶ�
                first1 = new_result;
                ++first1;
            }
        }
    }
}
//bidirectional iterator��
template<class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator1 _find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1, BidirectionalIterator2 first2, BidirectionalIterator2 last2, bidirectional_iterator_tag, bidirectional_iterator_tag) {
    typedef reverse_iterator<BidirectionalIterator1> reviter1;
    typedef reverse_iterator<BidirectionalIterator2> reviter2;
    reviter1 rlast1(first1);
    reviter1 rlast2(first2);
    reviter1 rresult = search(reviter1(last1), rlast1, reviter2(last2), rlast2);
    if (rresult == rlast1) { 
        return last1;
    }
    else {
        BidirectionalIterator1 result = rresult.base();
        advance(result, -distance(first2 - last2));
        return result;
    }
}
//�ҵ�[first2,last2)��[first1,last1)�����һ�γ��ֵص�
template<class InputIterator,class ForwardIterator>
InputIterator find_first_of(InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2)
{
    for (; first1 != last1; ++first1) {
        for (ForwardIterator iter = first2; iter != last2; ++iter) {
            if (*first1 == *iter) {
                return first1;
            }
        }
    }
    return last1;
}
template<class InputIterator, class ForwardIterator,class BinaryPredicate>
InputIterator find_first_of(InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2,BinaryPredicate comp)
{
    for (; first1 != last1; ++first1) {
        for (ForwardIterator iter = first2; iter != last2; ++iter) {
            if (comp(*first1 ,*iter) ) {
                return first1;
            }
        }
    }
    return last1;
}
//ÿ��Ԫ�ؽ��в���
template<class InputIterator,class Function>
Function for_each(InputIterator first, InputIterator last, Function f) {
    for (; first != last; ++first) {
        f(*first);
    }
    return f;
}
//ÿ��Ԫ�ؽ������㲢д���
template<class ForwardIterator, class Generator>
void generate(ForwardIterator first, ForwardIterator last, Generator gen) {
    for (; first != last; ++first) {
        *first=gen();
    }
    return first;
}
template<class OutputIterator, class Generator,class Size>
void generate_n(OutputIterator first, Size n, Generator gen) {
	for (; n>0; --n,++first) {
		*first = gen();
	}
	return first;
}
//S2��ÿ��Ԫ�ض���S1��,S2��S1������
template<class InputIterator1,class InputIterator2>
bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
{
    while (first1 != last1 && first2 != last2) {
        if (*first2 < *first1) {
            return false;
        }
        else if(*first1<*first2) {
            ++first1;
        }
        else {
            ++first1, ++first2;
        }
    }
    return first2 == last2;
}

template<class InputIterator1, class InputIterator2,class Compare>
bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2,Compare comp)
{
    while (first1 != last1 && first2 != last2) {
        if (comp(*first2 , *first1)) {
            return false;
        }
        else if (comp(*first1, *first2)) {
            ++first1;
        }
        else {
            ++first1, ++first2;
        }
    }
    return first2 == last2;
}
//�����������/СԪ��
template<class ForwardIterator>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last) {
    if (first == last)return first;
    ForwardIterator result = first;
    while (++first != last) {
        if (*result < *first) {
            result = first;
        }
    }
    return result;
}

template<class ForwardIterator,class Compare>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp) {
    if (first == last)return first;
    ForwardIterator result = first;
    while (++first != last) {
        if (comp( *result , *first)) {
            result = first;
        }
    }
    return result;
}
template<class ForwardIterator>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last) {
    if (first == last)return first;
    ForwardIterator result = first;
    while (++first != last) {
        if (*first < *result) {
            result = first;
        }
    }
    return result;
}

template<class ForwardIterator, class Compare>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare comp) {
    if (first == last)return first;
    ForwardIterator result = first;
    while (++first != last) {
        if (comp(*first, *result)) {
            result = first;
        }
    }
    return result;
}
//�����򼯺Ϻϲ�
template<class InputIterator1, class InputIterator2,class OutputIterator>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result)
{
    while (first1 != last1 && first2 != last2) {
        if (*first2 < *first1) {
            *result = *first2;
            ++first2;
        }
        else {
            *result = *first1;
            ++first1;
        }
        ++result;
    }
    return copy(first2, last2, copy(first1, last1, result));
}
template<class InputIterator1, class InputIterator2, class OutputIterator,class Compare>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result,Compare comp)
{
    while (first1 != last1 && first2 != last2) {
        if (comp(*first2 , *first1)) {
            *result = *first2;
            ++first2;
        }
        else {
            *result = *first1;
            ++first1;
        }
        ++result;
    }
    return copy(first2, last2, copy(first1, last1, result));
}
template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter1
merge_backward(BidirectionalIter1 first1, BidirectionalIter1 last1,
	BidirectionalIter2 first2, BidirectionalIter2 last2,
	BidirectionalIter1 result)
{
	if (first1 == last1)
		return copy_backward(first2, last2, result);
	if (first2 == last2)
		return copy_backward(first1, last1, result);
	--last1;
	--last2;
	while (true)
	{
		if (*last2 < *last1)
		{
			*--result = *last1;
			if (first1 == last1)
				return copy_backward(first2, ++last2, result);
			--last1;
		}
		else
		{
			*--result = *last2;
			if (first2 == last2)
				return copy_backward(first1, ++last1, result);
			--last2;
		}
	}
}



//��[first,last)�е�Ԫ���������У���һԪ����pred�ж�Ϊtrue��Ԫ�ط���ǰ��
template<class BidirectionalIterator,class Predicate>
BidirectionalIterator partition(BidirectionalIterator first, BidirectionalIterator last, Predicate pred)
{
    while (true) {
        while (true) {
            if (first == last) {
                return first;
            }
            else if (pred(*first)) {
                ++first;
            }
            else {
                break;
            }
        }
        --last;
        while (true) {
            if (first == last) {
                return first;
            }
            else if (pred(*last)) {
                --last;
            }
            else {
                break;
            }
        }
        iter_swap(first, last);
        ++first;
    }
}
//�Ƴ�����ɾ�������ں���
template<class ForwardIterator,class T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& value)
{
    first = find(first, last, value);
    ForwardIterator next = first;
    return first == last ? first : remove_copy(++next, last, first, value);
}
template<class InputIterator, class OutputIterator,class T>
OutputIterator remove_copy(InputIterator first, InputIterator last, OutputIterator result,const T& value)
{
    for (; first != last; ++first) {
        if (*first != value) {
            *result = *first;
            ++result;
        }
    }
    return result;
}

template<class ForwardIterator, class Predicate>
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, Predicate pred)
{
    first = find_if(first, last, pred);
    ForwardIterator next = first;
    return first == last ? first : remove_copy_if(++next, last, first, pred);
}
template<class InputIterator, class OutputIterator, class Predicate>
OutputIterator remove_copy_if(InputIterator first, InputIterator last, OutputIterator result, Predicate pred)
{
    for (; first != last; ++first) {
        if (!pred(*first)) {
            *result = *first;
            ++result;
        }
    }
    return result;
}

//��[first,last)�����е�oldvalue�滻Ϊnewvalue
template<class ForwardIterator,class T>
void replace(ForwardIterator first, ForwardIterator last, const T& old_value, const T& new_value) {
    for (; first != last; ++first) {
        if (*first == old_value)*first = *new_value;
    }
}
//�����д浽result
template<class InputIterator,class OutputIterator,class T>
OutputIterator replace_copy(InputIterator first, InputIterator last, OutputIterator result, const T& old_value, const T& new_value) {
    for (; first != last; ++first,++result) {
        *result = *first == old_value ? new_value : *first;
    }
    return result;
}
template<class ForwardIterator, class T,class Predicate>
void replace_if(ForwardIterator first, ForwardIterator last, Predicate pred, const T& new_value) {
    for (; first != last; ++first) {
        if (pred(* first))*first = *new_value;
    }
}

template<class InputIterator, class OutputIterator, class T,class Predicate>
OutputIterator replace_copy_if(InputIterator first, InputIterator last, OutputIterator result, Predicate pred, const T& new_value) {
    for (; first != last; ++first, ++result) {
        *result = pred( * first) ? new_value : *first;
    }
    return result;
}
//��[first,last)��Ԫ�صߵ�
template<class BidirectionalIterator>
inline void reverse(BidirectionalIterator first, BidirectionalIterator last) {
    _reverse(first, last, iterator_category(first));
}
//������ֵ������ֱ�ʵ��
template<class BidirectionalIterator>
void _reverse(BidirectionalIterator first, BidirectionalIterator last,bidirectional_iterator_tag) {
    while (true) {
        if (first == last || first == --last) {
            return;
        }
        else {
            iter_swap(first++, last);
        }
    }
}
template<class RandomAccessIterator>
void _reverse(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
    while (first < last) {
        iter_swap(first++, --last);
    }
}
template<class BidirectionalIterator,class OutputIterator>
OutputIterator reverse_copy(BidirectionalIterator first, BidirectionalIterator last, OutputIterator result) {
    while (first != last) {
        --last;
        *result = *last;
        ++result;
    }
    return result;
}
//����[first,middle),[middle,last)Ԫ��
template<class ForwardIterator>
inline void rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last) {
    if (first == middle || middle == last)return;
    _rotate(first, middle, last, distance_type(first), iterator_category(first));
}
//forward
template<class ForwardIterator,class Distance>
void _rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last, Distance*, forward_iterator_tag)
{
    for (ForwardIterator i = middle;;) {
        iter_swap(first,i);
        ++first;
        ++i;
        if (first == middle) {
            //ǰ�����
            if (i == last)return;//���Ҳ����������
            middle = i;
        }
        else if (i == last) {
            i = middle;
        }
    }
}
template<class BidirectionalIterator, class Distance>
void _rotate(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, Distance*, bidirectional_iterator_tag) {
    reverse(first, middle);
    reverse(middle, last);
    reverse(first, last);
}
//todo:rotate��random�汾
template<class RandomAccessIterator,class Distance>
void _rotate(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Distance*, random_access_iterator_tag) {
    //ȡȫ����ǰ�γ��ȵ��������
    Distance n = gcd(last - first, middle - first);
    while (n--) {
        _rotate_cycle(first, last, first + n, middle - first, value_type(first));
    }
}
//�������,շת�����
template<class EuclideanRingElement>
EuclideanRingElement gcd(EuclideanRingElement m, EuclideanRingElement n) {
    while (n != 0) {
        EuclideanRingElement t = m % n;
        m = n;
        n = t;
    }
    return m;
}
template<class RandomAccessIterator,class Distance,class T>
void _rotate_cycle(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator initial, Distance shift, T*)
{
    T value = *initial;
    RandomAccessIterator ptr1 = initial;
    RandomAccessIterator ptr2 = ptr1 + shift;
    while (ptr2 != initial) {
        *ptr1 = *ptr2;
        ptr1 = ptr2;
        if (last - ptr2 > shift) {
            ptr2 += shift;
        }
        else {
            ptr2 = first + (shift - (last - ptr2));
        }
    }
    *ptr1 = value;
}
template<class ForwardIterator,class OutputIterator>
OutputIterator rotate_copy(ForwardIterator first, ForwardIterator middle, ForwardIterator last, OutputIterator result)
{
	return copy(first, middle, copy(middle, last, result));
}
//����1�в�������2�����״εص�
template<class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2)
{
	return _search(first1, last1, first2, last2, distance_type(first1), distance_type(first2));
}
template<class ForwardIterator1, class ForwardIterator2,class Distance1,class Distance2>
ForwardIterator1 _search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, Distance1*, Distance2*) {
	Distance1 d1 = 0;
	d1=distance(first1, last1);
	Distance2 d2 = 0;
	d2 = distance(first2, last2);
	//1<2
	if (d1 < d2)return last1;
	ForwardIterator1 current1 = first1;
	ForwardIterator2 current2 = first2;
	while (current2 != last2) {
		if (*current1 == *current2) {
			++current1;
			++current2;
		}
		else {
			if (d1 == d2) {
				return last1;
			}
			else {
				current1 = ++first1;
				current2 = first2;
				--d1;
			}
		}
	}
	return first1;
}
//������������n��
template<class ForwardIterator,class Integer,class T>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Integer count, const T&value) {
	if (count <= 0) { 
		return first; 
	}
	else {
		first = find(first, last, value);
		while (first != last) {
			Integer n = count - 1;
			ForwardIterator i = first;
			++i;
			while (i != last && n != 0 && *i == value) {
				++i;
				--n;
			}
			if (n == 0) {
				return first;
			}
			else {
				first = find(first, last, value);
			}
		}
		return last;
	}
}
template<class ForwardIterator, class Integer, class T,class BinaryPredicate>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Integer count, const T&value,BinaryPredicate binary_pred) {
	if (count <= 0) {
		return first;
	}
	else {
		while (first != last) {
			if (binary_pred(*first, value))break;
			++first;
		}
		while (first != last) {
			Integer n = count - 1;
			ForwardIterator i = first;
			++i;
			while (i != last && n != 0 && binary_pred(*i, value)) {
				++i;
				--n;
			}
			if (n == 0) {
				return first;
			}
			else {
				while (i != last) {
					if (binary_pred(*i, value))break;
					++i;
				}
				first = i;
			}
		}
		return last;
	}
}
//swap_ranges����������Ԫ�ػ���
template<class ForwardIterator1, class ForwardIterator2>
ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2) {
	for (; first1 != last1; ++first1, ++first2) {
		iter_swap(first1, first2);
	}
	return first2;
}
//����������ÿ������
template<class InputIterator,class OutputIterator,class UnaryOperation>
OutputIterator transform(InputIterator first, InputIterator last, OutputIterator result, UnaryOperation op)
{
	for (; first != last; ++first, ++result) {
		*result = op(*first);
	}
	return result;
}

template<class InputIterator1, class InputIterator2, class OutputIterator, class BinaryOperation>
OutputIterator transform(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, BinaryOperation op)
{
	for (; first1 != last1; ++first1,++first2, ++result) {
		*result = op(*first1,*first2);
	}
	return result;
}
//�Ƴ��ظ�Ԫ��
template<class ForwardIterator>
ForwardIterator unique(ForwardIterator first, ForwardIterator last) {
	first = adjacent_find(first, last);
	return unique_copy(first, last, first);
}
template<class InputIterator,class OutputIterator>
inline OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator result) {
	if (first == last)return result;
	return _unique_copy(first, last, result, iterator_category(result));
}
//�����汾
template<class InputIterator, class ForwardIterator>
inline ForwardIterator _unique_copy(InputIterator first, InputIterator last, ForwardIterator result,forward_iterator_tag) {
	*result = *first;
	while (++first != last) {
		if (*result != *first) {
			*++result = *first;
		}
	}
	return ++result;
}
//outputΪwrite only���޷�ֱ�Ӷ�ȡ������*result != *first����ʹ��
template<class InputIterator, class OutputIterator,class T>
inline OutputIterator _unique_copy(InputIterator first, InputIterator last, OutputIterator result, T*) {
	T value = *first;
	*result = value;
	while (++first != last) {
		if (value != *first) {
			value = *first;
			*++result = value;
		}
	}
	return ++result;
}

//ָ���һ����С��value��Ԫ��
//�汾1��<����
//forward,����
//template<class ForwardIterator, class T, class Distance>
//ForwardIterator _lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Distance*, forward_iterator_tag) {
//	Distance len = 0;
//	len = distance(first, last);
//	Distance half;
//	ForwardIterator middle;
//	while (len > 0) {
//		half = len >> 1;
//		middle = first;
//		advance(middle, half);
//		if (*middle < value) {
//			first = middle;
//			++first;
//			len = len - half - 1;
//		}
//		else {
//			len = half;
//		}
//	}
//	return first;
//}
//template<class RandomAccessIterator, class T, class Distance>
//RandomAccessIterator _lower_bound(RandomAccessIterator first, RandomAccessIterator last, const T& value, Distance*, random_access_iterator_tag) {
//	Distance len = last - first;
//	Distance half;
//	RandomAccessIterator middle;
//	while (len > 0) {
//		half = len >> 1;
//		middle = first + half;
//		if (*middle < value) {
//			first = middle + 1;
//			len = len - half - 1;
//		}
//		else {
//			len = half;
//		}
//	}
//	return first;
//}
//template<class ForwardIterator,class T>
//inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value) {
//	return _lower_bound(first, last, value,distance_type(first), iterator_category(first));
//}


template<class ForwardIterator, class T>
ForwardIterator _lower_bound(ForwardIterator first, ForwardIterator last, const T& value, forward_iterator_tag) {
	auto len = distance(first, last);
	auto half=len;
	ForwardIterator middle;
	while (len > 0) {
		half = len >> 1;
		middle = first;
		advance(middle, half);
		if (*middle < value) {
			first = middle;
			++first;
			len = len - half - 1;
		}
		else {
			len = half;
		}
	}
	return first;
}
template<class RandomAccessIterator, class T>
RandomAccessIterator _lower_bound(RandomAccessIterator first, RandomAccessIterator last, const T& value, random_access_iterator_tag) {
	auto len = last - first;
	auto half=len;
	RandomAccessIterator middle;
	while (len > 0) {
		half = len >> 1;
		middle = first + half;
		if (*middle < value) {
			first = middle + 1;
			len = len - half - 1;
		}
		else {
			len = half;
		}
	}
	return first;
}
template<class ForwardIterator,class T>
inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value) {
	return _lower_bound(first, last, value,iterator_category(first));
}






//ָ���һ����С��value��Ԫ��
//�汾2��comp����
template<class ForwardIterator, class T,class Compare=less<T>>
inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value,Compare comp) {
	return _lower_bound(first, last, value, comp, distance_type(first), iterator_category(first));
}


//�����ڲ��ƻ�˳�������£��ɲ���value���һ��λ��
template<class ForwardIterator, class T>
inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value) {
	return _upper_bound(first, last, value, distance_type(first), iterator_category(first));
}
//forward,����
template<class ForwardIterator, class T, class Distance>
ForwardIterator _upper_bound(ForwardIterator first, ForwardIterator last, const T& value, Distance*, forward_iterator_tag) {
	Distance len = 0;
	len = distance(first, last);
	Distance half;
	ForwardIterator middle;
	while (len > 0) {
		half = len >> 1;
		middle = first;
		advance(middle, half);
		if (value< *middle) {
			len = half;
		}
		else {
			first = middle;
			++first;
			len = len - half - 1;
		}
	}
	return first;
}
template<class RandomAccessIterator, class T, class Distance>
RandomAccessIterator _upper_bound(RandomAccessIterator first, RandomAccessIterator last, const T& value, Distance*, random_access_iterator_tag) {
	Distance len = last - first;
	Distance half;
	RandomAccessIterator middle;
	while (len > 0) {
		half = len >> 1;
		middle = first + half;
		if (value < *middle) {
			len = half;
		}
		else {
			first = middle + 1;
			len = len - half - 1;
		}
	}
	return first;
}

template<class ForwardIterator, class T,class Compare>
inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value,Compare comp) {
	return _upper_bound(first, last, value, comp, distance_type(first), iterator_category(first));
}
//forward,����
template<class ForwardIterator, class T, class Distance,class Compare>
ForwardIterator _upper_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare comp,Distance*, forward_iterator_tag) {
	Distance len = 0;
	len = distance(first, last);
	Distance half;
	ForwardIterator middle;
	while (len > 0) {
		half = len >> 1;
		middle = first;
		advance(middle, half);
		if (comp(value, *middle)) {
			len = half;
		}
		else {
			first = middle;
			++first;
			len = len - half - 1;
		}
	}
	return first;
}
template<class RandomAccessIterator, class T, class Distance,class Compare>
RandomAccessIterator _upper_bound(RandomAccessIterator first, RandomAccessIterator last, const T& value, Compare comp,Distance*, random_access_iterator_tag) {
	Distance len = last - first;
	Distance half;
	RandomAccessIterator middle;
	while (len > 0) {
		half = len >> 1;
		middle = first + half;
		if (comp(value, *middle)) {
			len = half;
		}
		else {
			first = middle + 1;
			len = len - half - 1;
		}
	}
	return first;
}

//���ֲ���,�����汾
template<class ForwardIterator,class T>
bool binary_search(ForwardIterator first, ForwardIterator last, const T& value) {
	ForwardIterator i = lower_bound(first, last, value);
	return i != last && !(value < *i);
}
template<class ForwardIterator, class T,class Compare>
bool binary_search(ForwardIterator first, ForwardIterator last, const T& value,Compare comp) {
	ForwardIterator i = lower_bound(first, last, value, comp);
	return i != last && !comp(value, *i);
}
//���������и����ֵ��������һ��������������Ƿ����
template<class BidirectionalIterator>
bool next_permutation(BidirectionalIterator first, BidirectionalIterator last) {
	if (first == last)return false;
	BidirectionalIterator i = first;
	++i;
	if (i == last)return false;
	i = last;
	--i;
	while (true) {
		BidirectionalIterator ii = i;
		--i;
		//ǰһԪ��С�ں�һԪ��
		if (*i < *ii) {
			BidirectionalIterator j = last;
			while (!(*i < *--j));//�ҵ���*i��Ԫ��
			iter_swap(i, j);
			reverse(ii, last);
			return true;
		}
		if (i == first) {
			reverse(first, last);
			return false;
		}
	}	
}
template<class BidirectionalIterator>
bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last) {
	if (first == last)return false;
	BidirectionalIterator i = first;
	++i;
	if (i == last)return false;
	i = last;
	--i;
	while (true) {
		BidirectionalIterator ii = i;
		--i;
		//ǰһԪ��С�ں�һԪ��
		if (*ii < *i) {
			BidirectionalIterator j = last;
			while (!(*--j < *i));//�ҵ���*i��Ԫ��
			iter_swap(i, j);
			reverse(ii, last);
			return true;
		}
		if (i == first) {
			reverse(first, last);
			return false;
		}
	}
}
//������Ԫ���������
template<class RandomAccessIterator>
inline void random_shuffle(RandomAccessIterator first, RandomAccessIterator last) {
	_random_shuffle(first, last, distance_type(first));
}
template<class RandomAccessIterator,class Distance>
void random_shuffle(RandomAccessIterator first, RandomAccessIterator last,Distance*) {
	if (first == last)return;
	for (RandomAccessIterator i = first + 1; i != last; ++i)
//#ifdef _STL_NO_DRAND48
//		iter_swap(i, first + Distance(rand() % ((i - first) + 1)));
//#else
		iter_swap(i, first + Distance(rand() % ((i - first) + 1)));
//#endif
}
//�汾2
template<class RandomAccessIterator,class RandomNumberGenerator>
inline void random_shuffle(RandomAccessIterator first, RandomAccessIterator last, RandomNumberGenerator& rand) {
	if (first == last)return;
	for (RandomAccessIterator i = first + 1; i != last; ++i) {
		iter_swap(i, first + rand((i - first) + 1));
	}
}
//����middle��ʹmiddle-first��[first,middle)����
template<class RandomAccessIterator>
inline void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last) {
	_partial_sort(first, middle, last, value_type(first));
}
template<class RandomAccessIterator,class T>
void __partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, T*) {
	make_heap(first, middle);
	for (RandomAccessIterator i = middle; i < last; ++i) {
		if (*i < *first) {
			_pop_heap(first, middle, i, T(*i), distance_type(first));
		}
	}
	sort_heap(first, middle); 
}
//������������ʱQuick Sort����һ�������������Insertion Sort
//Insertion sort
template<class RandomAccessIterator>
void _insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
	if (first == last)return;
	for (RandomAccessIterator i = first + 1; i != last; ++i) {
		_linear_insert(first, i, value_type(first));
	}
}
template<class RandomAccessIterator,class T>
inline void _linear_insert(RandomAccessIterator first, RandomAccessIterator last, T*) {
	T value = *last;
	if (value < *first) {
		copy_backward(first, last, last + 1);
		*first = value;
	}
	else {
		//β��С��ͷ
		_unguarded_linear_insert(last, value);
	}
}
//�ҵ�valueλ��
template<class RandomAccessIterator,class T>
void _unguarded_linear_insert(RandomAccessIterator last, T value) {
	RandomAccessIterator next = last;
	--next;
	while (value < *next) {
		*last = *next;
		last = next;
		--next;
	}
	*last = value;
}
template<class RandomAccessIterator>
inline void _ungarded_insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
	_ungarded_insertion_sort_aux(first,last,value_type(first));
}
template<class RandomAccessIterator,class T>
void _ungarded_insertion_sort_aux(RandomAccessIterator first, RandomAccessIterator last,T*) {
	for (RandomAccessIterator i = first; i != last; ++i) {
		_unguarded_linear_insert(i, T(*i));
	}
}
//
template<class RandomAccessIterator>
void _final_insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
	if (last - first > _stl_threshold) {
		_insertion_sort(first, first + _stl_threshold);
		_unguarded_insertion_sort(first + _stl_threshold, last);
	}
	else {
		_insertion_sort(first, last);
	}
}

//QuickSort
//len=0or1 return
//ȡS�κ�һ��Ԫ��v��Ϊ��
//L<v,R>v
//�ݹ�
// 
//������ֵ��Ϊ��
template<class T>
inline const T& _median(const T& a, const T& b, const T& c) {
	if (a < b) {
		if (b < c) {
			return b;
		}
		else if (a < c) {
			return c;
		}
		else {
			return a;
		}
	}
	else if (a < c) {
		return a;
	}
	else if (b < c) {
		return c;
	}
	else {
		return b;
	}
}
//�ָ�
template<class RandomAccessIterator,class T>
RandomAccessIterator _unguarded_partition(RandomAccessIterator first, RandomAccessIterator last, T pivot)
{
	while (true) {
		while (*first < pivot)++first;
		--last;
		while (pivot < *last)--last;
		if (!(first < last))return first;
		iter_swap(first, last);
		++first;
	}
}
//introspective sorting��ʡʽ����
template<class RandomAccessIterator>
inline void sort(RandomAccessIterator first, RandomAccessIterator last) {
	if (first != last) {
		_introsort_loop(first, last, value_type(first), _lg(last - first) * 2);
		_final_insertion_sort(first, last);
	}
}
//lg���Ʒָ�����,�ҵ�2^k<=n�����k
template<class Size>
inline Size _lg(Size n) {
	Size k;
	for (k = 0; n > 1; n >>= 1)++k;
	return k;
}
template<class RandomAccessIterator,class T,class Size>
void _introsort_loop(RandomAccessIterator first, RandomAccessIterator last, T*, Size depth_limit)
{
	while (last - first > _stl_threshold) {
		if (depth_limit == 0) {
			partial_sort(first, last, last);
			return;
		}
	}
	--depth_limit;//ѡ��һ���õ���
	RandomAccessIterator cut = _unguarded_partition(first, last, T(_median(*first, *(first + (last - first) / 2), *(last - 1))));
	_introsort_loop(cut, last, value_type(first), depth_limit);//�ұߵݹ�
	last = cut;//��ߵݹ�
}
//���ƻ�˳��������value�ɲ���ĵ�һ�������һ��λ��
template<class ForwardIterator,class T>
inline pair<ForwardIterator, ForwardIterator> equal_range(ForwardIterator first, ForwardIterator last, const T& value)
{
	return _equal_range(first, last, value, distance_type(first), iterator_category(first));
}
//��ͬ�汾
template<class RandomAccessIterator, class T,class Distance>
pair<RandomAccessIterator, RandomAccessIterator> _equal_range(RandomAccessIterator first, RandomAccessIterator last, const T& value,Distance*,random_access_iterator_tag)
{
	Distance len = last - first;
	Distance half;
	RandomAccessIterator middle, left, right;
	while (len > 0) {
		half = len >> 1;
		middle = first + half;
		if (*middle < value) {
			first = middle + 1;
			len = len - half - 1;
		}
		else if (value < *middle) {
			len = half;
		}
		else {
			left = lower_bound(first, middle, value);
			right = upper_bound(++middle, first + len, value);
			return pair<RandomAccessIterator, RandomAccessIterator>(left, right);
		}
	}
	return pair<RandomAccessIterator, RandomAccessIterator>(left, right);
	//�������䶼û��ƥ��ֵ��ָ���һ������valueԪ��
}
template<class ForwardIterator, class T, class Distance>
pair<ForwardIterator, ForwardIterator> _equal_range(ForwardIterator first, ForwardIterator last, const T& value, Distance*, forward_iterator_tag)
{
	Distance len = distance(first,last);
	Distance half;
	ForwardIterator middle, left, right;
	while (len > 0) {
		half = len >> 1;
		middle = first;
		advance(middle, half);
		if (*middle < value) {
			first = middle;
			++first;
			len = len - half - 1;
		}
		else if (value < *middle) {
			len = half;
		}
		else {
			left = lower_bound(first, middle, value);
			advance(first, len);
			right = upper_bound(++middle, first, value);
			return pair<ForwardIterator, ForwardIterator>(left, right);
		}
	}
	return pair<ForwardIterator, ForwardIterator>(left, right);
	//�������䶼û��ƥ��ֵ��ָ���һ������valueԪ��
}
//todo
//�����������һ������ж������򣬽�֮�ϳɵ�������
template<class BidirectionalIterator>
inline void inplace_merge(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last)
{
	//ֻҪ�κ�һ������Ϊ�գ���ʲô������
	if (first == middle || middle == last)return;
	_inplace_merge_aux(first, middle, last, value_type(first), distance_type(first));
}
template<class BidirectionalIterator,class T,class Distance>
void _inplace_merge_aux(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, T*, Distance*)
{
	Distance len1 = 0;
	distance(first, middle, len1);
	Distance len2 = 0;
	distance(middle, last, len2);
	//���㷨ʹ�ö�����ڴ�ռ�
	temporary_buffer<BidirectionalIterator, T>buf(first, last);
	if (buf.begin() == 0) {
		_merge_without_buffer(first, middle, last, len1, len2);
	}
	else {
		_merge_adaptive(first, middle, last, len1, len2,buf.begin(),Distance(buf.size()));
	}
}
template <class BidirectionalIter, class Distance>
void _merge_without_buffer(BidirectionalIter first,BidirectionalIter middle,BidirectionalIter last,Distance len1, Distance len2) {
	if (len1 == 0 || len2 == 0)
		return;
	if (len1 + len2 == 2) {
		if (*middle < *first)
			iter_swap(first, middle);
		return;
	}
	BidirectionalIter first_cut = first;
	BidirectionalIter second_cut = middle;
	Distance len11 = 0;
	Distance len22 = 0;
	if (len1 > len2) {
		len11 = len1 / 2;
		advance(first_cut, len11);
		second_cut = lower_bound(middle, last, *first_cut);
		distance(middle, second_cut, len22);
	}
	else {
		len22 = len2 / 2;
		advance(second_cut, len22);
		first_cut = upper_bound(first, middle, *second_cut);
		distance(first, first_cut, len11);
	}
	BidirectionalIter new_middle
		= rotate(first_cut, middle, second_cut);
	_merge_without_buffer(first, first_cut, new_middle,
		len11, len22);
	_merge_without_buffer(new_middle, second_cut, last, len1 - len11,
		len2 - len22);
}
template<class BidirectionalIterator,class Distance,class Pointer>
void _merge_adaptive(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, Distance len1, Distance len2, Pointer buffer, Distance buffer_size)
{
	if (len1 <= len2 && len1 <= buffer_size) {
		//�������㹻��������1
		Pointer end_buffer = copy(first, middle, buffer);
		merge(buffer, end_buffer, middle, last, first);
	}
	else if (len2 <= buffer_size) {
		//�������㹻��������2
		Pointer end_buffer = copy(middle, last, buffer);
		merge_backward(buffer, end_buffer, middle, last, first);
	}
	else {
		BidirectionalIterator first_cut = first;
		BidirectionalIterator second_cut = middle;
		Distance len11 = 0;
		Distance len22 = 0;
		if (len1 > len2) {
			len11 = len1 / 2;
			advance(first_cut, len11);
			second_cut = lower_bound(middle, last, *first_cut);
			len22 = distance(middle, second_cut);
		}
		else {
			len22 = len2 / 2;
			advance(second_cut, len22);
			first_cut = upper_bound(first, middle, *second_cut);
			distance(first, first_cut, len11);
		}
		BidirectionalIterator new_middle = _rotate_adaptive(first_cut, middle, second_cut, len1 - len11, len22, buffer, buffer_size);
		//�����˵ݹ�
		_merge_adaptive(first, first_cut, new_middle, len11, len22, buffer, buffer_size);
		_merge_adaptive(new_middle, second_cut, last, len1-len11, len2-len22, buffer, buffer_size);
	}
}
template<class BidirectionalIterator1,class BidirectionalIterator2,class Distance>
BidirectionalIterator1 _rotate_adaptive(BidirectionalIterator1 first, BidirectionalIterator1 middle, BidirectionalIterator1 last, Distance len1, Distance len2, BidirectionalIterator2 buffer, Distance buffer_size)
{
	BidirectionalIterator2 buffer_end;
	if (len1 > len2 && len2 <= buffer_size) {
		buffer_end = copy(middle, last, buffer);
		copy_backward(first, middle, last);
		return copy(buffer, buffer_end, first);
	}
	else if (len1 <= buffer_size) {
		buffer_end = copy(first, middle, buffer);
		copy(middle, last, first);
		return copy_backward(buffer, buffer_end, last);
	}
	else {
		rotate(first, middle, last);
		advance(first, len2);
		return first;
	}
}
//�������У�ʹ������nth��ָԪ��ֵ����
template<class RandomAccessIterator>
inline void nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last)
{
	_nth_element(first, nth, last, value_type(first));
}
template<class RandomAccessIterator,class T>
void _nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last,T*)
{
	while (last-first>3)
	{
		RandomAccessIterator cut = _unguarded_partition(first, last, T(_median(*first, *(first + (last - first) / 2), *(last - 1))));
		if (cut <= nth) {
			first = cut;
		}
		else {
			last = cut;
		}
	}
	_insertion_sort(first, last);
}
//һ�������㷨
template<class BidrectionalIterator>
void mergesort(BidrectionalIterator first, BidrectionalIterator last) {
	typename iterator_traits<BidrectionalIterator>::difference_type n = distance(first, last);
	if (n == 0 || n == 1) {
		return;
	}
	else {
		BidrectionalIterator mid = first + n / 2;
		mergesort(first, mid);
		mergesort(mid, last);
		inplace_merge(first, mid, last);
	}
}
}
#endif