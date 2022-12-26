#pragma once
#ifndef XZESTL_ITERATOR_H_
#define XZESTL_ITERATOR_H_
//��ͷ�ļ�ʵ�ֵ�����
//todo:iostream iterator,inserter iterator,reves
#include<cstddef>//��ͷ�ļ�����ptrdiff_t
//���ֵ�����
namespace xze {
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag :public input_iterator_tag {};
	struct bidirectional_iterator_tag :public forward_iterator_tag {};
	struct random_access_iterator_tag :public bidirectional_iterator_tag {};

	//��ʱ��������Ҫ����һ��Ԫ�ص�����Ϊ��������������ͣ���ʱ��ʹ��function template�Ĳ����Ƶ�



	//���������ͱ�:�����Ϊ�ĵ��������Ե����������ڲ�ͬ���ԣ��в�ͬ�Ĵ�������ʹ��trait��ȡ�ͱ�
	//���ݱ�׼���κε�������Ӧ�ṩ������ͱ�
	template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T * , class Reference = T & >
	struct iterator {
		typedef Category iterator_category;
		typedef T value_type;
		typedef Distance different_type;
		typedef Pointer pointer;
		typedef Reference reference;
	};

	//traits:������ȡ����
	template<class Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type value_type;
		typedef typename Iterator::different_type difference_type;
		typedef typename Iterator::pointer pointer;
		typedef typename Iterator::reference reference;
	};
	//���ԭ��ָ��ƫ�ػ�
	template<class T>
	struct iterator_traits<T*>
	{
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t different_type;
		typedef T*  pointer;
		typedef T&  reference;
	};
	//���pointer2constƫ�ػ�
	template<class T>
	struct iterator_traits<const T*>
	{
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t different_type;
		typedef T*  pointer;
		typedef T&  reference;
	};

	//����������
	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator&) {
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	//������distance type
	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
		distance_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	//������value type
	template<class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
		value_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}

	//distance��input iterator�汾
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
	//distance��random�汾
	template<class RandomAccessIterator>
	inline typename iterator_traits<RandomAccessIterator>::difference_type
		_distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
		return last - first;
	}
	//��װcategory���ɲ�,ģ����������Ϊ֧�ֵ�������ĵ�����
	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
		distance(InputIterator first, InputIterator last) {

	}

	//advance:ǰ��ָ������
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