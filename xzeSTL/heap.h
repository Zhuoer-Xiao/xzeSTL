#pragma once
#ifndef XZESTL_HEAP_H
#define XZESTL_HEAP_H
#include "iterator.h"
namespace xze {
	//ʹ��vector�洢��vector��һ�����Ϊ��
template<class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
	//�ڱ�����֮ǰ��ֵ���ڵײ�������β��
	_push_heap_aux(first, last, distance_type(first), value_type(first));
}
template<class RandomAccessIterator,class Distance,class T>
inline void _push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*) {
	//��ֵ���ڵײ�������׶�
	_push_heap(first, Distance((last-first)-1),Distance(0),T(*(last-1)));
}
template<class RandomAccessIterator, class Distance, class T>
void _push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value) {
	//�����
	Distance parent = (holeIndex - 1) / 2;
	while (holeIndex > topIndex && *(first + parent) < value) {
		//δ���ﶥ�ˣ��Ҹ��ڵ�С����ֵ������Ϊ���������н���
		*(first + holeIndex) = *(first + parent);//holeΪ��
		holeIndex = parent;//�������������ڵ�
		parent = (holeIndex - 1) / 2;//hole�ĸ��ڵ�
	}
	*(first + holeIndex) = value;//holeΪ��ֵ
}

//���Ԫ��ֻ�Ƿ�����β��
template<class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
	_pop_heap_aux(first, last, value_type(first));
}
template<class RandomAccessIterator, class T>
inline void _pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last,T*) {
	_pop_heap(first, last-1,last-1,T(*(last-1)),distance_type(first));
}

template<class RandomAccessIterator, class Distance, class T>
inline void _pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*) {
	*result = *first;
	_adjust_heap(first, Distance(0), Distance(last - first), value);
}

template<class RandomAccessIterator, class Distance, class T>
inline void _adjust_heap(RandomAccessIterator first, Distance holeIndex,Distance len,T value) {
	Distance topIndex = holeIndex;
	Distance secondChild = 2 * holeIndex + 2;//���ӽڵ�
	while (secondChild < len) {//��С����
		if (*(first + secondChild) < *(first + (secondChild - 1))) {
			--secondChild;
		}
		*(first + holeIndex) = *(first + secondChild);
		holeIndex = secondChild;
		secondChild = 2 * (secondChild + 1);
	}
	if (secondChild == len) {
		//û���ҽ��
		*(first + holeIndex) = *(first + (secondChild - 1));
		holeIndex = secondChild - 1;
	}
}
template<class RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
	while (last - first > 1) {
		pop_heap(first, last--);
	}
}
//��������ת��Ϊheap
template<class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	_make_heap(first, last, value_type(first), distance_type(first));
}
template<class RandomAccessIterator,class T,class Distance>
void _make_heap(RandomAccessIterator first, RandomAccessIterator last,T*,Distance*)
{
	if (last - first < 2)return;//0��1������
	Distance len = last - first;
	Distance parent = (len - 2) / 2;
	while (true) {//Ҷ�ӽ�㲻��ִ��
		_adjust_heap(first, parent, len, T(*(first + parent)));
		if (parent == 0)return;
		parent--;
	}
}


}
#endif