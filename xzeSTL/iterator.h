#pragma once
#ifndef XZESTL_ITERATOR_H_
#define XZESTL_ITERATOR_H_
//此头文件实现迭代器
//todo:iostream iterator,inserter iterator,reves
#include<cstddef>//此头文件定义ptrdiff_t
//五种迭代器
namespace xze {
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag :public input_iterator_tag {};
	struct bidirectional_iterator_tag :public forward_iterator_tag {};
	struct random_access_iterator_tag :public bidirectional_iterator_tag {};

	//有时候我们想要定义一个元素的类型为传入迭代器的类型，这时后使用function template的参数推导



	//迭代器的型别:可理解为改迭代器特性的描述，对于不同特性，有不同的处理方法，使用trait提取型别。
	//根据标准，任何迭代器都应提供这五个型别
	template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T * , class Reference = T & >
	struct iterator {
		typedef Category iterator_category;
		typedef T value_type;
		typedef Distance different_type;
		typedef Pointer pointer;
		typedef Reference reference;
	};

	//traits:用于提取特性
	template<class Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type value_type;
		typedef typename Iterator::different_type difference_type;
		typedef typename Iterator::pointer pointer;
		typedef typename Iterator::reference reference;
	};
	//针对原生指针偏特化
	template<class T>
	struct iterator_traits<T*>
	{
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t different_type;
		typedef T*  pointer;
		typedef T&  reference;
	};
	//针对pointer2const偏特化
	template<class T>
	struct iterator_traits<const T*>
	{
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t different_type;
		typedef T*  pointer;
		typedef T&  reference;
	};

	//迭代器类型
	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator&) {
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	//迭代器distance type
	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
		distance_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	//迭代器value type
	template<class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
		value_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}

	//distance的input iterator版本
	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
		_distance(InputIterator first, InputIterator last, input_iterator_tag) {
		typename iterator_traits<InputIterator>::difference_type n = 0;
		while (first != last) {
			++first;
			++n;
		}
		return n;
	}
	//distance的random版本
	template<class RandomAccessIterator>
	inline typename iterator_traits<RandomAccessIterator>::difference_type
		_distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
		return last - first;
	}
	//包装category生成层,模板命名规则为支持的最基本的迭代器
	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
		distance(InputIterator first, InputIterator last) {

	}

	//advance:前进指定距离
	template<class InputIterator, class Distance>
	inline void _advance(InputIterator& start, Distance n, input_iterator_tag) {
		while (n--) {
			++start;
		}
	}
	template<class BidirectionalIterator, class Distance>
	inline void _advance(BidirectionalIterator& start, Distance n, bidirectional_iterator_tag) {
		if (n >= 0) {
			while (n--) {
				++start;
			}
		}
		else {
			while (n++) {
				--start;
			}
		}
	}
	template<class RandomIterator, class Distance>
	inline void _advance(RandomIterator& start, Distance n, random_access_iterator_tag) {
		start += n;
	}

	template<class InputIterator, class Distance>
	inline void advance(InputIterator& start, Distance n) {
		_advance(start, n, iterator_category(start));
	}

}
#endif