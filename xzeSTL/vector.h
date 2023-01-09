#pragma once
#ifndef XZESTL_VECTOR_H
#define XZESTL_VECTOR_H
#include "alloc.h"
#include "iterator.h"
#include "construct.h"
#include "uninitialized.h"
#include "exceptdef.h"
namespace xze{
template<class T,class Alloc=alloc>
class vector {
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type* iterator;
	typedef value_type& reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

protected:
	typedef simple_alloc<value_type, Alloc> data_allocator;
	//Ŀǰʹ�ÿռ�ͷ��β�Ϳ��ÿռ�β
	iterator start;
	iterator finish;
	iterator end_of_storage;

	void insert_aux(iterator position, const T& x) {
		if (finish != end_of_storage) {
			//���пռ�
			construct(finish, *(finish - 1));
			++finish;
			//T x_copy = x;
			//������
			copy_backward(position, finish - 2, finish - 1);
			*position = x;
		}
		else {
			//���㹻�ռ�
			const size_type old_size;
			const size_type len = old_size != 0 ? 2 * old_size : 1;
			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;
			try {
				new_finish = uninitialized_copy(start, position, new_start);
				construct(new_finish, x);
				++new_finish;
				new_finish = uninitialized_copy(position, finish, new_finish);
			}
			catch (...) {
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, len);
				throw;
			}

			destroy(begin(), end());
			deallocate();

			start = new_start;
			finish = new_finish;
			end_of_storage = end_of_storage + len;
		}
	}

	void deallocate() {
		if (start) {
			data_allocator::deallocate(start, end_of_storage - start);
		}
	}

	void fill_initialize(size_type n, const T& value) {
		start = allocate_and_fill(n, value);
		finish = start + n;
		end_of_storage = finish;
	}

	iterator allocate_and_fill(size_type n, const T& x) {
		iterator result = data_allocator::allocate(n);
		uinitialized_fill_n(result, n, x);
		return result;
	}

	template <class T>
	void init_space(size_type size, size_type cap)
	{
		try
		{
			start = data_allocator::allocate(cap);
			finish = start + size;
			end_of_storage = start + cap;
		}
		catch (...)
		{
			start = nullptr;
			finish = nullptr;
			end_of_storage = nullptr;
			throw;
		}
	}

	//��������ʼ��
	template <class Iter>
	void range_init(Iter first, Iter last)
	{
		const size_type init_size = std::max(static_cast<size_type>(last - first),
			static_cast<size_type>(16));
		init_space(static_cast<size_type>(last - first), init_size);
		std::copy(first, last, start);
	}

public:
	iterator begin() { return start; }
	iterator end() { return finish; }
	size_type size()const { return size_type(end() - begin()); }
	bool empty()const { return begin() == end(); }
	reference operator[](size_type n) { return *(begin() + n); }
	size_type capacity() {
		return size_type(end_of_storage-start);
	}
	size_type max_size() const noexcept
	{
		//�˴����ֵ��������
		return static_cast<size_type>(-1) / sizeof(T);
	}
	//���캯��
	vector():start(0),finish(0),end_of_storage(0){}
	vector(size_type n, const T& value) { fill_initialize(n, value); }
	vector(int n, const T& value) { fill_initialize(n, value); }
	vector(long n, const T& value) { fill_initialize(n, value); }
	//explicit�ؼ��ַ�ֹ�๹�캯������ʽת��
	explicit vector(size_type n) { fill_initialize(n, T()); }
	//enable_if��ʾ��������ʱ������Ч����Դ�����£���Ϊtrueʱ���Ͳ���Ч
	//template <bool, typename T = void>
	//struct enable_if {
	//};

	//template <typename T>
	//struct enable_if<true, T> {
	//	using type = T;
	//};
	template <class Iterator, typename std::enable_if<
		xze::is_input_iterator<Iterator>::value, int>::type = 0>
	vector(Iterator first, Iterator last)
	{
		XZESTL_DEBUG(!(last < first));
		range_init(first, last);
	}

	vector(const vector& v)
	{
		range_init(v.begin(), v.end());
	}
	//��������
	~vector() {
		destroy(start, finish);
		deallocate();
	}
	//���⹤��
	reference front() { return *begin(); }
	reference back() { return *(end()-1); }
	void push_back(const T&x) {
		if (finish != end_of_storage) {
			construct(finish, x);
			++finish;
		}
		else {
			insert_aux(end(), x);
		}
	}
	void pop_back() {
		--finish;
		destroy(finish);
	}
	//ֱ�Ӹ��Ǻ�destroyβ��
	iterator erase(iterator position) {
		if (position + 1 != end()) {
			copy(position + 1, finish, position);
		}
		--finish;
		destroy(finish);
		return position;
	}
	void resize(size_type new_sz, const T& x) {
		if (new_sz < size()) {
			erase(begin() + new_sz, end());
		}
		else {
			insert(end(), new_sz - size(), x);
		}
	}
	void resize(size_type new_size) { resize(new_size, T()); }
	void clear() {
		erase(begin(), end());
	}
	void insert(iterator position, size_type n, const T& x) {
		if (n != 0) {
			if (size_type(end_of_storage - finish) >= n) {
				//��ȫ����
				T x_copy = x;
				//���㽫Ҫ�ƶ��ĸ���
				const size_type elems_after = finish - position;
				iterator old_finish = finish;
				if (elems_after > n) {
					//Ҫ�ƶ�Ԫ�ش�������
					uninitialized_copy(finish - n, finish, finish);
					finish += n;
					copy_backward(position, old_finish - n, old_finish);
					fill(position, position + n, x_copy);
				}
				else {
					//Ҫ�ƶ�Ԫ��С������
					//����������Ԫ��
					uninitialized_fill_n(finish,n - elems_after, x_copy); 
					finish += n - elems_after;
					//�ƶ�ԭ����Ԫ��
					uninitialized_copy(position, old_finish, finish);
					finish += elems_after;
					fill(position, old_finish, x_copy);
				}
			}
			else {
				//���ÿռ䲻��
				const size_type old_size = size();
				const size_type len = old_size + max(old_size, n);

				iterator new_start = data_allocator::allocate(len);
				iterator new_finish = new_start;
				try {
					new_finish = uninitialized_copy(start, position, new_start);
					new_finish = uninitialized_fill_n(new_finish, n, x);
					new_finish = uninitialized_copy(position, finish, new_finish);
				}
				catch(...) {
					destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, len);
					throw;
				}
				destroy(start, finish);
				deallocate();
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + len;
			}
		} 
	}
	//���n����vector��ǰ��������reserve���vector�������ݡ�\
	��������¶��������·���vector�Ĵ洢�ռ�
	void reserve(size_type n)
	{
		if (capacity() < n)
		{
			THROW_LENGTH_ERROR_IF(n > max_size(),
				"n can not larger than max_size() in vector<T>::reserve(n)");
			const auto old_size = size();
			auto tmp = data_allocator::allocate(n);
			move(begin, end, tmp);
			data_allocator::deallocate(begin, end_of_storage - begin);
			begin = tmp;
			end = tmp + old_size;
			end_of_storage= begin + n;
		}
	}
	void swap(vector<T>& rhs) noexcept
	{
		if (this != &rhs)
		{
			swap(start, rhs.start);
			swap(finish, rhs.finish);
			swap(end_of_storage, rhs.end_of_storage);
		}
	}
	//����������
};




}



#endif // !XZESTL_VECTOR_H#

