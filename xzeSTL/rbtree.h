#pragma once
#ifndef XZESTL_RBTREE_H
#define XZESTL_RBTREE_H
#include "iterator.h"
#include "alloc.h"
#include "construct.h"
#include "exceptdef.h"
#include "util.h"
namespace xze {
typedef bool _rb_tree_color_type;
const _rb_tree_color_type _rb_tree_red = false;
const _rb_tree_color_type _rb_tree_black = true;

struct _rb_tree_node_base {
	typedef _rb_tree_color_type color_type;
	typedef _rb_tree_node_base* base_ptr;
	color_type color;
	base_ptr parent;
	base_ptr left;
	base_ptr right;

	static base_ptr minimum(base_ptr x) {
		while (x->left != 0)
		{
			x = x->left;
		}return x;
	}
	static base_ptr maximum(base_ptr x) {
		while (x->right != 0)
		{
			x = x->right;
		}return x;
	}
};

template<class Value>
struct _rb_tree_node:public _rb_tree_node_base
{
	typedef _rb_tree_node<Value>* link_type;
	Value value_field;
};
struct _rb_tree_base_iterator
{
	typedef _rb_tree_node_base::base_ptr base_ptr;
	typedef bidirectional_iterator_tag iterator_category;
	typedef ptrdiff_t difference_type;
	base_ptr node;
	void increment() {
		if (node->right != 0) {
			node = node->right;
			while (node->left != 0) {
				node = node->left;
			}
		}
		else {
			base_ptr y = node->parent;
			while (node == y->right) {
				node = y;
				y = y->parent;
			}
			if (node->right != y) {
				node = y;
			}
		}
	}

	void decrement() {
		//指向header
		if (node->color == _rb_tree_red && node->parent->parent == node) {
			node = node->right;
		}
		else if (node->left != 0) {
			base_ptr y = node->left;
			while (y->right != 0) {
				y = y->right;
			}
			node = y;
		}
		else {
			base_ptr y = node->parent;
			while (node == y->left) {
				node = y;
				y = y->parent;
			}
			node = y;
		}
	}
};
template<class Value,class Ref,class Ptr>
struct _rb_tree_iterator:public _rb_tree_base_iterator
{
	
	typedef Value value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef _rb_tree_iterator<Value, Value&, Value*> iterator;
	typedef _rb_tree_iterator<Value, const Value&, const Value*> const_iterator;
	typedef _rb_tree_iterator<Value, Ref, Ptr> self;
	typedef _rb_tree_node<Value>* link_type;
	_rb_tree_iterator(){}
	_rb_tree_iterator(link_type x) { node = x; }
	_rb_tree_iterator(const iterator& it) { node = it.node; }

	reference operator*() { return link_type(node)->value_field; }
	pointer operator->()const { return &(operator*()); }

	self& operator++() { 
		increment();
		return *this;
	}

	self& operator--() {
		decrement();
		return *this;
	}
	self operator++(int) {
		self tmp = *this;
		increment();
		return tmp;
	}

	self operator--(int) {
		self tmp = *this;
		decrement();
		return tmp;
	}
};
template<class Key,class Value,class KeyOfValue,class Compare,class Alloc=alloc>
class rb_tree {
protected:
	typedef void* void_pointer;
	typedef _rb_tree_node_base* base_ptr;
	typedef _rb_tree_node<Value> rb_tree_node;
	typedef simple_alloc<rb_tree_node,Alloc> rb_tree_node_allocator;
	typedef _rb_tree_color_type color_type;
public:
	typedef Key key_type;
	typedef Value value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef rb_tree_node* link_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
protected: 
	link_type get_node() {
		return rb_tree_node_allocator::allocate;
	}
	void put_node(link_type p) {
		rb_tree_node_allocator::deallocate(p);
	}
	link_type create_node(const value_type& x) {
		link_type tmp = get_node();
		try {
			construct(&tmp->value_field,x);
		}
		catch (...) {
			put_node(tmp);
		}
		return tmp;
	}
	link_type clone_node(link_type x) {
		link_type tmp = create_node(x->value_field);
		tmp->color = x->color;
		tmp->left = 0;
		tmp->right = 0;
		return tmp;
	}
	void destroy_node(link_type p) {
		destroy(&p->value_field);
		put_node(p);
	}
protected:
	size_t node_count;
	link_type header;
	Compare key_compare;

	link_type& root()const { return (link_type&)header->parent; }
	link_type& leftmost()const { return (link_type&)header->left; }
	link_type& rightmost()const { return (link_type&)header->right; }
	//取x的成员
	static link_type& left(link_type x) { return (link_type&)(x->left); }
	static link_type& right(link_type x) { return (link_type&)(x->right); }
	static link_type& parent(link_type x) { return (link_type&)(x->parent); }
	static reference value(link_type x) { return x->value_field; }
	static const Key& key(link_type x) { return KeyOfValue()(value(x)); }
	static color_type& color(link_type x) { return (color_type&)(x->color); }

	static link_type& left(base_ptr x) { return (link_type&)(x->left); }
	static link_type& right(base_ptr x) { return (link_type&)(x->right); }
	static link_type& parent(base_ptr x) { return (link_type&)(x->parent); }
	static reference value(base_ptr x) { return ((link_type)x)->value_field; }
	static const Key& key(base_ptr x) { return KeyOfValue()(value(x)); }
	static color_type& color(base_ptr x) { return (color_type&)(x->color); }

public:
	static link_type minimum(link_type x) {
		return (link_type)_rb_tree_node_base::minimum(x);
	}
	static link_type maximum(link_type x) {
		return (link_type)_rb_tree_node_base::maximum(x);
	}
	typedef _rb_tree_iterator<value_type, reference, pointer> iterator;
	typedef _rb_tree_iterator<value_type, const reference, const pointer> const_iterator;
	typedef reverse_iterator<iterator> reverse_iterator;
	typedef xze::reverse_iterator<const_iterator>  const_reverse_iterator;
private:
	//x为新值插入点，y为父节点
	iterator _insert(base_ptr x_, base_ptr y_, const value_type& v) {
		link_type x = (link_type)x_;
		link_type y = (link_type)y_;
		link_type z;
		//key_compare键值大小比较准则
		if (y == header || x != 0 || key_compare(KeyOfValue()(v), key(y))) {
			z = create_node(v);
			left(y) = z;
			if (y == header) {
				root() = z;
				rightmost = z;
			}
			else if (y == leftmost()) {
				leftmost() = z;
			}
		}
		else {
			z = create_node(v);
			right(y) = z;
			if (y == rightmost()) {
				rightmost() = z;
			}
		}
		parent(z) = y;
		left(z) = 0;
		right(z) = 0;
		_rb_tree_rebalance(z, header->parent);
		++node_count;
		return iterator(z);
	}
	//调整树
	inline void _rb_tree_rebalance(_rb_tree_node_base* x, _rb_tree_node_base*& root) {
		x->color = _rb_tree_red;//新结点为red
		while (x != root && x->parent->color = _rb_tree_red) {
			//父节点为红
			if (x->parent == x->parent->parent->left) {
				//父节点为左子结点
				_rb_tree_node_base* y = x->parent->parent->right;//y为伯父结点
				if (y&& y->color = _rb_tree_red) {
					//伯父结点存在且为红色,改上一层为黑
					x->parent->color = _rb_tree_black;
					y->color = _rb_tree_black;
					x->parent->parent->color = _rb_tree_red;//祖父为红
					x = x->parent->parent;
				}
				else {
					//无伯父或伯父为黑
					if (x == x->parent->right) {
						//新结点为右节点
						x = x->parent;
						_rb_tree_rotate_left(x, root);//x为左旋点
					}
					x->parent->color = _rb_tree_black;
					x->parent->parent->color = _rb_tree_red;
					_rb_tree_rotate_right(x->parent->parent, root);
				}
			}
			else {
				//父节点为右结点
				_rb_tree_node_base* y = x->parent->parent->left;
				if (y&& y->color = _rb_tree_red) {
					//伯父结点存在且为红
					y->color = _rb_tree_black;
					x->parent->color = _rb_tree_black;
					x->parent->parent->color = _rb_tree_red;//继续向上检查
				}
				else {
					if (x == x->parent->left) {
						//新结点为左结点
						x = x->parent;
						_rb_tree_rotate_right(x, root);
					}
					x->parent->color = _rb_tree_black;
					x->parent->parent->color = _rb_tree_red;
					_rb_tree_rotate_left(x->parent->parent, root);
				}
			}
		}
		root->color = _rb_tree_black;//根节点永远黑
	}

	inline void _rb_tree_rotate_left(_rb_tree_node_base* x, _rb_tree_node_base*& root) {
		_rb_tree_node_base* y = x->right;
		x->right = y->left;
		if (y->left != 0) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x == root) {
			root = y;
		}
		else if (x == x->parent->left) {
			x->parent->left = y;
		}
		else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}
	inline void _rb_tree_rotate_right(_rb_tree_node_base* x, _rb_tree_node_base*& root) {
		_rb_tree_node_base* y = x->left;
		x->left = y->right;
		if (y->right != 0) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x == root) {
			root = y;
		}
		else if (x == x->parent->right) {
			x->parent->right = y;
		}
		else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}
	
	void init() {
		header = get_node();
		color(header) = _rb_tree_red;
		root() = 0;
		leftmost() = header;
		rightmost() = header;
	}
public:
	rb_tree(const Compare& comp = Compare()) :node_count(0), key_compare(comp) { init(); }
	~rb_tree() {
		clear();
		put_node(header);
	}
	Compare key_comp()const { return key_compare; }
	iterator begin() { return leftmost(); }
	iterator end() { return header; }
	reverse_iterator rbegin()
	{
		return reverse_iterator(end());
	}
	const_reverse_iterator rbegin()
	{
		return const_reverse_iterator(end());
	}
	reverse_iterator rend()
	{
		return reverse_iterator(begin());
	}
	const_reverse_iterator rend()
	{
		return const_reverse_iterator(begin());
	}
	bool empty()const { return node_count == 0; }
	size_type size()const { return node_count; }
	size_type max_size()const { return size_type(-1); }
	//允许插入重复值
	typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator
		insert_equal(const Value& v) {
		link_type y = header;
		link_type x = root();
		while (x != 0) {//寻找插入位置
			y = x;
			x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
		}
		return _insert(x, y, v);
	}
	//不允许插入重复值
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
		insert_unique(const Value& v) {
		link_type y = header;
		link_type x = root();
		bool comp = true;
		while (x != 0) {//寻找插入位置,y为父节点
			y = x;
			comp = key_compare(KeyOfValue()(v), key(x));
			x = comp  ? left(x) : right(x);
		}

		iterator j = iterator(y);
		if (comp) {
			//离开时遇大，插入左侧
			return pair<iterator, bool>(_insert(x, y, v), true);
		}
		else {
			--j;
		}
		if (key_compare(key(j.node), KeyOfValue()(v))) {
			return pair<iterator, bool>(_insert(x, y, v), true);
		}
		return pair<iterator,bool>(j,false);
	}
	template <class InputIterator>
	void insert_unique(InputIterator first, InputIterator last)
	{
		size_type n = distance(first, last);
		THROW_LENGTH_ERROR_IF(node_count > max_size() - n, "rb_tree<T, Comp>'s size too big");
		for (; n > 0; --n, ++first)
			insert_unique(end(), *first);
	}


	iterator find(const Key&k) {
		link_type y = header;
		link_type x = root();
		while (x != 0) {
			if (!key_compare(key(x), k)) {
				y = x,x = left(x);
			}
			else {
				x = right(x);
			}
		}
		iterator j = iterator(y);
		return (j == end() || key_compare(k, key(j.node))) ? end() : j;
	}
	base_ptr copy(base_ptr x, base_ptr p) {
		link_type top = clone_node(x);
		top->parent = p;
		try
		{
			if (x->right)
				top->right = copy_from(x->right, top);
			p = top;
			x = x->left;
			while (x != nullptr)
			{
				link_type y = clone_node(x);
				p->left = y;
				y->parent = p;
				if (x->right)
					y->right = copy_from(x->right, y);
				p = y;
				x = x->left;
			}
		}
		catch (...)
		{
			erase_since(top);
			throw;
		}
		return top;
	}

	void erase_since(base_ptr x)
	{
		while (x != nullptr)
		{
			erase_since(x->right);
			auto y = x->left;
			destroy_node(x);
			x = y;
		}
	}

	void clear()
	{
		if (node_count != 0)
		{
			erase_since(root());
			leftmost() = header;
			root() = nullptr;
			rightmost() = header;
			node_count = 0;
		}
	}
	void swap(rb_tree& rhs) 
	{
		if (this != &rhs)
		{
			swap(header, rhs.header);
			swap(node_count, rhs.node_count);
			swap(key_comp, rhs.key_comp);
		}
	}

	template <class T, class Compare>
	iterator erase(iterator hint)
	{
		auto node = hint.node;
		iterator next(node);
		++next;

		rb_tree_erase_rebalance(hint.node, root(), leftmost(), rightmost());
		destroy_node(node);
		--node_count;
		return next;
	}

	void erase(iterator first, iterator last)
	{
		if (first == begin() && last == end())
		{
			clear();
		}
		else
		{
			while (first != last)
				erase(first++);
		}
	}

	size_type erase_unique(const key_type& key)
	{
		auto it = find(key);
		if (it != end())
		{
			erase(it);
			return 1;
		}
		return 0;
	}

	size_type erase(const key_type& key)
	{
		auto p = equal_range_multi(key);
		size_type n = distance(p.first, p.second);
		erase(p.first, p.second);
		return n;
	}

	iterator lower_bound(const key_type& key)
	{
		auto y = header;
		auto x = root();
		while (x != nullptr)
		{
			if (!key_comp(key(x->value_field), key))
			{ // key <= x
				y = x, x = x->left;
			}
			else
			{
				x = x->right;
			}
		}
		return iterator(y);
	}

	const_iterator lower_bound(const key_type& key) const
	{
		auto y = header;
		auto x = root();
		while (x != nullptr)
		{
			if (!key_comp(key(x->value_field), key))
			{ // key <= x
				y = x, x = x->left;
			}
			else
			{
				x = x->right;
			}
		}
		return const_iterator(y);
	}

	// 键值不小于 key 的最后一个位置
	iterator upper_bound(const key_type& key)
	{
		auto y = header;
		auto x = root();
		while (x != nullptr)
		{
			if (key_comp(key, key(x->value_field)))
			{ // key < x
				y = x, x = x->left;
			}
			else
			{
				x = x->right;
			}
		}
		return iterator(y);
	}

	const_iterator upper_bound(const key_type& key) const
	{
		auto y = header;
		auto x = root();
		while (x != nullptr)
		{
			if (key_comp(key, key(x->value_field)))
			{ // key < x
				y = x, x = x->left;
			}
			else
			{
				x = x->right;
			}
		}
		return const_iterator(y);
	}

	pair<iterator, iterator>
		equal_range_multi(const key_type& key)
	{
		return pair<iterator, iterator>(lower_bound(key), upper_bound(key));
	}
	pair<const_iterator, const_iterator>
		equal_range_multi(const key_type& key) const
	{
		return pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key));
	}

	size_type count_multi(const key_type& key) const
	{
		auto p = equal_range_multi(key);
		return static_cast<size_type>(distance(p.first, p.second));
	}
	size_type count_unique(const key_type& key) const
	{
		return find(key) != end() ? 1 : 0;
	}
};



}


#endif // !XZESTL_RBTREE_H
