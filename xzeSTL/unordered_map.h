#pragma once
#ifndef XZESTL_UNORDEREDMAP_H
#define XZESTL_UNORDEREDMAP_H
#include "adaptor.h"
#include "hashtable.h"
#include "alloc.h"
#include "algo.h"
namespace xze {
	template<class Key, class T,class HashFcn = hash<Key>, class EqualKey = equal_to<Key>, class Alloc = alloc>
	class hash_map {
	private:
		typedef hashtable<pair<const Key,T>, Key, HashFcn, select1st<pair<const Key, T>>, EqualKey, Alloc>ht;
		ht rep;
	public:
		typedef typename ht::key_type key_type;
		typedef T data_type;
		typedef T mapped_type;
		typedef typename ht::value_type value_type;
		typedef typename ht::hasher hasher;
		typedef typename ht::key_equal key_equal;
		typedef typename ht::size_type size_type;
		typedef typename ht::difference_type difference_type;
		typedef typename ht::const_pointer const_pointer;
		typedef typename ht::const_pointer pointer;
		typedef typename ht::const_reference const_reference;
		typedef typename ht::const_reference reference;
		typedef typename ht::const_iterator const_iterator;
		typedef typename ht::const_iterator iterator;

		hasher hash_funct()const { return rep.hash; }
		key_equal key_eq()const { return rep.equals; }
	public:
		//ππ‘Ï
		hash_map() :rep(100, hasher(), key_equal()) {}
		explicit hash_map(size_type n) :rep(n, hasher(), key_equal()) {}
		hash_map(size_type n, const hasher& hf) :rep(n, hf, key_equal()) {}
		hash_map(size_type n, const hasher& hf, const key_equal& eql) :rep(n, hf, eql) {}

		template<class InputIterator>
		hash_map(InputIterator f, InputIterator l) : rep(100, hasher(), key_equal()) {
			rep.insert_unique(f, l);
		}
		template<class InputIterator>
		hash_map(InputIterator f, InputIterator l, size_type n) : rep(n, hasher(), key_equal()) {
			rep.insert_unique(f, l);
		}
		template<class InputIterator>
		hash_map(InputIterator f, InputIterator l, size_type n, const hasher& hf) : rep(n, hf, key_equal()) {
			rep.insert_unique(f, l);
		}
		template<class InputIterator>
		hash_map(InputIterator f, InputIterator l, size_type n, const hasher& hf, const key_equal& eql) : rep(n, hf, eql) {
			rep.insert_unique(f, l);
		}

		size_type size() const { return rep.size(); }
		bool empty()const noexcept { return rep.empty(); }
		size_type max_size() const noexcept { return rep.max_size(); }
		void swap(hash_map& hs) { rep.swap(hs.rep); }
		friend bool operator==(const hash_map& hs1, const hash_map& hs2) {
			return hs1.rep == hs2.rep;
		}
		iterator begin()const { return rep.begin(); }
		pair<iterator, bool> insert(const value_type& obj) {
			pair<typename ht::iterator, bool>p = rep.insert_unique(obj);
			return pair<iterator, bool>(p.first, p.second);
		}
		template<class InputIterator>
		void insert(InputIterator f, InputIterator l) {
			rep.insert_unique(f, l);
		}
		pair<iterator, bool> insert_noresize(const value_type& obj) {
			pair<typename ht::iterator, bool>p = rep.insert_unique_noresize(obj);
			return pair<iterator, bool>(p.first, p.second);
		}
		iterator find(const key_type& key)const { return rep.find(key); }
		size_type count(const key_type& key)const { return rep.count(key); }
		pair<iterator, iterator> equal_range(const key_type& key)const {
			return rep.equal_range_unique(key);
		}
		size_type erase(const key_type& key) { return rep.erase(find(key)); }
		void erase(iterator it) { rep.erase(it); }
		void erase(iterator f, iterator l) { rep.erase(f, l); }
		void clear() { rep.clear(); }
		void resize(size_type hint) { rep.resize(); }
		size_type bucket_count()const { return rep.bucket_count(); }
		size_type max_bucket_count()const { return rep.max_bucket_count(); }
		size_type elems_in_bucket(size_type n)const { return rep.bkt_num(n); }

	};
}

#endif // !XZESTL_UNORDEREDSET_H
