#pragma once
#ifndef XZESTL_SLIST_H
#define XZESTL_SLIST_H
#include "iterator.h"
#include "alloc.h"
namespace xze {
	//结点结构
struct _slist_node_base
{
	_slist_node_base* next;
};
template<class T>
struct _slist_node:_slist_node_base
{
	T data;
};
//在后面插入新结点
inline _slist_node_base* _slist_make_link(_slist_node_base*prev_node,_slist_node_base*new_node) {
	new_node->next = prev_node->next;
	prev_node->next = new_node;
	return new_node;
}
//链表大小
inline size_t _slist_size(_slist_node_base* node) {
	size_t result = 0;
	for (; node != 0; node = node->next) {
		++result;
	}
	return result;
}
//迭代器基本结构
struct _slist_iterator_base {
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef forward_iterator_tag iterator_category;
	_slist_node_base* node;
	//构造
	_slist_iterator_base(_slist_node_base*x):node(x){}
	//前进
	void incr() { node = node->next; }
	bool operator==(const _slist_iterator_base& x)const { return node == x.node; }
	bool operator!=(const _slist_iterator_base& x)const { return node != x.node; }

};
//迭代器结构
template<class T,class Ref,class Ptr>
struct _slist_iterator :public _slist_iterator_base {
	typedef _slist_iterator<T, T&, T*> iterator;
	typedef _slist_iterator<T, const T&, const T*> const_iterator;
	typedef _slist_iterator<T, Ref, Ptr> self;

	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef _slist_node<T> list_node;

	_slist_iterator(list_node* x):_slist_iterator_base(x){}
	_slist_iterator() :_slist_iterator_base(0) {}
	_slist_iterator(const iterator& x) :_slist_iterator_base(x.node) {}

	reference operator*()const { return ((list_node*)node)->data; }
	pointer operator->()const { return&(operator*()); }
		
	self& operator++() {
		incr();
		return *this;
	}
	//后置
	self operator++(int) {
		self tmp = *this;
		incr();
		return tmp;
	}
};
template<class T,class Alloc=alloc>
class slist {
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef _slist_iterator<T, T&, T*>iterator;
	typedef _slist_iterator<T, const T&, const T*>const_iterator;
private:
	typedef _slist_node<T>list_node;
	typedef _slist_node_base list_node_base;
	typedef _slist_iterator_base iterator_base;
	typedef simple_alloc<list_node, Alloc>list_node_allocator;
	static list_node* create_node(const value_type& x) {
		list_node* node = list_node_allocator::allocate();
		try {
			construct(&node->data, x);
			node->next = 0;
		}
		catch (...) {
			list_node_allocator::deallocate(node);
		}
		return node;
	}
	static void destroy_node(list_node* node) {
		destroy(&node->data);
		list_node_allocator::deallocate(node);
	}
private:
	list_node_base head;
public:
	slist() { head.next = 0; }
	~slist() { clear(); }
	iterator begin() { return iterator((list_node*)head.next); }
	iterator end() { return iterator(0); }
	size_type size()const { return _slist_size(head.next); }
	bool empty()const { return head.next == 0; }
	void swap(slist& L) {
		list_node_base* tmp = head.next;
		head.next = L.head.next;
		L.head.next = tmp;
	}
	reference front() { return ((list_node*)head.next)->data; }
	void push_front(const value_type& x) {
		_slist_make_link(&head, create_node(x));
	}
	void pop_front() {
		list_node* node = (list_node*)head.next;
		head.next = node->next;
		destroy_node(node);
	}
	void clear() {
		if (head.next == 0) { return; }
		while (head.next!=0) {
			list_node tmp = head.next;
			head.next = tmp.next;
			destroy_node(tmp);
		}
	}
};



}

#endif // !XZESTL_SLIST_H
