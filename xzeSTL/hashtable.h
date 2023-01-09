#pragma once
#ifndef XZESTL_HASHTABLE_H
#define XZESTL_HASHTABLE_H
#include "iterator.h"
#include "alloc.h"
#include "vector.h"
#include "construct.h"
#include "adaptor.h"
#include "function.h"
namespace xze {
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc = alloc>
class hashtable;

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
struct _hashtable_const_iterator;

template<class Value>
struct _hashtable_node {
	_hashtable_node* next;
	Value val;
};

//迭代器
template<class Value,class Key,class HashFcn,class ExtractKey,class EqualKey,class Alloc>
struct _hashtable_iterator {
	typedef xze::hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> hashtable;
	typedef xze::_hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
	typedef _hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;
	typedef typename _hashtable_node<Value> node;
	typedef forward_iterator_tag iterator_category;
	typedef Value value_type;
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;
	typedef Value& reference;
	typedef Value* pointer;
	
	node* cur;
	hashtable* ht;
	_hashtable_iterator(node*n,hashtable*tab):cur(n),ht(tab){}
	_hashtable_iterator(){}
	reference operator*()const { return cur->val; }
	pointer operator->()const { return &(operator*()); }
	iterator& operator++() {
		const node* old = cur;
		cur = cur->next;
		if (!cur) {
			size_type bucket = ht->bkt_num(old->val);
			while (!cur && ++bucket < ht->buckets.size()) {
				cur = ht->buckets[bucket];
			}
		}
		return *this;
	}
	iterator operator++(int) {
		iterator tmp = *this;
		++* this;
		return tmp;
	}
	bool operator==(const iterator& it)const { return cur == it.cur; }
	bool operator!=(const iterator& it)const { return cur != it.cur; }
};
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
struct _hashtable_const_iterator {
	typedef _hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
	typedef _hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;
	typedef _hashtable_node<Value> node;
	typedef forward_iterator_tag iterator_category;
	typedef Value value_type;
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;
	typedef Value& reference;
	typedef Value* pointer;
	node* cur;
	typedef typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> hashtable;
	hashtable* ht;
	_hashtable_const_iterator(node* n, hashtable* tab) :cur(n), ht(tab) {}
	_hashtable_const_iterator() {}
	reference operator*()const { return cur->val; }
	pointer operator->()const { return &(operator*()); }
	const_iterator& operator++() {
		const node* old = cur;
		cur = cur->next;
		if (!cur) {
			size_type bucket = ht->bkt_num(old->val);
			while (!cur && ++bucket < ht->buckets.size()) {
				cur = ht->buckets[bucket];
			}
		}
		return *this;
	}
	const_iterator operator++(int) {
		const_iterator tmp = *this;
		++* this;
		return tmp;
	}
	bool operator==(const iterator& it)const { return cur == it.cur; }
	bool operator!=(const iterator& it)const { return cur != it.cur; }
};

// bucket 使用的大小

#if (_MSC_VER && _WIN64) || ((__GNUC__ || __clang__) &&__SIZEOF_POINTER__ == 8)
#define SYSTEM_64 1
#else
#define SYSTEM_32 1
#endif

#ifdef SYSTEM_64

#define PRIME_NUM 99

// 1. start with p = 101
// 2. p = next_prime(p * 1.7)
// 3. if p < (2 << 63), go to step 2, otherwise, go to step 4
// 4. end with p = prev_prime(2 << 63 - 1)
static const unsigned long ht_prime_list[] = {
  101ull, 173ull, 263ull, 397ull, 599ull, 907ull, 1361ull, 2053ull, 3083ull,
  4637ull, 6959ull, 10453ull, 15683ull, 23531ull, 35311ull, 52967ull, 79451ull,
  119179ull, 178781ull, 268189ull, 402299ull, 603457ull, 905189ull, 1357787ull,
  2036687ull, 3055043ull, 4582577ull, 6873871ull, 10310819ull, 15466229ull,
  23199347ull, 34799021ull, 52198537ull, 78297827ull, 117446801ull, 176170229ull,
  264255353ull, 396383041ull, 594574583ull, 891861923ull, 1337792887ull,
  2006689337ull, 3010034021ull, 4515051137ull, 6772576709ull, 10158865069ull,
  15238297621ull, 22857446471ull, 34286169707ull, 51429254599ull, 77143881917ull,
  115715822899ull, 173573734363ull, 260360601547ull, 390540902329ull,
  585811353559ull, 878717030339ull, 1318075545511ull, 1977113318311ull,
  2965669977497ull, 4448504966249ull, 6672757449409ull, 10009136174239ull,
  15013704261371ull, 22520556392057ull, 33780834588157ull, 50671251882247ull,
  76006877823377ull, 114010316735089ull, 171015475102649ull, 256523212653977ull,
  384784818980971ull, 577177228471507ull, 865765842707309ull, 1298648764060979ull,
  1947973146091477ull, 2921959719137273ull, 4382939578705967ull, 6574409368058969ull,
  9861614052088471ull, 14792421078132871ull, 22188631617199337ull, 33282947425799017ull,
  49924421138698549ull, 74886631708047827ull, 112329947562071807ull, 168494921343107851ull,
  252742382014661767ull, 379113573021992729ull, 568670359532989111ull, 853005539299483657ull,
  1279508308949225477ull, 1919262463423838231ull, 2878893695135757317ull, 4318340542703636011ull,
  6477510814055453699ull, 9716266221083181299ull, 14574399331624771603ull, 18446744073709551557ull
};

#else

#define PRIME_NUM 44

// 1. start with p = 101
// 2. p = next_prime(p * 1.7)
// 3. if p < (2 << 31), go to step 2, otherwise, go to step 4
// 4. end with p = prev_prime(2 << 31 - 1)
static constexpr size_t ht_prime_list[] = {
  101u, 173u, 263u, 397u, 599u, 907u, 1361u, 2053u, 3083u, 4637u, 6959u,
  10453u, 15683u, 23531u, 35311u, 52967u, 79451u, 119179u, 178781u, 268189u,
  402299u, 603457u, 905189u, 1357787u, 2036687u, 3055043u, 4582577u, 6873871u,
  10310819u, 15466229u, 23199347u, 34799021u, 52198537u, 78297827u, 117446801u,
  176170229u, 264255353u, 396383041u, 594574583u, 891861923u, 1337792887u,
  2006689337u, 3010034021u, 4294967291u,
};

#endif

// 找出最接近并大于等于 n 的那个质数
inline unsigned long ht_next_prime(unsigned long n)
{
	const unsigned long* first = ht_prime_list;
	const unsigned long* last = ht_prime_list + PRIME_NUM;
	const unsigned long* pos = lower_bound(first, last, n);
	return pos == last ? *(last - 1) : *pos;
}


template<class Value,class Key,class HashFcn,class ExtractKey, class EqualKey, class Alloc>
class hashtable {
public:
	typedef HashFcn hasher;
	typedef EqualKey key_equal;//判断相同方法.
	typedef Value value_type;
	typedef Key key_type;
	typedef _hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;
	typedef _hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
	typedef  Value*          pointer;
	typedef  const Value* const_pointer;
	typedef  Value&          reference;
	typedef  const Value& const_reference;
	typedef size_t          size_type;
	typedef ptrdiff_t    difference_type;

	typedef _hashtable_node<Value>  node_type;
	typedef node_type*  node_ptr;
	typedef vector<node_ptr> bucket_type;
private:
	hasher hash;
	key_equal equals;
	ExtractKey get_key;
	size_type bucket_size;
	typedef _hashtable_node<Value> node;
	typedef simple_alloc<node, Alloc>node_allocator;
	vector<node*, Alloc>buckets;
	size_type num_elements;
	float mlf_;
	bool is_equal(const key_type& key1, const key_type& key2)
	{
		return equals(key1, key2);
	}

	bool is_equal(const key_type& key1, const key_type& key2) const
	{
		return equals(key1, key2);
	}
public:
	size_type size()const noexcept { return num_elements; }
	bool empty()const noexcept { return num_elements==0; }
	size_type max_size() const noexcept { return static_cast<size_type>(-1); }
	size_type bucket_count()const { return buckets.size(); }
	size_type max_bucket_count()const {
		return ht_prime_list[PRIME_NUM - 1];
	}
	node* new_node(const value_type& obj) {
		node* n = node_allocator::allocate();
		n->next = 0;
		try {
			construct(&n->val, obj);
			return n;
		}
		catch (...) {
			node_allocator::deallocate(n);
		}
	}
	void delete_node(node* n) {
		destroy(&n->val);
		node_allocator::deallocate(n);
	}
	hashtable(size_type n, const HashFcn& hf, const EqualKey& eql) :hash(hf), equals(eql), get_key(ExtractKey()), num_elements(0) {
		initialize_buckets(n);
	}


	hashtable(const hashtable& rhs)
		:hash(rhs.hash), equals(rhs.equals)
	{
		copy_from(rhs);
	}
	hashtable(hashtable&& rhs) noexcept
		: bucket_size(rhs.bucket_size),
		size(rhs.size),
		hash(rhs.hash),
		equals(rhs.equals)
	{
		buckets = move(rhs.buckets);
	}
	void initialize_buckets(size_type n) {
		const size_type n_buckets = next_size();
		buckets.reserve(n_buckets);
		buckets.insert(buckets.end(), n_buckets, (node*)0);
		num_elements = 0;
	}
	size_type next_size(size_type n)const { return ht_next_prime(n); }
	//负载因子控制
	float load_factor() const noexcept
	{
		return buckets.size() != 0 ? (float)num_elements / buckets.size() : 0.0f;
	}

	float max_load_factor() const noexcept
	{
		return mlf_;
	}
	void max_load_factor(float ml)
	{
		THROW_OUT_OF_RANGE_IF(ml != ml || ml < 0, "invalid hash load factor");
		mlf_ = ml;
	}

	void rehash_if_need(size_type n)
	{
		if (static_cast<float>(num_elements + n) > buckets.size() * max_load_factor())
			rehash(size + n);
	}

	void rehash(size_type count)
	{
		auto n = ht_next_prime(count);
		if (n > buckets.size())
		{
			replace_bucket(n);
		}
		else
		{
			if ((float)size / (float)n < max_load_factor() - 0.25f &&
				(float)n < (float)buckets.size() * 0.75)  // worth rehash
			{
				replace_bucket(n);
			}
		}
	}


	//插入
	template <class InputIter>
	void copy_insert_unique(InputIter first, InputIter last, input_iterator_tag)
	{
		rehash_if_need(distance(first, last));
		for (; first != last; ++first)
			insert_unique_noresize(*first);
	}

	template <class InputIter>
	void insert_unique(InputIter first, InputIter last)
	{
		copy_insert_unique(first, last, iterator_category(first));
	}

	pair<iterator, bool> insert_unique(const value_type& obj) {
		resize(num_elements + 1);
		return insert_unique_noresize(obj);
	}
	//表格重整
	void resize(size_type num_elements_hint) {
		const size_type old_n = buckets.size();
		if (num_elements_hint > old_n) {
			const size_type n = next_size(num_elements_hint);//下一个质数
			if (n > old_n) {
				vector<node*, Alloc>tmp(n, (node*)0);//设立新bucket
				try {
					for (size_type bucket = 0; bucket < old_n; ++bucket) {
						node* first = buckets[bucket];
						while (first) {
							size_type new_bucket = bkt_num(first->val, n);
							buckets[bucket] = first->next;
							first->next = tmp[new_bucket];
							tmp[new_bucket] = first;
							first = buckets[bucket];
						}
					}
					buckets.swap(tmp);
				}	
				catch (...) {
					throw;
				}
			}

		}
	}

	pair<iterator, bool> insert_unique_noresize(const value_type& obj) {
		const size_type n = bkt_num(obj);
		node* first = buckets[n];
		//如果buckets[n]被占用，走过整个链表
		for (node* cur; cur; cur = cur->next) {
			if (equals(get_key(cur->val), get_key(obj))) {
				//有相同值，返回
				return pair<iterator, bool>(iterator(cur, this), false);
			}
		}
		//制作新结点
		node* tmp = new_node(obj);
		tmp->next = first;
		buckets[n] = tmp;
		++num_elements;
		return pair<iterator, bool>(iterator(tmp, this), true);
	}
	//插入

	template <class InputIter>
	void copy_insert_equal(InputIter first, InputIter last, input_iterator_tag)
	{
		rehash_if_need(distance(first, last));
		for (; first != last; ++first)
			insert_equal_noresize(*first);
	}

	template <class InputIter>
	void insert_equal(InputIter first, InputIter last)
	{
		copy_insert_equal(first, last, iterator_category(first));
	}

	iterator insert_equal_noresize(const value_type& obj) {
		const size_type n = bkt_num(obj);
		node* first = buckets[n];
		//如果buckets[n]被占用，走过整个链表
		for (node* cur; cur; cur = cur->next) {
			if (equals(get_key(cur->val), get_key(obj))) {
				node* tmp = new_node(obj);
				tmp->next = cur->next;
				cur->next = tmp;
				++num_elements;
				return iterator(tmp, this);
			}
		}
		//制作新结点
		node* tmp = new_node(obj);
		tmp->next = first;
		buckets[n] = tmp;
		++num_elements;
		return iterator(tmp, this);
	}
	//判断元素落脚处
	//value和元素个数
	size_type bkt_num(const value_type& obj, size_t n)const {
		return bkt_num_key(get_key(obj), n);
	}
	size_type bkt_num(const value_type& obj)const {
		return bkt_num_key(get_key(obj));
	}
	size_type bkt_num_key(const key_type& key)const {
		return bkt_num_key(key,buckets.size());
	}
	size_type bkt_num_key(const key_type& key,size_t n)const {
		return hash(key)%n;
	}
	void reserve(size_type n) {
		if (this->buckets.size() > n) {
			return;
		}
		else {
			this->resize(n);
		}
	}
	void clear() {
		for (size_type i = 0; i < buckets.size(); ++i) {
			node* cur = buckets[i];
			while (cur != 0) {
				node* next = cur->next;
				delete_node(cur);
				cur = next;
			}
			buckets[i] = nullptr;
		}
		num_elements = 0;
	}
	void copy_from(const hashtable& ht) {
		buckets.clear();
		buckets.reserve(ht.buckets.size());
		//初始化
		buckets.insert(buckets.end(), ht.buckets.size(), nullptr);
		try {
			for (size_type i = 0; i < ht.buckets.size(); ++i) {
				if (const node* cur = ht.buckets[i]) {
					//该bucket不为空
					node* copy = new_node(cur->val);
					buckets[i] = copy;
					for (node* next = cur->next; next; next = cur->next) {
						copy->next = new_node(next->val);
						copy = copy->next;
					}
				}
			}
			num_elements = ht.num_elements;
		}
		catch (...) {
			clear();
			throw;
		}
	}
	iterator find(const key_type& key) {
		size_type n = bkt_num_key(key);
		node* first;
		for(first=buckets[n];first&&!equals(get_key(first->val),key);first=first->next){}
		return iterator(first, this);
	}
	size_type count(const key_type& key)const {
		const size_type n = bkt_num_key(key);
		size_type result = 0;
		for (const node* cur = buckets[n]; cur; cur = cur->next) {
			if (equals(get_key(cur->val), key)) {
				++result;
			}
		}
		return result;
	}
	
	// replace_bucket 函数
	void replace_bucket(size_type bucket_count)
	{
		bucket_type bucket(bucket_count);
		if (size != 0)
		{
			for (size_type i = 0; i < bucket_size; ++i)
			{
				for (auto first = buckets[i]; first; first = first->next)
				{
					auto tmp = create_node(first->value);
					const auto n = hash(get_key(first->value), bucket_count);
					auto f = bucket[n];
					bool is_inserted = false;
					for (auto cur = f; cur; cur = cur->next)
					{
						if (is_equal(get_key(cur->value), get_key(first->value)))
						{
							tmp->next = cur->next;
							cur->next = tmp;
							is_inserted = true;
							break;
						}
					}
					if (!is_inserted)
					{
						tmp->next = f;
						bucket[n] = tmp;
					}
				}
			}
		}
		buckets.swap(bucket);
		bucket_size = buckets.size();
	}
	
	void swap(hashtable& rhs) noexcept
	{
		if (this != &rhs)
		{
			buckets.swap(rhs.buckets);
			swap(bucket_size, rhs.bucket_size);
			swap(num_elements, rhs.num_elements);
			swap(mlf_, rhs.mlf_);
			swap(hash, rhs.hash);
			swap(equal, rhs.equal);
		}
	}

	const_iterator M_cit(node_ptr node) const noexcept
	{
		return const_iterator(node, const_cast<hashtable*>(this));
	}

	iterator M_begin() noexcept
	{
		for (size_type n = 0; n < buckets.size(); ++n)
		{
			if (buckets[n])  // 找到第一个有节点的位置就返回
				return iterator(buckets[n], this);
		}
		return iterator(nullptr, this);
	}

	const_iterator M_begin() const noexcept
	{
		for (size_type n = 0; n < bucket_size; ++n)
		{
			if (buckets[n])  // 找到第一个有节点的位置就返回
				return M_cit(buckets[n]);
		}
		return M_cit(nullptr);
	}


	iterator       begin()        noexcept
	{
		return M_begin();
	}
	const_iterator begin()  const noexcept
	{
		return M_begin();
	}
	iterator       end()          noexcept
	{
		return iterator(nullptr, this);
	}
	const_iterator end()    const noexcept
	{
		return M_cit(nullptr);
	}

	const_iterator cbegin() const noexcept
	{
		return begin();
	}
	const_iterator cend()   const noexcept
	{
		return end();
	}

	// 查找与键值 key 相等的区间，返回一个 pair，指向相等区间的首尾
	pair<iterator,iterator> equal_range_multi(const key_type& key)
	{
		const auto n = hash(key);
		for (node_ptr first = buckets[n]; first; first = first->next)
		{
			if (is_equal(get_key(first->value), key))
			{ // 如果出现相等的键值
				for (node_ptr second = first->next; second; second = second->next)
				{
					if (!is_equal(get_key(second->value), key))
						return make_pair(iterator(first, this), iterator(second, this));
				}
				for (auto m = n + 1; m < bucket_size; ++m)
				{ // 整个链表都相等，查找下一个链表出现的位置
					if (buckets[m])
						return make_pair(iterator(first, this), iterator(buckets[m], this));
				}
				return make_pair(iterator(first, this), end());
			}
		}
		return make_pair(end(), end());
	}


	pair<const_iterator,const_iterator> equal_range_multi(const key_type& key) const
	{
		const auto n = hash(key);
		for (node_ptr first = buckets[n]; first; first = first->next)
		{
			if (is_equal(get_key(first->value), key))
			{
				for (node_ptr second = first->next; second; second = second->next)
				{
					if (!is_equal(get_key(second->value), key))
						return make_pair(M_cit(first), M_cit(second));
				}
				for (auto m = n + 1; m < bucket_size; ++m)
				{ // 整个链表都相等，查找下一个链表出现的位置
					if (buckets[m])
						return make_pair(M_cit(first), M_cit(buckets[m]));
				}
				return make_pair(M_cit(first), cend());
			}
		}
		return make_pair(cend(), cend());
	}

	pair<iterator,iterator> equal_range_unique(const key_type& key)
	{
		const auto n = hash(key);
		for (node_ptr first = buckets[n]; first; first = first->next)
		{
			if (is_equal(get_key(first->value), key))
			{
				if (first->next)
					return make_pair(iterator(first, this), iterator(first->next, this));
				for (auto m = n + 1; m < bucket_size; ++m)
				{ // 整个链表都相等，查找下一个链表出现的位置
					if (buckets[m])
						return make_pair(iterator(first, this), iterator(buckets[m], this));
				}
				return make_pair(iterator(first, this), end());
			}
		}
		return make_pair(end(), end());
	}

	pair<const_iterator,const_iterator> equal_range_unique(const key_type& key) const
	{
		const auto n = hash(key);
		for (node_ptr first = buckets[n]; first; first = first->next)
		{
			if (is_equal(get_key(first->value), key))
			{
				if (first->next)
					return make_pair(M_cit(first), M_cit(first->next));
				for (auto m = n + 1; m < bucket_size; ++m)
				{ // 整个链表都相等，查找下一个链表出现的位置
					if (buckets[m])
						return make_pair(M_cit(first), M_cit(buckets[m]));
				}
				return make_pair(M_cit(first), cend());
			}
		}
		return make_pair(cend(), cend());
	}

	// 删除迭代器所指的节点
	void erase(const_iterator position)
	{
		auto p = position.node;
		if (p)
		{
			const auto n = hash(get_key(p->value));
			auto cur = buckets[n];
			if (cur == p)
			{ // p 位于链表头部
				buckets[n] = cur->next;
				destroy_node(cur);
				--size;
			}
			else
			{
				auto next = cur->next;
				while (next)
				{
					if (next == p)
					{
						cur->next = next->next;
						destroy_node(next);
						--size;
						break;
					}
					else
					{
						cur = next;
						next = cur->next;
					}
				}
			}
		}
	}

	// 删除[first, last)内的节点
	void erase(const_iterator first, const_iterator last)
	{
		if (first.node == last.node)
			return;
		auto first_bucket = first.node
			? hash(get_key(first.node->value))
			: bucket_size;
		auto last_bucket = last.node
			? hash(get_key(last.node->value))
			: bucket_size;
		if (first_bucket == last_bucket)
		{ // 如果在 bucket 在同一个位置
			erase_bucket(first_bucket, first.node, last.node);
		}
		else
		{
			erase_bucket(first_bucket, first.node, nullptr);
			for (auto n = first_bucket + 1; n < last_bucket; ++n)
			{
				if (buckets[n] != nullptr)
					erase_bucket(n, nullptr);
			}
			if (last_bucket != bucket_size)
			{
				erase_bucket(last_bucket, last.node);
			}
		}
	}

	// 删除键值为 key 的节点
	size_type erase_multi(const key_type& key)
	{
		auto p = equal_range_multi(key);
		if (p.first.node != nullptr)
		{
			erase(p.first, p.second);
			return distance(p.first, p.second);
		}
		return 0;
	}

	size_type erase_unique(const key_type& key)
	{
		const auto n = hash(key);
		auto first = buckets[n];
		if (first)
		{
			if (is_equal(get_key(first->value), key))
			{
				buckets[n] = first->next;
				destroy_node(first);
				--num_elements;
				return 1;
			}
			else
			{
				auto next = first->next;
				while (next)
				{
					if (is_equal(get_key(next->value), key))
					{
						first->next = next->next;
						destroy_node(next);
						--size;
						return 1;
					}
					first = next;
					next = first->next;
				}
			}
		}
		return 0;
	}


};

}
#endif // !XZESTL_HASHTABLE_H
