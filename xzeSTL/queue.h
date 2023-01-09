#pragma once
#ifndef XZESTL_QUEUE_H
#define XZESTL_QUEUE_H
#include "deque.h"
namespace xze {
//使用deque实现
template<class T,class Sequence=deque<T>>
class queue {
	friend bool operator==_STL_NULL_TMPL_ARGS(const queue& x, const queue& y) { return x.c == y.c; }
	friend bool operator<_STL_NULL_TMPL_ARGS(const queue& x, const queue& y) { return x.c < y.c; }
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
	reference front() { return c.front(); }
	const_reference front()const { return c.front(); }
	reference back() { return c.back(); }
	const_reference back()const { return c.back(); }
	void push(const value_type& x) { c.push_back(x); }
	void pop() { c.pop_front(); }
};
}
#endif // !XZESTL_QUEUE_H
