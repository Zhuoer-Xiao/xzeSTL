#pragma once
#ifndef XZESTL_DEQUE_H
#define XZESTL_DEQUE_H
#include "iterator.h"
#include "alloc.h"
#include "construct.h"
#include "uninitialized.h"
namespace xze {
	//������������С��szΪ���͵Ĵ�С�����ػ���������
inline size_t _deque_buf_size(size_t n, size_t sz) {
	return n != 0 ? n:(sz < 512 ? size_t(512 / sz) : size_t(1));
}

template<class T,class Ref,class Ptr,size_t BufSiz>
struct _deque_iterator
{
	typedef _deque_iterator<T, T&, T*, BufSiz> iterator;
	typedef _deque_iterator<T, const T&, const T*, BufSiz> const_iterator;
	static size_t buffer_size() { return _deque_buf_size(BufSiz, sizeof(T)); }
	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T** map_pointer;

	typedef _deque_iterator self;

	T* cur;//�˵�������ָ������������Ԫ��
	T* first;//������������ͷ
	T* last;//������������β(�����ÿռ�)
	map_pointer node;//�ܿ�����
	//��һ��������
	void set_node(map_pointer new_node) {
		node = new_node;
		first = *new_node;
		last = first + difference_type(buffer_size());
	}

	reference operator*()const { return *cur; }
	pointer operator->()const { return &(operator*()); }
	difference_type operator-(const self& x) {
		//��������С*���Ļ�����+this��first����+x��last����
		return difference_type(buffer_size()) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
	}

	self& operator++() {
		++cur;
		if (cur == last) {//��ĩβ�л�������
			set_node(node + 1);
			cur = first;
		}
		return *this;
	}
	self operator++(int) {//����д��
		self* tmp = *this;
		++* this;
		return tmp;
	}
	self& operator--() {
		++cur;
		if (cur == first) {//��ĩβ�л�������
			set_node(node - 1);
			cur = last;
		}
		return *this;
	}
	self operator--(int) {//����д��
		self* tmp = *this;
		--* this;
		return tmp;
	}
	self& operator+=(difference_type n) {
		difference_type offset = n + (cur - first);
		if (offset >= 0 && offset < difference_type(buffer_size())) {
			//��ͬһ��������
			cur += n;
		}
		else {
			difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size())
				: -difference_type((-offset - 1) / buffer_size()) - 1;
			set_node(node + node_offset);
			cur = first + (offset - node_offset * difference_type(buffer_size()));
		}
		return *this;
	}
	self operator+(difference_type n)const {
		self tmp = *this;
		return tmp += n;
	}
	self operator-(difference_type n)const {
		self tmp = *this;
		return tmp -= n;
	}
	self operator-=(difference_type n) { return *this += -n; }
	reference operator[](difference_type n)const { return *(*this + n); }
	bool operator==(const self& x)const { return cur == x.cur; }
	bool operator!=(const self& x)const { return !(*this == x); }
	bool operator<(const self& x)const {
		return (node == x.node) ? (cur < x.cur) : (node < x.node);
	}
};


	//deque˫�˶���
	//����ռ䲢��һ�����壬���Ƿֶε������ռ�,Ĭ��0ʹ��512bytes������
template<class T,class Alloc=alloc,size_t BufSiz=0>
class deque {
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef size_t size_type;
	typedef T& reference;
	typedef _deque_iterator<T, T&, T*, BufSiz> iterator;
	typedef ptrdiff_t difference_type;
	static size_t buffer_size() { return _deque_buf_size(BufSiz, sizeof(T)); }
protected:
	typedef pointer* map_pointer;
	iterator start;
	iterator finish;
	map_pointer map;
	size_type map_size;//map�ڶ���ָ��
	typedef simple_alloc<value_type, Alloc>data_allocator;
	typedef simple_alloc<pointer, Alloc>map_allocator;
	//�����ʼ��С
	static size_type initial_map_size() { return 8; }
	pointer allocate_node() {
		return data_allocator::allocate(_deque_buf_size(sizeof(T)));
	}
	void deallocate_node(pointer p) {
		data_allocator::deallocate(p);
	}
	//����deque�ṹ
	void create_map_and_nodes(size_type num_elements) {
		//�պ�����������һ��
		size_type num_nodes = num_elements / buffer_size() + 1;
		//����map
		map_size = max(initial_map_size(), num_nodes + 2);
		map = map_allocator::allocate(map_size);
		//��start��finishָ���м�
		map_pointer nstart = map + (map_size - num_nodes) / 2;
		map_pointer nfinish = nstart + num_nodes - 1;
		map_pointer cur;
		try {
			for (cur = nstart.node; cur <= nfinish; ++cur) {
				*cur = allocate_node();
			}
		}
		catch (...) {

		}
		start.set_node(nstart);
		finish.set_node(nfinish);
		start.cur = start.first;
		finish.cur = finish.first + num_elements % buffer_size();
	}
	//���������ź�deque���������ֵ
	void fill_initialize(size_type n, const value_type& value) {
		create_map_and_nodes(n);
		map_pointer cur;
		try {
			for (cur = start.node; cur < finish.node; ++cur) {
				uninitialized_fill(*cur, *cur + buffer_size(), value);
			}
			uninitialized_fill(finish.first, finish.cur, value);
		}
		catch (...) {

		}
	}

	void push_back_aux(const value_type &t) {
		value_type t_copy = t;
		reserve_map_at_back();
		*(finish.node + 1) = allocate_node();
		try {
			construct(finish.cur, t_copy);
			finish.set_node(finish.node + 1);
			finish.cur = finish.first;
		}
		catch (...) {
			deallocate_node(*(finish.node + 1));
		}
	}

	void push_front_aux(const value_type& t) {
		value_type t_copy = t;
		//�ж��Ƿ���Ҫ����map
		reserve_map_at_front();
		*(start.node - 1) = allocate_node();
		try {
			start.set_node(start.node - 1);
			start.cur = start.last - 1;
			construct(start.cur, t_copy);
		}
		catch (...) {
			start.set_node(start.node + 1);
			start.cur = start.first;
			deallocate_node(*(start.node - 1));
			throw;
		}
	}
	//���·���map
	void reallocate_map(size_type nodes_to_add, bool add_at_front) {
		size_type old_num_nodes = finish.node - start.node + 1;
		size_type new_num_nodes = old_num_nodes + nodes_to_add;
		map_pointer new_nstart;
		if (map_size > 2 * new_num_nodes) {
			//map����ռ乻��ԭ�ط���
			new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);                
			if (new_nstart < start.node) {
				copy(start.node, finish.node + 1, new_nstart);
			}
			else {
				copy_backward(start.node, finish.node + 1, new_nstart);
			}
		}
		else {
			//�����¿ռ�
			size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;
			map_pointer new_map= map_allocator::allocate(new_map_size);
			new_nstart = map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
			copy(start.node, finish.node + 1, new_nstart);
			map_allocator::deallocate(map, map_size);
			map = new_map;
			map_size = new_map_size;
		}
		start.set_node(new_nstart);
		finish.set_node(new_nstart + old_num_nodes - 1);
	}
	//β������
	void reserve_map_at_back(size_type nodes_to_add=1) {
		if (nodes_to_add + 1 > map_size - (finish.node - map)) {
			//β�˿ռ䲻��
			reallocate_map(nodes_to_add);
		}
	}
	//ͷ������
	void reserve_map_at_front(size_type nodes_to_add = 1) {
		if (nodes_to_add > start.node-map) {
			//β�˿ռ䲻��
			reallocate_map(nodes_to_add,true);
		}
	}
	//���һ���������ͷ�,
	void pop_back_aux() {
		deallocate_node(finish.first);
		finish.set_node(finish.node - 1);
		finish.cur = finish.last - 1;
		destroy(finish.cur);
	}
	//��һ�����������һ��Ԫ���ͷ�
	void pop_front_aux() {
		destroy(start.cur);
		deallocate_node(start.first);
		start.set_node(start.node + 1);
		start.cur = start.first;
		
	}
	iterator insert_aux(iterator pos, const value_type& x) {
		difference_type index = pos - start;
		value_type x_copy = x;
		if (index < size() / 2) {
			//ǰ���Ԫ����
			push_front(front());
			iterator front1 = start;
			++front1;
			iterator front2 = front1;
			++front2;
			iterator pos1 = pos;
			++pos1;
			copy(front2, pos1, front1);
		}
		else {
			push_back(back());
			iterator back1 = finish;
			--back1;
			iterator back2 = back1;
			--back2;
			copy_back_ward(pos, back2, back1);
		}
		*pos = x_copy;
		return pos;
	}

public:
	iterator begin() { return start; }
	iterator end() { return finish; }
	reference operator[](size_type n) {
		return start[difference_type(n)];
	}
	reference front() { return *start; }
	reference back() {
		iterator tmp = finish;
		--tmp;
		return *tmp;//û�и�finish����������
	}
	size_type size()const { return finish - start; }
	//TODO
	size_type max_size()const { return size_type(-1); }
	bool empty()const { return finish == start;  }
	//�������캯��
	deque(int n, const value_type& value) :start(), finish(), map(0), map_size(0) {
		fill_initialize(n, value);
	}
	deque(const Alloc&)
		:start(), finish(), map(0), map_size(0) {}
	void push_back(const value_type &t) {
		if (finish.cur != finish.last - 1) {
			construct(finish.cur, t);
			++finish.cur;
		}
		else {
			push_back_aux(t);
		}
	}
	void push_front(const value_type& t) {
		if (start.cur != start.first ) {
			construct(start.cur-1, t);
			--start.cur;
		}
		else {
			push_front_aux(t);
		}
	}
	void pop_back() {
		if (finish.cur != finish.first) {
			//��ֹһ��Ԫ��
			--finish.cur;
			destroy(finish.cur);
		}
		else {
			//�������ͷ�
			pop_back_aux();
		}
	}

	void pop_front() {
		if (start.cur != start.last-1) {
			//��ֹһ��Ԫ��
			
			destroy(start.cur);
			++start.cur;
		}
		else {
			//�������ͷ�
			pop_front_aux();
		}
	}
	//��յ�����һ��������
	void clear() {
		//���ͷβ֮��
		for (map_pointer node = start.node + 1; node < finish.node; ++node) {
			destroy(*node, *node + buffer_size());
			data_allocator::deallocate(*node, buffer_size());
		}
		if (start.node != finish.node) {
			destroy(start.cur, start.last);
			destroy(finish.first, finish.cur);
			data_allocator::deallocate(finish.first, buffer_size());
		}
		else {
			destroy(start.cur, finish.cur);
		}
		finish = start;
	}

	iterator erase(iterator pos) {
		iterator next = pos;
		++next;
		difference_type index = pos - start;
		if (index < (size() >> 1)) {//�����ı��ƴ���С
			copy_backward(start, pos, next);
			pop_front();
		}
		else {
			copy(next, finish, pos);
			pop_back();
		}
		return start + index;
	}
	iterator erase(iterator first, iterator last) {
		if (first == start && last == finish) {
			clear();
			return finish;
		}
		else {//�����Χ������������
			difference_type n = last - first;
			difference_type elems_before = first - start;
			if (elems_before < (size() - n) / 2) {
				//����ƶ�
				copy_backward(start, finish, last);
				iterator new_start = start + n;
				destroy(start, new_start);
				for (map_pointer cur = start.node; cur < new_start.node; ++cur) {
					data_allocator::deallocate(*cur, buffer_size());
				}
				start = new_start;
			}
			else {
				copy(last, finish, first);
				iterator new_finish = finish - n;
				destroy(new_finish, finish);
				for (map_pointer cur = new_finish + 1; cur <= finish.node; ++cur) {
					data_allocator::deallocate(*cur, buffer_size());
				}
				finish = new_finish;
			}
			return start + elems_before;
		}
	}

	iterator insert(iterator position, const value_type& x) {
		if (position.cur == start.cur) {
			push_front(x);
			return start;
		}
		else if (position.cur == finish.cur) {
			push_back(x);
			iterator tmp = finish;
			--tmp;
			return tmp;
		}
		else {
			return insert_aux(position, x);
		}
	}
};
}


#endif // !XZESTL_DEQUE_H
