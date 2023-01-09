#pragma once
#ifndef XZESTL_ITERATOR_H_
#define XZESTL_ITERATOR_H_
//��ͷ�ļ�ʵ�ֵ�����
//todo:iostream iterator,inserter iterator,reserve iterator
#include<cstddef>//��ͷ�ļ�����ptrdiff_t
#include <iostream>
#include "type_traits.h"

namespace xze {
	//���ֵ�����
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

	//����value���ж���T���Ƿ����iterator_category���ԣ�\
	���Ϊtrue�������̳���iterator_traits_impl��\
	��󽫸���is_convertible��ģ�壨����value���ж��Ƿ���T��ת��Ϊinputָ���outputָ�룩\
	�����Ƿ����typedefһϵ�в�����
	template <class T>
	struct has_iterator_cat
	{
	private:
		struct two { char a; char b; };
		template <class U> static two test(...);
		template <class U> static char test(typename U::iterator_category* = 0);
		//����iterator_category���ԣ��������ƥ�䵽
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

	//traits:������ȡ����
	template<class Iterator>
	struct iterator_traits:public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {};

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

	//std::is_convertibleģ�����ڼ���Ƿ���Խ��κ���������A��ʽת��Ϊ�κ���������B��\
	�����ز���ֵtrue��false��
	template <class T, class U, bool = has_iterator_cat<iterator_traits<T>>::value>
	struct has_iterator_cat_of
		: public m_bool_constant<std::is_convertible<
		typename iterator_traits<T>::iterator_category, U>::value>
	{
	};
	//�ж�ĳ�ֵ������Ƿ�߱�ĳ������
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
		typedef typename iterator_traits<InputIterator>::iterator_category category;
		return _distance(first, last, category());
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

	//insert iterator��һ������ֵ����ת��Ϊ��������ĵ����������ڲ�ά��һ��������\
	operator������������insert�������أ���ǰ�������ˣ�ȡֵ����Աȡ�õȲ�������Υ����
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

		//���캯��
		explicit back_insert_iterator(Container&x):container(&x){}
		back_insert_iterator<Container>& operator=(const typename Container::value_type& value) {
			container->push_back(value);
			return *this;
		}
		//��������������������
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

	//��������������ʹ��back_insert_iterator
	template<class Container>
	inline back_insert_iterator<Container> back_inserter(Container& x) {
		return back_insert_iterator<Container>(x);
	}

	//����ͷ����
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

		//���캯��
		explicit front_insert_iterator(Container& x) :container(&x) {}
		front_insert_iterator<Container>& operator=(const typename Container::value_type& value) {
			container->push_front(value);
			return *this;
		}
		//��������������������
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

	//��������������ʹ��front_insert_iterator
	template<class Container>
	inline front_insert_iterator<Container> front_inserter(Container& x) {
		return front_insert_iterator<Container>(x);
	}
	//�����޸�Ϊ������ָ��λ�ã���������������
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

		//���캯��
		insert_iterator(Container& x,typename Container::iterator i) :container(&x),iter(i) {}
		insert_iterator<Container>& operator=(const typename Container::value_type& value) {
			iter = container->insert(iter, value);
			++iter;
			return *this;
		}
		//��������������������
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

	//��������������ʹ��front_insert_iterator
	template<class Container,class Iterator>
	inline insert_iterator<Container> inserter(Container& x,Iterator i) {
		typedef typename Container::iterator iter;
		return insert_iterator<Container>(x,iter(i));
	}

	//���������ƶ���Ϊ��ת
	template <class Iterator>
	class reverse_iterator
	{
	private:
		Iterator current;  // ��¼��Ӧ�����������

	public:
		// �����������������Ӧ�ͱ�
		typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
		typedef typename iterator_traits<Iterator>::value_type        value_type;
		typedef typename iterator_traits<Iterator>::difference_type   difference_type;
		typedef typename iterator_traits<Iterator>::pointer           pointer;
		typedef typename iterator_traits<Iterator>::reference         reference;

		typedef Iterator                                              iterator_type;
		typedef reverse_iterator<Iterator>                            self;

	public:
		// ���캯��
		reverse_iterator() {}
		explicit reverse_iterator(iterator_type i) :current(i) {}
		reverse_iterator(const self& x) :current(x.current) {}

	public:
		// ȡ����Ӧ�����������
		iterator_type base() const
		{
			return current;
		}

		// ���ز�����
		reference operator*() const
		{ // ʵ�ʶ�Ӧ�����������ǰһ��λ��
			auto tmp = current;
			return *--tmp;
		}
		pointer operator->() const
		{
			return &(operator*());
		}

		// ǰ��(++)��Ϊ����(--)
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
		// ����(--)��Ϊǰ��(++)
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

	//stream iterator�����������󶨵�һ��������������
	template <class T,class Distance=ptrdiff_t>
	class istream_iterator {
		friend bool operator==(const istream_iterator<T, Distance>& x, const istream_iterator<T, Distance>& y);
	public:
		std::istream* stream;
		T value;
		bool end_marker;
		void read() {
			end_marker = (*stream) ? true : false;
			//����EOF���ͱ𲻷��Ĵ���false
			if (end_marker)*stream >> value;
			end_marker = (*stream) ? true : false;
		}
	
		typedef input_iterator_tag iterator_category;
		typedef T value_type;
		typedef Distance difference_type;
		typedef const T* pointer;
		typedef const T& reference;
		//��Ϊinput iterator������const����

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
		const char* string;//ÿ�������ļ������

	public:
		typedef output_iterator_tag iterator;
		typedef void value_type;
		typedef void difference_type;
		typedef void pointer;
		typedef void reference;

		ostream_iterator(std::ostream& s):stream(&s),string(0){}
		ostream_iterator(std::ostream& s,const char*c):stream(&s),string(c){}
		ostream_iterator<T>& operator=(const T& value) {
			*stream << value;//�����ֵ
			if (string)*stream << string;//������Ų��������
			return *this;
		}
		ostream_iterator<T>& operator*() { return *this; }
		ostream_iterator<T>& operator++() { return *this; }
		ostream_iterator<T>& operator++(int) { return *this; }
	};

	
}
#endif