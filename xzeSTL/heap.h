#pragma once
#ifndef XZESTL_HEAP_H
#define XZESTL_HEAP_H
#include "iterator.h"
namespace xze {
	//使用vector存储，vector第一个结点为空
template<class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
	//在被调用之前新值置于底部容器最尾端
	_push_heap_aux(first, last, distance_type(first), value_type(first));
}
template<class RandomAccessIterator,class Distance,class T>
inline void _push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*) {
	//新值置于底部容器最底端
	_push_heap(first, Distance((last-first)-1),Distance(0),T(*(last-1)));
}
template<class RandomAccessIterator, class Distance, class T>
void _push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value) {
	//父结点
	Distance parent = (holeIndex - 1) / 2;
	while (holeIndex > topIndex && *(first + parent) < value) {
		//未到达顶端，且父节点小于新值（大者为父），进行交换
		*(first + holeIndex) = *(first + parent);//hole为父
		holeIndex = parent;//向上提升至父节点
		parent = (holeIndex - 1) / 2;//hole的父节点
	}
	*(first + holeIndex) = value;//hole为新值
}

//最大元素只是放在最尾端
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
	Distance secondChild = 2 * holeIndex + 2;//右子节点
	while (secondChild < len) {//右小于左
		if (*(first + secondChild) < *(first + (secondChild - 1))) {
			--secondChild;
		}
		*(first + holeIndex) = *(first + secondChild);
		holeIndex = secondChild;
		secondChild = 2 * (secondChild + 1);
	}
	if (secondChild == len) {
		//没有右结点
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
//现有数据转换为heap
template<class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	_make_heap(first, last, value_type(first), distance_type(first));
}
template<class RandomAccessIterator,class T,class Distance>
void _make_heap(RandomAccessIterator first, RandomAccessIterator last,T*,Distance*)
{
	if (last - first < 2)return;//0或1不用排
	Distance len = last - first;
	Distance parent = (len - 2) / 2;
	while (true) {//叶子结点不用执行
		_adjust_heap(first, parent, len, T(*(first + parent)));
		if (parent == 0)return;
		parent--;
	}
}


}
#endif