#pragma once
#ifndef XZESTL_LIST_H
#define XZESTL_LIST_H
#include "iterator.h"
#include "alloc.h"
#include "construct.h"
namespace xze {

//list�����list����Ƿֿ���
template <class T>
struct _list_node {
	typedef void* void_pointer;
	void_pointer prev;
	void_pointer next;
	T data;
};
//list�ĵ��������
template<class T,class Ref,class Ptr>
struct _list_iterator {
	typedef _list_iterator<T, T&, T*> iterator;
	typedef _list_iterator<T, Ref, Ptr>self;
	//������Ϊ˫��
	typedef bidirectional_iterator_tag iterator_category;
	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef _list_node<T>* link_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	//ָ��list���
	link_type node;
	//���캯��
	_list_iterator(link_type x):node(x){}
	_list_iterator(){}
	_list_iterator(const iterator&x):node(x.node){}

	bool operator==(const self& x)const { return node == x.node; }
	bool operator!=(const self& x)const { return node != x.node; }
	reference operator*()const { return (*node).data; }
	pointer operator->()const { return &(operator*()); }
	self& operator++() {
		node = (link_type)((*node).next);
		return* this;
	}
	self operator++(int) {
		self tmp = *this;
		++* this;
		return tmp;
	}

	self& operator--() {
		node = (link_type)((*node).prev);
		return* this;
	}

	self operator--(int) {
		self tmp = *this;
		--* this;
		return tmp;
	}
};
template<class T,class Alloc=alloc>
class list {
protected:
	typedef _list_node<T> list_node;
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;
	typedef simple_alloc<list_node, Alloc>list_node_allocator;
	typedef size_t size_type;
public:
	typedef list_node* link_type;
	typedef _list_iterator<T, T&, T*> iterator;
protected:
	link_type node;
	//����һ����㲢����
	link_type get_node() { return list_node_allocator::allocate(); }
	//�ͷ�һ�����
	void put_node(link_type p) { list_node_allocator::deallocate(p); }
	//���ò�����һ�����
	link_type create_node(const T& x) {
		link_type p = get_node();
		construct(&p->data, x);
		return p;
	}
	//����һ�����
	void destroy_node(link_type p) {
		destroy(&p->data);
		put_node(p);
	}

	void empty_initialize() {
		node = get_node();
		node->next = node;
		node->prev = node;
	}


public:
	//list��һ����״˫������
	iterator begin() { return (link_type)((*node).next); }
	iterator end() { return node; }
	bool empty()const { return node->next == node; }
	size_type size()const {
		size_type result = 0;
		distance(begin(), end(),result);
		return result;
	}

	reference front() { return *begin(); }
	reference back() { return *(--end()); }
	//���캯��
	list() { empty_initialize(); }

	iterator insert(iterator position, const T& x) {
		link_type tmp = create_node(x);
		tmp->next = position.node;
		tmp->prev = position.node->prev;
		(link_type(position.node->prev))->next = tmp;
		position.node->prev = tmp;
		return tmp;
	}

	void push_back(const T& x) { insert(end(), x); }
	void push_front(const T& x) { insert(begin(), x); }
	//�Ƴ���������ָ���
	iterator erase(iterator position) {
		link_type next_node = link_type(position.node->next);
		link_type prev_node = link_type(position.node->prev);
		prev_node->next = next_node;
		next_node->prev = prev_node;
		destroy_node(position);
		return iterator(next_node);
	}
	void pop_front() { erase(begin()); }
	void pop_back() {
		iterator tmp = end();
		erase(--tmp);
	}
	//�������
	void clear() {
		link_type cur = (link_type)node->next;
		while (cur != node) {
			link_type tmp = cur;
			cur = (link_type)cur->next;
			destroy_node(tmp);
		}
		node->next = node;
		node->prev = node;
	}
	//�Ƴ�ֵΪvalueԪ��
	void remove(const T& value) {
		iterator first = begin();
		iterator last = end();
		while (first != last) {
			iterator next = first;
			++next;
			if (*first == value) {
				erase(first);
			}
			first = next;
		}
	}
	//�Ƴ�������ͬԪ��
	void unique() {
		iterator first = begin();
		iterator last = end();
		if (first == last)return;
		iterator next = first;
		while (++next!=last) {
			if (*first == *next) {
				erase(next);
			}
			else {
				first = next;
			}
			next = first;
		}
	}
	//��[first,last)Ԫ���Ƶ�positionǰ
	void transfer(iterator position, iterator first, iterator last) {
		if (position != last) {
			(*(link_type((*last.node).prev))).next = position.node;
			(*(link_type((*first.node).prev)) ).next = last.node;
			(*(link_type((*position.node).prev))).next = first.node;
			link_type tmp = link_type((*position.node).prev);
			(*position.node).prev = (*last.node).prev;
			//���ɵ�����ƴ����
			(*last.node).prev = (*first.node).prev;
			(*first.node).prev = tmp;
		}
	}
	//��x�ӵ�positionǰ������list��ͬ
	void splice(iterator position, list& x) {
		if (!x.empty()) {
			transfer(position, x.begin(), end());
		}
	}
	//��i��ָ�ӵ�positionǰ
	void splice(iterator position, list&, iterator i) {
		iterator j = i;
		++j;
		if (position == i || position == j)return;
		transfer(position, i, j);
	}
	//position������first��last֮��
	void splice(iterator position,list&, iterator first, iterator last) {
		if (first != last) {
			transfer(position, first, last);
		}
	}
	//merge�����������������������򣨵�����
	void merge(list<T, Alloc>& x) {
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = x.begin();
		iterator last2 = x.end();

		while (first1!=last1&&first2!=last2) {
			if (*first2 < *first1) {
				iterator next = first2;
				transfer(first1, first2, ++next);
				first2 = next;
			}
			else {
				++first1;
			}
		}
		if (first2 != last2) {
			transfer(last1, first2, last2);
		}
	}

	void reverse() {
		//list�ջ�Ԫ�ظ���Ϊ1������
		if (node->next == node || link_type(node->next)->next == node)return;
		iterator first = begin();
		++first;
		while (first != end()) {
			iterator old = first;
			++first;
			transfer(begin(), old, first);
		}
	}

	//list������stl��sort����Ϊ��Ҫ��iteratorΪrandomAccessIterator
	//����ʵ��,���ƹ鲢����
	//��counter[64]��ʾһ������Ϊ2^64�Ĵ�����ͬ��������㷨������������������2^64 - 1
	void sort() {
		//list�ջ�Ԫ�ظ���Ϊ1������
		if (node->next == node || link_type(node->next)->next == node)return;
		list<T, Alloc> carry;
		list<T, Alloc> counter[64];
		int fill = 0;

		while (!empty()) {
			carry.splice(carry.begin(), *this, begin());
			int i = 0;
			while (i < fill && !counter[i].empty()) {
				counter[i].merge(carry);
				carry.swap(counter[i++]);
			}
			carry.swap(counter[i]);
			if (i == fill)++fill;
		}
		for (int i = 1; i < fill; ++i) {
			counter[i].merge(counter[i - 1]);
		}
		swap(counter[fill - 1]);
	}

};

}

#endif // !XZESTL_LIST_H
