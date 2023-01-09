#pragma once
#ifndef XZESTL_SET_H
#define XZESTL_SET_H
#include "rbtree.h"
#include "alloc.h"
#include "function.h"

namespace xze {
template<class Key,class Compare=less<Key>,class Alloc=alloc>
class set {
public: 
	typedef Key key_type;
	typedef Key value_type;
	typedef Compare key_compare;
	typedef Compare value_compare;
private:
	template<class T>
	struct identity :public unary_function<T, T> {
		const T& operator()(const T& x)const { return x; }
	};
	typedef rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;

	rep_type t;
public:
	typedef typename rep_type::const_pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::const_reference reference;
	typedef typename rep_type::const_reference const_reference;
	typedef typename rep_type::const_iterator iterator;
	typedef typename rep_type::const_iterator const_iterator;
	typedef typename rep_type::const_reverse_iterator reverse_iterator;
	typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
	typedef typename rep_type::size_type size_type;
	typedef typename rep_type::difference_type difference_type;
	typedef pair<iterator, bool> pair_iterator_bool;
	set():t(Compare()){}
	explicit set(const Compare& comp):t(comp){}

	template<class InputIterator>
	set(InputIterator first, InputIterator last) : t(Compare()) { t.insert_unique(first,last); }
	template<class InputIterator>
	set(InputIterator first, InputIterator last,const Compare& comp) : t(comp) { t.insert_unique(first, last); }

	set(const set<Key,Compare,Alloc>&x):t(x.t){}
	set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc>& x) {
		t = x.t;
		return *this;
	}
	key_compare key_comp()const { return t.key_comp(); }
	value_compare value_comp()const{return t.key_comp(); }

	iterator begin() const{ return t.begin(); }
	iterator end() const { return t.end(); }

	reverse_iterator rbegin() const { return t.rbegin(); }
	reverse_iterator rend() const { return t.rend(); }
	bool empty()const { return t.empty(); }
	size_type size()const { return t.size(); }
	size_type max_size()const { return t.max_size(); }
	void swap(set<Key, Compare, Alloc>& x) {
		t.swap(x.t);
	}

	pair_iterator_bool insert(const value_type& x) {
		pair_iterator_bool p = t.insert_unique(x);
		return pair_iterator_bool(p.first, p.second);
	}

	iterator insert(iterator position,const value_type& x) {
		
		return t.insert_unique(position, x);
	}

	template <class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		t.insert_unique(first, last);
	}

	void clear() {
		t.clear();
	}
	void erase(iterator position) {
		iterator rep_iterator;
		t.erase(position);
	}
	void erase(const key_type& x) {
		return t.erase(x);
	}
	void  erase(iterator first, iterator last) {
		t.erase(first, last);
	}
	iterator find(const key_type& x)const { return t.find(x); }
	size_type count(const key_type& x)const { return t.count_multi(x); }
	iterator lower_bound(const key_type& x)const {
		return t.lower_bound(x);
	}
	iterator upper_bound(const key_type& x)const {
		return t.upper_bound(x);
	}
	pair<iterator, iterator> equal_range(const key_type& x)const {
		return t.equal_range_multi(x);

	}

	friend bool operator==(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
		return x.t == y.t;
	}
	friend bool operator<(const set<Key, Compare, Alloc>&x, const set<Key, Compare, Alloc>&y) {
		return x.t < y.t;
	}
};


template<class Key, class Compare = less<Key>, class Alloc = alloc>
class multiset {
public:
	typedef Key key_type;
	typedef Key value_type;
	typedef Compare key_compare;
	typedef Compare value_compare;
private:
	template<class T>
	struct identity :public unary_function<T, T> {
		const T& operator()(const T& x)const { return x; }
	};
	typedef rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;

	rep_type t;
public:
	typedef typename rep_type::const_pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::const_reference reference;
	typedef typename rep_type::const_reference const_reference;
	typedef typename rep_type::const_iterator iterator;
	typedef typename rep_type::const_iterator const_iterator;
	typedef typename rep_type::const_reverse_iterator reverse_iterator;
	typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
	typedef typename rep_type::size_type size_type;
	typedef typename rep_type::difference_type difference_type;
	typedef pair<iterator, bool> pair_iterator_bool;
	multiset() :t(Compare()) {}
	explicit multiset(const Compare& comp) :t(comp) {}

	template<class InputIterator>
	multiset(InputIterator first, InputIterator last) : t(Compare()) { t.insert_equal(first, last); }
	template<class InputIterator>
	multiset(InputIterator first, InputIterator last, const Compare& comp) : t(comp) { t.insert_equal(first, last); }

	multiset(const multiset<Key, Compare, Alloc>& x) :t(x.t) {}
	multiset<Key, Compare, Alloc>& operator=(const multiset<Key, Compare, Alloc>& x) {
		t = x.t;
		return *this;
	}
	key_compare key_comp()const { return t.key_comp(); }
	value_compare value_comp()const { return t.key_comp(); }

	iterator begin() const { return t.begin(); }
	iterator end() const { return t.end(); }

	reverse_iterator rbegin() const { return t.rbegin(); }
	reverse_iterator rend() const { return t.rend(); }
	bool empty()const { return t.empty(); }
	size_type size()const { return t.size(); }
	size_type max_size()const { return t.max_size(); }
	void swap(multiset<Key, Compare, Alloc>& x) {
		t.swap(x.t);
	}

	iterator insert(const value_type& x) {
		return t.insert_equal(x);
	}

	iterator insert(iterator position, const value_type& x) {

		return t.insert_equal(position, x);
	}

	template <class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		t.insert_equal(first, last);
	}

	void clear() {
		t.clear();
	}
	void erase(iterator position) {
		iterator rep_iterator;
		t.erase(position);
	}
	void erase(const key_type& x) {
		return t.erase(x);
	}
	void  erase(iterator first, iterator last) {
		t.erase(first, last);
	}
	iterator find(const key_type& x)const { return t.find(x); }
	size_type count(const key_type& x)const { return t.count_multi(x); }
	iterator lower_bound(const key_type& x)const {
		return t.lower_bound(x);
	}
	iterator upper_bound(const key_type& x)const {
		return t.upper_bound(x);
	}
	pair<iterator, iterator> equal_range(const key_type& x)const {
		return t.equal_range_multi(x);

	}

	friend bool operator==(const multiset<Key, Compare, Alloc>& x, const multiset<Key, Compare, Alloc>& y) {
		return x.t == y.t;
	}
	friend bool operator<(const multiset<Key, Compare, Alloc>& x, const multiset<Key, Compare, Alloc>& y) {
		return x.t < y.t;
	}
};
}


#endif // !XZESTL_SET_H
