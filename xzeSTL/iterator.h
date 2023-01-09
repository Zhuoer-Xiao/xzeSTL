#pragma once
#ifndef XZESTL_ITERATOR_H_
#define XZESTL_ITERATOR_H_
//此头文件实现迭代器
//todo:iostream iterator,inserter iterator,reserve iterator
#include<cstddef>//此头文件定义ptrdiff_t
#include <iostream>
#include "type_traits.h"

namespace xze {
	//五种迭代器
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

	//根据value可判断类T中是否具有iterator_category属性，\
	如果为true，进而继承于iterator_traits_impl，\
	最后将根据is_convertible类模板（根据value可判断是否类T可转变为input指针和output指针）\
	决定是否进行typedef一系列操作。
	template <class T>
	struct has_iterator_cat
	{
	private:
		struct two { char a; char b; };
		template <class U> static two test(...);
		template <class U> static char test(typename U::iterator_category* = 0);
		//若有iterator_category属性，则会优先匹配到
	public:
		static const bool value = sizeof(test<T>(0)) == sizeof(char);
	};

	template <class Iterator, bool>
	struct iterator_traits_impl {};

	template <class Iterator>
	struct iterator_traits_impl<Iterator, true>
	{
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type        value_type;
		typedef typename Iterator::pointer           pointer;
		typedef typename Iterator::reference         reference;
		typedef typename Iterator::difference_type   difference_type;
	};

	template <class Iterator, bool>
	struct iterator_traits_helper {};

	template <class Iterator>
	struct iterator_traits_helper<Iterator, true>
		: public iterator_traits_impl<Iterator,
		std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
		std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value>
	{
	};

	//traits:用于提取特性
	template<class Iterator>
	struct iterator_traits:public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {};

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

	//std::is_convertible模板用于检查是否可以将任何数据类型A隐式转换为任何数据类型B。\
	它返回布尔值true或false。
	template <class T, class U, bool = has_iterator_cat<iterator_traits<T>>::value>
	struct has_iterator_cat_of
		: public m_bool_constant<std::is_convertible<
		typename iterator_traits<T>::iterator_category, U>::value>
	{
	};
	//判定某种迭代器是否具备某种特性
	template<class T, class U>
	struct has_iterator_cat_of<T, U, false> :public m_false_type {};


	template <class Iter>
	struct is_input_iterator : public has_iterator_cat_of<Iter, input_iterator_tag> {};

	template <class Iter>
	struct is_output_iterator : public has_iterator_cat_of<Iter, output_iterator_tag> {};

	template <class Iter>
	struct is_forward_iterator : public has_iterator_cat_of<Iter, forward_iterator_tag> {};

	template <class Iter>
	struct is_bidirectional_iterator : public has_iterator_cat_of<Iter, bidirectional_iterator_tag> {};

	template <class Iter>
	struct is_random_access_iterator : public has_iterator_cat_of<Iter, random_access_iterator_tag> {};

	template <class Iterator>
	struct is_iterator : public m_bool_constant<is_input_iterator<Iterator>::value ||
		is_output_iterator<Iterator>::value>
	{
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
		typedef typename iterator_traits<InputIterator>::iterator_category category;
		return _distance(first, last, category());
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

	//insert iterator是一个将赋值操作转换为插入操作的迭代器，其内部维护一个容器，\
	operator操作被容器的insert操作重载，其前进，后退，取值，成员取用等操作都是违法的
	template <class Container>
	class back_insert_iterator {
	protected:
		Container* container;
	public:
		typedef output_iterator_tag iterator_category;
		typedef void value_type;
		typedef void difference_type;
		typedef void pointer;
		typedef void referenve;

		//构造函数
		explicit back_insert_iterator(Container&x):container(&x){}
		back_insert_iterator<Container>& operator=(const typename Container::value_type& value) {
			container->push_back(value);
			return *this;
		}
		//下面三个操作返回自身
		back_insert_iterator<Container>& operator*() {
			return *this;
		}
		back_insert_iterator<Container>& operator++() {
			return *this;
		}
		back_insert_iterator<Container>& operator++(int) {
			return *this;
		}
	};

	//辅助函数，便于使用back_insert_iterator
	template<class Container>
	inline back_insert_iterator<Container> back_inserter(Container& x) {
		return back_insert_iterator<Container>(x);
	}

	//容器头插入
	template <class Container>
	class front_insert_iterator {
	protected:
		Container* container;
	public:
		typedef output_iterator_tag iterator_category;
		typedef void value_type;
		typedef void difference_type;
		typedef void pointer;
		typedef void referenve;

		//构造函数
		explicit front_insert_iterator(Container& x) :container(&x) {}
		front_insert_iterator<Container>& operator=(const typename Container::value_type& value) {
			container->push_front(value);
			return *this;
		}
		//下面三个操作返回自身
		front_insert_iterator<Container>& operator*() {
			return *this;
		}
		front_insert_iterator<Container>& operator++() {
			return *this;
		}
		front_insert_iterator<Container>& operator++(int) {
			return *this;
		}
	};

	//辅助函数，便于使用front_insert_iterator
	template<class Container>
	inline front_insert_iterator<Container> front_inserter(Container& x) {
		return front_insert_iterator<Container>(x);
	}
	//操作修改为插入在指定位置，并将迭代器右移
	template <class Container>
	class insert_iterator {
	protected:
		Container* container;
		typename Container::iterator iter;
	public:
		typedef output_iterator_tag iterator_category;
		typedef void value_type;
		typedef void difference_type;
		typedef void pointer;
		typedef void referenve;

		//构造函数
		insert_iterator(Container& x,typename Container::iterator i) :container(&x),iter(i) {}
		insert_iterator<Container>& operator=(const typename Container::value_type& value) {
			iter = container->insert(iter, value);
			++iter;
			return *this;
		}
		//下面三个操作返回自身
		front_insert_iterator<Container>& operator*() {
			return *this;
		}
		front_insert_iterator<Container>& operator++() {
			return *this;
		}
		front_insert_iterator<Container>& operator++(int) {
			return *this;
		}
	};

	//辅助函数，便于使用front_insert_iterator
	template<class Container,class Iterator>
	inline insert_iterator<Container> inserter(Container& x,Iterator i) {
		typedef typename Container::iterator iter;
		return insert_iterator<Container>(x,iter(i));
	}

	//迭代器的移动行为倒转
	template <class Iterator>
	class reverse_iterator
	{
	private:
		Iterator current;  // 记录对应的正向迭代器

	public:
		// 反向迭代器的五种相应型别
		typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
		typedef typename iterator_traits<Iterator>::value_type        value_type;
		typedef typename iterator_traits<Iterator>::difference_type   difference_type;
		typedef typename iterator_traits<Iterator>::pointer           pointer;
		typedef typename iterator_traits<Iterator>::reference         reference;

		typedef Iterator                                              iterator_type;
		typedef reverse_iterator<Iterator>                            self;

	public:
		// 构造函数
		reverse_iterator() {}
		explicit reverse_iterator(iterator_type i) :current(i) {}
		reverse_iterator(const self& x) :current(x.current) {}

	public:
		// 取出对应的正向迭代器
		iterator_type base() const
		{
			return current;
		}

		// 重载操作符
		reference operator*() const
		{ // 实际对应正向迭代器的前一个位置
			auto tmp = current;
			return *--tmp;
		}
		pointer operator->() const
		{
			return &(operator*());
		}

		// 前进(++)变为后退(--)
		self& operator++()
		{
			--current;
			return *this;
		}
		self operator++(int)
		{
			self tmp = *this;
			--current;
			return tmp;
		}
		// 后退(--)变为前进(++)
		self& operator--()
		{
			++current;
			return *this;
		}
		self operator--(int)
		{
			self tmp = *this;
			++current;
			return tmp;
		}

		self& operator+=(difference_type n)
		{
			current -= n;
			return *this;
		}
		self operator+(difference_type n) const
		{
			return self(current - n);
		}
		self& operator-=(difference_type n)
		{
			current += n;
			return *this;
		}
		self operator-(difference_type n) const
		{
			return self(current + n);
		}

		reference operator[](difference_type n) const
		{
			return *(*this + n);
		}
	};

	//stream iterator，将迭代器绑定到一个数据流对象上
	template <class T,class Distance=ptrdiff_t>
	class istream_iterator {
		friend bool operator==(const istream_iterator<T, Distance>& x, const istream_iterator<T, Distance>& y);
	public:
		std::istream* stream;
		T value;
		bool end_marker;
		void read() {
			end_marker = (*stream) ? true : false;
			//读到EOF或型别不符的处于false
			if (end_marker)*stream >> value;
			end_marker = (*stream) ? true : false;
		}
	
		typedef input_iterator_tag iterator_category;
		typedef T value_type;
		typedef Distance difference_type;
		typedef const T* pointer;
		typedef const T& reference;
		//身为input iterator，采用const保险

		istream_iterator():stream(&std::cin),end_marker(false){}
		istream_iterator(std::istream& s) :stream(&s) { read(); }

		reference operator*()const { return value; }
		pointer operator->()const { return &(operator*()); }
		istream_iterator<T, Distance>& operator++() {
			read();
			return *this;
		}
		istream_iterator<T, Distance> operator++(int) {
			istream_iterator<T, Distance> tmp = *this;
			read();
			return tmp;
		}
	};

	//
	template <class T>
	class ostream_iterator {
	private:
		std::ostream* stream;
		const char* string;//每次输出后的间隔符号

	public:
		typedef output_iterator_tag iterator;
		typedef void value_type;
		typedef void difference_type;
		typedef void pointer;
		typedef void reference;

		ostream_iterator(std::ostream& s):stream(&s),string(0){}
		ostream_iterator(std::ostream& s,const char*c):stream(&s),string(c){}
		ostream_iterator<T>& operator=(const T& value) {
			*stream << value;//输出数值
			if (string)*stream << string;//间隔符号不空则输出
			return *this;
		}
		ostream_iterator<T>& operator*() { return *this; }
		ostream_iterator<T>& operator++() { return *this; }
		ostream_iterator<T>& operator++(int) { return *this; }
	};

	
}
#endif