#pragma once
#ifndef XZESTL_SETALGO_H
#define XZESTL_SETALGO_H
#include "iterator.h"
#include "algobase.h"
namespace xze {
//�󲢼�
template<class InputIterator1, class InputIterator2,class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
		}
		else if (*first2 < *first1) {
			*result = *first2;
			++first2;
		}
		else {
			*result = *first1;
			++first1;
			++first2;
		}
		++result;
	}
	//��һ������������������һ��������β��Ԫ�ظ���
	return copy(first2, last2, copy(first1, last1, result));
}

//�󽻼�
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			++first1;
		}
		else if (*first2 < *first1) {
			++first2;
		}
		else {
			*result = *first1;
			++first1;
			++first2;
			++result;
		}
	}
	return result;
}

//��
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
			++result;
		}
		else if (*first2 < *first1) {
			++first2;
		}
		else {
			++first1;
			++first2;
		}
	}
	return copy(first1, last1, result);
}

//��ԳƲ(S1-S2)U(S2-S1)
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
			++result;
		}
		else if (*first2 < *first1) {
			*result = *first2;
			++first2;
			++result;
		}
		else {
			++first1;
			++first2;
		}
	}
	return copy(first2,last2,copy(first1, last1, result));
}

}
#endif // !XZESTL_SETALGO_H
