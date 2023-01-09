#pragma once
#ifndef XZESTL_MAP_H
#define XZESTL_MAP_H
#include "util.h"
#include "iterator.h"
#include "alloc.h"
#include "function.h"
#include "rbtree.h"
namespace xze {
template<class T,class Key,class Compare=less<Key>,class Alloc=alloc>
class map {
public:
	typedef Key key_type;
	typedef T data_type;
	typedef T mapped_type;
	typedef pair<const Key, T>value_type;
	typedef Compare key_compare;
	class value_compare : public binary_function <value_type, value_type, bool>
	{
		friend class map<Key, T, Compare,Alloc>;
	protected:
		Compare comp;
		value_compare(Compare c) : comp(c) {}
	public:
		bool operator()(const value_type& x, const value_type& y) const
		{
			return comp(x.first, y.first);  // 比较键值的大小
		}
	};
private:
	template<class T>
	struct identity :public unary_function<T, T> {
		const T& operator()(const T& x)const { return x; }
	};
	typedef rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc>  rep_type;
	rep_type t;
public:
	typedef typename rep_type::pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::reference reference;
	typedef typename rep_type::const_reference const_reference;
	typedef typename rep_type::iterator iterator;
	typedef typename rep_type::const_iterator const_iterator;
	typedef typename rep_type::reverse_iterator reverse_iterator;
	typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
	typedef typename rep_type::size_type size_type;
	typedef typename rep_type::difference_type difference_type_type;
	map():t(Compare()){}
	explicit map(const Compare& comp) :t(comp) {}

	template<class InputIterator>
	map(InputIterator first, InputIterator last) : t(Compare()) { t.insert_unique(first, last); }
	template<class InputIterator>
	map(InputIterator first, InputIterator last, const Compare& comp) : t(comp) { t.insert_unique(first, last); }

	map(const map<Key,T, Compare, Alloc>& x) :t(x.t) {}
	set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc>& x) {
		t = x.t;
		return *this;
	}

	key_compare key_comp()const { return t.key_comp(); }
	value_compare value_comp()const { return t.key_comp(); }
	const_iterator begin()const { return t.begin(); }
	const_iterator end()const { return t.end(); }
	iterator begin() { return t.begin(); }
	iterator end() { return t.end(); }
	const_reverse_iterator rbegin()const { return t.rbegin(); }
	const_reverse_iterator rend()const { return t.rend(); }
	reverse_iterator rbegin() { return t.rbegin(); }
	reverse_iterator rend() { return t.rend(); }
	bool empty()const { return t.empty(); }
	size_type size()const { return t.size(); }
	size_type max_size()const { return t.max_size(); }
	void swap(set<Key, Compare, Alloc>& x) {
		t.swap(x.t);
	}
	T& operator[](const key_type& k) {
		return (*((insert(value_type(k, T()))).first)).second;
	}
	pair<iterator, bool>insert(const value_type& x) {
		return t.insert_unique(x);
	}
	iterator insert(iterator position, const value_type& x) {

		return t.insert_unique(position, x);
	}

	template <class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		t.insert_unique(first, last);
	}
	void erase(iterator position) {
		t.erase(position);
	}
	void erase(const key_type& x) {
		return t.erase(x);
	}
	void  erase(iterator first, iterator last) {
		t.erase(first, last);
	}
	void clear() {
		t.clear();
	}
	iterator find(const key_type& x) { return t.find(x); }
	const_iterator find(const key_type& x)const { return t.find(x); }
	size_type count(const key_type& x)const { return t.count_multi(x); }
	iterator lower_bound(const key_type& x) {
		return t.lower_bound(x);
	}
	iterator upper_bound(const key_type& x) {
		return t.upper_bound(x);
	}
	const_iterator lower_bound(const key_type& x)const {
		return t.lower_bound(x);
	}
	const_iterator upper_bound(const key_type& x)const {
		return t.upper_bound(x);
	}
	pair<iterator, iterator> equal_range(const key_type& x) {
		return t.equal_range_multi(x);
	}
	pair<const_iterator, const_iterator> equal_range(const key_type& x)const {
		return t.equal_range_multi(x);
	}

	friend bool operator==(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
		return x.t == y.t;
	}
	friend bool operator<(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
		return x.t < y.t;
	}
};

}



#endif