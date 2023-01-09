#pragma once
#ifndef XZESTL_STACK_H
#define XZESTL_STACK_H

#include "config.h"
#include "deque.h"
namespace xze {
template<class T,class Sequence=deque<T>>
class stack {
	friend bool operator==_STL_NULL_TMPL_ARGS(const stack& x, const stack& y) { return x.c == y.c; }
	friend bool operator<_STL_NULL_TMPL_ARGS(const stack& x, const stack& y) { return x.c < y.c; }
public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;
protected:
	//底层为deque实现,也可用list
	Sequence c;
public:
	bool empty()const { return c.empty(); }
	size_type size()const { return c.size(); }
	reference top() { return c.back(); }
	const_reference top()const { return c.back(); }
	void push(const value_type& x) { c.push_back(x); }
	void pop() { c.pop_back(); }
};

}
#endif // !XZESTL_STACK_H
