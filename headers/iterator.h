#ifndef XZESTL_ITERATOR_H_
#define XZESTL_ITERATOR_H_
//此头文件实现迭代器
#include<cstddef>//此头文件定义ptrdiff_t
//五种迭代器
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag{};
struct bidirectional_iterator_tag{};
struct random_access_iterator_tag{};

//迭代器的型别:可理解为改迭代器特性的描述，对于不同特性，有不同的处理方法，使用trait提取型别。
//根据标准，任何迭代器都应提供这五个型别
template<class Category, class T,class Distance=ptrdiff_t,class Pointer=T*,class Reference=T&>
struct iterator{
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
	typedef typename Iterator::different_type different_type;
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
	typedef T& reference;
};
//针对pointer2const偏特化
template<class T>
struct iterator_traits<const T*>
{
	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef ptrdiff_t different_type;
	typedef T* pointer;
	typedef T& reference;
};

//迭代器类型
template<class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last) {
	typedef typename iterator_traits<InputIterator>::iterator_category category;
	return _distance(first, last, category);
}


#endif